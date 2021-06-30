# !/bin/sh
rm 2.3.log 2.3.p.log 

n=1024000
for i in `seq 1 50`;
do
    ./2.3  ${n} 10 ${i} >> 2.3.log  
    echo p= ${i} done
done
cat 2.3.log | grep "RunningTime"  | awk '{print $5}' >> 2.3.p.log 
