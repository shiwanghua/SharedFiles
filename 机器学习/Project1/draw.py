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

epochNum = 10
epoch = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10]

alexnet_b16_test_small = [
    0, 18.5, 18.5, 18.5, 18.5, 18.5, 18.5, 18.5, 30.8, 24.0, 31.4
]
alexnet_b16_pool5_test_small = [
    0, 18.6, 18.5, 18.5, 18.5, 18.5, 24.9, 33.2, 37.3, 44.2, 50.2
]
alexnet_b16_test = [
    0, 24.5, 70.0, 80.0, 84.7, 87.7, 89.4, 89.3, 90.0, 91.1, 91.3
]
alexnet_b16_256_test = [
    0, 19.6, 68.8, 82.0, 85.4, 88.2, 89.6, 90.4, 90.7, 91.5, 91.4
]
alexnet_b8_test = [
    0, 45.3, 81.3, 85.8, 89.1, 90.0, 90.6, 91.3, 91.4, 91.6, 91.7
]

c1 = (
    Line(init_opts=opts.InitOpts(theme=ThemeType.ROMA))
    # theme=ThemeType.LIGHT INFOGRAPHIC ROMA WESTEROS
    .add_xaxis(epoch).add_yaxis(
        series_name="B16_miniData",
        y_axis=alexnet_b16_test_small,
        label_opts=opts.LabelOpts(font_size=6.0,
                                  is_show=True,
                                  position="top",
                                  color="#C71585"),
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
            color="#C71585",
        )).add_yaxis(
            series_name="B16_p5_miniData",
            y_axis=alexnet_b16_pool5_test_small,
            is_smooth=True,
            #    color=Faker.rand_color(),
            label_opts=opts.LabelOpts(is_show=True,
                                      position="bottom",
                                      font_size=6.0,
                                      color="#00BFFF"),
            linestyle_opts=opts.LineStyleOpts(
                is_show=True,
                color="#00BFFF",
                opacity=1.0,
                width=0.5,
                curve=0.5,
                type_="solid")).add_yaxis(
                    series_name="B16_allData",
                    y_axis=alexnet_b16_test,
                    is_smooth=True,
                    #    color=Faker.rand_color(),
                    label_opts=opts.LabelOpts(is_show=True,
                                              position="right",
                                              font_size=6.0,
                                              color="#111111"),
                    linestyle_opts=opts.LineStyleOpts(
                        is_show=True,
                        color="#111111",
                        opacity=1.0,
                        width=0.5,
                        curve=0.5,
                        type_="solid")).add_yaxis(
                            series_name="B16_256_allData",
                            y_axis=alexnet_b16_256_test,
                            is_smooth=True,
                            #    color=Faker.rand_color(),
                            label_opts=opts.LabelOpts(is_show=True,
                                                      position="bottom",
                                                      font_size=6.0,
                                                      color="#BA55D3"),
                            linestyle_opts=opts.LineStyleOpts(
                                is_show=True,
                                color="#BA55D3",
                                opacity=1.0,
                                width=0.5,
                                curve=0.5,
                                type_="solid")).add_yaxis(
                                    series_name="B8_allData",
                                    y_axis=alexnet_b8_test,
                                    is_smooth=True,
                                    #    color=Faker.rand_color(),
                                    label_opts=opts.LabelOpts(is_show=True,
                                                              position="top",
                                                              font_size=6.0,
                                                              color="#FF8C00"),
                                    linestyle_opts=opts.LineStyleOpts(
                                        is_show=True,
                                        color="#FF8C00",
                                        opacity=1.0,
                                        width=0.5,
                                        curve=0.5,
                                        type_="solid"))
    #areastyle_opts=opts.AreaStyleOpts(opacity=0.7),curve弯曲度
    .set_colors(["#C71585", "#00BFFF", "#111111", "#BA55D3", "#FF8C00"])  #
    # .set_series_opts(
    #     axisline_opts=opts.AxisLineOpts(linestyle_opts=opts.LineStyleOpts(
    #         color=[[0.3, "#67e0e3"], [0.7, "#37a2da"], [1, "#fd666d"]],
    #         width=300)),
    #     label_opts=opts.LabelOpts(
    #         font_size=7.0, position="top", is_show=True))
    .set_global_opts(
        title_opts=opts.TitleOpts(title="AlexNet",
                                  subtitle="Test set accuracy"),
        xaxis_opts=opts.AxisOpts(
            type_="value",
            name="训 练 轮 数",
            is_show=True,
            is_scale=True,
            name_location='middle',
            name_gap=20,
            min_=0,
            max_=epochNum,
            min_interval=1,
            max_interval=1,
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
            name="测 试 集 准 确 率",
            is_show=True,
            is_scale=False,
            name_location='middle',
            name_gap=27,
            min_=0,
            max_=100,
            min_interval=10,
            max_interval=20,
            axislabel_opts=opts.LabelOpts(formatter="{value}",
                                          margin=5,
                                          color="#044161"),
            axisline_opts=opts.AxisLineOpts(is_show=True,
                                            is_on_zero=True,
                                            symbol=['none', 'arrow'])),
        legend_opts=opts.LegendOpts(type_='scroll',
                                    is_show=True,
                                    pos_right=170,
                                    pos_top=50),
        tooltip_opts=opts.TooltipOpts(
            trigger="axis", axis_pointer_type="cross")  # 苗点功能 cross shadow
    )  #.render("out/AlexnetAccuracy.html")  #.overlap(line_mpi_p2)
)

