#!/usr/bin/env python
import cv2
import sys
sys.path.append('/home/rche/ART/ROS_wrapper/challenge3/script/module/')
print 'Start caffe......'
import caffe
import numpy as np
import os
import argparse
import detection as dt
import time
import ImageProcessor as imgp
import forwarding as fwd
from laser import laser

caffe_root = '/home/' + os.popen("whoami").read().strip('\n') +'/caffe/'
pwd = os.popen("pwd").read().strip('\n') + '/'
sys.path.insert(0, caffe_root + 'python')
batch_size = 256

if __name__ == '__main__':    
    parser = argparse.ArgumentParser(description='Argument Checker') 
    parser.add_argument("-i", "--image", type=str, help="image path")
    parser.add_argument("-gpu", "--gpu", type=int, nargs='?', const=0 , help="use gpu to forwarding the classification")
    parser.add_argument("-n", "--net", type=str, help="the path of net definition file", default='./model/cifar10_predict.prototxt')
    parser.add_argument("-w", "--weight", type=str, help="the path of trained weight file", default='./model/cifar10_predict.caffemodel')
    parser.add_argument("-l", "--label", type=str, help="the path of label file", default='./model/label.txt')
    parser.add_argument("-m", "--mean", type=str, help="the path of mean file", default='./model/mean.npy')
    args = parser.parse_args()

    # select computing device
    if args.gpu >= 0: 
        caffe.set_device(args.gpu)
        caffe.set_mode_gpu()
    else:
        caffe.set_mode_cpu()

    print 'Loading......'
    
    # Load weights file
    if args.weight:
        weights = args.weight
        assert os.path.exists(weights)
    else:
        print 'should define the path of trained weight file'
        sys.exit()
    
    # load net definition
    if args.net:
        net_definition = args.net
        assert os.path.exists(net_definition)
        sys.stderr.close()
        os.close(2)
        net = caffe.Net(net_definition, weights, caffe.TRAIN)
        net.blobs['data'].reshape(batch_size, 3, 32, 32)
    else:
        print 'should define the path of network definition protobuf'
        sys_exit()
    
    # Load labels file
    if args.label:
        label_file = args.label
        assert os.path.exists(label_file)
        labels = list(np.loadtxt(label_file, str, delimiter='\t'))
    else:
        print 'should define the path of label file'
        sys_exit()

    # load the mean ImageNet image (as distributed with Caffe) for subtraction
    if args.mean:
        assert os.path.exists(args.mean)
        mu = np.load(args.mean)
        mu = mu.mean(1).mean(1)  # average over pixels to obtain the mean (BGR) pixel values
    else:
        print 'should define the path of image mean' 
        sys_exit()
    
    print 'Loading success......'
    transformer = fwd.get_transformer(net, mu)

    print 'Start training......pleae wait......'
    tStart = time.time()
    
    tEnd = time.time()
    # show the training time
    print "It cost %f sec" % (tEnd - tStart) 
