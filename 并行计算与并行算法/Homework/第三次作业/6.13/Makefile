EXECS=6.13
MPICC?=mpic++

all: ${EXECS}  # 编译所有的目标

6.13: 6.13.c
	${MPICC} -o 6.13 6.13.c -lstdc++ -lm

clean:        # 删除所有被make创建的文件
	rm -f ${EXECS}