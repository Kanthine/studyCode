import cv2

# 创建 VideoWriter 写入多媒体文件
fource = cv2.VideoWriter_fourcc(*'MJPG')
# 注意：(1280, 720) 是摄像头的分辨率
vw = cv2.VideoWriter('../out.mp4', fource, 25, (1280, 720))

# 创建窗口：WINDOW_NORMAL 窗口的尺寸可以根据内容被撑大
cv2.namedWindow("video", cv2.WINDOW_NORMAL)
# 虽然设置窗口 640x480，但可能被内容改变为更大的尺寸
cv2.resizeWindow("video", 640, 480)
# 获取视频设备
cap = cv2.VideoCapture(0)
# 判断摄像头是否打开
while cap.isOpened():
    # 从摄像头读取视频帧
    ret, frame = cap.read()
    if ret == True:
        # 将视频帧显示在窗口
        cv2.imshow("video", frame)
        # 有可能被撑大，再次设置一次
        cv2.resizeWindow("video", 640, 480)
        # 写数据
        vw.write(frame)
        # 等待键盘事件/ 20 毫秒一帧数据
        key = cv2.waitKey(20)
        if (key & 0xFF == ord('q')):
            break

# 释放资源
cap.release()
vw.release()
# 关闭窗口
cv2.destroyAllWindows()
