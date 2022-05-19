import numpy
import cv2

# 创建矩阵

# 通过 Array 定义矩阵
a = numpy.array([1, 2, 3]) 
b = numpy.array([[1, 2, 3], [4, 5, 6], [7, 8, 9]])
print(a)
print(b)

# 定义 zeros 矩阵: (行数, 列数, 通道数)
c = numpy.zeros((480, 640, 3), numpy.uint8)

# 定义 ones 矩阵: 
d = numpy.ones((480, 640, 3), numpy.uint8)

# 定义 full 矩阵:
e = numpy.full((480, 640, 3), 100, numpy.uint8)

# 定义单位矩阵
f = numpy.identity(4)
print(f)

# 定义单位矩阵(长方形)
# g = numpy.eye(5)
# g = numpy.eye(4, 6)
g = numpy.eye(4, 6, k = 1)
print(g)


# 检索与赋值

# img = numpy.zeros((480, 640, 3), numpy.uint8)

# count = 0
# while count < 480 :
#     if count < 255 :
#         # 索引全通道 [y, x]
#         img[count, count] = count
#     else :
#         # 索引某个通道 [y, x, channel]
#         img[count, count, 2] = count - 255
#     count = count + 1

# cv2.imshow('win', img)
# key = cv2.waitKey(0)
# if key & 0xFF == ord('q'):
#     cv2.destroyAllWindows()



# ROI
img = cv2.imread("/Users/i7y/Desktop/1.png")
# 获取图像的某块区域
roi = img[100:740, 200:680]
# 修改该区域的所有像素
# roi[:, :] = [0, 0, 255]
roi[:] = [255, 0, 0]
# 修改该区域的某部分像素
# roi[30:60, 50:80] = [0, 255, 0]
cv2.imshow('win', roi)
key = cv2.waitKey(0)
if key & 0xFF == ord('q'):
    cv2.destroyAllWindows()