## Note

1. g(n)的n指单个job的数据块分布在n个核上运行，不同主机之间执行相同Job的核会相互干扰，同一台主机之间不同任务的核不会相互干扰
2. 一个 Job 开始运行时一次性占用所有分配给它的核，一直占用所有的核直到运行完最后一个数据块，再一次性释放所有的核
3. 所有主机有相同的核数