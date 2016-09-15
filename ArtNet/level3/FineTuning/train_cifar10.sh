#!/usr/bin/env sh

set -e

TOOLS=~/caffe/build/tools
DBTYPE=leveldb
SRC_DIRECTORY=~/Collage/test/
DEST_DIRECTORY=./Collage/
POSITION=5
INTENSITY=100

#make the destination directory if it doesn't exist
if [ ! -d ${DEST_DIRECTORY} ]; then
  mkdir ${DEST_DIRECTORY}
fi

# transform image
if [ ! -d ${DEST_DIRECTORY}airplane/ ]; then
  mkdir ${DEST_DIRECTORY}airplane
  ./image_transform ${SRC_DIRECTORY}airplane/ ${DEST_DIRECTORY}airplane/ ${POSITION} ${INTENSITY}
fi
if [ ! -d ${DEST_DIRECTORY}automobile/ ]; then
  mkdir ${DEST_DIRECTORY}automobile
  ./image_transform ${SRC_DIRECTORY}automobile/ ${DEST_DIRECTORY}automobile/ ${POSITION} ${INTENSITY}
fi
if [ ! -d ${DEST_DIRECTORY}bird/ ]; then
  mkdir ${DEST_DIRECTORY}bird
  ./image_transform ${SRC_DIRECTORY}bird/ ${DEST_DIRECTORY}bird/ ${POSITION} ${INTENSITY}
fi
if [ ! -d ${DEST_DIRECTORY}cat/ ]; then
  mkdir ${DEST_DIRECTORY}cat
  ./image_transform ${SRC_DIRECTORY}cat/ ${DEST_DIRECTORY}cat/ ${POSITION} ${INTENSITY}
fi
if [ ! -d ${DEST_DIRECTORY}deer/ ]; then
  mkdir ${DEST_DIRECTORY}deer
  ./image_transform ${SRC_DIRECTORY}deer/ ${DEST_DIRECTORY}deer/ ${POSITION} ${INTENSITY}
fi
if [ ! -d ${DEST_DIRECTORY}dog/ ]; then
  mkdir ${DEST_DIRECTORY}dog
  ./image_transform ${SRC_DIRECTORY}dog/ ${DEST_DIRECTORY}dog/ ${POSITION} ${INTENSITY}
fi
if [ ! -d ${DEST_DIRECTORY}frog/ ]; then
  mkdir ${DEST_DIRECTORY}frog
  ./image_transform ${SRC_DIRECTORY}frog/ ${DEST_DIRECTORY}frog/ ${POSITION} ${INTENSITY}
fi
if [ ! -d ${DEST_DIRECTORY}horse/ ]; then
  mkdir ${DEST_DIRECTORY}horse
  ./image_transform ${SRC_DIRECTORY}horse/ ${DEST_DIRECTORY}horse/ ${POSITION} ${INTENSITY}
fi
if [ ! -d ${DEST_DIRECTORY}ship/ ]; then
  mkdir ${DEST_DIRECTORY}ship
  ./image_transform ${SRC_DIRECTORY}ship/ ${DEST_DIRECTORY}ship/ ${POSITION} ${INTENSITY}
fi
if [ ! -d ${DEST_DIRECTORY}truck/ ]; then
  mkdir ${DEST_DIRECTORY}truck
  ./image_transform ${SRC_DIRECTORY}truck/ ${DEST_DIRECTORY}truck/ ${POSITION} ${INTENSITY}
fi

# create train.txt and test.tx
python makelist.py ./Collage/ distort
python makelist.py ~/Collage/test/

# delete the directories which may cause the name conflict
rm -rf cifar10_train_distort_${DBTYPE}
rm -rf cifar10_test_distort_${DBTYPE}
rm -rf cifar10_train_${DBTYPE}
rm -rf cifar10_test_${DBTYPE}
# build training data to leveldb or lmdb
# create train data
$TOOLS/convert_imageset -shuffle -backend ${DBTYPE} ${DEST_DIRECTORY} train_distort.txt cifar10_train_distort_${DBTYPE}
$TOOLS/convert_imageset -shuffle -backend ${DBTYPE} ${SRC_DIRECTORY} train.txt cifar10_train_${DBTYPE}
# create test data
$TOOLS/convert_imageset -shuffle -backend ${DBTYPE} ${DEST_DIRECTORY} test_distort.txt cifar10_test_distort_${DBTYPE}
$TOOLS/convert_imageset -shuffle -backend ${DBTYPE} ${SRC_DIRECTORY} test.txt cifar10_test_${DBTYPE}

if [ ! -d "model/" ]; then
  mkdir model
fi

# start training
index=0
for ((index=0; index < 2; index++)); do
  $TOOLS/caffe train -solver ./cifar10_quick_solver_distort.prototxt -weights ./model/cifar10_predict_iter_4000.caffemodel -gpu 0 2>error_distort 
  echo training  ${index}  success
done
 $TOOLS/caffe train \
    -solver ./cifar10_quick_solver.prototxt \
    -weights ./model/cifar10_predict_iter_4000.caffemodel \
    -gpu 0 \
    2>error
  echo all training success
