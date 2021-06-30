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
from pyecharts.globals import CurrentConfig

CurrentConfig.ONLINE_HOST = "D:\\js\\"

p = [0]
y_mpi_p = [0]
y_swh_p = [0]
num = 29
with open("./log/1.1.MPI.p.log", "r") as f:
    i = 0
    for line in f:
        i += 1
        p.append(i)
        y_mpi_p.append(round(float(line), 1))
        if i == num:
            break
with open("./log/1.1.Swh.p.log", "r") as f:
    i = 0
    for line in f:
        i += 1
        y_swh_p.append(round(float(line), 1))
        if i == num:
            break
line_mpi_p = Line().add_xaxis(p).add_yaxis(
    "MPI_Allgather运行时间(s)",
    y_mpi_p,
    yaxis_index=1,
    label_opts=opts.LabelOpts(font_size=7.0,
                              is_show=True,
                              position="top",
                              color="black"),
)
c = (
    Line(init_opts=opts.InitOpts(theme=ThemeType.ROMA))
    # theme=ThemeType.LIGHT INFOGRAPHIC ROMA WESTEROS
    .add_xaxis(p).add_yaxis("Swhua_Allgather运行时间(s)",
                            y_swh_p,
                            label_opts=opts.LabelOpts(font_size=1.0,
                                                      is_show=True,
                                                      position="top",
                                                      color="black"),
                            is_smooth=True,
                            symbol_size=3,
                            is_connect_nones=True,
                            is_selected=True,
                            is_clip=True,
                            is_hover_animation=True
                            )  #areastyle_opts=opts.AreaStyleOpts(opacity=0.7),
    .extend_axis(yaxis=opts.AxisOpts(type_="value",
                                     name="MPI_Allgather运行时间(s)",
                                     name_gap=35,
                                     name_location='middle',
                                     axislabel_opts=opts.LabelOpts(
                                         formatter="{value} s"),
                                     interval=2.0)).
    set_series_opts(label_opts=opts.LabelOpts(
        font_size=7.0, position="right", is_show=True)).set_global_opts(
            title_opts=opts.TitleOpts(title="进程个数实验", subtitle="每个进程传256MB数据"),
            xaxis_opts=opts.AxisOpts(
                type_="value",
                name="进 程 个 数",
                is_show=True,
                is_scale=True,
                name_location='middle',
                name_gap=25,
                # min_=1,
                max_=num + 1,
                min_interval=1,
                max_interval=2,
                axislabel_opts=opts.LabelOpts(margin=5, color="black"),
                axisline_opts=opts.AxisLineOpts(is_show=True,
                                                is_on_zero=True,
                                                symbol=['none', 'arrow']),
                axistick_opts=opts.AxisTickOpts(
                    is_show=True,
                    length=1000,
                    is_inside=True,
                    linestyle_opts=opts.LineStyleOpts(color="#ff1f1f1f"),
                ),
            ),
            yaxis_opts=opts.AxisOpts(
                type_="value",
                name="Swhua_Allgather运行时间(s)",
                is_show=True,
                is_scale=False,
                name_location='middle',
                name_gap=35,
                min_=0,
                max_=27,
                min_interval=2,
                max_interval=2,
                axislabel_opts=opts.LabelOpts(formatter="{value} s",
                                              margin=5,
                                              color="black"),
                axisline_opts=opts.AxisLineOpts(is_show=True,
                                                is_on_zero=True,
                                                symbol=['none', 'arrow'])),
            legend_opts=opts.LegendOpts(
                type_='scroll', is_show=True, pos_right=400,
                pos_top=80)).overlap(line_mpi_p).render("./log/1.1.p.html"))

p2 = [0]
y_mpi_p2 = [0]
y_swh_p2 = [0]
num2 = 32
with open("./log/1.1.MPI.p2.log", "r") as f:
    i = 0
    for line in f:
        i += 1
        p2.append(i)
        y_mpi_p2.append(round(float(line) * 1000))
        if i == num2:
            break
with open("./log/1.1.Swh.p2.log", "r") as f:
    i = 0
    for line in f:
        i += 1
        y_swh_p2.append(round(float(line) * 1000))
        if i == num2:
            break
