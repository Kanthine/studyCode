# 开运算
import cv2
import numpy

img = cv2.imread("./img/wordPoint.png")  

# 灰度化
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
# 自定义卷积核
kernel = numpy.ones((7, 7), numpy.uint8)
dst = cv2.morphologyEx(gray, cv2.MORPH_OPEN, kernel, iterations = 1)

kernel_rect = cv2.getStructuringElement(cv2.MORPH_RECT, (7, 7))
dst_rect = cv2.morphologyEx(gray, cv2.MORPH_OPEN, kernel_rect, iterations = 1)

kernel_cross = cv2.getStructuringElement(cv2.MORPH_CROSS, (7, 7))
dst_cross = cv2.morphologyEx(gray, cv2.MORPH_OPEN, kernel_cross, iterations = 1)

kernel_ELLIPSE = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (7, 7))
dst_ELLIPSE = cv2.morphologyEx(gray, cv2.MORPH_OPEN, kernel_ELLIPSE, iterations = 1)

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

