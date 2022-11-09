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

            # Convert to grayscale
            gray = cv2.cvtColor(crop, cv2.COLOR_BGR2GRAY)
            # Gaussian blur
            blur = cv2.GaussianBlur(gray,(5,5),0)
            # Color thresholding
            ret,thresh = cv2.threshold(blur,60,255,cv2.THRESH_BINARY_INV)
            # Find the contours of the frame
            contours,hierarchy = cv2.findContours(thresh.copy(), 1, cv2.CHAIN_APPROX_NONE)
            # Find the biggest contour (if detected)
            if len(contours) > 0:
                c = max(contours, key=cv2.contourArea)
                M = cv2.moments(c)
                cx = int(M['m10']/M['m00'])
                cy = int(M['m01']/M['m00'])
                cv2.line(crop,(cx,0),(cx,720),(255,0,0),1)
                cv2.line(crop,(0,cy),(1280,cy),(255,0,0),1)
                cv2.drawContours(crop, contours, -1, (0,255,0), 1)
                if cx >= 120:
                    print ("Turn Left!")
                if cx < 120 and cx > 50:
                    print ("On Track!")
                if cx <= 50:
                    print ("Turn Right")
            else:
                print("I don't see the line")
            #Display the resulting frame
            cv2.imshow('thresh',crop)
            #remove_tag = crop[y:y+h,x:x+w]
            #cv2.imshow('tag_remove', remove_tag)

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
