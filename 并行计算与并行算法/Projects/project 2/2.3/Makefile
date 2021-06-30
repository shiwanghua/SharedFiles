EXECS=2.3
MPICC?=mpic++

all: ${EXECS}  # 编译所有的目标

2.3: 2.3.c
	${MPICC} -o 2.3 2.3.c -fopenmp -lstdc++ -lm 

clean:        # 删除所有被make创建的文件
	rm -f ${EXECS}
