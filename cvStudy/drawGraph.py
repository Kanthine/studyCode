import cv2
import numpy

# 通过鼠标进行基本图形的绘制
# 1、l 键画线
# 2、r 键画矩形
# 3、c 键画圆
# ...

img = cv2.imread("/Users/i7y/Desktop/1.png")
currentShape = 0
startPos = (0, 0)

# 鼠标回调函数
def mouse_callback(event, x, y, flags, userdata):
    global currentShape, startPos
    if (event & cv2.EVENT_LBUTTONDOWN == cv2.EVENT_LBUTTONDOWN) : # 左键按下
        startPos = (x, y)
    if (event & cv2.EVENT_LBUTTONUP == cv2.EVENT_LBUTTONUP) : # 左键抬起
        if currentShape == 1:
            cv2.line(img, startPos, (x, y), (0, 0, 255), 5, cv2.LINE_AA)
        elif currentShape == 2:
            cv2.rectangle(img, startPos, (x, y), (0, 0, 255),cv2. FILLED, cv2.LINE_AA)
        elif currentShape == 3:
            radius = int(((x - startPos[0])**2 + (y - startPos[1])**2)**0.5)
            cv2.circle(img, startPos, radius, (0, 0, 255), 5, cv2.LINE_AA)

# 创建窗口
cv2.namedWindow("win", cv2.WINDOW_NORMAL)
cv2.resizeWindow("win", 640, 480)
# 设置鼠标回调事件
cv2.setMouseCallback("win", mouse_callback, 'userdata')

while True:
    cv2.imshow("win", img)
    key = cv2.waitKey(1) & 0xFF
    if key == ord('q'):
        break
    elif key == ord('l'):
        currentShape = 1
    elif key == ord('r'):
        currentShape = 2
    elif key == ord('c'):
        currentShape = 3

cv2.destroyAllWindows




# # 绘制椭圆
# img = cv2.imread("/Users/i7y/Desktop/1.png")

# # 在图片上绘制一个扇形
# cv2.ellipse(img, (320, 240), (100, 60), 45, 0, 250, (0, 0, 255), 5, LINE_AA)

# cv2.imshow('win', img)
# key = cv2.waitKey(0)
# if key & 0xFF == ord('q'):
    # cv2.destroyAllWindows()

# # 绘制多边形
# img = cv2.imread("/Users/i7y/Desktop/1.png")

# # 在图片上绘制一个多边形
# pts = numpy.array([(300, 10), (150, 100), (450, 100)], numpy.int32)
# cv2.polylines(img, [pts], True, (0, 0, 255), 5, LINE_AA)

# # 填充多边形
# pts2 = numpy.array([(300, 10), (150, 350), (450, 100)], numpy.int32)
# cv2.fillPoly(img, [pts2], (255, 0, 0))

# cv2.imshow('win', img)
# key = cv2.waitKey(0)
# if key & 0xFF == ord('q'):
#     cv2.destroyAllWindows()




# # 绘制文本
# img = cv2.imread("/Users/i7y/Desktop/1.png")

# cv2.putText(img, "Hello!", (200, 300), cv2.FONT_HERSHEY_SIMPLEX, 10, (0, 0, 255), 5, cv2.LINE_AA, False)

# cv2.imshow('win', img)
# key = cv2.waitKey(0)
# if key & 0xFF == ord('q'):
#     cv2.destroyAllWindows()