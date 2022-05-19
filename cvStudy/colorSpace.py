import cv2
import numpy

# 回调函数
def bar_callback(pos):
    print(pos)

# 创建窗口
cv2.namedWindow("win", cv2.WINDOW_NORMAL)
cv2.resizeWindow("win", 640, 480)

colorSpace = [cv2.COLOR_BGR2RGB,  cv2.COLOR_BGR2BGRA, 
              cv2.COLOR_BGR2GRAY, cv2.COLOR_BGR2HSV_FULL,
              cv2.COLOR_BGR2YUV]
cv2.createTrackbar('index', 'win', 0, len(colorSpace), bar_callback)

img = cv2.imread("/Users/i7y/Desktop/1.png")

while True:

    # 更改背景色
    index = cv2.getTrackbarPos('index', 'win')
    # 颜色空间转换
    cvt_img = cv2.cvtColor(img, colorSpace[index])
    cv2.imshow("win", cvt_img)
    key = cv2.waitKey(1)
    if key & 0xFF == ord('q'):
        break

cv2.destroyAllWindows