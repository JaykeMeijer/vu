from pylab import *

data = {'cgl': 0, 'fgl': 1, 'lfl': 2, 'cgt': 3, 'fgt': 4, 'lft': 5}
file = open("test2.txt", 'r')

plots = [[]*6 for x in xrange(6)]

while 1:
    line = file.readline()
    if not line:
        break
    line = line.split(' ')
    plots[data[line[0]]].append(int(line[4][:-1]))

x = [10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000, 90000, 100000]


figure()
title("List elements benchmark")
plot(x, plots[0], label="CGL")
plot(x, plots[1], label="FGL")
plot(x, plots[2], label="LFL")
legend(loc=2)
show()

figure(1)
title("Tree elements benchmark")
plot(x, plots[3], label="CGT")
plot(x, plots[4], label="FGT")
plot(x, plots[5], label="LFT")
legend(loc=2)
show()
