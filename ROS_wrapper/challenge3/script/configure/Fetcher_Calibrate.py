# Example for Fetcher.cpp
import sys
import cv2
import fetcher
import signal

key = 1
def handler(signal, frame):
    global key
    key = 27

if len(sys.argv) < 3:
    print "need argument for video and blocksize"
    sys.exit()
cap = cv2.VideoCapture(int(sys.argv[1]))
myFetcher = fetcher.Fetcher()
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

# When everything done, release the capture
cap.release()
cv2.destroyAllWindows()
