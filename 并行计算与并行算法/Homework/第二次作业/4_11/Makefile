EXECS=4.11
MPICC?=mpicc

all: ${EXECS}  # 编译所有的目标

4.11: 4.11.c
	${MPICC} -o 4.11 4.11.c -lstdc++ -lm

clean:        # 删除所有被make创建的文件
	rm -f ${EXECS}