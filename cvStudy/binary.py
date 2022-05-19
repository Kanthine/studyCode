# 全局二值化

import cv2

img = cv2.imread("./img/cat.png")  

# 灰度化
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
# 低于阀值，则设置为 0；高于阀值，则设置为 maxval
ret, bin = cv2.threshold(gray, 100, 255, cv2.THRESH_BINARY)
# 低于阀值，则设置为 maxval；高于阀值，则设置为 0
ret, bin_inv = cv2.threshold(gray, 100, 255, cv2.THRESH_BINARY_INV)
# 消峰：低于阀值不做处理；高于阀值，则抹平为阀
ret, bin_trunc = cv2.threshold(gray, 100, 255, cv2.THRESH_TRUNC)
# 低于阀值，则设置为 0；高于阀值不做处理
ret, bin_tozero = cv2.threshold(gray, 100, 255, cv2.THRESH_TOZERO)
# 低于阀值不做处理；高于阀值，则设置为 0
ret, bin_tozero_inv = cv2.threshold(gray, 100, 255, cv2.THRESH_TOZERO_INV)

ret, bin_OTSU = cv2.threshold(gray, 200, 220, cv2.THRESH_OTSU)

ret, bin_TRIANGLE = cv2.threshold(gray, 200, 220, cv2.THRESH_TRIANGLE)

while True:
    cv2.imshow("org", img)
    cv2.imshow("gray", gray)
    cv2.imshow("bin", bin)
    cv2.imshow("bin_inv", bin_inv)
    cv2.imshow("bin_trunc", bin_trunc)
    cv2.imshow("bin_tozero", bin_tozero)
    cv2.imshow("bin_tozero_inv", bin_tozero_inv)
    cv2.imshow("bin_OTSU", bin_OTSU)
    cv2.imshow("bin_TRIANGLE", bin_TRIANGLE)
    key = cv2.waitKey(1) & 0xFF
    if key == ord('q'):
        break
cv2.destroyAllWindows

