import cv2
import numpy


img = cv2.imread("./img/gaussian.png")  

# 高斯滤波
dst = cv2.GaussianBlur(img, (7, 7), 3)

while True:
    cv2.imshow("win", img)
    cv2.imshow("dst", dst)
    key = cv2.waitKey(1) & 0xFF
    if key == ord('q'):
        break
cv2.destroyAllWindows





# # 边缘检测
# img = cv2.imread("./img/lena.png")  

# # Canny 边缘检测
# dst = cv2.Canny(img, 100, 200)

# while True:
#     cv2.imshow("win", img)
#     cv2.imshow("dst", dst)
#     key = cv2.waitKey(1) & 0xFF
#     if key == ord('q'):
#         break
# cv2.destroyAllWindows


# # 原始图片
# img = cv2.imread("/Users/i7y/Desktop/1.png")  
# h, w, ch = img.shape

# # 设置卷积核
# kernal = numpy.ones((5, 5), numpy.float32) / 25
# # 卷积操作得到卷积后的图像
# dst = cv2.filter2D(img, -1, kernal)

# # 均值滤波
# dst = cv2.blur(img, (5, 5))

# # 高斯滤波
# dst = cv2.GaussianBlur(img, (5, 5), 1)

# # 中值滤波
# dst = cv2.medianBlur(img, 3)

# # 双边滤波
# dst = cv2.bilateralFilter(img, 7, 20, 50)

# # 索贝尔算子
# s1 = cv2.Sobel(img, cv2.CV_64F, 1, 0, ksize= 5)
# s2 = cv2.Sobel(img, cv2.CV_64F, 0, 1, ksize= 5)
# dst = cv2.add(s1, s2)

# # 沙尔算子
# s1 = cv2.Scharr(img, cv2.CV_64F, 1, 0)
# s2 = cv2.Scharr(img, cv2.CV_64F, 0, 1)
# dst = cv2.add(s1, s2)


# # Canny 边缘检测
# dst = cv2.Canny(img, 100, 200)

# cv2.namedWindow("win", cv2.WINDOW_AUTOSIZE)
# while True:
#     cv2.imshow("win", dst)
#     key = cv2.waitKey(1) & 0xFF
#     if key == ord('q'):
#         break
# cv2.destroyAllWindows






# # 原始图片
# img = cv2.imread("/Users/i7y/Desktop/1.png")  
# h, w, ch = img.shape

# # 设置卷积核
# kernal = numpy.ones((5, 5), numpy.float32) / 25
# # 卷积操作得到卷积后的图像
# dst = cv2.filter2D(img, -1, kernal)

# cv2.namedWindow("win", cv2.WINDOW_AUTOSIZE)
# while True:
#     cv2.imshow("win", dst)
#     key = cv2.waitKey(1) & 0xFF
#     if key == ord('q'):
#         break
# cv2.destroyAllWindows