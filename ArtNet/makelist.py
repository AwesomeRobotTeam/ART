import sys
import os
import random

images_root = '/home/ubuntu/sd_space/notmnist/notMNIST_large/'
dirList = ['A/','B/','C/','D/','E/','F/','G/','H/','I/','J/']
trainSetName = 'train.txt'
testSetName = 'test.txt'
itemList = []
#read item in directories
print 'read items'
for i in range(len(dirList)):
  dirItemList = os.listdir(images_root + dirList[i])
  for j in range(len(dirItemList)):
    itemList.append(dirItemList[j] + ' ' + str(i) + '\n')

#shuffling
print 'shuffling'
for i in range(3):
  for j in range(3):
    random.shuffle(itemList)

#distribute the data to files
totalCount = len(itemList) / 50
testSetCount = totalCount / 4
#open files
print 'make testing file'
with open(testSetName, 'w') as f:
  for i in range(testSetCount):
    f.write(itemList[i])
print 'make training file'
with open(trainSetName, 'w') as f:
  for i in range(testSetCount, totalCount):
    f.write(itemList[i])
