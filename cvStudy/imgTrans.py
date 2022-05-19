import cv2
import numpy



img = cv2.imread("/Users/i7y/Desktop/1.png")  # 原始图片
h, w, ch = img.shape

# 计算一个透视矩阵
srcPots = numpy.float32([[0,0], [100, 0], [0, 100], [100, 100]])
dstPots = numpy.float32([[30,30], [70, 30], [30, 70], [70, 70]])
cop = cv2.getPerspectiveTransform(srcPots, dstPots, cv2.DECOMP_LU)

# 透视变换
imgNew = cv2.warpPerspective(img, cop, (w, h))

cv2.namedWindow("win", cv2.WINDOW_AUTOSIZE)
while True:
    cv2.imshow("win", imgNew)
    key = cv2.waitKey(1) & 0xFF
    if key == ord('q'):
        break
cv2.destroyAllWindows






# img = cv2.imread("/Users/i7y/Desktop/1.png")  # 原始图片
# h, w, ch = img.shape

# # 平移矩阵
# move = numpy.float32([[1, 0, 100], [0, 1, 0]])
# # 旋转矩阵、缩放矩阵
# angle = cv2.getRotationMatrix2D((w / 2, h / 2), 45, 0.3)

# # 计算一个仿射矩阵
# srcPots = numpy.float32([[0,0], [10, 0], [0, 10]])
# dstPots = numpy.float32([[0,0], [0, 10], [10, 0]])
# cop = cv2.getAffineTransform(srcPots, dstPots)

# # 仿射变换
# imgNew = cv2.warpAffine(img, cop, 
#                         (w, h), 
#                         cv2.INTER_LINEAR, cv2.BORDER_CONSTANT, 0)

# cv2.namedWindow("win", cv2.WINDOW_AUTOSIZE)
# while True:
#     cv2.imshow("win", imgNew)
#     key = cv2.waitKey(1) & 0xFF
#     if key == ord('q'):
#         break
# cv2.destroyAllWindows






# img = cv2.imread("/Users/i7y/Desktop/1.png")  # 原始图片

# # imgNew = cv2.resize(img, (400, int(400 * img.shape[0] / img.shape[1])), 0, 0, cv2.INTER_AREA)
# # imgNew = cv2.resize(img, None, 1.5, 1.5, cv2.INTER_LINEAR)

# # imgNew = cv2.flip(img, 0)

# imgNew = cv2.rotate(img, cv2.ROTATE_90_CLOCKWISE)

# cv2.namedWindow("win", cv2.WINDOW_AUTOSIZE)
# while True:
#     cv2.imshow("win", imgNew)
#     key = cv2.waitKey(1) & 0xFF
#     if key == ord('q'):
#         break
# cv2.destroyAllWindows