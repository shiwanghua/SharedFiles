import matplotlib.pyplot as plt
import numpy as np
from pyecharts.charts import Bar
from pyecharts.render import make_snapshot
from pyecharts import options as opts
from pyecharts.globals import ThemeType
from pyecharts.commons.utils import JsCode

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


# bar = (
#     Bar(
#         init_opts=opts.InitOpts(theme=ThemeType.ESSOS)
#     )  # theme=ThemeType.LIGHT INFOGRAPHIC ROMA WESTEROS
#     .add_xaxis(columns)
#     .add_yaxis("MPI_Bcast", y)
#     .add_yaxis("Swh_Bcast", y2)
#     .set_global_opts(
#         title_opts=opts.TitleOpts(title="不同进程个数", subtitle="MPI_Bcast与Swh_Bcast")
#     )
# )
# make_snapshot(snapshot, bar.render(), "6.10.comparison.png")

row_num = 5
columns = []
y = []
with open("6.13.r10.o.log", "r") as f:
    i = 1
    for line in f:
        i *= 10
        columns.append("(" + str(i) + ",10)")
        y.append(round(float(line), 4))
c1 = (
    Line(init_opts=opts.InitOpts(theme=ThemeType.ROMA))
    .add_xaxis(columns)
    .add_yaxis("行倍增运行时间", y, areastyle_opts=opts.AreaStyleOpts(opacity=0.5))
    .set_global_opts(title_opts=opts.TitleOpts(title="行数每次乘10的运行时间"))
    .render("6.13.r10.2.html")
)

row_num = 6
columns = ["(10,100)", "(100,10)", "(10,1000)", "(1000,10)", "(100,1000)", "(1000,100)"]
y = []
with open("6.13.rc.o.log", "r") as f:
    i = 1
    for line in f:
        y.append(round(float(line), 4))
bar = (
    Bar(
        init_opts=opts.InitOpts(theme=ThemeType.ROMA)
    )  # theme=ThemeType.LIGHT INFOGRAPHIC ROMA WESTEROS
    .add_xaxis(columns)
    .add_yaxis("运行时间(s)", y)
    .set_global_opts(title_opts=opts.TitleOpts(title="行列互换的影响", subtitle="三对情况"))
    .render("6.13.rc.1.html")
)


row_num = 41
columns = [0]
y = [0]
with open("6.13.p.o.log", "r") as f:
    i = 0
    for line in f:
        i += 1
        y.append(round(float(line), 1))
        columns.append(i)
        if i == row_num:
            break
background_color_js = (
    "new echarts.graphic.LinearGradient(0, 0, 0, 1, "
    "[{offset: 0, color: '#c86589'}, {offset: 1, color: '#06a7ff'}], false)"
)
area_color_js = (
    "new echarts.graphic.LinearGradient(0, 0, 0, 1, "
    "[{offset: 0, color: '#eb64fb'}, {offset: 1, color: '#3fbbff0d'}], false)"
)
c = (
    Line(init_opts=opts.InitOpts(bg_color=JsCode(background_color_js)))
    .add_xaxis(xaxis_data=columns)
    .add_yaxis(
        series_name="注册总量",
        y_axis=y,
        is_smooth=True,
        is_symbol_show=True,
        symbol="circle",
        symbol_size=6,
        linestyle_opts=opts.LineStyleOpts(color="#fff"),
        label_opts=opts.LabelOpts(
            font_size=8, is_show=True, position="top", color="white"
        ),
        itemstyle_opts=opts.ItemStyleOpts(
            color="blue", border_color="#fff", border_width=3
        ),
        tooltip_opts=opts.TooltipOpts(is_show=False),
        areastyle_opts=opts.AreaStyleOpts(color=JsCode(area_color_js), opacity=0.5),
    )
    .set_global_opts(
        title_opts=opts.TitleOpts(
            title="进程个数实验",
            pos_top="5%",
            pos_left="center",
            title_textstyle_opts=opts.TextStyleOpts(color="#fff", font_size=16),
        ),
        xaxis_opts=opts.AxisOpts(
            type_="category",
            boundary_gap=False,
            axislabel_opts=opts.LabelOpts(margin=30, color="blue"),
            axisline_opts=opts.AxisLineOpts(is_show=False),
            axistick_opts=opts.AxisTickOpts(
                is_show=True,
                length=25,
                linestyle_opts=opts.LineStyleOpts(color="#ffffff1f"),
            ),
            splitline_opts=opts.SplitLineOpts(
                is_show=True, linestyle_opts=opts.LineStyleOpts(color="#ffffff1f")
            ),
        ),
        yaxis_opts=opts.AxisOpts(
            type_="value",
            name="运行时间(s)",
            position="right",
            axislabel_opts=opts.LabelOpts(margin=20, color="#ffffff63"),
            axisline_opts=opts.AxisLineOpts(
                linestyle_opts=opts.LineStyleOpts(width=2, color="#fff")
            ),
            axistick_opts=opts.AxisTickOpts(
                is_show=True,
                length=15,
                linestyle_opts=opts.LineStyleOpts(color="#ffffff1f"),
            ),
            splitline_opts=opts.SplitLineOpts(
                is_show=True, linestyle_opts=opts.LineStyleOpts(color="#ffffff1f")
            ),
        ),
        legend_opts=opts.LegendOpts(is_show=False),
    )
    .render("6.13.p.html")
)

columns = []
y = []
with open("6.13.t.o.log", "r") as f:
    i = 1
    for line in f:
        i *= 10
        columns.append(str(i))  # 为数值时画不出来！！
        y.append(round(float(line), 3))
c = (
    Line(init_opts=opts.InitOpts(theme=ThemeType.LIGHT))
    # theme=ThemeType.LIGHT INFOGRAPHIC ROMA WESTEROS
    .add_xaxis(columns)
    .add_yaxis(
        "运行时间", y, is_smooth=True, areastyle_opts=opts.AreaStyleOpts(opacity=0.5)
    )
    .set_global_opts(title_opts=opts.TitleOpts(title="迭代次数实验"))
    .render("6.13.t.html")
)

