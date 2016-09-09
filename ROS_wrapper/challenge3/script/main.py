#!/usr/bin/env python
import cv2
import sys
sys.path.append('/home/rche/ART/ROS_wrapper/challenge3/script/module/')
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
    parser.add_argument("-v", "--video", type=int, help="set video source", default=-1)
    parser.add_argument("-i", "--image", type=str, help="image path")
    parser.add_argument("-gpu", "--gpu", type=int, help="use gpu to forwarding the classification")
    parser.add_argument("-n", "--net", type=str, help="the path of net definition file", default='./model/cifar10_predict.prototxt')
    parser.add_argument("-w", "--weight", type=str, help="the path of trained weight file", default='./model/cifar10_predict.caffemodel')
    parser.add_argument("-l", "--label", type=str, help="the path of label file", default='./model/label.txt')
    parser.add_argument("-m", "--mean", type=str, help="the path of mean file", default='./model/mean.npy')
    parser.add_argument("-d", "--device", type=str, help="the device name")
    args = parser.parse_args()

    # select computing device
    if args.gpu >= 0: 
        caffe.set_device(args.gpu)
        caffe.set_mode_gpu()
    else:
        caffe.set_mode_cpu()

    # Load weights file
    if args.weight:
        weights = args.weight
    else:
        print 'should define the path of trained weight file'
        sys.exit()
    print weights
    assert os.path.exists(weights)

    # load net definition
    if args.net:
        net_definition = args.net
        print net_definition
        assert os.path.exists(net_definition)
        net = caffe.Net(net_definition, weights, caffe.TEST)
        net.blobs['data'].reshape(batch_size, 3, 32, 32)
    else:
        print 'should define the path of network definition protobuf'

    # Load labels file
    if args.label:
        label_file = args.label
        assert os.path.exists(label_file)
        labels = list(np.loadtxt(label_file, str, delimiter='\t'))
    else:
        print 'should define the path of label file'

    # load the mean ImageNet image (as distributed with Caffe) for subtraction
    if args.mean:
        assert os.path.exists(args.mean)
    mu = np.load(args.mean)
    mu = mu.mean(1).mean(1)  # average over pixels to obtain the mean (BGR) pixel values
    print 'mean-subtracted values:', zip('BGR', mu)
    transformer = fwd.get_transformer(net, mu)
    img_list = list()

    tStart = time.time()
    # select input source: video, image directory or a picture
    if args.video >= 0:
        cap = cv2.VideoCapture(args.video)		
        img_list = imgp.getRealtimeImage(cap)
        output = fwd.disp_preds(net, img_list, labels, transformer)
        cap.release()
        cv2.destroyAllWindows() 
    elif args.image:
        dirItemList = sorted(os.listdir(args.image))
        for i in range(batch_size):
            input_image = caffe.io.load_image(pwd + args.image + dirItemList[i])
            img_list.append(input_image)
        output = fwd.disp_preds(net, img_list, labels, transformer)
    else:
        image = cv2.imread('./collage.png')
        cv2.imshow('img', image)
        while cv2.waitKey(0) & 0xFF != ord('q'):
            continue
        cv2.destroyAllWindows()
        img_list = imgp.getCrops(image, 16)
        output = fwd.disp_preds(net, img_list, labels, transformer)
    tEnd = time.time()
    # show the forwarding time
    print "It cost %f sec" % (tEnd - tStart) 

    # show the output tables
    dt.print_catagory_table(output)
    dt.print_probability_table(output)

    # get targets coordinates 
    coordinates = dt.get_coordinates(output)

    # send the message to laser fort via ROS
    if args.device:
        laser = laser(10, True, 0, 0)
        for coordinate in coordinates:
            laser.shoot1(coordinate['X-Axis'], coordinate['Y-Axis'])
        laser.shutdown()
