import cv2
import numpy

# 鼠标回调函数
def mouse_callback(event, x, y, flags, userdata):
    print(event, x, y, flags, userdata)

# 创建窗口
cv2.namedWindow("win", cv2.WINDOW_NORMAL)
cv2.resizeWindow("win", 640, 480)
# 设置鼠标回调
cv2.setMouseCallback("win", mouse_callback, 'userdata')
# 用于窗口背景
#img = numpy.zeros((480, 640, 3), numpy.uint8)
img = numpy.full((480, 640, 3), 100, numpy.uint8)

while True:
    cv2.imshow("win", img)
    key = cv2.waitKey(1)
    if key & 0xFF == ord('q'):
        break

cv2.destroyAllWindows