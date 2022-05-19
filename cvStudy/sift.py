# 角点检测 
import cv2
import numpy

img = cv2.imread("./img/edgeFind.png")  
# 灰度化
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

# 创建 SIFT 对象
sift = cv2.SIFT_create()
# 进行检测
kp, des = sift.detectAndCompute(gray, None)
print(des[0])
# 绘制关键点
cv2.drawKeypoints(gray, kp, img)

while True:
    cv2.imshow("org", img)
    key = cv2.waitKey(1) & 0xFF
    if key == ord('q'):
        break
cv2.destroyAllWindows



# img = cv2.imread("./img/edgeFind.png")  
# # 灰度化
# gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

# # 创建 SIFT 对象
# sift = cv2.SIFT_create()
# # 进行检测
# kp = sift.detect(gray, None)
# # 绘制关键点
# cv2.drawKeypoints(gray, kp, img)

# while True:
#     cv2.imshow("org", img)
#     key = cv2.waitKey(1) & 0xFF
#     if key == ord('q'):
#         break
# cv2.destroyAllWindows

