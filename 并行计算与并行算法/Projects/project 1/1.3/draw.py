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

# 1.3.bf.nf.log 1.3.ba.nf.log 1.3.su.nf.log 1.3.ss.nf.log
# 1.3.bf.f.log 1.3.ba.f.log 1.3.su.f.log 1.3.ss.f.log
num = 50
times = 1000
digit = 0
p = [0]
y_bfnf = [0]
y_banf = [0]
y_sunf = [0]
y_ssnf = [0]

y_bff = [0]
y_baf = [0]
y_suf = [0]
y_ssf = [0]

with open("./log/1.3.bf.nf.log", "r") as f:
    i = 0
    for line in f:
        i += 1
        p.append(i)
        y_bfnf.append(round(float(line) * times, digit))
        if i == num:
            break
with open("./log/1.3.ba.nf.log", "r") as f:
    i = 0
    for line in f:
        i += 1
        y_banf.append(round(float(line) * times, digit))
        if i == num:
            break
with open("./log/1.3.su.nf.log", "r") as f:
    i = 0
    for line in f:
        i += 1
        y_sunf.append(round(float(line) * times, digit))
        if i == num:
            break
with open("./log/1.3.ss.nf.log", "r") as f:
    i = 0
    for line in f:
        i += 1
        y_ssnf.append(round(float(line) * times, digit))
        if i == num:
            break
with open("./log/1.3.bf.f.log", "r") as f:
    i = 0
    for line in f:
        i += 1
        y_bff.append(round(float(line) * times, digit))
        if i == num:
            break

with open("./log/1.3.ba.f.log", "r") as f:
    i = 0
    for line in f:
        i += 1
        y_baf.append(round(float(line) * times, digit))
        if i == num:
            break
with open("./log/1.3.su.f.log", "r") as f:
    i = 0
    for line in f:
        i += 1
        y_suf.append(round(float(line) * times, digit))
        if i == num:
            break
with open("./log/1.3.ss.f.log", "r") as f:
    i = 0
    for line in f:
        i += 1
        y_ssf.append(round(float(line) * times, digit))
        if i == num:
            break

c1 = (
    Line(init_opts=opts.InitOpts(theme=ThemeType.ROMA))
    # theme=ThemeType.LIGHT INFOGRAPHIC ROMA WESTEROS
    .add_xaxis(p).add_yaxis(
        series_name="大文件-固定长度轮询分发",
        y_axis=y_bff,
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
        series_name="大文件-进程平均单轮分发",
        y_axis=y_baf,
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
            type_="solid")).set_colors(["#ff427f", "lightblue"])  #"#111111"
    # .set_series_opts(
    #     axisline_opts=opts.AxisLineOpts(linestyle_opts=opts.LineStyleOpts(
    #         color=[[0.3, "#67e0e3"], [0.7, "#37a2da"], [1, "#fd666d"]],
    #         width=300)),
    #     label_opts=opts.LabelOpts(
    #         font_size=7.0, position="top", is_show=True))
    .set_global_opts(
        title_opts=opts.TitleOpts(title="大文件计数进程个数实验", subtitle="定长分发/动态分发"),
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
            name="运 行 时 间 (ms)",
            is_show=True,
            is_scale=False,
            name_location='middle',
            name_gap=27,
            min_=0,
            max_=300,
            min_interval=30,
            max_interval=50,
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
    ).render("pic/1.3.b.f.html")  #.overlap(line_mpi_p2)
)

c2 = (
    Line(init_opts=opts.InitOpts(theme=ThemeType.ROMA))
    # theme=ThemeType.LIGHT INFOGRAPHIC ROMA WESTEROS
    .add_xaxis(p).add_yaxis(
        series_name="小文件-按原始顺序分发",
        y_axis=y_suf,
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
        series_name="小文件-按文本大小分发",
        y_axis=y_ssf,
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
            type_="solid")).set_colors(["#ff427f", "lightblue"])  #"#111111"
    # .set_series_opts(
    #     axisline_opts=opts.AxisLineOpts(linestyle_opts=opts.LineStyleOpts(
    #         color=[[0.3, "#67e0e3"], [0.7, "#37a2da"], [1, "#fd666d"]],
    #         width=300)),
    #     label_opts=opts.LabelOpts(
    #         font_size=7.0, position="top", is_show=True))
    .set_global_opts(
        title_opts=opts.TitleOpts(title="小文件计数进程个数实验", subtitle="不排序/排序"),
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
            name="运 行 时 间 (ms)",
            is_show=True,
            is_scale=False,
            name_location='middle',
            name_gap=27,
            min_=150,
            max_=850,
            min_interval=35,
            max_interval=50,
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
    ).render("pic/1.3.s.f.html")  #.overlap(line_mpi_p2)
)

