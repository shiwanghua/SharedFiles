EXECS=2.1
MPICC?=mpic++

all: ${EXECS}  # 编译所有的目标

2.1: 2.1.c 2.1.experiment.c
	${MPICC} -o 2.1 2.1.c -lstdc++ -lm -fopenmp
	${MPICC} -o 2.1.experiment 2.1.experiment.c -lstdc++ -lm -fopenmp
clean:        # 删除所有被make创建的文件
	rm -f ${EXECS}