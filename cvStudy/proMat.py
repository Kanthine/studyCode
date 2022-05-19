import cv2

img = cv2.imread("/Users/i7y/Desktop/1.png")

# 通道分离
b, g, r = cv2.split(img)
# 修改通道的值
b[100:740, 100:580] = 255
# cv2.imshow('win', b)

# 通道合并
img2 = cv2.merge((b, g, r))
# 展示合并后的图像
cv2.imshow('win', img2)

key = cv2.waitKey(0)
if key & 0xFF == ord('q'):
    cv2.destroyAllWindows()