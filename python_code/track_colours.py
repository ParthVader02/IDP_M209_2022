import cv2
import numpy as np
import imutils
 
cam = cv2.VideoCapture("http://localhost:8081/stream/video.mjpeg")
kernel = np.ones((5 ,5), np.uint8)
 
while (True):
    __, frame = cam.read()
    crop = frame[240:550, 210:350]
    hsv_frame = cv2.cvtColor(crop, cv2.COLOR_BGR2HSV)
#
    # Blue color
    low_blue = np.array([100, 125, 4])
    high_blue = np.array([140, 255, 255])
    blue_mask = cv2.inRange(hsv_frame, low_blue, high_blue)
    blue = cv2.bitwise_and(crop, crop, mask=blue_mask)

    cnts1 = cv2.findContours(blue_mask,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
    cnts1 = imutils.grab_contours(cnts1)

    for c in cnts1:
        cX=0
        cY=0
        cv2.drawContours(crop,[c],-1,(0,255,0),3)
        # compute the center of the contour
        M = cv2.moments(c)
        if M["m00"] != 0:
            cX = int(M["m10"] / M["m00"])
            cY = int(M["m01"] / M["m00"])   
            
        cv2.circle(crop, (cX, cY), 7, (255, 255, 255), 1)
        print(cX)
        print(cY)

    cv2.imshow("crop", crop)
    cv2.imshow("Frame", frame)
    cv2.imshow("Blue", blue)

    key = cv2.waitKey(1)
    if key == 27:
        break