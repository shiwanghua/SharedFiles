## Note

1. g(n)的n指单个job的数据块分布在n个核上运行，不同主机之间执行相同Job的核会相互干扰，同一台主机之间不同任务的核不会相互干扰
2. 给一个Job分配了3个核运行，则一直使用系数g(3)，尽管两个核运行完了，第三个核还是以g(3)而不是g(1)为系数，即：对同一个Job而言，所有数据块在所有核上任意时刻的运算速度都一样
3. 一个 Job 开始运行时一次性占用所有分配给它的核，一直占用所有的核直到运行完最后一个数据块，再一次性释放所有的核
4. 核是不参与传输的, 一个单核的主机可以同时传输多个Job的多个数据块，但是对每个Job而言必须拿到它所属的所有核后才可以开始其所有传输任务
6. 对同一个Job而言，给它分配的所有核不需要都要等最后一个核数据传输完才开始工作，每个核的计算是异步的，传输完分配给它的block或者不传输就可以开始时计算了

#### 数据输入顺序

1. Job数，主机数，g函数的alpha值，传输速度(Task1中不会输入速度)
2. 每个主机的核数
3. 每个Job的块数
4. 每个Job的速度
5. 每个Job的每个数据块的大小
6. 每个Job的每个数据块的初始位置

#### 注：

* 任何本项目中与Project原文不同之处，都可以按Project里的来
* cppFiles/Util.cpp 里实现了一个随机用例生成器，可以根据参数范围给一个 ResourceScheduler 赋值

#### 提 示 （不一定对）

1. 可以算出每个使用了多核进行计算的Job的加速比 (单核运行时间除以多核运行时间)
2. 可以给出多个可行解，然后比较每个解的总的加速比(总的加速比可能需要定义一下)，越大说明你的算法越优秀
3. 你们还可以算出在哪种给定条件下，不管其他条件是什么，理论上最大的加速比是多少，这个上界越紧越好，换而言之，理论上最小的完成时间（最后一个数据块的完成时间）和最小的平均响应时间（每个Job内部最后一个数据块的完成时间的平均值）
4. 以退为进：有没有可能传输一步后效果变差了，但不立刻回溯，继续多往下调度几步，效果总体上会变好？如果有可能，什么时候可能？怎么解决这类情况？
5. 机器学习中的注意力机制？
6. 把离线调度扩展成在线调度？进而用强化学习？转换成动态规划问题，就相当于把离线数据集变成了在线数据集？

