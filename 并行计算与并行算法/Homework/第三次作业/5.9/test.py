import matplotlib.pyplot as plt
import numpy as np

row_num=100
p = np.arange(1, row_num+1)
y = []
average=0.0

with open("5.9.o_2i.log", "r") as f:
    i=0
    for line in f:
        y.append(float(line))
        average+=float(line)
        i+=1
        if row_num == i:
            break

print(average/row_num)
y = np.array(y)


y2 = []
average2=0.0

with open("5.9.o_ii.log", "r") as f:
    i=0
    for line in f:
        y2.append(float(line))
        average2+=float(line)
        i+=1
        if row_num == i:
            break

print(average2/row_num)
y2 = np.array(y2)


plt.plot(p, y,'s-',color = 'r',label="5.9.2i")
plt.plot(p, y2,'o-',color = 'b',label="5.9.ii")
plt.xlabel("Process Number")#横坐标名字
plt.ylabel("Run Time (s)")#纵坐标名字
plt.legend(loc = "best")#图例
plt.savefig("./result.png")
plt.close()


