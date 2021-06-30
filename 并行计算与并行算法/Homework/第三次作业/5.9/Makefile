EXECS=5.9
MPICC?=mpic++

all: ${EXECS}  # 编译所有的目标

5.9: 5.9.c
	${MPICC} -o 5.9 5.9.c -lstdc++ -lm

clean:        # 删除所有被make创建的文件
	rm -f ${EXECS}