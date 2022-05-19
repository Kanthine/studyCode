# 开运算 与 闭运算
import cv2
import numpy


# 黑帽运算
img = cv2.imread("./img/wordPoint2.png")  

# 灰度化
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
# 自定义卷积核
kernel = numpy.ones((7, 7), numpy.uint8)
dst = cv2.morphologyEx(gray, cv2.MORPH_BLACKHAT, kernel, iterations = 1)

kernel_rect = cv2.getStructuringElement(cv2.MORPH_RECT, (7, 7))
dst_rect = cv2.morphologyEx(gray, cv2.MORPH_BLACKHAT, kernel_rect, iterations = 1)

kernel_cross = cv2.getStructuringElement(cv2.MORPH_CROSS, (7, 7))
dst_cross = cv2.morphologyEx(gray, cv2.MORPH_BLACKHAT, kernel_cross, iterations = 1)

kernel_ELLIPSE = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (7, 7))
dst_ELLIPSE = cv2.morphologyEx(gray, cv2.MORPH_BLACKHAT, kernel_ELLIPSE, iterations = 1)

while True:
    cv2.imshow("org", img)
    cv2.imshow("dst", dst)
    cv2.imshow("dst_rect", dst_rect)
    cv2.imshow("dst_cross", dst_cross)
    cv2.imshow("dst_ELLIPSE", dst_ELLIPSE)
    key = cv2.waitKey(1) & 0xFF
    if key == ord('q'):
        break
cv2.destroyAllWindows


# # 顶帽运算
# img = cv2.imread("./img/wordPoint.png")  

# # 灰度化
# gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
# # 自定义卷积核
# kernel = numpy.ones((7, 7), numpy.uint8)
# dst = cv2.morphologyEx(gray, cv2.MORPH_TOPHAT, kernel, iterations = 1)

# kernel_rect = cv2.getStructuringElement(cv2.MORPH_RECT, (7, 7))
# dst_rect = cv2.morphologyEx(gray, cv2.MORPH_TOPHAT, kernel_rect, iterations = 1)

# kernel_cross = cv2.getStructuringElement(cv2.MORPH_CROSS, (7, 7))
# dst_cross = cv2.morphologyEx(gray, cv2.MORPH_TOPHAT, kernel_cross, iterations = 1)

# kernel_ELLIPSE = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (7, 7))
# dst_ELLIPSE = cv2.morphologyEx(gray, cv2.MORPH_TOPHAT, kernel_ELLIPSE, iterations = 1)

# while True:
#     cv2.imshow("org", img)
#     cv2.imshow("dst", dst)
#     cv2.imshow("dst_rect", dst_rect)
#     cv2.imshow("dst_cross", dst_cross)
#     cv2.imshow("dst_ELLIPSE", dst_ELLIPSE)
#     key = cv2.waitKey(1) & 0xFF
#     if key == ord('q'):
#         break
# cv2.destroyAllWindows


# # 梯度运算
# img = cv2.imread("./img/word.png")  

# # 灰度化
# gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
# # 自定义卷积核
# kernel = numpy.ones((7, 7), numpy.uint8)
# dst = cv2.morphologyEx(gray, cv2.MORPH_GRADIENT, kernel, iterations = 1)

# kernel_rect = cv2.getStructuringElement(cv2.MORPH_RECT, (7, 7))
# dst_rect = cv2.morphologyEx(gray, cv2.MORPH_GRADIENT, kernel_rect, iterations = 1)

# kernel_cross = cv2.getStructuringElement(cv2.MORPH_CROSS, (7, 7))
# dst_cross = cv2.morphologyEx(gray, cv2.MORPH_GRADIENT, kernel_cross, iterations = 1)

# kernel_ELLIPSE = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (7, 7))
# dst_ELLIPSE = cv2.morphologyEx(gray, cv2.MORPH_GRADIENT, kernel_ELLIPSE, iterations = 1)

# while True:
#     cv2.imshow("org", img)
#     cv2.imshow("dst", dst)
#     cv2.imshow("dst_rect", dst_rect)
#     cv2.imshow("dst_cross", dst_cross)
#     cv2.imshow("dst_ELLIPSE", dst_ELLIPSE)
#     key = cv2.waitKey(1) & 0xFF
#     if key == ord('q'):
#         break
# cv2.destroyAllWindows




# # 闭运算
# img = cv2.imread("./img/wordPoint2.png")  

# # 灰度化
# gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
# # 自定义卷积核
# kernel = numpy.ones((7, 7), numpy.uint8)
# dst = cv2.morphologyEx(gray, cv2.MORPH_CLOSE, kernel, iterations = 1)

# kernel_rect = cv2.getStructuringElement(cv2.MORPH_RECT, (7, 7))
# dst_rect = cv2.morphologyEx(gray, cv2.MORPH_CLOSE, kernel_rect, iterations = 1)

# kernel_cross = cv2.getStructuringElement(cv2.MORPH_CROSS, (7, 7))
# dst_cross = cv2.morphologyEx(gray, cv2.MORPH_CLOSE, kernel_cross, iterations = 1)

# kernel_ELLIPSE = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (7, 7))
# dst_ELLIPSE = cv2.morphologyEx(gray, cv2.MORPH_CLOSE, kernel_ELLIPSE, iterations = 1)

# while True:
#     cv2.imshow("org", img)
#     cv2.imshow("dst", dst)
#     cv2.imshow("dst_rect", dst_rect)
#     cv2.imshow("dst_cross", dst_cross)
#     cv2.imshow("dst_ELLIPSE", dst_ELLIPSE)
#     key = cv2.waitKey(1) & 0xFF
#     if key == ord('q'):
#         break
# cv2.destroyAllWindows


# 开运算
# img = cv2.imread("./img/wordPoint.png")  

# # 灰度化
# gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
# # 自定义卷积核
# kernel = numpy.ones((7, 7), numpy.uint8)
# dst = cv2.morphologyEx(gray, cv2.MORPH_OPEN, kernel, iterations = 1)

# kernel_rect = cv2.getStructuringElement(cv2.MORPH_RECT, (7, 7))
# dst_rect = cv2.morphologyEx(gray, cv2.MORPH_OPEN, kernel_rect, iterations = 1)

# kernel_cross = cv2.getStructuringElement(cv2.MORPH_CROSS, (7, 7))
# dst_cross = cv2.morphologyEx(gray, cv2.MORPH_OPEN, kernel_cross, iterations = 1)

# kernel_ELLIPSE = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (7, 7))
# dst_ELLIPSE = cv2.morphologyEx(gray, cv2.MORPH_OPEN, kernel_ELLIPSE, iterations = 1)

# while True:
#     cv2.imshow("org", img)
#     cv2.imshow("dst", dst)
#     cv2.imshow("dst_rect", dst_rect)
#     cv2.imshow("dst_cross", dst_cross)
#     cv2.imshow("dst_ELLIPSE", dst_ELLIPSE)
#     key = cv2.waitKey(1) & 0xFF
#     if key == ord('q'):
#         break
# cv2.destroyAllWindows


