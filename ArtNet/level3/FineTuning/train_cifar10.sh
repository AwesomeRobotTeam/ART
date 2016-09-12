#!/usr/bin/env sh

TOOLS=~/caffe/build/tools
DBTYPE=leveldb
SRC_DIRECTORY=~/Collage/test/
DEST_DIRECTORY=./Collage/
POSITION=5
INTENSITY=100

# transform image
./image_transform ${SRC_DIRECTORY}airplane/ ${DEST_DIRECTORY}airplane/ ${POSITION} ${INTENSITY}
./image_transform ${SRC_DIRECTORY}automobile/ ${DEST_DIRECTORY}automobile/ ${POSITION} ${INTENSITY}
./image_transform ${SRC_DIRECTORY}bird/ ${DEST_DIRECTORY}bird/ ${POSITION} ${INTENSITY}
./image_transform ${SRC_DIRECTORY}cat/ ${DEST_DIRECTORY}cat/ ${POSITION} ${INTENSITY}
./image_transform ${SRC_DIRECTORY}deer/ ${DEST_DIRECTORY}deer/ ${POSITION} ${INTENSITY}
./image_transform ${SRC_DIRECTORY}dog/ ${DEST_DIRECTORY}dog/ ${POSITION} ${INTENSITY}
./image_transform ${SRC_DIRECTORY}frog/ ${DEST_DIRECTORY}frog/ ${POSITION} ${INTENSITY}
./image_transform ${SRC_DIRECTORY}horse/ ${DEST_DIRECTORY}horse/ ${POSITION} ${INTENSITY}
./image_transform ${SRC_DIRECTORY}ship/ ${DEST_DIRECTORY}ship/ ${POSITION} ${INTENSITY}
./image_transform ${SRC_DIRECTORY}truck/ ${DEST_DIRECTORY}truck/ ${POSITION} ${INTENSITY}

# create train.txt and test.txt
python makelist.py 

# delete the directories which may cause the name conflict
rm -rf cifar10_train_${DBTYPE}
rm -rf cifar10_test_${DBTYPE}
# build training data to leveldb or lmdb
# create train data
$TOOLS/convert_imageset -shuffle -backend ${DBTYPE} ${DEST_DIRECTORY} train.txt cifar10_train_${DBTYPE}
# create test data
$TOOLS/convert_imageset -shuffle -backend ${DBTYPE} ${DEST_DIRECTORY} test.txt cifar10_test_${DBTYPE}

# start training
$TOOLS/caffe train \
  -solver ./cifar10_quick_solver.prototxt \
  -weights ./model/cifar10_predict.caffemodel \
  -gpu 0
