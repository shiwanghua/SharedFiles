# !/bin/sh
rm 8.6.log 8.6.o.log  

p=1
for i in `seq 1 18`;
do
    mpiexec -n ${i} ./8.6  1000 1000 >> 8.6.log  
    echo ${i} processes done
done

cat 8.6.log | grep Time | awk '{print $3}' >> 8.6.o.log 

