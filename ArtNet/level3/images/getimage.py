import numpy as np
import cv2
import sys

if len(sys.argv) > 1:
    camera = int(sys.argv[1])
else:
    camera = 0
cap = cv2.VideoCapture(camera)
index = 0

while(True):
    # Capture frame-by-frame
    ret, frame = cap.read()
    frame = frame[:,80:560,:]
    # Display the resulting frame
    cv2.imshow('frame', frame)
    key = cv2.waitKey(1) & 0xFF
    if key == ord(' '):
        cv2.imwrite('img_'+str(index)+".jpg", frame)
        print "write image"
        index = index + 1
    elif key == ord('q'):
        break

# When everything done, release the capture
cap.release()
cv2.destroyAllWindows()
