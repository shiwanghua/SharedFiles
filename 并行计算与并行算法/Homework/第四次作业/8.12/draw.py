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

p = [0]
y = [0]
num=38
with open("8.12.o.log", "r") as f:
    i = 0
    for line in f:
        i += 1
        p.append(i)
        y.append(round(float(line), 1))
        if i == num:
            break
c = (
    Line(init_opts=opts.InitOpts(theme=ThemeType.LIGHT))
    # theme=ThemeType.LIGHT INFOGRAPHIC ROMA WESTEROS
    .add_xaxis(p)
    .add_yaxis("运行时间", y,label_opts=opts.LabelOpts(
            font_size=7, is_show=True, position="top", color="black"
        ), is_smooth=True, areastyle_opts=opts.AreaStyleOpts(opacity=0.7),symbol_size=3, is_connect_nones = True, is_selected = True,is_clip=True,is_hover_animation=True)
    .set_global_opts(
        title_opts=opts.TitleOpts(title="进程个数实验"),
    xaxis_opts=opts.AxisOpts(
            type_="value",
            name="进 程 个 数",
            is_show=True,
            is_scale=True,
            name_location='middle',
            name_gap=25,
            # min_=1,
            max_=num+2,
            min_interval=1,
            max_interval=2,
            axislabel_opts=opts.LabelOpts(margin=5, color="black"),
            axisline_opts=opts.AxisLineOpts(is_show=True,is_on_zero=True,symbol=['none','arrow']),
            axistick_opts=opts.AxisTickOpts(
                is_show=True,
                length=200,
                is_inside=True,
                linestyle_opts=opts.LineStyleOpts(color="#ffffff1f"),
            ),
            # splitline_opts=opts.SplitLineOpts(
            #     is_show=True, linestyle_opts=opts.LineStyleOpts(color="#red")
            # )
        ),
        yaxis_opts=opts.AxisOpts(
            type_="value",
            name="运行时间(s)",
            is_show=True,
            is_scale=False,
            name_location='middle',
            name_gap=25,
            min_=0,
            # max_=10,
            min_interval=1,
            max_interval=2,
            axislabel_opts=opts.LabelOpts(margin=5, color="black"),
            axisline_opts=opts.AxisLineOpts(is_show=True,is_on_zero=True,symbol=['none','arrow'])
        ),
        legend_opts=opts.LegendOpts(type_='scroll',is_show=True,pos_right=190,pos_top=60)
        
    ).render("8.12.html")
)
