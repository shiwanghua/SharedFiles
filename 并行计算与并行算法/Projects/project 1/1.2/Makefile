EXECS=1.2
MPICC?=mpic++

all: ${EXECS}  # 编译所有的目标

1.2: 1.2.c
	${MPICC} -o 1.2 1.2.c -lstdc++ -lm

clean:        # 删除所有被make创建的文件
	rm -f ${EXECS}