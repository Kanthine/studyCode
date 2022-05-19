# 图像拼接
import cv2
import numpy

def get_homo(img1, img2):
    # 1、创建特征转换对象：SIFT 速度慢、但准确率高
    # 2、通过特征转换对象获得特征点、描述子
    # 3、创建特征匹配器
    # 4、进行特征匹配
    # 5、过滤特征、找出有效的特征匹配点
    
    sift = cv2.SIFT_create()
    kp1, des1 = sift.detectAndCompute(img1, None)
    kp2, des2 = sift.detectAndCompute(img2, None)

    # 创建一个暴力特征匹配器，匹配特征点
    bf = cv2.BFMatcher()
    match = bf.knnMatch(des1, des2, k=2)

    # 过滤特征、找出有效的特征匹配点
    verify_ratio = 0.8
    verify_match = []
    min_matchs = 8
    for m1, m2 in match:
        if m1.distance < 0.8 * m2.distance:
            verify_match.append(m1)
    
    # 计算单应性矩阵
    if (len(verify_match) > min_matchs):
        img1_pts = []
        img2_pts = []
        for m in verify_match:
            img1_pts.append(kp1[m.queryIdx].pt)
            img2_pts.append(kp2[m.trainIdx].pt)
        
        # [(x1, y1), (x2, y2), ...] => [[x1, y1], [x2, y2], ...]
        img1_pts = numpy.float32(img1_pts).reshape(-1, 1, 2)
        img2_pts = numpy.float32(img2_pts).reshape(-1, 1, 2)
        H, mask = cv2.findHomography(img1_pts, img2_pts, cv2.RANSAC, 5.0)
        return H
    print("error, Not enough matches")
    exit()

def stitch_image(img1, img2, H):
    # 1、获得每张图像的四个角点: 逆时针排序
    # 2、对图片进行变换（单应性矩阵使图进行旋转，平移）
    # 3、创建一张大图，将两张图拼接到一起
    # 4、将结果输出

    h1, w1 = img1.shape[:2]
    h2, w2 = img2.shape[:2]
    # 齐次坐标（二维升三维）
    img1_dims = numpy.float32([[0, 0], [0, h1], [w1, h1], [w1 ,0]]).reshape(-1, 1, 2)
    img2_dims = numpy.float32([[0, 0], [0, h2], [w2, h2], [w2 ,0]]).reshape(-1, 1, 2)
    
    img1_transform = cv2.perspectiveTransform(img1_dims, H)

    result_dims = numpy.concatenate((img2_dims, img1_transform), axis=0)

    [x_min, y_min] = numpy.int32(result_dims.min(axis=0).ravel() - 0.5)
    [x_max, y_max] = numpy.int32(result_dims.max(axis=0).ravel() + 0.5)

    # 平移
    transform_dist = [-x_min, -y_min]
    # 齐次
    transform_array = numpy.array([
        [1, 0, transform_dist[0]],
        [0, 1, transform_dist[1]],
        [0, 0,                 1],
    ])
    
    # 步骤四：拼接并输出最终结果
    res_img = cv2.warpPerspective(img1, transform_array.dot(H), (x_max - x_min, y_max - y_min))
    res_img[transform_dist[1] : transform_dist[1] + h2,
            transform_dist[0] : transform_dist[0] + w2] = img2
    return res_img

# 步骤一：读取图片、将图片设置成一样大小
img1 = cv2.imread("./img/left.png")  
img2 = cv2.imread("./img/right.png")  

img1 = cv2.resize(img1, (460, 400))
img2 = cv2.resize(img2, (460, 400))

inputs = numpy.hstack((img1, img2))

# 步骤二：找特征点，描述子，计算单应性矩阵
H = get_homo(img1, img2)

# 步骤三：计算单应性矩阵对图像进行变换，然后平移
result_img = stitch_image(img1, img2, H)


while True: 
    cv2.imshow("img1", img1)
    cv2.imshow("img2", img2)
    cv2.imshow("result_img", result_img)
    key = cv2.waitKey(1) & 0xFF
    if key == ord('q'):
        break
cv2.destroyAllWindows
