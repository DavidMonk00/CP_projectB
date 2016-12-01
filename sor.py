import numpy as np
import matplotlib.pyplot as plt
import os

f = [file for file in os.listdir("./data/cable/") if file.endswith(".lf")]
f.sort()

V = 2*np.loadtxt("./data/cable/"+f[-1], 'float64')


plt.imshow(V,interpolation='nearest')#  cmap='hot')
plt.show()

x = np.linspace(-1,1,len(V[0]))
plt.plot(x,V[len(V)/2])# - len(V)/10])
plt.show()
