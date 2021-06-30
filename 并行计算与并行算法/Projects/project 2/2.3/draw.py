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
y_p = [0]
num = 50
with open("./2.3.p.log", "r") as f:
    i = 0
    for line in f:
        i += 1
        p.append(i)
        y_p.append(round(float(line) * 1000))
        if i == num:
            break

c = (
    Line(init_opts=opts.InitOpts(theme=ThemeType.ROMA))
    # theme=ThemeType.LIGHT INFOGRAPHIC ROMA WESTEROS
    .add_xaxis(p).add_yaxis("运行时间(ms)",
                            y_p,
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
    .set_series_opts(label_opts=opts.LabelOpts(
        font_size=5.2, position="top", is_show=True)).set_global_opts(
            title_opts=opts.TitleOpts(title="线程个数实验",
                                      subtitle="1024000 pages,1-10 links"),
            xaxis_opts=opts.AxisOpts(
                type_="value",
                name="进 程 个 数",
                is_show=True,
                is_scale=True,
                name_location='middle',
                name_gap=25,
                # min_=1,
                max_=num,
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
                name="运行时间(ms)",
                is_show=True,
                is_scale=False,
                name_location='middle',
                name_gap=45,
                min_=3700,
                max_=62000,
                min_interval=3000,
                max_interval=5000,
                axislabel_opts=opts.LabelOpts(formatter="{value}",
                                              margin=5,
                                              color="black"),
                axisline_opts=opts.AxisLineOpts(is_show=True,
                                                is_on_zero=True,
                                                symbol=['none', 'arrow'])),
            legend_opts=opts.LegendOpts(
                type_='scroll', is_show=True, pos_right=380,
                pos_top=80)).render("./2.3.html")  #.overlap(line_mpi_p2)
)
