import sys
import os
import cv2
sys.path.append('/home/'+os.popen("whoami").read().strip('\n')+'/ART/ROS_wrapper/challenge3/script/module/')

from laser import laser
import fetcher

# laser config
laser = laser()

x, y = laser.controlLaser()
if (not x == 0) and (not y == 0):
    print "[Error]The point is not at center."
    sys_exit()
else:
    for i in range(2):
        laser.calculateSteps()


#root.mainloop()
#with open('steps.config', 'w') as fp:
#    fp.write("")

# image fetcher configure
#cap = cv2.VideoCapture(0)
#myFetcher = fetcher.Fetcher()
#
#while(True):
#    # Capture frame-by-frame
#    ret, frame = cap.read()
#
#    key = cv2.waitKey(1)
#
#    # Our operations on the frame come here
#    fileName = myFetcher.calibrate(frame, 5, 200, key)
#    if not fileName == "":
#        print fileName
#        break
#
## When everything done, release the capture
#cap.release()
#cv2.destroyAllWindows() 
