import caffe

def disp_preds(net, images, labels, transformer, name='ArtNotMNISTNet'):
    k = 2
    input_blob = net.blobs['data']
    for index in range(len(images)):
        transformed_image = transformer.preprocess('data', images[index])
        net.blobs['data'].data[index] = transformed_image
    output_labels = list()
    output = net.forward()
    for index in range(len(images)):
        probs = output['prob'][index]
        top_k = (-probs).argsort()[:k]
        output_labels.insert(index, {'catagory':labels[top_k[0]], 
                                    'probability':probs[top_k[0]], 
                                    'catagory2':labels[top_k[1]], 
                                    'probability2':probs[top_k[1]],        
                                    'index':index})
    #    print output_labels[index]['catagory2'], labels[top_k[1]]
    return  output_labels

def get_transformer(net, mu):
    # create transformer for the input called 'data'
    transformer = caffe.io.Transformer({'data': net.blobs['data'].data.shape})
    transformer.set_transpose('data', (2,0,1))  # move image channels to outermost dimension
    transformer.set_mean('data', mu)            # subtract the dataset-mean value in each channel
    transformer.set_raw_scale('data', 255)      # rescale from [0, 1] to [0, 255]
    transformer.set_channel_swap('data', (2,1,0))  # swap channels from RGB to BGR
    return transformer
