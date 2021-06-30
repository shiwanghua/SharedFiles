import matplotlib.pyplot as plt
import numpy as np
from pyecharts.charts import Bar
from pyecharts.charts import Grid
from pyecharts.render import make_snapshot
from pyecharts import options as opts
from pyecharts.globals import ThemeType
from pyecharts.commons.utils import JsCode

# import pyecharts.options as opts
from pyecharts.charts import Line
from pyecharts.faker import Faker

# 使用 snapshot-selenium 渲染图片
from snapshot_selenium import snapshot
from pyecharts.globals import CurrentConfig

CurrentConfig.ONLINE_HOST = "D:\\js\\"

n = [
    "10", "100", "1000", "10000", "100000", "1000000", "10000000", "100000000",
    "1000000000", "10000000000"
]
# print(type(n))
na = []
nt = []
p = [0]
pt = [0]
num = 40

with open("./log/2.1.n2.a.log", "r") as f:
    for line in f:
        na.append(round(float(line), 6))

with open("./log/2.1.n2.t.log", "r") as f:
    for line in f:
        nt.append(round(float(line), 4))  # 3 4

with open("./log/2.1.p.t.log", "r") as f:
    i = 0
    for line in f:
        i += 1
        p.append(i)
        pt.append(round(float(line), 2))
        if i == num:
            break

line_nt = Line().add_xaxis(n).add_yaxis(
    series_name="准 确 率",
    y_axis=na,
    yaxis_index=1,  # 次坐标
    is_smooth=True,
    symbol_size=3,
    is_connect_nones=True,
    is_selected=True,
    is_clip=True,
    itemstyle_opts=opts.ItemStyleOpts(color="#f6c065"),  # 标记的颜色
    linestyle_opts=opts.LineStyleOpts(width=3, color='#f6c065'),  # 线条颜色和宽度
    is_hover_animation=True,
    label_opts=opts.LabelOpts(font_size=10.0,
                              is_show=True,
                              position="top",
                              color="black"))
c = (
    Bar(init_opts=opts.InitOpts(
        theme=ThemeType.ROMA, height="500px"))  #width="1400px", height="700px"
    # theme=ThemeType.LIGHT INFOGRAPHIC ROMA WESTEROS
    .add_xaxis(xaxis_data=n).add_yaxis(
        "运行时间(s)",
        y_axis=nt,
        label_opts=opts.LabelOpts(font_size=7.0,
                                  is_show=True,
                                  position="left",
                                  formatter="{value}",
                                  color="blue"),
        itemstyle_opts=opts.ItemStyleOpts(color="#78c4d4", opacity=0.5),
        color="black")
    # 柱状图颜色和透明度  #areastyle_opts=opts.AreaStyleOpts(opacity=0.7),
    .extend_axis(yaxis=opts.AxisOpts(
        type_="value",
        name="准确率",
        name_gap=35,
        name_location='middle',
        min_=0.6,
        max_=1.0,
        is_show=True,
        min_interval=0.01,
        max_interval=0.02,
        axisline_opts=opts.AxisLineOpts(
            is_show=True,  # y轴线显示
            linestyle_opts=opts.LineStyleOpts(
                color='#f6c065')),  # 设置线颜色, 字体颜色也变
        axistick_opts=opts.AxisTickOpts(is_show=True),  # 刻度线显示
        axislabel_opts=opts.LabelOpts(
            formatter="{value}", margin=5, color="#f6c065", interval=1))).
    set_series_opts(label_opts=opts.LabelOpts(
        font_size=9.0, position="center", is_show=True)).set_global_opts(
            title_opts=opts.TitleOpts(
                title="点数实验",
                title_textstyle_opts=opts.TextStyleOpts(
                    font_size=20),  #主标题字体大小
                subtitle="线程为1",
                pos_left='1%'),
            xaxis_opts=opts.AxisOpts(
                type_="category",  #log time
                name="点数",
                is_show=True,
                is_scale=True,
                name_location='middle',
                name_gap=46,
                # min_=1,
                # max_=num + 1,
                min_interval=1,
                max_interval=1,
                name_rotate=60,
                axislabel_opts=opts.LabelOpts(margin=10,
                                              color="black",
                                              rotate=45),
                axisline_opts=opts.AxisLineOpts(is_show=True,
                                                is_on_zero=True,
                                                symbol=['none', 'arrow']),
                axistick_opts=opts.AxisTickOpts(  # 刻度线
                    is_show=True,
                    length=1000,
                    is_inside=True,
                    linestyle_opts=opts.LineStyleOpts(color="#ff1f1f1f"),
                ),
            ),
            yaxis_opts=opts.AxisOpts(
                type_="value",
                name="运 行 时 间 (s)",
                is_show=True,
                is_scale=True,
                name_location='middle',
                name_gap=35,
                min_=0,
                max_=24,
                min_interval=2,
                max_interval=3,
                axistick_opts=opts.AxisTickOpts(is_show=True),  # 刻度线
                splitline_opts=opts.SplitLineOpts(is_show=True),  # y轴网格线
                axislabel_opts=opts.LabelOpts(
                    formatter="{value}",  # y轴上的数字
                    margin=5,
                    color="black"),
                axisline_opts=opts.AxisLineOpts(
                    is_show=True,  # y 轴线
                    is_on_zero=True,
                    symbol=['none', 'arrow'])),
            tooltip_opts=opts.TooltipOpts(trigger="axis"),  # 提示框
            legend_opts=opts.LegendOpts(
                type_='scroll', is_show=True, pos_right=280,
                pos_top=80)).overlap(line_nt).render("./log/2.1.n2.html"))

