import numpy as np
import matplotlib.pyplot as plt

V = np.loadtxt("file.txt", 'float64')

plt.imshow(V,interpolation='nearest',cmap='hot')
plt.show()
    
