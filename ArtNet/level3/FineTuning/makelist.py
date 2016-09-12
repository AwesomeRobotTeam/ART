import sys
import os
import random

images_root = './Collage/'
dirList = ['airplane/','automobile/','bird/','cat/','deer/','dog/','frog/','horse/','ship/','truck/']
trainSetName = 'train.txt'
testSetName = 'test.txt'
itemList = []

#read item in directories
print 'read items'
for i in range(len(dirList)):
  dirItemList = os.listdir(images_root + dirList[i])
  for j in range(len(dirItemList)):
    itemList.append(dirList[i] + dirItemList[j] + ' ' + str(i) + '\n')

#shuffling
print 'shuffling'
for i in range(3):
  for j in range(3):
    random.shuffle(itemList)

#distribute the data to files : 9/10 for training, 1/10 for testing 
totalCount = len(itemList)
testSetCount = totalCount / 10
#open files
print 'make testing file'
with open(testSetName, 'w') as f:
  for i in range(testSetCount):
    f.write(itemList[i])
print 'make training file'
with open(trainSetName, 'w') as f:
  for i in range(testSetCount, totalCount):
    f.write(itemList[i])
