import matplotlib.pyplot as plt
import numpy as np

p = np.arange(1, 51)
y = []

with open("4.11.o.log", "r") as f:
    for line in f:
        y.append(float(line))

y = np.array(y)*1000
plt.plot(p, y,'s-',color = 'r',label="4.11")
plt.xlabel("Process Number")#横坐标名字
plt.ylabel("Run Time (ms)")#纵坐标名字
plt.legend(loc = "best")#图例
plt.savefig("./result.png")
plt.close()


