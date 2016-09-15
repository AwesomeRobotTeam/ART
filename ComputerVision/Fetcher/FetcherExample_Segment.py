# Example for Fetcher.cpp

import cv2
import fetcher

cap = cv2.VideoCapture(0)

myFetcher = fetcher.Fetcher()

myFetcher.init("calibration_data.out")

while(True):
    # Capture frame-by-frame
    ret, frame = cap.read()

    if cv2.waitKey(1) & 0xFF == 27:
	break

    patch = myFetcher.segment(frame)

    cv2.imshow("Video Captured", frame)
    cv2.imshow("Patch Resized", patch)

# When everything done, release the capture
cap.release()
cv2.destroyAllWindows()
