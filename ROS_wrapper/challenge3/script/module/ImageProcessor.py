import cv2
import fetcher
import signal
import os
import caffe

myFetcher = fetcher.Fetcher()
myFetcher.init("./configure/calibration_data.out")

def getRealtimeImage(cap, autostretch=False):
    if autostretch:
        patch = getStretch(cap)
    else:
        patch = onlySquare(cap)
    cv2.imwrite("patch.jpg", patch)
    #padding or resize
    if not patch.shape == (512, 512, 3):
        patch = convert(patch,'resize')
    #get crops
    img_list = getCrops(patch, 16)
    return img_list

def convert(patch, method):
    height, width, depth = patch.shape
    if method == 'padding':
        #padding
        h_padding = (16 - (height % 16)) / 2
        w_padding = (16 - (width % 16)) / 2
        return cv2.copyMakeBorder(patch,h_padding,h_padding,w_padding,w_padding,cv2.BORDER_CONSTANT,value=[0,0,0])
    elif method == 'resize':
        length = 512
        return cv2.resize(patch, (length,length))

def getStretch(cap):
    while True:
        ret, frame = cap.read()
        patch = myFetcher.segment(frame)
        cv2.imshow('img',patch)
        cv2.imshow('square', cv2.resize(frame[:,80:560,:], (512,512)))
        if cv2.waitKey(1) & 0xFF == ord('q'):  
            return patch


def onlySquare(cap):
    while True:
        ret, frame = cap.read()
        frame = frame[:,80:560,:]
        cv2.imshow('img',frame)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            return frame  

def get2digits(num):
    if num <= 9:
        return '0'+str(num)
    else:
        return str(num)

def getCrops(img, num=16):
    crop_imgs = list()
    height, width, depth = img.shape
    crop_height = height / num
    crop_width = width / num
    if not os.path.isdir('/tmp/crops/'):
        os.mkdir('/tmp/crops/')
    for i in range(num):
        for j in range(num):
            crop_img = img[i*crop_height : ((i+1) * crop_height) , j * crop_width: ((j+1) * crop_width), :]
            cv2.imwrite('/tmp/crops/img'+get2digits(i)+get2digits(j)+'.jpg', crop_img)    
    dirItemList = sorted(os.listdir('/tmp/crops/'))
    for i in range(len(dirItemList)):
        input_image = caffe.io.load_image('/tmp/crops/' + dirItemList[i])
        crop_imgs.append(input_image)
    return crop_imgs       
