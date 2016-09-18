import os
import sys

if len(sys.argv) == 1:
    images_root = './test/'
else:
    image_root = sys.argv[1]

dirList = ['airplane/','automobile/','bird/','cat/','deer/','dog/','frog/','horse/','ship/','truck/']
prefix = ['airplane','automobile','bird','cat','deer','dog','frog','horse','ship','truck']

#rename items in directories
print 'rename items'
for i in range(len(dirList)):
  itemList = list()
  dirItemList = os.listdir(images_root + dirList[i])
  for j in range(len(dirItemList)):
    src = images_root + dirList[i] + dirItemList[j]
    dst = images_root + dirList[i] +prefix[i]+str(j)+'.png' 
    os.rename(src, dst)
