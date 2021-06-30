# !/bin/sh
rm 8.12.log 8.12.o.log  

p=1
for i in `seq 1 40`;
do
    mpiexec -n ${i} ./8.12  1000 >> 8.12.log  
    echo ${i} processes done
done

cat 8.12.log | grep Time | awk '{print $3}' >> 8.12.o.log 
