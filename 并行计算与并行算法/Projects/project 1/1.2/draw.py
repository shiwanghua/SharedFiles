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
y_ab = [0]
y_ak = [0]
y_ap = [0]
num = 50
with open("./log/ab.log", "r") as f:
    i = 0
    for line in f:
        i += 1
        p.append(i)
        y_ab.append(round(float(line), 2))
        if i == num:
            break
with open("./log/ak.log", "r") as f:
    i = 0
    for line in f:
        i += 1
        y_ak.append(round(float(line), 2))
        if i == num:
            break
with open("./log/ap.log", "r") as f:
    i = 0
    for line in f:
        i += 1
        y_ap.append(round(float(line), 2))
        if i == num:
            break

c = (
    Line(init_opts=opts.InitOpts(theme=ThemeType.ROMA))
    # theme=ThemeType.LIGHT INFOGRAPHIC ROMA WESTEROS
    .add_xaxis(p).add_yaxis(
        series_name="乘法",
        y_axis=y_ab,
        label_opts=opts.LabelOpts(font_size=5.0,
                                  is_show=True,
                                  position="top",
                                  color="#ff427f"),
        is_smooth=True,
        symbol_size=3,
        is_connect_nones=True,
        is_selected=True,
        is_clip=True,
        is_hover_animation=True,
        linestyle_opts=opts.LineStyleOpts(
            is_show=True,
            width=0.5,
            opacity=0.8,
            curve=0.5,
            type_="solid",
            color="#ff427f",
        ))  #areastyle_opts=opts.AreaStyleOpts(opacity=0.7),curve弯曲度
    .add_yaxis(
        series_name="卷积",
        y_axis=y_ak,
        is_smooth=True,
        #    color=Faker.rand_color(),
        label_opts=opts.LabelOpts(is_show=True,
                                  position="top",
                                  font_size=5.0,
                                  color="lightblue"),
        linestyle_opts=opts.LineStyleOpts(
            is_show=True,
            color="lightblue",
            opacity=1.0,
            width=0.5,
            curve=0.5,
            type_="solid")).add_yaxis(
                series_name="池化",
                y_axis=y_ap,
                is_smooth=True,
                # color=Faker.rand_color(),
                label_opts=opts.LabelOpts(is_show=True,
                                          position="bottom",
                                          color="#111111",
                                          font_size=5.0),
                linestyle_opts=opts.LineStyleOpts(is_show=True,
                                                  color="#111111",
                                                  opacity=0.8,
                                                  width=0.5,
                                                  curve=0.5,
                                                  type_="solid"),
            ).set_colors(["#ff427f", "lightblue", "#111111"])
    # .set_series_opts(
    #     axisline_opts=opts.AxisLineOpts(linestyle_opts=opts.LineStyleOpts(
    #         color=[[0.3, "#67e0e3"], [0.7, "#37a2da"], [1, "#fd666d"]],
    #         width=300)),
    #     label_opts=opts.LabelOpts(
    #         font_size=7.0, position="top", is_show=True))
    .set_global_opts(
        title_opts=opts.TitleOpts(title="进程个数实验", subtitle="1-50个进程"),
        xaxis_opts=opts.AxisOpts(
            type_="value",
            name="进 程 个 数",
            is_show=True,
            is_scale=True,
            name_location='middle',
            name_gap=20,
            # min_=1,
            max_=num,
            min_interval=1,
            max_interval=2,
            axislabel_opts=opts.LabelOpts(margin=5, color="#00508b"),
            axisline_opts=opts.AxisLineOpts(is_show=True,
                                            is_on_zero=True,
                                            symbol=['none', 'arrow']),
            axistick_opts=opts.AxisTickOpts(
                is_show=True,
                length=1000,
                is_inside=True,
                is_align_with_label=True,
                linestyle_opts=opts.LineStyleOpts(color="#ff1f1f1f"),
            ),  # 栅栏颜色
        ),
        yaxis_opts=opts.AxisOpts(
            type_="value",
            name="运 行 时 间 (s)",
            is_show=True,
            is_scale=False,
            name_location='middle',
            name_gap=18,
            min_=0,
            max_=10,
            min_interval=0.5,
            max_interval=2,
            axislabel_opts=opts.LabelOpts(formatter="{value}",
                                          margin=5,
                                          color="#044161"),
            axisline_opts=opts.AxisLineOpts(is_show=True,
                                            is_on_zero=True,
                                            symbol=['none', 'arrow'])),
        legend_opts=opts.LegendOpts(type_='scroll',
                                    is_show=True,
                                    pos_right=400,
                                    pos_top=70),
        tooltip_opts=opts.TooltipOpts(
            trigger="axis", axis_pointer_type="cross")  # 苗点功能 cross shadow
    ).render("./1.2.html")  #.overlap(line_mpi_p2)
)