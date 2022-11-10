import numpy as np
import cv2
import cv2.aruco as aruco
cap = cv2.VideoCapture("http://localhost:8081/stream/video.mjpeg")

def track(mtx, dist, num):
    while True:
        ret, frame = cap.read()
        # operations on the frame come here
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)  # Change grayscale
        aruco_dict = aruco.Dictionary_get(aruco.DICT_4X4_100)  # Use 4x4 dictionary to find markers
        parameters = aruco.DetectorParameters_create()  # Marker detection parameters
        # lists of ids and the corners beloning to each id seen in camera
        corners, ids, rejected_img_points = aruco.detectMarkers(gray, aruco_dict,
                                                                parameters=parameters)
        if np.all(ids is not None):  # If there are markers found by detector
            aruco.drawDetectedMarkers(frame, corners)  # Draw A square around the markers
            x,y,w,h = cv2.boundingRect(corners[0])
            crop=frame[y-num:y+h+num,x-num:x+w+num]
            cv2.imshow('crop', crop)
            print(x,y,w,h)
            pts2 = np.float32([[0,0],[200,0],[0,200],[200,200]])
            M = cv2.getPerspectiveTransform(corners[0],pts2)
            dst = cv2.warpPerspective(frame,M,(500,400))
            cv2.imshow('prospective_transf',dst)

        cv2.imshow('frame', frame)
        # Display the resulting frrame
        # Wait 3 miliseconds for an interaction. Check the key and do the corresponding job.
        key = cv2.waitKey(3) & 0xFF
        if key == ord('q'):  # Quit
            break
    
    # When everything done, release the capture
    cap.release()
    cv2.destroyAllWindows()

dist = np.array([-0.12118978, -0.0826376, -0.00406247, 0.00588043, 0.02394516])

mtx = np.array([[496.87906648, 0, 483.3363284 ],[0, 404.03355182 ,383.52212627],[0, 0, 1]])

num = 35

track(mtx, dist, num)
