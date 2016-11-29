import numpy as np
import matplotlib.pyplot as plt
import os

V = np.loadtxt("file.lf", 'float64')

dated_files = [(os.path.getmtime(fn), os.path.basename(fn))
               for fn in os.listdir(".") if fn.lower().endswith('.lf')]
dated_files.sort()
dated_files.reverse()
newest = dated_files[0][1]
print(newest)

'''plt.imshow(V,interpolation='nearest')#  cmap='hot')
plt.show()

x = np.linspace(-1,1,len(V[0]))
plt.plot(x,V[len(V)/2])# - len(V)/10])
plt.show()'''
