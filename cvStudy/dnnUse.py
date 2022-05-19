# 1、导入模型、创建神经网络
# 2、读取图片、转成张量
# 3、将张量输入到网络中、并进行预测
# 4、得到结果并显示

import cv2
from cv2 import dnn
import numpy 

# 1、导入模型、创建神经网络
config = "./model/bvlc_googlenet.prototxt"
model = "./model/bvlc_googlenet.caffemodel"
net = dnn.readNetFromCaffe(config, model)

# 2、读取图片、转成张量
img = cv2.imread("./img/lena.png")
blob = cv2.dnn.blobFromImage(img, 
							 1, 		 # 缩放因子
							 (224, 224), # 输入图像尺寸（固定参数）
							 (104, 117, 123)) # 平均差值（固定参数）

# 3、将张量输入到网络中、并进行预测
net.setInput(blob)
preds = net.forward()

# 读入类目

