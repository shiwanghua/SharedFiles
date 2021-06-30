# !/bin/sh
rm 2.1.n.log 2.1.n.t.log 2.1.n.a.log 2.1.p.log 2.1.p.t.log 

# n=10
# for i in `seq 1 10`;
# do
#     ./2.1  1 ${n} >> 2.1.n2.log  
#     echo n= ${n} done
#     n=$((${n}*10))
# done
# cat 2.1.n2.log | awk '{print $4}' >> 2.1.n2.t.log  # 时间
# cat 2.1.n2.log | awk '{print $9}' >> 2.1.n2.a.log  # 准确率

n=100000000
for i in `seq 1 40`;
do
    ./2.1  ${i} ${n} >> 2.1.p.log  
    echo p= ${i} done
done
cat 2.1.p.log  | awk '{print $4}' >> 2.1.p.t.log 
