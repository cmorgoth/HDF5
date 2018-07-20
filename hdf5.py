import h5py

f = h5py.File('/Users/cmorgoth/Dropbox/3BA100_LF1-20K_80GS_6BW-12BW_test-seg_1550nm_P233_19.4uA_1of3.h5', 'r')

#print f.visit()

for key in f.keys():
    print key

group = f[key]
print group

for key in group.keys():
    print key
    sgroup =  group[key]
    #print sgroup.keys()
    for kk in sgroup.keys():
        data = sgroup[kk]
        #print kk 
