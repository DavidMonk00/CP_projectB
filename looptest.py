import numpy as np

x = np.zeros(30).reshape(3,10)
for i in range(4):
	x[1][i+3] = 1
print x

breakstart = []
breakend = []

for i in range(len(x)):
	bstart = []
	bend = [0]
	b = False
	for j in range(len(x[0])):
		if (x[i][j] == 1 and b == False):
			b = True
			bstart.append(j)
		elif (x[i][j] == 0 and b == True):
			b = False
			bend.append(j)
	bstart.append(len(x[0]))
	breakstart.append(tuple(bstart))
	breakend.append(tuple(bend))
	
print breakstart
print breakend	

for i in range(len(x)):
	for j in range(len(breakstart[i])):
		for k in range(breakend[i][j],breakstart[i][j]):
			print i,k,x[i][k]	
