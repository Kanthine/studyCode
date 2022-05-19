# 角点检测 
import cv2
import numpy

img = cv2.imread("./img/edgeFind.png")  
# 灰度化
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

# 创建 ORB 对象
surf = cv2.ORB_create()
# 进行检测
kp, des = surf.detectAndCompute(gray, None)

# 绘制关键点
cv2.drawKeypoints(gray, kp, img)

while True:
    cv2.imshow("org", img)
    key = cv2.waitKey(1) & 0xFF
    if key == ord('q'):
        break
cv2.destroyAllWindows
