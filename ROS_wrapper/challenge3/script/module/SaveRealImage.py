import os
import cv2
import time
import random

catagory = 10

image_root = "./test/"
catagory_dir = ['airplane/','automobile/','bird/','cat/','deer/','dog/','frog/','horse/','ship/','truck/']
catagory_name = ['airplane','automobile','bird','cat','deer','dog','frog','horse','ship','truck']
catagory_num = [23, 3, 23, 4, 20, 31, 40, 50, 59, 3]

def saveImage(img_list):
    count = 0 
    if not os.path.isdir(image_root):
        os.mkdir(image_root)
    for i in range(catagory):
        if not os.path.isdir(image_root + catagory_dir[i]):
            os.mkdir(image_root + catagory_dir[i])
        for index, img in enumerate(img_list[count:count+catagory_num[i]]):
            cv2.imwrite(image_root + catagory_dir[i] + catagory_name[i] + str(index)+ "_" + time.strftime("%d%H%M%S")+ ".jpg", img)  
        count = count + catagory_num[i]
