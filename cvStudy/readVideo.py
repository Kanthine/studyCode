import cv2

# 创建窗口
cv2.namedWindow("video", cv2.WINDOW_NORMAL)
cv2.resizeWindow("video", 640, 480)
# 从视频文件中读取视频帧
cap = cv2.VideoCapture("/Users/i7y/Desktop/2.mov")

while cap.isOpened():
    ret, frame = cap.read()
    if ret == True:
        # 将视频帧显示在窗口
        cv2.imshow("video", frame)
        key = cv2.waitKey(1)
        if (key & 0xFF == ord('q')):
            break

# 释放资源
cap.release()
# 关闭窗口
cv2.destroyAllWindows()