make_snapshot(snapshot, c1.render(), "out/AlexnetAccuracy.png")
# c2 = (
#     Line(init_opts=opts.InitOpts(theme=ThemeType.ROMA))
#     # theme=ThemeType.LIGHT INFOGRAPHIC ROMA WESTEROS
#     .add_xaxis(epoch).add_yaxis(
#         series_name="ZFNet训练集准确率",
#         y_axis=zfnet_b8_pool5_train,
#         label_opts=opts.LabelOpts(font_size=14.0,
#                                   is_show=True,
#                                   position="top",
#                                   color="#ff427f"),
#         is_smooth=True,
#         symbol_size=3,
#         is_connect_nones=True,
#         is_selected=True,
#         is_clip=True,
#         is_hover_animation=True,
#         linestyle_opts=opts.LineStyleOpts(
#             is_show=True,
#             width=0.5,
#             opacity=0.8,
#             curve=0.5,
#             type_="solid",
#             color="#ff427f",
#         ))  #areastyle_opts=opts.AreaStyleOpts(opacity=0.7),curve弯曲度
#     .add_yaxis(
#         series_name="ZFNet测试集准确率",
#         y_axis=zfnet_b8_pool5_test,
#         is_smooth=True,
#         #    color=Faker.rand_color(),
#         label_opts=opts.LabelOpts(is_show=True,
#                                   position="bottom",
#                                   font_size=14.0,
#                                   color="#008080"),
#         linestyle_opts=opts.LineStyleOpts(
#             is_show=True,
#             color="#008080",
#             opacity=1.0,
#             width=0.5,
#             curve=0.5,
#             type_="solid")).set_colors(["#ff427f", "#008080"])  #"#111111"
#     # .set_series_opts(
#     #     axisline_opts=opts.AxisLineOpts(linestyle_opts=opts.LineStyleOpts(
#     #         color=[[0.3, "#67e0e3"], [0.7, "#37a2da"], [1, "#fd666d"]],
#     #         width=300)),
#     #     label_opts=opts.LabelOpts(
#     #         font_size=7.0, position="top", is_show=True))
#     .set_global_opts(
#         title_opts=opts.TitleOpts(title="ZFNet", subtitle="batch=8, pool=5"),
#         xaxis_opts=opts.AxisOpts(
#             type_="value",
#             name="训 练 轮 数",
#             is_show=True,
#             is_scale=True,
#             name_location='middle',
#             name_gap=20,
#             min_=0,
#             max_=epochNum,
#             min_interval=1,
#             max_interval=1,
#             axislabel_opts=opts.LabelOpts(margin=5, color="#00508b"),
#             axisline_opts=opts.AxisLineOpts(is_show=True,
#                                             is_on_zero=True,
#                                             symbol=['none', 'arrow']),
#             axistick_opts=opts.AxisTickOpts(
#                 is_show=True,
#                 length=1000,
#                 is_inside=True,
#                 is_align_with_label=True,
#                 linestyle_opts=opts.LineStyleOpts(color="#ff1f1f1f"),
#             ),  # 栅栏颜色
#         ),
#         yaxis_opts=opts.AxisOpts(
#             type_="value",
#             name="准 确 率",
#             is_show=True,
#             is_scale=False,
#             name_location='middle',
#             name_gap=27,
#             min_=0,
#             max_=1,
#             min_interval=0.1,
#             max_interval=0.2,
#             axislabel_opts=opts.LabelOpts(formatter="{value}",
#                                           margin=5,
#                                           color="#044161"),
#             axisline_opts=opts.AxisLineOpts(is_show=True,
#                                             is_on_zero=True,
#                                             symbol=['none', 'arrow'])),
#         legend_opts=opts.LegendOpts(type_='scroll',
#                                     is_show=True,
#                                     pos_right=400,
#                                     pos_top=50),
#         tooltip_opts=opts.TooltipOpts(
#             trigger="axis", axis_pointer_type="cross")  # 苗点功能 cross shadow
#     ).render("Pictures/zfnet.b8.p5.html")  #.overlap(line_mpi_p2)
# )