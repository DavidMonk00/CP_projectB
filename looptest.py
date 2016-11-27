import numpy as np


nx = 1000
ny = 1000
factor = 10

x = np.linspace(0,1,(nx/factor)*(ny/factor)).reshape(nx/factor,ny/factor)
for i in range(nx/(3*factor),2*nx/(3*factor)):
	for j in range(ny/(3*factor),2*ny/(3*factor)):
		x[i][j] = 1
print x

y = np.zeros(nx*ny).reshape(nx,ny)
for i in range(nx):
	for j in range(ny):
		y[i][j] = x[i/factor][j/factor]
			
print y[nx/2]

