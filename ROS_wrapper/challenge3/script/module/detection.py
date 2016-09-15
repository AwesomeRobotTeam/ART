from termcolor import colored

row = 16
col = 16
threshold = 0.99 #the probability threshold

def print_catagory_table(item_list,targetlist=['cat','truck','automobile']):
    print "Catagory table"
    k = 0
    for i in range(row):
        for j in range(col):
            if (i*col + j) < len(item_list):
                if item_list[k]['catagory'] in targetlist:
                        if item_list[k]['probability'] > threshold:   
                            print colored("|%6.6s" % item_list[k]['catagory'], 'red') ,
                        else:
                            print colored("|%6.6s" % item_list[k]['catagory'], 'green') ,    
                else:
                    print ("|%6.6s" % item_list[k]['catagory']) ,
            else:
                print ("|%6.6s" % 'x') ,
            k = k + 1
        print "|"

def print_probability_table(item_list, targetlist=['cat','truck','automobile']):
    print "Probability table"
    k = 0
    for i in range(row):
        for j in range(col):
            if (i*col + j) < len(item_list):
                if item_list[k]['catagory'] in targetlist:
                    if item_list[k]['probability'] > threshold:
                        print colored("|%6.6s" % item_list[k]['probability'], 'red') ,
                    else:
                        print colored("|%6.6s" % item_list[k]['probability'], 'green') ,
                else:
                    print ("|%6.6s" % item_list[k]['probability']) ,
            else:
                print ("|%6.6s" % 'x') ,
            k = k + 1
        print "|"

def get_coordinate(index):
    return {'X-Axis':(index / row), "Y-Axis":(index % col)}

def get_coordinates(item_list, targetlist=['cat','truck','automobile'], num=10):
    targets = list()
    for item in item_list:
        if item['catagory'] in targetlist:
            targets.append(item)
    targets = sorted(targets, key = lambda x : x['probability'], reverse=True)
    coordinates = list(get_coordinate(target['index']) for target in targets[:num])
    print [coordinate for coordinate in coordinates]
    return coordinates

def select_coordinates(candidates, num=10):
    if len(candidates) > num:
        return candidates[:num]
    else:
        return candidates
