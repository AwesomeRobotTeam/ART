import sys
import caffe
import numpy as np
import os
import argparse
import time
sys.path.append('../')
import detection as dt
import ImageProcessor as imgp

caffe_root = '/home/' + os.popen("whoami").read().strip('\n') +'/caffe/'
pwd = os.popen("pwd").read().strip('\n') + '/'
sys.path.insert(0, caffe_root + 'python')
batch_size = 256

def disp_preds(net, images, labels, k=5, name='ArtCifar10Net'):
    input_blob = net.blobs['data']
    for index in range(batch_size):
        transformed_image = transformer.preprocess('data', images[index])
        net.blobs['data'].data[index] = transformed_image
    output_labels = list()
    output = net.forward()
    for index in range(batch_size):
        probs = output['prob'][index]
        output_labels.insert(index, {'catagory':labels[probs.argmax()], 'probability':probs[probs.argmax()], 'index':index})
        
    return  output_labels

def get_transformer(mu):
    # create transformer for the input called 'data'
    transformer = caffe.io.Transformer({'data': net.blobs['data'].data.shape})
    transformer.set_transpose('data', (2,0,1))  # move image channels to outermost dimension
    transformer.set_mean('data', mu)            # subtract the dataset-mean value in each channel
    transformer.set_raw_scale('data', 255)      # rescale from [0, 1] to [0, 255]
    transformer.set_channel_swap('data', (2,1,0))  # swap channels from RGB to BGR
    return transformer

def print_recall(item_list, target):
    count = count_target(item_list, target)
    print 'Total : %d' % count
    print 'Recall : %d %%' % (count * 100 / len(item_list))

def count_target(item_list, target, row=16, col=16):
    count = 0
    for i in range(len(item_list)):
        if item_list[i]['catagory'] == target:
            count = count + 1
    return count       
 
if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Argument Checker') 
    parser.add_argument("-i", "--image", type=str, help="image path")
    parser.add_argument("-gpu", "--gpu", type=int, help="use gpu to forwarding the classification")
    parser.add_argument("-n", "--net", type=str, help="the path of net definition file", default='./cifar10_quick.prototxt')
    parser.add_argument("-w", "--weight", type=str, help="the path of trained weight file", default='./cifar10_quick_iter_4000.caffemodel')
    parser.add_argument("-l", "--label", type=str, help="the path of label file", default='./label.txt')
    parser.add_argument("-m", "--mean", type=str, help="the path of mean file", default='./mean.npy')
    parser.add_argument("-debug", "--debug_level", type=str, help="set debug level", default=0)
    args = parser.parse_args()
   
    # select computation device
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
    
    #load net definition
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
    transformer = get_transformer(mu) 
    
    if args.image:
        directories = [args.image+'Collage/test/cat/', args.image+'Collage/test/truck/', args.image+'Collage/test/automobile/']
    target = ['cat', 'truck', 'automobile']
    for index, directory in enumerate(directories):
        tStart = time.time()
        transformed_images = list()
        dirItemList = os.listdir(directory)
        for i in range(len(dirItemList)):
            input_image = caffe.io.load_image(pwd + directory + dirItemList[i])
            transformed_images.append(input_image)
        print 'Forwarding images in %s' % directory
        output = disp_preds(net, transformed_images, labels)
        tEnd = time.time()
        # show the forwarding time
    	print "It cost %f sec" % (tEnd - tStart) 
    
        if int(args.debug_level) > 0:
            # show the output tables
            dt.print_catagory_table(output)
            dt.print_probability_table(output)
        print_recall(output, target[index])
