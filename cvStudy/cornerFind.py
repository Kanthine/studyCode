# 角点检测 
import cv2
import numpy

img = cv2.imread("./img/edgeFind.png")  
# 灰度化
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
# Harris 角点检测
dst = cv2.cornerHarris(gray, 2, 3, 0.04)
# Harris 角点展示
img[dst > 0.01 * dst.max()] = [0, 0, 255]

# Shi-Tomasi 角点检测
corners = cv2.goodFeaturesToTrack(gray, 1000, 0.01, 10, )
corners = numpy.int0(corners)
# 绘制角度
for index in corners:
    x, y = index.ravel()
    cv2.circle(img, (x, y), 3, (255, 0, 0), -1)


while True:
    cv2.imshow("org", img)
    key = cv2.waitKey(1) & 0xFF
    if key == ord('q'):
        break
cv2.destroyAllWindows

