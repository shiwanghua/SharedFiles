EXECS=9.7
MPICC?=mpic++

all: ${EXECS}  # 编译所有的目标

9.7: 9.7.c
	${MPICC} -o 9.7 9.7.c -lstdc++ -lm

clean:        # 删除所有被make创建的文件
	rm -f ${EXECS}