# line_mpi_p2 = Line().add_xaxis(p2).add_yaxis("MPI_Allgather运行时间(ms)",y_mpi_p2 , yaxis_index=1,label_opts=opts.LabelOpts(
#             font_size=7.0, is_show=True, position="top", color="black"
#         ))
c = (
    Line(init_opts=opts.InitOpts(theme=ThemeType.ROMA))
    # theme=ThemeType.LIGHT INFOGRAPHIC ROMA WESTEROS
    .add_xaxis(p2).add_yaxis(
        "Swhua_Allgather运行时间(ms)",
        y_swh_p2,
        label_opts=opts.LabelOpts(font_size=1.0,
                                  is_show=True,
                                  position="top",
                                  color="black"),
        is_smooth=True,
        symbol_size=3,
        is_connect_nones=True,
        is_selected=True,
        is_clip=True,
        is_hover_animation=True
    )  #areastyle_opts=opts.AreaStyleOpts(opacity=0.7),
    # .extend_axis(
    # yaxis=opts.AxisOpts(
    #     type_="value",name="MPI_Allgather运行时间(ms)", name_gap=35,name_location='middle',
    #     axislabel_opts=opts.LabelOpts(formatter="{value}"), interval=100)
    # )
    .add_yaxis(
        "MPI_Allgather运行时间(ms)",
        y_mpi_p2,
        label_opts=opts.LabelOpts(
            font_size=7.0, is_show=True,
            position="top",
            color="black")).set_series_opts(label_opts=opts.LabelOpts(
                font_size=7.0, position="top", is_show=True)).set_global_opts(
                    title_opts=opts.TitleOpts(title="进程个数实验2",
                                              subtitle="所有进程共传256MB数据"),
                    xaxis_opts=opts.AxisOpts(
                        type_="value",
                        name="进 程 个 数",
                        is_show=True,
                        is_scale=True,
                        name_location='middle',
                        name_gap=25,
                        # min_=1,
                        max_=num + 1,
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
                                             name="运行时间(ms)",
                                             is_show=True,
                                             is_scale=False,
                                             name_location='middle',
                                             name_gap=35,
                                             min_=0,
                                             max_=800,
                                             min_interval=100,
                                             max_interval=100,
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
                            "./log/1.1.p2.html")  #.overlap(line_mpi_p2)
)

# n 不同
row_num = 6
columns = ["8B", "1KB", "1MB", "64MB", "256MB", "512MB"]
y_mpi_n = []
y_swh_n = []

with open("./log/1.1.MPI.n.log ", "r") as f:
    i = 0
    for line in f:
        y_mpi_n.append(round(float(line) * 1000, 2))
        i += 1
        if row_num == i:
            break

with open("./log/1.1.Swh.n.log ", "r") as f:
    i = 0
    for line in f:
        y_swh_n.append(round(float(line) * 1000, 2))
        i += 1
        if row_num == i:
            break

bar = (
    Bar(init_opts=opts.InitOpts(theme=ThemeType.ESSOS)
        )  # theme=ThemeType.LIGHT INFOGRAPHIC ROMA WESTEROS
    .add_xaxis(columns).add_yaxis("MPI_Allgather", y_mpi_n).add_yaxis(
        "Swh_Allgather", y_swh_n).set_series_opts(label_opts=opts.LabelOpts(
            font_size=8.0, position="top", is_show=True)).set_global_opts(
                title_opts=opts.TitleOpts(title="数据传输实验", subtitle="固定16个进程"),
                xaxis_opts=opts.AxisOpts(
                    type_="category",
                    name="传 输 数 据 大 小",
                    is_show=True,
                    is_scale=True,
                    name_location='middle',
                    name_gap=25,
                    # min_=1,
                    max_=row_num,
                    min_interval=1,
                    max_interval=1,
                    axislabel_opts=opts.LabelOpts(margin=5, color="black"),
                    axisline_opts=opts.AxisLineOpts(is_show=True,
                                                    is_on_zero=True,
                                                    symbol=['none', 'arrow']),
                    axistick_opts=opts.AxisTickOpts(
                        is_show=True,
                        length=450,
                        is_inside=True,
                        linestyle_opts=opts.LineStyleOpts(color="#ffffff1f"),
                    ),
                ),
                yaxis_opts=opts.AxisOpts(
                    type_="value",
                    name="运行时间(ms)",
                    is_show=True,
                    is_scale=False,
                    name_location='middle',
                    name_gap=40,
                    min_=0,
                    max_=20000,
                    min_interval=4000,
                    # max_interval=2,
                    axislabel_opts=opts.LabelOpts(margin=5, color="black"),
                    axisline_opts=opts.AxisLineOpts(is_show=True,
                                                    is_on_zero=True,
                                                    symbol=['none', 'arrow'])),
                legend_opts=opts.LegendOpts(
                    type_='scroll', is_show=True, pos_right=500,
                    pos_top=60)).render("./log/1.1.n.html"))
