EXECS=2.2
MPICC?=mpic++

all: ${EXECS}  # 编译所有的目标

2.2: 2.2.c
	${MPICC} -o 2.2 2.2.c -lstdc++ -lm -fopenmp

clean:        # 删除所有被make创建的文件
	rm -f ${EXECS}