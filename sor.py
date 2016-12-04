import numpy as np
import matplotlib.pyplot as plt
import os

def derivative(x,row):
    l = len(x[row])
    xprime = np.empty(l)
    for i in range(l):
        xprime[i] = np.abs(-x[row-2][i] + 8*x[row-1][i] - 8*x[row+1][i] + x[row+2][i])
    return xprime

f = [file for file in os.listdir("./data/edm/") if file.endswith(".lf")]
f.sort()

V = 2*np.loadtxt("./data/edm/"+f[-1], 'float64')


plt.imshow(V,interpolation='nearest')#  cmap='hot')
plt.show()

l = len(V[0])
y = derivative(V,len(V)/2)[3*l/16:13*l/16]
l = len(y)
y = y[l/10:9*l/10]
m = np.mean(y)
y_i = np.abs((m-y)/m)
x = np.linspace(0,1,len(y))
plt.plot(x,y_i)# - len(V)/10])
plt.show()
