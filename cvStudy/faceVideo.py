# 角点检测 
import cv2
import numpy

#第一步，创建Haar级联器
facer = cv2.CascadeClassifier('./haarcascades/haarcascade_frontalface_default.xml')
eye = cv2.CascadeClassifier('./haarcascades/haarcascade_eye.xml')
mouth = cv2.CascadeClassifier('./haarcascades/haarcascade_mcs_mouth.xml')
nose = cv2.CascadeClassifier('./haarcascades/haarcascade_mcs_nose.xml')

# 获取视频设备
cap = cv2.VideoCapture(0)

# 判断摄像头是否打开
while cap.isOpened():
    # 从摄像头读取视频帧
    ret, frame = cap.read()
    if ret == True:
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

        #第二步，导入人脸识别的图片并将其灰度化
        faces = facer.detectMultiScale(gray, 1.1, 3)
        for (x,y,w,h) in faces:
            cv2.rectangle(frame, (x, y), (x+w, y+h), (0, 0, 255), 2)
        
        # 将视频帧显示在窗口
        cv2.imshow("video", frame)
        # 等待键盘事件/ 20 毫秒一帧数据
        key = cv2.waitKey(10)
        if (key & 0xFF == ord('q')):
            break

# 释放资源
cap.release()
# 关闭窗口