import numpy as np

x = np.zeros(81).reshape(9,9)
for i in range(9):
    x[i,i] = -4
for i in range(8):
    x[i,i+1] = 1
    x[i+1,i] = 1
for i in range(6):
    x[i,i+3] = 1
    x[i+3,i] = 1

print x
