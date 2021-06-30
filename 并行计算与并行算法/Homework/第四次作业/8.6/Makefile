EXECS=8.6
MPICC?=mpic++

all: ${EXECS}  # 编译所有的目标

8.6: 8.6.c
	${MPICC} -o 8.6 8.6.c -lstdc++ -lm

clean:        # 删除所有被make创建的文件
	rm -f ${EXECS}