import cv2
import fetcher
import signal

global key
global patch
key = 1
def handler(signal, frame):
    global key
    key = 27
signal.signal(signal.SIGTSTP, handler)
myFetcher = fetcher.Fetcher()

def getFetcher(cap):
    global key
    global patch    
    while(True):
        # Capture frame-by-frame
        ret, frame = cap.read()
        cv2.waitKey(1)
        print key
        if myFetcher.calibrate(frame, key):
            break
    key = 1
    while(True):
        # Capture frame-by-frame
        ret, frame = cap.read() 
        cv2.waitKey(1)
        if key == 27:
            break
        patch = myFetcher.segment(frame) 
    cv2.destroyAllWindows()
    return patch

def getSquare(patch, method):
    height, width, depth = patch.shape
    if method == 'padding':
        #padding
        h_padding = (16 - (height % 16)) / 2
        w_padding = (16 - (width % 16)) / 2
        return cv2.copyMakeBorder(patch,h_padding,h_padding,w_padding,w_padding,cv2.BORDER_CONSTANT,value=[0,0,0])
    elif method == 'resize':
        length = min(heigth, width)
        return cv2.resize(patch, (length,length))

def getCrops(img, num=16):
    crop_imgs = list()
    height, width, depth = img.shape
    crop_height = height / num
    crop_width = width / num 
    for i in range(num):
        for j in range(num):
            crop_img = img[i*crop_height : ((i+1) * crop_height) , j * crop_width: ((j+1) * crop_width), :]
            crop_imgs.append(crop_img)
    return crop_imgs       
