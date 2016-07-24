row = 16
col = 16
def print_catagory_table(item_list):
    print "Catagory table"
    k = 0
    for i in range(row):
        for j in range(col):
            if (i*col + j) < len(item_list):
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

def get_coordinates(item_list, targetlist=['Cat','Trunk','Automobile']):
    coordinates = list()
    for i, item in enumerate(item_list):
        if item['catagory'] in targetlist:
            coordinates.append(get_coordinate(i))
    coordinates = list(select_coordinates(coordinates))
    print [coordinate for index, coordinate in enumerate(coordinates)]

def select_coordinates(candidates, num=10):
    if len(candidates) > num:
        return candidates[:num]
    else:
        return candidates
