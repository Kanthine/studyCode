import cv2

# 创建窗口：WINDOW_NORMAL 窗口的尺寸可以根据内容被撑大
cv2.namedWindow("video", cv2.WINDOW_NORMAL)
# 获取视频设备
cap = cv2.VideoCapture(0)

#
# mog = cv2.bgsegm.createBackgroundSubtractorMOG()

# #
# mog = cv2.createBackgroundSubtractorMOG2(10)

# 
mog = cv2.bgsegm.createBackgroundSubtractorGMG()


# 判断摄像头是否打开
while cap.isOpened():
    # 从摄像头读取视频帧
    ret, frame = cap.read()
    if ret == True:
        fgmask = mog.apply(frame)
        # 将视频帧显示在窗口
        cv2.imshow("video", fgmask)
        # 等待键盘事件/ 20 毫秒一帧数据
        key = cv2.waitKey(10)
        if (key & 0xFF == ord('q')):
            break

# 释放资源
cap.release()
# 关闭窗口
cv2.destroyAllWindows()
