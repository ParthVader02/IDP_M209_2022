import cv2
import numpy as np
 
cam = cv2.VideoCapture("http://localhost:8081/stream/video.mjpeg")
kernel = np.ones((5 ,5), np.uint8)
 
while (True):
    __, frame = cam.read()

    img_gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    img = cv2.medianBlur(img_gray,5)
    ret,th1 = cv2.threshold(img,127,255,cv2.THRESH_BINARY)
    th2 = cv2.adaptiveThreshold(img,255,cv2.ADAPTIVE_THRESH_MEAN_C,\
                cv2.THRESH_BINARY,11,2)
    th3 = cv2.adaptiveThreshold(img,255,cv2.ADAPTIVE_THRESH_GAUSSIAN_C,\
                cv2.THRESH_BINARY,11,2)

    edges = cv2.Canny(img, 100, 200)
    cv2.imshow("Canny",edges)
    cv2.imshow("Frame", img)
    cv2.imshow("globle", th1)
    cv2.imshow("adp_mean", th2)
    cv2.imshow("adp_Gaussian", th3)
    key = cv2.waitKey(1)
    if key == 27:
        break