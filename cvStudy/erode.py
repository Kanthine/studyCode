# 腐蚀
import cv2
import numpy

img = cv2.imread("./img/word.png")  

# 灰度化
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
# 自定义卷积核
kernel = numpy.ones((5, 5), numpy.uint8)
dst = cv2.erode(gray, kernel, iterations = 2)
# 矩形卷积核
kernel_rect = cv2.getStructuringElement(cv2.MORPH_RECT, (5, 5))
dst_rect = cv2.erode(gray, kernel_rect, iterations = 2)
# 十字形卷积核
kernel_cross = cv2.getStructuringElement(cv2.MORPH_CROSS, (5, 5))
dst_cross = cv2.erode(gray, kernel_cross, iterations = 2)
# 椭圆卷积核
kernel_ELLIPSE = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (5, 5))
dst_ELLIPSE = cv2.erode(gray, kernel_ELLIPSE, iterations = 2)

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

