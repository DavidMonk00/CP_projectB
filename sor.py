import numpy as np
import matplotlib.pyplot as plt

V = np.loadtxt("file.lf", 'float64')

plt.imshow(V,interpolation='nearest')#  cmap='hot')
plt.show()

x = np.linspace(-1,1,len(V[0]))
plt.plot(x,V[len(V)/2])# - len(V)/10])
plt.show()
