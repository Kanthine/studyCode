import cv2

# 创建窗口
cv2.namedWindow("video", cv2.WINDOW_NORMAL)
cv2.resizeWindow("video", 640, 480)
# 获取视频设备
cap = cv2.VideoCapture(0)

while cap.isOpened():
    # 从摄像头读取视频帧
    ret, frame = cap.read()
    if ret == True:
        # 将视频帧显示在窗口
        cv2.imshow("video", frame)
        # 等待键盘事件/ 20 毫秒一帧数据
        key = cv2.waitKey(20)
        if (key & 0xFF == ord('q')):
            break

# 释放资源
cap.release()
# 关闭窗口
cv2.destroyAllWindows()
