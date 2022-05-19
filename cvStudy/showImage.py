import cv2

# 创建一个窗口
cv2.namedWindow("win", cv2.WINDOW_NORMAL)
# 读取一张图片
img = cv2.imread("/Users/i7y/Desktop/1.png")
# 将图片展示到窗口
cv2.imshow("win", img)
# 等待键盘事件
key = cv2.waitKey(0)
if (key & 0xFF == ord('q')):
    cv2.destroyAllWindows()
