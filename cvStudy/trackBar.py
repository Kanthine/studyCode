import cv2
import numpy

# 回调函数
def bar_callback():
    pass

# 创建窗口
cv2.namedWindow("win", cv2.WINDOW_NORMAL)
cv2.resizeWindow("win", 640, 480)

# 创建 RGB
cv2.createTrackbar('bar_R', 'win', 0, 255, bar_callback)
cv2.createTrackbar('bar_G', 'win', 0, 255, bar_callback)
cv2.createTrackbar('bar_B', 'win', 0, 255, bar_callback)

# 用于窗口背景
img = numpy.zeros((480, 640, 3), numpy.uint8)

while True:

    # 更改背景色
    r = cv2.getTrackbarPos('bar_R', 'win')
    g = cv2.getTrackbarPos('bar_G', 'win')
    b = cv2.getTrackbarPos('bar_B', 'win')
    # BGR 格式
    img[:] = [b, g, r]

    cv2.imshow("win", img)
    key = cv2.waitKey(1)
    if key & 0xFF == ord('q'):
        break

cv2.destroyAllWindows