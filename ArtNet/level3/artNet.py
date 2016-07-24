import sys
import caffe
import numpy as np
import os
import cv2
import argparse
import detection as dt

caffe_root = '/home/' + os.popen("whoami").read().strip('\n') +'/caffe/'
pwd = os.popen("pwd").read().strip('\n') + '/'
sys.path.insert(0, caffe_root + 'python')

def disp_preds(net, images, labels, k=5, name='ArtNotMNISTNet'):
    input_blob = net.blobs['data']
    for index, image in enumerate(images):
        transformed_image = transformer.preprocess('data', image)
        net.blobs['data'].data[index] = transformed_image
    output_labels = list()
    output = net.forward()
    for index in range(len(images)):
        probs = output['prob'][index]
        top_k = (-probs).argsort()[:k]
        print 'top %d predicted %s labels =' % (k, name)
        print '\n'.join('\t(%d) %5.2f%% %s' % (i+1, 100*probs[p], labels[p])
                       for i, p in enumerate(top_k))
        output_labels.insert(index, {'catagory':labels[probs.argmax()], 'probability':probs[probs.argmax()]})
        
    return  output_labels

def run_real_recognition(cap, transformer):
    while 1:
        ret, frame = cap.read()
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        cv2.imshow('frame',gray)
        # Our operations on the frame come here 
        disp_preds(net, frame, labels)    
        # Display the resulting frame
        if cv2.waitKey(2000) & 0xFF == ord('q'):
            break

def get_transformer(mu):
    # create transformer for the input called 'data'
    transformer = caffe.io.Transformer({'data': net.blobs['data'].data.shape})
    transformer.set_transpose('data', (2,0,1))  # move image channels to outermost dimension
    transformer.set_mean('data', mu)            # subtract the dataset-mean value in each channel
    transformer.set_raw_scale('data', 255)      # rescale from [0, 1] to [0, 255]
    transformer.set_channel_swap('data', (2,1,0))  # swap channels from RGB to BGR
    return transformer

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Argument Checker') 
    parser.add_argument("-v", "--video", type=int, help="set video source", default=-1)
    parser.add_argument("-i", "--image", type=str, help="image path")
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
    warning_message = '\nuse -h to watch more description'
    # Load weights file
    if args.weight:
        weights = pwd + args.weight
    else:
        print 'should define the path of trained weight file' + warning_message
        sys.exit()
    print weights
    assert os.path.exists(weights)
    #load net definition
    if args.net:
        net_definition = pwd + args.net
        print net_definition
        assert os.path.exists(net_definition)
        net = caffe.Net(net_definition, weights, caffe.TEST)
        net.blobs['data'].reshape(50, 3, 32, 32)
    else:
        print 'should define the path of network definition protobuf' + warning_message
    # Load labels file
    if args.label:
        label_file = pwd + args.label
        assert os.path.exists(label_file)
        labels = list(np.loadtxt(label_file, str, delimiter='\t'))
    else:
        print 'should define the path of label file' + warning_message
    
    # load the mean ImageNet image (as distributed with Caffe) for subtraction
    if args.mean:
        assert os.path.exists(caffe_root + args.mean)
    mu = np.load(caffe_root + args.mean)
    mu = mu.mean(1).mean(1)  # average over pixels to obtain the mean (BGR) pixel values
    print 'mean-subtracted values:', zip('BGR', mu)
    transformer = get_transformer(mu)
    transformed_images = list()
    if args.video >= 0:
        cap = cv2.VideoCapture(args.video)
        run_realtime_recognition(cap,transformer)    
    elif args.image:
        dirItemList = os.listdir(pwd + args.image)
        for i in range(len(dirItemList)):
            input_image = caffe.io.load_image(pwd + args.image + dirItemList[i])
            transformed_images.append(input_image)
        output = disp_preds(net, transformed_images, labels)
        dt.print_catagory_table(output)
        dt.print_probability_table(output)
        dt.get_coordinates(output)
