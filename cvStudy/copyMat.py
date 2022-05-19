import numpy
import cv2


imgA = cv2.imread("/Users/i7y/Desktop/1.png")
# 浅拷贝
imgB = imgA
# 深拷贝
imgC = imgA.copy()

# 修改图像的某块区域
imgA[100:740, 200:680] = [0, 0, 255]

# cv2.imshow('win', imgB)
cv2.imshow('win', imgC)
key = cv2.waitKey(0)
if key & 0xFF == ord('q'):
    cv2.destroyAllWindows()