import cv2
import numpy


img = cv2.imread("/Users/i7y/Desktop/1.png")  # 原始图片
logo = cv2.imread("/Users/i7y/Desktop/logo.png") # logo
mask = numpy.full(logo.shape, 0, numpy.uint8)  # 掩码

img[0:logo.shape[0], 0:logo.shape[1]] = logo

cv2.namedWindow("win", cv2.WINDOW_NORMAL)
cv2.resizeWindow("win", 640, 480)
while True:
    cv2.imshow("win", img)
    key = cv2.waitKey(1) & 0xFF
    if key == ord('q'):
        break
cv2.destroyAllWindows