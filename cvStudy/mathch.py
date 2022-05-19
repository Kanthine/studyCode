# 特征匹配
import cv2
import numpy

img1 = cv2.imread("./img/left.png")  
img2 = cv2.imread("./img/right.png")  

# 灰度化
gray1 = cv2.cvtColor(img1, cv2.COLOR_BGR2GRAY)
gray2 = cv2.cvtColor(img2, cv2.COLOR_BGR2GRAY)

# 创建 SIFT 对象: 检测特征点
sift = cv2.SIFT_create()
# 进行检测
kp1, des1 = sift.detectAndCompute(gray1, None)
kp2, des2 = sift.detectAndCompute(gray2, None)

# 创建匹配器，匹配特征点
index_params = dict(algorithm = 1, trees = 5)
search_params = dict(checks = 50)
flann = cv2.FlannBasedMatcher(index_params, search_params)
# 对描述子进行匹配计算
match = flann.knnMatch(des1, des2, k=2)

# 过滤：对所有匹配点进行优化
good = []
for i, (m, n) in enumerate(match):
    # 距离越小，近似度越高
    if m.distance < 0.8 * n.distance:
        good.append(m)

if (len(good)) >= 4:
    # 计算单应性矩阵
    srcpts = numpy.float32([kp1[m.queryIdx].pt for m in good]).reshape(-1, 1, 2)
    dtspts = numpy.float32([kp2[m.trainIdx].pt for m in good]).reshape(-1, 1, 2)
    H, _ = cv2.findHomography(srcpts, dtspts, cv2.RANSAC, 5.0)

    h, w = img1.shape[:2]
    pts = numpy.float32([[0, 0], [0, h - 1], [w - 1, h - 1], [w - 1, 0]]).reshape(-1, 1, 2)
    dst = cv2.perspectiveTransform(pts, H)

    cv2.polylines(img2, [numpy.int32(dst)], True, (0, 0, 255))

# 将相似特征点连线
img3 = cv2.drawMatchesKnn(img1, kp1, img2, kp2, [good], None)

while True: 
    cv2.imshow("org", img3)
    key = cv2.waitKey(1) & 0xFF
    if key == ord('q'):
        break
cv2.destroyAllWindows
