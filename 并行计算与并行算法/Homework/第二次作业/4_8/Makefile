EXECS=4_8_3
MPICC?=mpicc

all: ${EXECS}  # 编译所有的目标

4_8_3: 4_8_3.c
	${MPICC} -o 4_8_3 4_8_3.c -lstdc++ -lm

clean:        # 删除所有被make创建的文件
	rm -f ${EXECS}