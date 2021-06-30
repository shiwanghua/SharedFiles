EXECS=8.12
MPICC?=mpic++

all: ${EXECS}  # 编译所有的目标

8.12: 8.12.c
	${MPICC} -o 8.12 8.12.c -lstdc++ -lm

clean:        # 删除所有被make创建的文件
	rm -f ${EXECS}