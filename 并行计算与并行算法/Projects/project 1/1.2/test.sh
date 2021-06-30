# !/bin/sh
rm 1.2.log ab.log ak.log ap.log 

for i in `seq 1 50`;
do
    mpiexec -n ${i} -f config ./1.2  1024 1024 1024 4 >> 1.2.log  
    echo ${i} processes done
done

cat 1.2.log | grep ab | awk '{print $4}' >> ab.log
cat 1.2.log | grep ak | awk '{print $4}' >> ak.log
cat 1.2.log | grep max_pooling | awk '{print $4}' >> ap.log