import numpy as np
import matplotlib.pyplot as plt

nx,ny = 20,20
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
for i in range(1,nx):
    A[i*ny - 1, i*ny] = 0
    A[i*ny, i*ny - 1] = 0

x = np.zeros(N).reshape(N,1)
b = np.zeros(N).reshape(N,1)
b[0] = -1
b[N-1] = 1
b[ny-1] = -1

def Jacobi(A,x,b):
    for i in range(100):
        x = np.dot(np.identity(N)/4,np.dot(A + 4*np.identity(N),x)-b)
    return x.reshape(N).reshape(nx,ny)

plt.imshow(Jacobi(A,x,b),interpolation='nearest',cmap='hot')
plt.show()
