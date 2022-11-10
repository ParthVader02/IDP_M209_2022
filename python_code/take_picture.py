import cv2

cam = cv2.VideoCapture("http://localhost:8081/stream/video.mjpeg")

result, image = cam.read()

if result:
    # showing result, it take frame name and image 
    # output
    cv2.imshow("test", image)
  
    # saving image in local storage
    cv2.imwrite("aruco_ref.png", image)
  
    # If keyboard interrupt occurs, destroy image 
    # window
    cv2.waitKey(0)
    cv2.destroyWindow("test")
  
# If captured image is corrupted, moving to else part
else:
    print("No image detected. Please! try again")