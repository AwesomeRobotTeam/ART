MODEL=quick
ITERNUM=10000
DEBUG_LEVEL=0
export GLOG_minloglevel=1
python recall_test.py -w "../cifar10_${MODEL}_iter_${ITERNUM}.caffemodel" \
            -n "../cifar10_${MODEL}.prototxt" \
            -m ../mean.npy \
            -i ../ \
            --debug_level ${DEBUG_LEVEL}  -l ../label.txt  
export GLOG_minloglevel=0
