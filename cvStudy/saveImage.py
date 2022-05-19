import cv2

cv2.namedWindow("win",cv2.WINDOW_NORMAL)
# 读取一张图片
img = cv2.imread("/Users/i7y/Desktop/1.png")

while True:
    # 展示一张图片
    cv2.imshow("img",img )
    key = cv2.waitKey(0)
    if (key & 0xFF == ord('q')):
        break
    elif (key & 0xFF == ord('s')):
        #保存一张图片
        cv2.imwrite("/Users/i7y/Desktop/1_1.png", img)
        
cv2.destroyAllWindows()
