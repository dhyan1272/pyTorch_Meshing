import numpy as np
data=np.load('/home/dhyan/Downloads/speedy/speedy_numpy_file_train.npz')
list1 = data.files
for item in list1:
    print(item)
    sample=data[item]
    print(sample.shape)

