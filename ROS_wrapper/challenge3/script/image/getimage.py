import numpy as np
import cv2
import sys
import os
user = os.popen("whoami").read().strip('\n')
sys.path.append('/home/'+user+'/ART/ROS_wrapper/challenge3/script/module/')
import ImageProcessor as imgp
import SaveRealImage as sri
import fetcher

myFetcher = fetcher.Fetcher()
myFetcher.init("../configure/calibration_data.out")

def getSquareImage():
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

def getSquareImageAndCrops():    
    if len(sys.argv) > 1:
        camera = int(sys.argv[1])
    else:
        camera = 0
    cap = cv2.VideoCapture(camera)
    while(True):
        # Capture frame-by-frame
        ret, frame = cap.read()
        # Display the resulting frame
        patch = myFetcher.segment(frame) 
        cv2.imshow('patch', patch)
        cv2.imshow('frame', frame)
        key = cv2.waitKey(1) & 0xFF
        if key == ord(' '):
            imgp.getCrops(patch)
            images = list()
            images_name = sorted(os.listdir('/tmp/crops/'))
            for image_name in images_name:
                tmp = cv2.imread('/tmp/crops/' + image_name)
                images.append(cv2.imread('/tmp/crops/' + image_name))
            sri.saveImage(images)
            print "save crops"
        elif key == ord('q'):
            break

if __name__ == "__main__":
    getSquareImageAndCrops()
