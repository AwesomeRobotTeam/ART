# Example for Fetcher.cpp

import cv2
import fetcher

cap = cv2.VideoCapture(0)

myFetcher = fetcher.Fetcher()

while(True):
    # Capture frame-by-frame
    ret, frame = cap.read()

    key = cv2.waitKey(1)

    # Our operations on the frame come here
    fileName = myFetcher.calibrate(frame, 5, 200, key)
    if not fileName == "":
	print fileName
	break

# When everything done, release the capture
cap.release()
cv2.destroyAllWindows()
