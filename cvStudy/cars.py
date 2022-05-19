# 车辆统计
import cv2

def center(x, y, w, h):
    cx = x + w / 2
    cy = y + h / 2
    return cx, cy
 

min_w = 90
min_h = 90

# 检测线的高度要设置的足够合理
# 检测线太宽：车辆中心点可能多次在线范围内，多次计数
# 检测线偏上、偏下：刚进入界面的车辆可能就遇到检测线，有可能没被检测到 
line_height = 550
line_offet = 7

# 统计车辆数量
car_count = 0
# 存放车辆信息
cars = []

cap = cv2.VideoCapture("./img/video.mp4")

# 去除背景
# bgsubmog = cv2.createBackgroundSubtractorMOG2()
bgsubmog = cv2.bgsegm.createBackgroundSubtractorMOG()

# 卷积核
kernel_rect = cv2.getStructuringElement(cv2.MORPH_RECT, (7, 7))

while cap.isOpened():
    # 头读取视频帧
    ret, frame = cap.read()
    if ret == True:
        # 灰度
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        # 二值化
        #ret, bin = cv2.threshold(gray, 100, 255, cv2.THRESH_BINARY)
        # 去躁
        blur = cv2.GaussianBlur(gray, (3, 3), 5)
        # 去除背景
        mask = bgsubmog.apply(blur)
        # 腐蚀 ：去除小噪点
        erode = cv2.erode(mask, kernel_rect)
        # 膨胀 ：还原腐蚀后的小图像
        dilate = cv2.dilate(erode, kernel_rect, iterations= 3)
        # 闭操作：去除物体内部小方块
        close = cv2.morphologyEx(dilate, cv2.MORPH_CLOSE, kernel_rect)
        close = cv2.morphologyEx(close, cv2.MORPH_CLOSE, kernel_rect)
        # 轮廓查找
        contours, hierarchy = cv2.findContours(close, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

        finimg = frame

        #画一条检测线
        cv2.line(finimg, (10, line_height), (1200, line_height), (255, 255, 0), 3)
        
        # 遍历所有轮廓
        for(index, contour) in enumerate(contours):
            (x, y, w, h) = cv2.boundingRect(contour)
            # 过滤比较小的矩形
            isValid = (w >= min_w) and (h >= min_h)
            if (not isValid):
                continue
            cv2.rectangle(finimg, (x, y), (x + w, y + h), (0, 0, 255), 2)
            # 计算车辆中心点
            cpoint = center(x, y, w, h)
            cars.append(cpoint)
            cv2.circle(finimg, (int(cpoint[0]), int(cpoint[1])), 5, (0, 0, 255), -1)

            for (x, y) in cars:
                if( (y > line_height - line_offet) and (y < line_height + line_offet) ):
                    car_count += 1
                    cars.remove(cpoint)

        cv2.putText(finimg, "Cars count: " + str(car_count), (500, 60), cv2.FONT_HERSHEY_SIMPLEX, 2, (255, 255, 0), 5)
        # 将视频帧显示在窗口
        cv2.imshow("video", finimg)
        # 等待键盘事件/ 16 毫秒一帧数据
        key = cv2.waitKey(16)
        if (key & 0xFF == ord('q')):
            break

# 释放资源
cap.release()
# 关闭窗口
cv2.destroyAllWindows()