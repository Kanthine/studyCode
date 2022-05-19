# 轮廓检测
import cv2
import numpy

img = cv2.imread("./img/edge.png")  
# 灰度化
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

# 二值化
ret, bin = cv2.threshold(gray, 150, 255, cv2.THRESH_BINARY)

# 轮廓查找
contours, hierarchy = cv2.findContours(bin, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

# 轮廓绘制
cv2.drawContours(img, contours, -1, (0, 0, 255), 10)

# 计算轮廓面积
area = cv2.contourArea(contours[0])

# 计算轮廓周长
len = cv2.arcLength(contours[0], True)

print(contours, hierarchy, area, len)

while True:
    cv2.imshow("org", img)
    cv2.imshow("bin", bin)
    key = cv2.waitKey(1) & 0xFF
    if key == ord('q'):
        break
cv2.destroyAllWindows

