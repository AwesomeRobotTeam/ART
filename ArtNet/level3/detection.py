from termcolor import colored

row = 16
col = 16

def print_catagory_table(item_list,targetlist=['cat','truck','automobile']):
    print "Catagory table"
    k = 0
    for i in range(row):
        for j in range(col):
            if (i*col + j) < len(item_list):
                if item_list[k]['catagory'] in targetlist:
                    print colored("|%6.6s" % item_list[k]['catagory'], 'red') ,
                else:
                    print ("|%6.6s" % item_list[k]['catagory']) ,
            else:
                print ("|%6.6s" % 'x') ,
            k = k + 1
        print "|"

def print_probability_table(item_list):
    print "Probability table"
    k = 0
    for i in range(row):
        for j in range(col):
            if (i*col + j) < len(item_list):
                print ("|%6.6s" % item_list[k]['probability']) ,
            else:
                print ("|%6.6s" % 'x') ,
            k = k + 1
        print "|"


def get_coordinate(index):
    return '%sb' % (index / row) + '%sb' % (index % col) + 'c'

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
