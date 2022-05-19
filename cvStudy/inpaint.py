# 修复图像
import cv2

# 破损图片
img = cv2.imread("./img/bad.png")
# mask 图片
mask = cv2.imread("./img/badMask.png") 
gray = cv2.cvtColor(mask, cv2.COLOR_BGR2GRAY)

dst = cv2.inpaint(img, gray, 5, cv2.INPAINT_TELEA)

while True:
    cv2.imshow("org", img)
    cv2.imshow("dst", dst)
    key = cv2.waitKey(1) & 0xFF
    if key == ord('q'):
        break
cv2.destroyAllWindows