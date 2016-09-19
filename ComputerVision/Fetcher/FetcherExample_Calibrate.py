# Example for Fetcher.cpp

import cv2
import fetcher

myFetcher = fetcher.Fetcher()

ptNumber = 0
def onMouse(event, x, y, flags, param):
    global ptNumber

    if event == cv2.EVENT_LBUTTONUP:
        myFetcher.setPt(x, y, ptNumber)
	ptNumber = ptNumber + 1

cv2.namedWindow("Video Captured")

cv2.setMouseCallback("Video Captured", onMouse)

cap = cv2.VideoCapture(0)
while(True):
    # Capture frame-by-frame
    ret, frame = cap.read()

    key = cv2.waitKey(1)

    # Our operations on the frame come here
    fileName = myFetcher.calibrate(frame, 5, 200, key)
    if not fileName == "":
	print fileName
	break

    cv2.imshow("Video Captured", frame)

# When everything done, release the capture
cap.release()
cv2.destroyAllWindows()
