# 自适应二值化
import cv2

img = cv2.imread("./img/binary.png")  

# 灰度化
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
# 自适应二值化
dst = cv2.adaptiveThreshold(gray, 255, 
                            cv2.ADAPTIVE_THRESH_GAUSSIAN_C, 
                            cv2.THRESH_BINARY,
                            9,0)
while True:
    cv2.imshow("org", img)
    cv2.imshow("gray", gray)
    cv2.imshow("bin", dst)
    key = cv2.waitKey(1) & 0xFF
    if key == ord('q'):
        break
cv2.destroyAllWindows

