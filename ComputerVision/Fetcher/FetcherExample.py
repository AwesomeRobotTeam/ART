# Example for Fetcher_v1.cpp, Fetcher_v2.cpp and Fetcher_v3.cpp

import cv2
import fetcher

cap = cv2.VideoCapture(0)

myFetcher = fetcher.Fetcher()

while(True):
    # Capture frame-by-frame
    ret, frame = cap.read()

    key = cv2.waitKey(1)

    # Our operations on the frame come here
    if myFetcher.calibrate(frame, key):
	break

while(True):
    # Capture frame-by-frame
    ret, frame = cap.read()

    if cv2.waitKey(1) & 0xFF == 27:
	break

    patch = myFetcher.segment(frame)

    # Based on patch image, do your segmentation and object classification here

# When everything done, release the capture
cap.release()
cv2.destroyAllWindows()
