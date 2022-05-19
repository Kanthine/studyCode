# 图像分割·GrabCut
import cv2
import numpy

class App:
    flag_left = False
    start_x = 0
    start_y = 0
    rect = (0, 0, 0, 0)

    def onmouse(self, event, x, y, flags, param):
        if event == cv2.EVENT_LBUTTONDOWN:
            self.flag_left = True
            self.start_x = x
            self.start_y = y
            print("左键按下")
        elif event == cv2.EVENT_LBUTTONUP:
            print("左键抬起")
            self.flag_left = False
            self.img = self.img_ori.copy()
            self.rect = (min(self.start_x, x), min(self.start_y, y), 
                         abs(x - self.start_x), abs(self.start_y - y))
            
            cv2.rectangle(self.img, 
                          (self.start_x, self.start_y), 
                          (x, y), 
                          (0, 0, 255), 3)
        elif event == cv2.EVENT_MOUSEMOVE:
            if self.flag_left == True:
                self.img = self.img_ori.copy()
                cv2.rectangle(self.img, 
                             (self.start_x, self.start_y), 
                             (x, y), 
                             (255, 0, 0), 3)

    def run(self):
        cv2.namedWindow("input")
        cv2.setMouseCallback("input", self.onmouse)
        self.img = cv2.imread("./img/lena.png")
        self.img_ori = self.img.copy()
        self.mask = numpy.zeros(self.img.shape[:2], numpy.uint8)
        self.output = numpy.zeros(self.img.shape, numpy.uint8)

        while True:
            cv2.imshow("input", self.img)
            cv2.imshow("output", self.output)
            key = cv2.waitKey(1) & 0xFF
            if key == ord('q'):
                break
            elif key == ord('g'):
                bgdModel = numpy.zeros((1, 65), numpy.float64)
                fgdModel = numpy.zeros((1, 65), numpy.float64)
                cv2.grabCut(self.img_ori, self.mask, self.rect, bgdModel, fgdModel, 1, cv2.GC_INIT_WITH_RECT)
                mask2 = numpy.where((self.mask == cv2.GC_FGD) | (self.mask == cv2.GC_PR_FGD) ,255 ,0).astype('uint8')
                self.output = cv2.bitwise_and(self.img_ori, self.img_ori, mask= mask2)

App().run() 