# 图像分割·分水岭法
from dis import dis
import cv2
from matplotlib import pyplot
import numpy

img = cv2.imread("./img/coins.png")  
img2 = cv2.imread("./img/coins.png")  
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

# 自适应阀值大小
ret, thresh = cv2.threshold(gray, 0, 255, cv2.THRESH_BINARY_INV + cv2.THRESH_OTSU)

# 开运算
kernel_rect = cv2.getStructuringElement(cv2.MORPH_RECT, (3, 3))
open = cv2.morphologyEx(thresh, cv2.MORPH_OPEN, kernel_rect, iterations=2)

# 膨胀
bg = cv2.dilate(open, kernel_rect, iterations=1)

# 获取前景物体
dist = cv2.distanceTransform(open, cv2.DIST_L2, 5)
# 缩小后的前景
ret, fg = cv2.threshold(dist, 0.7 * dist.max() , 255, cv2.THRESH_BINARY)
fg = numpy.uint8(fg)

# 获取未知区域
unknow = cv2.subtract(bg, fg)

# 创建联通域
ret, marker = cv2.connectedComponents(fg)
marker += 1
marker[unknow == 255] = 0

# 图像分割
result = cv2.watershed(img, marker)
img[result == -1] = (0, 0, 255)

while True: 
    cv2.imshow("win", img2)
    cv2.imshow("img", img)
    cv2.imshow("bg", bg)
    cv2.imshow("fg", fg)
    cv2.imshow("unknow", unknow)

    # 展示灰度化的梯度变化
    pyplot.imshow(dist, cmap='gray')
    pyplot.show()

    key = cv2.waitKey(1) & 0xFF
    if key == ord('q'):
        break
cv2.destroyAllWindows
