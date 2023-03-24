import cv2
import numpy as np
import warnings

warnings.filterwarnings("ignore")

cap = cv2.VideoCapture(0)

while True:
    ret, frame = cap.read()

    if ret == True:
        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
       
        # define range of yellow color in HSV
        lower_yellow = np.array([20, 0, 0])
        upper_yellow = np.array([40, 255, 255])
       
        # define range of green color in HSV
        lower_green = np.array([36, 25, 25])
        upper_green = np.array([70, 255,255])
       
        # define range of red color in HSV
        lower_red = np.array([0, 70, 50])
        upper_red= np.array([10, 255, 255])
       
        # Threshold the HSV image to get only green, yellow, red colors
        green_mask = cv2.inRange(hsv, lower_green, upper_green)
        yellow_mask = cv2.inRange(hsv, lower_yellow, upper_yellow)
        red_mask = cv2.inRange(hsv, lower_red, upper_red)
       
       
        res_yellow = cv2.bitwise_and(frame, frame, mask=yellow_mask)
        res_green = cv2.bitwise_and(frame, frame, mask=green_mask)
        res_red = cv2.bitwise_and(frame, frame, mask=red_mask)
       

        img_yellow = cv2.medianBlur(res_yellow, 5)
        ccimg_yellow = cv2.cvtColor(img_yellow, cv2.COLOR_HSV2BGR)
        cimg_yellow = cv2.cvtColor(ccimg_yellow, cv2.COLOR_BGR2GRAY)
        circles_yellow = cv2.HoughCircles(cimg_yellow, cv2.HOUGH_GRADIENT, 1, 20, param1=50, param2=30, minRadius=3, maxRadius=30)
       
        img_green = cv2.medianBlur(res_green, 5)
        ccimg_green = cv2.cvtColor(img_green, cv2.COLOR_HSV2BGR)
        cimg_green = cv2.cvtColor(ccimg_green, cv2.COLOR_BGR2GRAY)
        circles_green = cv2.HoughCircles(cimg_green, cv2.HOUGH_GRADIENT, 1, 20, param1=50, param2=30, minRadius=3, maxRadius=30)
       
       
        img_red = cv2.medianBlur(res_red, 5)
        ccimg_red = cv2.cvtColor(img_red, cv2.COLOR_HSV2BGR)
        cimg_red = cv2.cvtColor(ccimg_red, cv2.COLOR_BGR2GRAY)
        circles_red = cv2.HoughCircles(cimg_red, cv2.HOUGH_GRADIENT, 1, 20, param1=50, param2=30, minRadius=3, maxRadius=30)
       
       
        if circles_yellow is not None:
            print("yellow: ",circles_yellow)
       
        if circles_red is not None:
            print("red: ",circles_red)
       
        if circles_green is not None:
            print("green: ",circles_green)
           

        cv2.imshow('detected circles', cimg_yellow)
        cv2.imshow('res', res_yellow)


    if cv2.waitKey(1) & 0xFF == ord("q"):
        break

cv2.destroyAllWindows()
