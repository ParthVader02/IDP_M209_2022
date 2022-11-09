import cv2
import numpy as np
 
cam = cv2.VideoCapture("http://localhost:8081/stream/video.mjpeg")
kernel = np.ones((5 ,5), np.uint8)
 
while (True):
    __, frame = cam.read()
    hsv_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

    # Red color
    low_red = np.array([150, 75, 25])
    high_red = np.array([180, 255, 255])
    red_mask = cv2.inRange(hsv_frame, low_red, high_red)
    red = cv2.bitwise_and(frame, frame, mask=red_mask)#

    # Blue color
    low_blue = np.array([100, 150, 4])
    high_blue = np.array([140, 255, 255])
    blue_mask = cv2.inRange(hsv_frame, low_blue, high_blue)
    blue = cv2.bitwise_and(frame, frame, mask=blue_mask)
    # Green color   
    low_green = np.array([70, 50,4])
    high_green = np.array([110, 255, 255])
    green_mask = cv2.inRange(hsv_frame, low_green, high_green)
    green = cv2.bitwise_and(frame, frame, mask=green_mask)

    cv2.imshow("Frame", frame)
    cv2.imshow("Red", red)
    cv2.imshow("Blue", blue)
    cv2.imshow("Green", green)
    key = cv2.waitKey(1)
    if key == 27:
        break