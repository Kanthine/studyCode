# mask 
import cv2

img = cv2.imread("./img/time.png")  
# 灰度化
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
# 椭圆卷积核
kernel_ELLIPSE = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (21, 21))
dst_ELLIPSE = cv2.erode(gray, kernel_ELLIPSE, iterations = 2)

while True:
    cv2.imshow("org", gray)
    cv2.imshow("erode", dst_ELLIPSE)
    key = cv2.waitKey(1) & 0xFF
    if key == ord('q'):
        break
cv2.destroyAllWindows

