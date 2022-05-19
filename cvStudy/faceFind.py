# 角点检测 
import cv2
import numpy

img = cv2.imread("./img/lena.png")
# 灰度化
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

#第一步，创建Haar级联器
facer = cv2.CascadeClassifier('./haarcascades/haarcascade_frontalface_default.xml')
eye = cv2.CascadeClassifier('./haarcascades/haarcascade_eye.xml')
mouth = cv2.CascadeClassifier('./haarcascades/haarcascade_mcs_mouth.xml')
nose = cv2.CascadeClassifier('./haarcascades/haarcascade_mcs_nose.xml')

#第二步，导入人脸识别的图片并将其灰度化
#检测出的人脸上再检测眼睛
faces = facer.detectMultiScale(gray, 1.1, 3)
i = 0
j = 0
for (x,y,w,h) in faces:
    cv2.rectangle(img, (x, y), (x+w, y+h), (0, 0, 255), 2)
    roi_img = img[y:y+h, x:x+w]
    eyes = eye.detectMultiScale(roi_img, 1.1, 3)
    for (x,y,w,h) in eyes:
        cv2.rectangle(roi_img, (x, y), (x+w, y+h), (0, 255, 0), 2)
        roi_eye=roi_img[y:y+h, x:x+w]
        eyename = 'eye' + str(j)
        j = j+1 
        cv2.imshow(eyename, roi_eye)

    i = i+1
    winname = 'face' + str(i)
    cv2.imshow(winname, roi_img)

while True:
    cv2.imshow("org", img)
    key = cv2.waitKey(1) & 0xFF
    if key == ord('q'):
        break
cv2.destroyAllWindows