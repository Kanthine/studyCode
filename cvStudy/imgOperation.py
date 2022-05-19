import cv2
import numpy

# 两张图片的分辨率相同
img1 = cv2.imread("/Users/i7y/Desktop/1.png")
img2 = cv2.imread("/Users/i7y/Desktop/2.png")
# img2 = numpy.ones(img1.shape, numpy.uint8) * 50


# 图像的加法运算
img3 = cv2.add(img1, img2)

# 图像的减法运算
img4 = cv2.subtract(img2, img1)

# 图像的乘法运算
img5 = cv2.multiply(img1 * 0.01, img2 * 0.01)

# 图像的除法运算
img6 = cv2.divide(img1 , img2)

# 图像的溶合运算
img7 = cv2.addWeighted(img1, 0.7, img2, 0.3, 0)

# 图像的位运算·非
img8 = cv2.bitwise_not(img1)

# 图像的位运算·与
img9 = cv2.bitwise_and(img1, img2)

# 图像的位运算·或
img10 = cv2.bitwise_or(img1, img2)

# 图像的位运算·异或
img11 = cv2.bitwise_xor(img1, img2)

cv2.namedWindow("win", cv2.WINDOW_NORMAL)
cv2.resizeWindow("win", 640, 480)
while True:
    cv2.imshow("win", img11)
    key = cv2.waitKey(1) & 0xFF
    if key == ord('q'):
        break
cv2.destroyAllWindows