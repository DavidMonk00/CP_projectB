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
        for line in f:
            if (i > self.rows/2 - 10 and i < self.rows/2 + 10):
                arr.append([float(j) for j in line.strip().split(' ')])
            i += 1
        self.V = 2*np.array(arr)

    def derivative(self):
        row = len(self.V)/2
        l = len(self.V[row])
        xprime = np.empty(l)
        for i in range(l):
            xprime[i] = np.abs(-self.V[row-2][i] + 8*self.V[row-1][i] - 8*self.V[row+1][i] + self.V[row+2][i])
        self.E = xprime

    def homogenous(self):
        err_max = 0
        i = 0
        l = len(self.E)/2
        while (err_max < self.tolerance):
            err_max = np.abs((self.E[l + i] - self.E[l])/self.E[l])
            err_m = np.abs((self.E[l - i] - self.E[l])/self.E[l])
            if (err_m > err_max):
                err_max = err_m
            i += 1
        self.homogenousField = self.E[l-(i-1):l+(i-1)]

    def doItAllForMe(self):
        self.getLargestFileName()
        self.getFileRows()
        self.loadFileToArray()
        self.derivative()
        self.homogenous()
        print "Length of field homogenous to "+str(self.tolerance)+": "+str(float(len(self.homogenousField))*10/(self.rows/9))+"mm"

def main():
    if (len(sys.argv) > 1):
        tol = float(sys.argv[1])*100
    else:
        tol = 1e-6
    d = DataAnalysis(tol)
    #d.doItAllForMe()

    d.getFilename()
    d.getFileRows()
    print 'Loading file...'
    d.loadFileToArray()
    print 'File loaded. Calculating field...'
    d.derivative()
    d.homogenous()
    print "Length of field homogenous to "+str(d.tolerance)+": "+str(float(len(d.homogenousField))*10/(d.rows/9))+"mm"


def Plot2D():
    f = [file for file in os.listdir("./data/edm/") if file.endswith(".lf")]
    f.sort()
    V = np.loadtxt("./data/edm/"+f[-1])
    plt.imshow(V)
    plt.show()

if (__name__ == '__main__'):
    main()
    #Plot2D()
