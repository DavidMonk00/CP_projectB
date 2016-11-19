import numpy as np
import matplotlib.pyplot as plt

nx,ny = 10,10
N = nx*ny
A = np.zeros(N**2).reshape(N,N)
for i in range(N):
    A[i,i] = -4
for i in range(N-1):
    A[i,i+1] = 1
    A[i+1,i] = 1
for i in range(N-ny):
    A[i,i+ny] = 1
    A[i+ny,i] = 1

x = np.zeros(N).reshape(N,1)
b = np.zeros(N).reshape(N,1)
b[0] = -1
b[N-1] = 1

print A

for i in range(100):
    x = np.dot(np.identity(N)/4,np.dot(A + 4*np.identity(N),x)-b)

x = x.reshape(N).reshape(nx,ny)

plt.imshow(x,interpolation='nearest',cmap='hot')
plt.show()