# p2 = [0]
# y_mpi_p2 = [0]
# y_swh_p2 = [0]
# num2 = 32
# with open("./log/1.1.MPI.p2.log", "r") as f:
#     i = 0
#     for line in f:
#         i += 1
#         p2.append(i)
#         y_mpi_p2.append(round(float(line) * 1000))
#         if i == num2:
#             break
# with open("./log/1.1.Swh.p2.log", "r") as f:
#     i = 0
#     for line in f:
#         i += 1
#         y_swh_p2.append(round(float(line) * 1000))
#         if i == num2:
#             break
# # line_mpi_p2 = Line().add_xaxis(p2).add_yaxis("MPI_Allgather运行时间(ms)",y_mpi_p2 , yaxis_index=1,label_opts=opts.LabelOpts(
# #             font_size=7.0, is_show=True, position="top", color="black"
# #         ))
c = (
    Line(init_opts=opts.InitOpts(theme=ThemeType.ROMA))
    # theme=ThemeType.LIGHT INFOGRAPHIC ROMA WESTEROS
    .add_xaxis(p).add_yaxis(
        "运行时间(s)",
        pt,
        label_opts=opts.LabelOpts(
            font_size=7.0, is_show=True, position="top",
            color="black")).set_series_opts(label_opts=opts.LabelOpts(
                font_size=7.0, position="top", is_show=True)).set_global_opts(
                    title_opts=opts.TitleOpts(title="线程个数实验",
                                              subtitle="n=100000000"),
                    xaxis_opts=opts.AxisOpts(
                        type_="value",
                        name="线 程 个 数",
                        is_show=True,
                        is_scale=True,
                        name_location='middle',
                        name_gap=25,
                        # min_=1,
                        max_=num,
                        min_interval=1,
                        max_interval=2,
                        axislabel_opts=opts.LabelOpts(margin=5, color="black"),
                        axisline_opts=opts.AxisLineOpts(
                            is_show=True,
                            is_on_zero=True,
                            symbol=['none', 'arrow']),
                        axistick_opts=opts.AxisTickOpts(
                            is_show=True,
                            length=1000,
                            is_inside=True,
                            linestyle_opts=opts.LineStyleOpts(
                                color="#ff1f1f1f"),
                        ),
                    ),
                    yaxis_opts=opts.AxisOpts(type_="value",
                                             name="运行时间(s)",
                                             is_show=True,
                                             is_scale=False,
                                             name_location='middle',
                                             name_gap=35,
                                             min_=15,
                                             max_=25,
                                             min_interval=1,
                                             max_interval=2,
                                             axislabel_opts=opts.LabelOpts(
                                                 formatter="{value}",
                                                 margin=5,
                                                 color="black"),
                                             axisline_opts=opts.AxisLineOpts(
                                                 is_show=True,
                                                 is_on_zero=True,
                                                 symbol=['none', 'arrow'])),
                    legend_opts=opts.LegendOpts(
                        type_='scroll',
                        is_show=True,
                        pos_right=200,
                        pos_top=80)).render(
                            "./log/2.1.p.html")  #.overlap(line_mpi_p2)
)

# # n 不同
# row_num = 6
# columns = ["8B", "1KB", "1MB", "64MB", "256MB", "512MB"]
# y_mpi_n = []
# y_swh_n = []

# with open("./log/1.1.MPI.n.log ", "r") as f:
#     i = 0
#     for line in f:
#         y_mpi_n.append(round(float(line) * 1000, 2))
#         i += 1
#         if row_num == i:
#             break

# with open("./log/1.1.Swh.n.log ", "r") as f:
#     i = 0
#     for line in f:
#         y_swh_n.append(round(float(line) * 1000, 2))
#         i += 1
#         if row_num == i:
#             break

# bar = (
#     Bar(init_opts=opts.InitOpts(theme=ThemeType.ESSOS)
#         )  # theme=ThemeType.LIGHT INFOGRAPHIC ROMA WESTEROS
#     .add_xaxis(columns).add_yaxis("MPI_Allgather", y_mpi_n).add_yaxis(
#         "Swh_Allgather", y_swh_n).set_series_opts(label_opts=opts.LabelOpts(
#             font_size=8.0, position="top", is_show=True)).set_global_opts(
#                 title_opts=opts.TitleOpts(title="数据传输实验", subtitle="固定16个进程"),
#                 xaxis_opts=opts.AxisOpts(
#                     type_="category",
#                     name="传 输 数 据 大 小",
#                     is_show=True,
#                     is_scale=True,
#                     name_location='middle',
#                     name_gap=25,
#                     # min_=1,
#                     max_=row_num,
#                     min_interval=1,
#                     max_interval=1,
#                     axislabel_opts=opts.LabelOpts(margin=5, color="black"),
#                     axisline_opts=opts.AxisLineOpts(is_show=True,
#                                                     is_on_zero=True,
#                                                     symbol=['none', 'arrow']),
#                     axistick_opts=opts.AxisTickOpts(
#                         is_show=True,
#                         length=450,
#                         is_inside=True,
#                         linestyle_opts=opts.LineStyleOpts(color="#ffffff1f"),
#                     ),
#                 ),
#                 yaxis_opts=opts.AxisOpts(
#                     type_="value",
#                     name="运行时间(ms)",
#                     is_show=True,
#                     is_scale=False,
#                     name_location='middle',
#                     name_gap=40,
#                     min_=0,
#                     max_=20000,
#                     min_interval=4000,
#                     # max_interval=2,
#                     axislabel_opts=opts.LabelOpts(margin=5, color="black"),
#                     axisline_opts=opts.AxisLineOpts(is_show=True,
#                                                     is_on_zero=True,
#                                                     symbol=['none', 'arrow'])),
#                 legend_opts=opts.LegendOpts(
#                     type_='scroll', is_show=True, pos_right=500,
#                     pos_top=60)).render("./log/2.1.n.html"))
