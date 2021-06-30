## Hadoop MapReduce Project3 - 计算气温最大/最小值


#### 1、 转换文件

将.rda文件用R语言转成.txt文件

```shell
R
load(file="Ram-N-weatherData-4326bdc/data/IntlWxStations.rda")
write.table(London2013, "Ram-N-weatherData-4326bdc/txtData/IntlWxStations.txt", sep=",")
q() # 退出R编译环境
```

#### 2、启动Hadoop
> start-all.sh;jps

#### 3、Upload files
> hadoop fs -mkdir /txtData
> 
> hadoop fs -put Ram-N-weatherData-4326bdc/txtData /
> 
> hadoop fs -ls /txtData

#### 4、打包运行
> mvn clean install package
>
> hadoop fs -rm -r /Result /tmp
>
> hadoop jar target/Project3-final.jar GetMaxMinTemperature
>
> hadoop fs -get /Result ./
>
> cat Result/part-r-00000
>
> stop-all.sh