c3 = (
    Line(init_opts=opts.InitOpts(theme=ThemeType.ROMA))
    # theme=ThemeType.LIGHT INFOGRAPHIC ROMA WESTEROS
    .add_xaxis(p).add_yaxis(
        series_name="单机运行",
        y_axis=y_bfnf,
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
        series_name="集群运行",
        y_axis=y_bff,
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
            type_="solid")).set_colors(["#ff427f", "lightblue"])  #"#111111"
    # .set_series_opts(
    #     axisline_opts=opts.AxisLineOpts(linestyle_opts=opts.LineStyleOpts(
    #         color=[[0.3, "#67e0e3"], [0.7, "#37a2da"], [1, "#fd666d"]],
    #         width=300)),
    #     label_opts=opts.LabelOpts(
    #         font_size=7.0, position="top", is_show=True))
    .set_global_opts(
        title_opts=opts.TitleOpts(title="单机/集群实验1", subtitle="大文件定长分发文本"),
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
            name="运 行 时 间 (ms)",
            is_show=True,
            is_scale=False,
            name_location='middle',
            name_gap=27,
            # min_=0,
            # max_=350,
            min_=0,
            max_=300,
            min_interval=30,
            max_interval=50,
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
    ).render("pic/1.3.bf.fnf.html")  #.overlap(line_mpi_p2)
)

columns = ["Big_Fixed", "Big_Avg", "Small_Unsort", "Small_Sorted"]
y_nf = []
y_f = []

with open("./log/1.3.fnf.log ", "r") as f:
    i = 0
    for line in f:
        y_nf.append(round(float(line) * 1000, 0))
        i += 1
        if i == 4:
            break
    for line in f:
        y_f.append(round(float(line) * 1000, 0))

bar = (
    Bar(init_opts=opts.InitOpts(theme=ThemeType.ESSOS)
        )  # theme=ThemeType.LIGHT INFOGRAPHIC ROMA WESTEROS
    .add_xaxis(columns).add_yaxis("单机运行", y_nf).add_yaxis(
        "集群运行", y_f).set_series_opts(label_opts=opts.LabelOpts(
            font_size=8.0, position="top", is_show=True)).set_global_opts(
                title_opts=opts.TitleOpts(title="单机/集群实验2", subtitle="固定4个进程"),
                xaxis_opts=opts.AxisOpts(
                    type_="category",
                    name="四 种 单 词 计 数",
                    is_show=True,
                    is_scale=True,
                    name_location='middle',
                    name_gap=25,
                    # min_=1,
                    max_=4,
                    min_interval=1,
                    max_interval=1,
                    axislabel_opts=opts.LabelOpts(margin=5, color="black"),
                    axisline_opts=opts.AxisLineOpts(is_show=True,
                                                    is_on_zero=True,
                                                    symbol=['none', 'arrow']),
                    axistick_opts=opts.AxisTickOpts(
                        is_show=True,
                        length=250,
                        is_inside=True,
                        linestyle_opts=opts.LineStyleOpts(color="#ffffff1f"),
                    ),
                ),
                yaxis_opts=opts.AxisOpts(
                    type_="value",
                    name="运 行 时 间 (ms)",
                    is_show=True,
                    is_scale=False,
                    name_location='middle',
                    name_gap=38,
                    min_=0,
                    max_=300,
                    min_interval=30,
                    max_interval=50,
                    axislabel_opts=opts.LabelOpts(margin=5, color="black"),
                    axisline_opts=opts.AxisLineOpts(is_show=True,
                                                    is_on_zero=True,
                                                    symbol=['none', 'arrow'])),
                legend_opts=opts.LegendOpts(
                    type_='scroll', is_show=True, pos_right=500,
                    pos_top=60)).render("./pic/1.3.fnf.html"))
