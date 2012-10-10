from pylab import *

data = {'cgl': 0, 'fgl': 1, 'lfl': 2, 'cgt': 3, 'fgt': 4, 'lft': 5}

file = open("test1.txt", 'r')

plots = [[]*6 for x in xrange(6)]

while 1:
    line = file.readline()
    if not line:
        break
    line = line.split(' ')
    plots[data[line[0]]].append(int(line[4][:-1]))

x = [1, 2, 4, 5, 8, 10]

figure(1)
title("List threads benchmark")
plot(x, plots[0], label="CGT")
plot(x, plots[1], label="FGT")
plot(x, plots[2], label="LFT")
legend()
show()

figure(1)
title("Tree threads benchmark")
plot(x, plots[3], label="CGT")
plot(x, plots[4], label="FGT")
plot(x, plots[5], label="LFT")
legend()
show()
