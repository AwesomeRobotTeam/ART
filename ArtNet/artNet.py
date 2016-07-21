import sys
import caffe
import numpy as np
from pylab import *
import os
from caffe.proto import caffe_pb2
import time
import cv2
import argparse

caffe_root = '/home/' + os.popen("whoami").read().strip('\n') +'/caffe/'
sys.path.insert(0, caffe_root + 'python')

def disp_preds(net, image, labels, k=5, name='ArtNotMNISTNet'):
    input_blob = net.blobs['data']
    net.blobs['data'].data[0, ...] = image
    probs = net.forward()['prob'][0]
    #print 'predicted class is : ', prob.argmax()
    top_k = (-probs).argsort()[:k]
    print 'top %d predicted %s labels =' % (k, name)
    print '\n'.join('\t(%d) %5.2f%% %s' % (i+1, 100*probs[p], labels[p])
                    for i, p in enumerate(top_k))

def run_image_recognition(cap, transformer):
    while 1:
        ret, frame = cap.read()
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        cv2.imshow('frame',gray)
        # Our operations on the frame come here
        transformed_image = transformer.preprocess('data', frame)
        disp_preds(net, transformed_image, labels)    
        # Display the resulting frame
        if cv2.waitKey(2000) & 0xFF == ord('q'):
            break

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Argument Checker') 
    parser.add_argument("-v", "--video", type=int, help="set video source", default=0)
    parser.add_argument("-gpu", "--gpu", type=int, help="use gpu to forwarding the classification")
    parser.add_argument("-n", "--net", type=str, help="the path of net definition file")
    parser.add_argument("-w", "--weight", type=str, help="the path of trained weight file")
    parser.add_argument("-l", "--label", type=str, help="the path of label file")
    parser.add_argument("-m", "--mean", type=str, help="the path of mean file", default='python/caffe/imagenet/ilsvrc_2012_mean.npy')
    args = parser.parse_args()
   
    if args.gpu: 
        caffe.set_device(args.gpu)
        caffe.set_mode_gpu()
    else:
        caffe.set_mode_cpu()

    # Load weights file
    if args.weight:
        weights = caffe_root + args.weight
    else:
        print 'should define the path of trained weight file'
        sys.exit()
    print weights
    assert os.path.exists(weights)
    if args.net:
        net_definition = caffe_root + args.net
        print net_definition
        assert os.path.exists(net_definition)
        net = caffe.Net(net_definition, weights, caffe.TEST)
    else:
        print 'should define the path of network definition protobuf'
    # Load labels file
    if args.label:
        label_file = caffe_root + args.label
        assert os.path.exists(label_file)
        labels = list(np.loadtxt(label_file, str, delimiter='\t'))
    else:
        print 'should define the path of label file'
    
    # load the mean ImageNet image (as distributed with Caffe) for subtraction
    if args.mean:
        assert os.path.exists(caffe_root + args.mean)
    mu = np.load(caffe_root + args.mean)
    mu = mu.mean(1).mean(1)  # average over pixels to obtain the mean (BGR) pixel values
    print 'mean-subtracted values:', zip('BGR', mu)
    # create transformer for the input called 'data'
    transformer = caffe.io.Transformer({'data': net.blobs['data'].data.shape})
    transformer.set_transpose('data', (2,0,1))  # move image channels to outermost dimension
    transformer.set_mean('data', mu)            # subtract the dataset-mean value in each channel
    transformer.set_raw_scale('data', 255)      # rescale from [0, 1] to [0, 255]
    transformer.set_channel_swap('data', (2,1,0))  # swap channels from RGB to BGR

    cap = cv2.VideoCapture(args.video)
    run_image_recognition(cap,transformer)
