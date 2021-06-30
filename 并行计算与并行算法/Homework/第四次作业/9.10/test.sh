# !/bin/sh
rm 9.10.log 9.10.o.log  

p=1
for i in `seq 2 32`;
do
    mpiexec -n ${i} ./9.10 >> 9.10.log  
    echo ${i} processes done
done

cat 9.10.log | grep Time | awk '{print $3}' >> 9.10.o.log 
