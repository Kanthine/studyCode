# 图像分割·MeanShift 分割
import cv2

img = cv2.imread('./img/cat.png')
img_mean = cv2.pyrMeanShiftFiltering(img, 20, 30)
img_canny = cv2.Canny(img_mean, 150, 300)

contours, _ = cv2.findContours(img_canny, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

cv2.drawContours(img, contours,  -1, (0, 0, 255), 2)

while True:
    cv2.imshow("org", img)
    cv2.imshow("img_mean", img_mean)
    cv2.imshow("img_canny", img_canny)
    key = cv2.waitKey(1) & 0xFF
    if key == ord('q'):
        break
cv2.destroyAllWindows()