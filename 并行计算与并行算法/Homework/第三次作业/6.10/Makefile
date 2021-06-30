EXECS=6.10
MPICC?=mpic++

all: ${EXECS}  # 编译所有的目标

6.10: 6.10.c
	${MPICC} -o 6.10 6.10.c -lstdc++ -lm

clean:        # 删除所有被make创建的文件
	rm -f ${EXECS}