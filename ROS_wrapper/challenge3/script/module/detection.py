from termcolor import colored

row = 16
col = 16
k2color = 'blue'
SelectThreshold = 0.90 #the probability threshold to select candidate
CofusionThreshold = 0.60 #the threshold indicating need to check k2 

def print_catagory_table(item_list,targetlist=['cat','truck','automobile']):
    print "Catagory table"
    k = -1
    for i in range(row):
        for j in range(col):
            k = k + 1 
            if (i*col + j) < len(item_list):
                if item_list[k]['catagory'] in targetlist:
                    if item_list[k]['probability'] > SelectThreshold:
                        print colored("|%6.6s" % item_list[k]['catagory'], 'red') ,
                    else:
                        print colored("|%6.6s" % item_list[k]['catagory'], 'green') ,
                else:
                    if item_list[k]['catagory2'] in targetlist:
                        print colored("|%6.6s" % item_list[k]['catagory'], k2color) ,
                    else:
                        print ("|%6.6s" % item_list[k]['catagory']) ,
            else:
                print ("|%6.6s" % 'x') ,
        print "|"

def print_probability_table(item_list, targetlist=['cat','truck','automobile']):
    print "Probability table"
    k = -1
    for i in range(row):
        for j in range(col):
            k = k + 1
            if (i*col + j) < len(item_list):
                if item_list[k]['catagory'] in targetlist:
                    if item_list[k]['probability'] > SelectThreshold:
                        print colored("|%6.6s" % item_list[k]['probability'], 'red') ,
                    else:
                        print colored("|%6.6s" % item_list[k]['probability'], 'green') ,
                else:
                    if item_list[k]['catagory2'] in targetlist:
                        print colored("|%6.6s" % item_list[k]['probability'], k2color) ,
                    else:
                        print ("|%6.6s" % item_list[k]['probability']) ,
            else:
                print ("|%6.6s" % 'x') ,
        print "|"

def get_coordinate(index):
    return {'X-Axis':(index / row), "Y-Axis":(index % col)}

def get_coordinates(item_list, targetlist=['cat','truck','automobile'], num=10):
    targets = list()
    for item in item_list:
        if item['catagory'] in targetlist:
            targets.append(item)
    targets = sorted(targets, key = lambda x : x['probability'], reverse=True)
    targets = select_candidates(targets)
    coordinates = list(get_coordinate(target['index']) for target in targets)
    print [coordinate for coordinate in coordinates]
    return coordinates

def select_candidates(candidates, num=10, strict=0):
    bp = None
    seeds = None
    print len(candidates)
    # get only first 10 objects 
    if len(candidates) > num:
        candidates = candidates[:num]
        seeds = candidates
    else:
        seeds = candidates
    # add threshold
    if strict < 1:
        return seeds
    for index in range(len(candidates)):
        if candidates[index]['probability'] < SelectThreshold:
            seeds = candidates[:index]
            bp = index
            break
    if bp is None:
        return seeds
    # special case select
    if strict < 2:
        return seeds
    for candidate in candidates[bp:]:
        if candidate['catagory'] == 'cat':
            seeds.insert(0, candidate)
    return seeds
