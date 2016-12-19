import numpy as np
import matplotlib.pyplot as plt
import os
import csv
import sys


class DataAnalysis:
    def __init__(self, tolerance, recent=True):
        self.tolerance = tolerance
        if (recent):
            self.recent = -1

    def getFilename(self):
        f = [file for file in os.listdir("./data/edm/") if file.endswith(".lf")]
        f.sort()
        self.filename = f[self.recent]

    def getLargestFileName(self):
        f = [file for file in os.listdir("./data/edm/") if file.endswith(".lf")]
        largest = 0
        for i in range(len(f)):
            if (int(os.path.getsize("./data/edm/"+f[i])) > int(os.path.getsize("./data/edm/"+f[largest]))):
                largest = i
        self.filename = f[largest]

    def getFileRows(self):
        start, end = 0,0
        for i in range(len(self.filename)-1,0,-1):
            if (self.filename[i] == '_' and end == 0):
                end = i
            else:
                if (self.filename[i] == '_'):
                    start = i
                    break
        self.rows = int(self.filename[start+1:end])

    def loadFileToArray(self):
        f = open("./data/edm/"+self.filename)
        arr = []
        i = 0
        prog = 0
        for line in f:
            if (i > self.rows/2 - 10 and i < self.rows/2 + 10):
                arr.append([float(j) for j in line.strip().split(' ')])
                prog +=1
                #print (str(float(prog)/20*100)+'%')
            i += 1
            if (i > self.rows/2 + 10):
                break
        self.V = 2*np.array(arr)/1000

    def derivative(self):
        row = len(self.V)/2
        l = len(self.V[row])
        yprime = np.empty(l)
        xprime = np.empty(l)
        for i in range(l):
            yprime[i] = (-self.V[row-2][i] + 8*self.V[row-1][i] - 8*self.V[row+1][i] + self.V[row+2][i])*100*l/(32*12)
        for i in range(2,l-2):
            xprime[i] = (-self.V[row][i-2] + 8*self.V[row][i-1] - 8*self.V[row][i+1] + self.V[row][i+2])*100*l/(32*12)
        self.E = np.sqrt(yprime*yprime + xprime*xprime)
        plt.plot(self.E)
        plt.xlim(0,l)
        plt.ylim(0,2)
        plt.xlabel('x (mm/100)')
        plt.ylabel('Magnitude of Electric Field (MV/m)')
        #plt.show()

    def homogenous(self,tolerance):
        err_max_left = 0
        err_max_right = 0
        left, right = 0,0
        i = 0
        l = len(self.E)/2
        while (err_max_left < tolerance or err_max_right < tolerance):
            err_max_right = np.abs((self.E[l + i] - self.E[l])/self.E[l])
            err_max_left = np.abs((self.E[l - i] - self.E[l])/self.E[l])
            if (err_max_right > tolerance):
                right = i - 1
            if (err_max_left > tolerance):
                left = i - 1
            i += 1
        self.homogenousField = self.E[l-(left):l+(right)]

    def doItAllForMe(self):
        self.getFilename()
        self.getFileRows()
        self.loadFileToArray()
        self.derivative()
        t = [1e-10,1e-9,1e-8,1e-7,1e-6,1e-5,1e-4,1e-3,1e-2]
        for i in t:
            self.homogenous(i)
            print "Length of field homogenous to "+str(i)+": "+str(float(len(self.homogenousField))*10/(self.rows/9))+"mm"

def main():
    if (len(sys.argv) > 1):
        tol = float(sys.argv[1])*100
    else:
        tol = 1e-6
    d = DataAnalysis(tol)
    d.doItAllForMe()

    '''d.getFilename()
    d.getFileRows()
    print 'Loading file...'
    d.loadFileToArray()
    print 'File loaded. Calculating field...'
    d.derivative()
    d.homogenous()
    print "Length of field homogenous to "+str(d.tolerance)+": "+str(float(len(d.homogenousField))*10/(d.rows/9))+"mm"
    '''

def Plot2D():
    f = [file for file in os.listdir("./data/cable/") if file.endswith(".lf")]
    f.sort()
    V = np.loadtxt("./data/cable/"+f[-1])
    plt.imshow(V, interpolation='nearest',cmap='hot')
    plt.colorbar(orientation='horizontal')
    plt.show()
    row = len(V)/2
    l = len(V[row])
    yprime = np.empty(l)
    xprime = np.empty(l)
    for i in range(l):
        yprime[i] = (-V[row-2][i] + 8*V[row-1][i] - 8*V[row+1][i] + V[row+2][i])*l/(12)
    for i in range(2,l-2):
        xprime[i] = (-V[row][i-2] + 8*V[row][i-1] - 8*V[row][i+1] + V[row][i+2])*l/(12)
    E = np.sqrt(yprime*yprime + xprime*xprime)
    x = np.arange(0,l/10,1)
    #plt.plot(V)
    plt.plot(x,E[9*l/10:])
    plt.xlabel('x')
    plt.ylabel('Field Strength (V/m)')
    plt.show()

if (__name__ == '__main__'):
    main()
    #Plot2D()
