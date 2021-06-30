import matplotlib.pyplot as plt
import numpy as np
from pyecharts.charts import Bar
from pyecharts.render import make_snapshot
from pyecharts import options as opts
from pyecharts.globals import ThemeType

# import pyecharts.options as opts
from pyecharts.charts import Line
from pyecharts.faker import Faker


# 使用 snapshot-selenium 渲染图片
from snapshot_selenium import snapshot

# n 不同
# row_num = 7
# columns = [
#     "8B",
#     "1KB",
#     "1MB",
#     "64MB",
#     "256MB",
#     "512MB",
#     "1024MB",
# ]
# y = []
# y2 = []

# with open("6.10.MPI.o.log ", "r") as f:
#     i = 0
#     for line in f:
#         y.append(round(float(line), 4))
#         i += 1
#         if row_num == i:
#             break


# y2 = []

# with open("6.10.Swh.o.log ", "r") as f:
#     i = 0
#     for line in f:
#         y2.append(round(float(line), 4))
#         i += 1
#         if row_num == i:
#             break


# 起始进程不同
# row_num = 9
# columns = []
# y = []
# y2 = []
# with open("6.10.MPI.o.log ", "r") as f:
#     i = 0
#     for line in f:
#         columns.append(i)
#         y.append(round(float(line), 4))
#         i += 1
#         if row_num == i:
#             break
# with open("6.10.Swh.o.log ", "r") as f:
#     i = 0
#     for line in f:
#         y2.append(round(float(line), 4))
#         i += 1
#         if row_num == i:
#             break


# 进程个数不同
row_num = 6
columns = []
y = []
y2 = []
with open("6.13.MPI.o.log ", "r") as f:
    i = 2
    for line in f:
        columns.append(i)
        y.append(round(float(line), 4))
        i = i * 2


bar = (
    Bar(
        init_opts=opts.InitOpts(theme=ThemeType.ESSOS)
    )  # theme=ThemeType.LIGHT INFOGRAPHIC ROMA WESTEROS
    .add_xaxis(columns)
    .add_yaxis("MPI_Bcast", y)
    .add_yaxis("Swh_Bcast", y2)
    .set_global_opts(
        title_opts=opts.TitleOpts(title="不同进程个数", subtitle="MPI_Bcast与Swh_Bcast")
    )
)
make_snapshot(snapshot, bar.render(), "6.10.comparison.png")

