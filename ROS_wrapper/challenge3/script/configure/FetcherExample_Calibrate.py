# Example for Fetcher.cpp

import cv2
import fetcher
import sys 
import signal

if len(sys.argv) < 3:
    print "need argument for video and blocksize"
    sys.exit()

key = 1
def handler(signal, frame):
    global key
    key = 27

myFetcher = fetcher.Fetcher()

ptNumber = 0
def onMouse(event, x, y, flags, param):
    global ptNumber

    if event == cv2.EVENT_LBUTTONUP:
        myFetcher.setPt(x, y, ptNumber)
	ptNumber = ptNumber + 1

cv2.namedWindow("Video Captured")
cv2.setMouseCallback("Video Captured", onMouse)
cap = cv2.VideoCapture(int(sys.argv[1]))

signal.signal(signal.SIGTSTP, handler)

while(True):
    # Capture frame-by-frame
    ret, frame = cap.read()

    cv2.waitKey(1)

    # Our operations on the frame come here
    fileName = myFetcher.calibrate(frame, int(sys.argv[2]), 200, key)
    if not fileName == "":
	    print fileName
	    break

    cv2.imshow("Video Captured", frame)

# When everything done, release the capture
cap.release()
cv2.destroyAllWindows()
