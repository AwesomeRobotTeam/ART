def get_crops(img, num=16):
    crop_imgs = list()
    height, width, depth = img.shape
    crop_height = height / num
    crop_width = width / num 
    for i in range(num):
        for j in range(num):
            crop_img = img[i*crop_height : ((i+1) * crop_height) , j * crop_width: ((j+1) * crop_width), :]
            crop_imgs.append(crop_img)
    return crop_imgs
