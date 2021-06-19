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

epochNum = 10
epoch = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
epoch2 = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]

alexnet_b16_pool3_test = [0]
alexnet_b8_pool3_test = [0]
alexnet_b8_pool5_test = [
    0, 0.4928, 0.6316, 0.6965, 0.7283, 0.7288, 0.7879, 0.7821, 0.7802, 0.7888,
    0.7945
]
alexnet_b8_pool5_256_test = [
    0, 0.4748, 0.6453, 0.6889, 0.7281, 0.7525, 0.7821, 0.7713, 0.8103, 0.8061,
    0.806
]

alexnet_b16_pool3_train = [0]
alexnet_b8_pool3_train = [0]
alexnet_b8_pool5_train = [
    0, 0.31734, 0.56082, 0.66924, 0.72602, 0.7602, 0.78614, 0.80328, 0.82198,
    0.83388, 0.84338
]
alexnet_b8_pool5_256_train = [
    0, 0.3206, 0.5677, 0.68004, 0.73596, 0.76892, 0.79556, 0.81396, 0.82852,
    0.84294, 0.85476
]

alexnet_b8_loss = [11448, 7804, 6085, 5142, 4527, 4080, 3697, 3424, 3130, 2924]
alexnet_b16_loss = [6209, 4536, 3675, 3019, 2587, 2282, 2058, 1873, 1715, 1575]

zfnet_b8_loss = [11282, 7662, 5660, 4409, 3525, 2802, 2211, 1722, 1387, 1114]
zfnet_b16_loss = [6069, 4375, 3440, 2735, 2206, 1805, 1470, 1145, 886, 667]

zfnet_b16_pool3_test = [
    0,
]
zfnet_b16_pool5_test = [
    0, 0.4489, 0.5808, 0.6893, 0.7336, 0.7561, 0.7655, 0.7984, 0.8055, 0.7988,
    0.8078
]
zfnet_b8_pool5_test = [
    0, 0.4809, 0.6801, 0.6987, 0.7817, 0.792, 0.7715, 0.8122, 0.8109, 0.7917,
    0.7993
]

zfnet_b16_pool3_train = [0]
zfnet_b16_pool5_train = [
    0, 0.27722, 0.49976, 0.63258, 0.7194, 0.77244, 0.81462, 0.84676, 0.87642,
    0.89758, 0.919688
]
zfnet_b8_pool5_train = [
    0, 0.35188, 0.60246, 0.72276, 0.78304, 0.82586, 0.85836, 0.88206, 0.90022,
    0.91866, 0.92924
]

c1 = (
    Line(init_opts=opts.InitOpts(theme=ThemeType.ROMA))
    # theme=ThemeType.LIGHT INFOGRAPHIC ROMA WESTEROS
    .add_xaxis(epoch).add_yaxis(
        series_name="AlexNet训练集准确率",
        y_axis=alexnet_b8_pool5_256_train,
        label_opts=opts.LabelOpts(font_size=15.0,
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
            series_name="AlexNet测试集准确率",
            y_axis=alexnet_b8_pool5_256_test,
            is_smooth=True,
            #    color=Faker.rand_color(),
            label_opts=opts.LabelOpts(is_show=True,
                                      position="bottom",
                                      font_size=15.0,
                                      color="#00BFFF"),
            linestyle_opts=opts.LineStyleOpts(is_show=True,
                                              color="#00BFFF",
                                              opacity=1.0,
                                              width=0.5,
                                              curve=0.5,
                                              type_="solid"))
    #areastyle_opts=opts.AreaStyleOpts(opacity=0.7),curve弯曲度
    .set_colors(["#C71585", "#00BFFF"])  #"#111111"
    # .set_series_opts(
    #     axisline_opts=opts.AxisLineOpts(linestyle_opts=opts.LineStyleOpts(
    #         color=[[0.3, "#67e0e3"], [0.7, "#37a2da"], [1, "#fd666d"]],
    #         width=300)),
    #     label_opts=opts.LabelOpts(
    #         font_size=7.0, position="top", is_show=True))
    .set_global_opts(
        title_opts=opts.TitleOpts(title="AlexNet",
                                  subtitle="batch=8, pool=5,cn=256"),
        xaxis_opts=opts.AxisOpts(
            type_="value",
            name="训 练 轮 数",
            is_show=True,
            is_scale=True,
            name_location='middle',
            name_gap=20,
            min_=0,
            max_=epochNum,
            min_interval=0,
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
            name="准 确 率",
            is_show=True,
            is_scale=False,
            name_location='middle',
            name_gap=27,
            min_=0,
            max_=1,
            min_interval=0.1,
            max_interval=0.2,
            axislabel_opts=opts.LabelOpts(formatter="{value}",
                                          margin=5,
                                          color="#044161"),
            axisline_opts=opts.AxisLineOpts(is_show=True,
                                            is_on_zero=True,
                                            symbol=['none', 'arrow'])),
        legend_opts=opts.LegendOpts(type_='scroll',
                                    is_show=True,
                                    pos_right=400,
                                    pos_top=50),
        tooltip_opts=opts.TooltipOpts(
            trigger="axis", axis_pointer_type="cross")  # 苗点功能 cross shadow
    ).render("Pictures/alexnet.b8.p5.256.html")  #.overlap(line_mpi_p2)
)

c2 = (
    Line(init_opts=opts.InitOpts(theme=ThemeType.ROMA))
    # theme=ThemeType.LIGHT INFOGRAPHIC ROMA WESTEROS
    .add_xaxis(epoch).add_yaxis(
        series_name="ZFNet训练集准确率",
        y_axis=zfnet_b8_pool5_train,
        label_opts=opts.LabelOpts(font_size=14.0,
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
        series_name="ZFNet测试集准确率",
        y_axis=zfnet_b8_pool5_test,
        is_smooth=True,
        #    color=Faker.rand_color(),
        label_opts=opts.LabelOpts(is_show=True,
                                  position="left",
                                  font_size=14.0,
                                  color="#00BFFF"),
        linestyle_opts=opts.LineStyleOpts(
            is_show=True,
            color="#00BFFF",
            opacity=1.0,
            width=0.5,
            curve=0.5,
            type_="solid")).set_colors(["#ff427f", "#00BFFF"])  #"#111111"
    # .set_series_opts(
    #     axisline_opts=opts.AxisLineOpts(linestyle_opts=opts.LineStyleOpts(
    #         color=[[0.3, "#67e0e3"], [0.7, "#37a2da"], [1, "#fd666d"]],
    #         width=300)),
    #     label_opts=opts.LabelOpts(
    #         font_size=7.0, position="top", is_show=True))
    .set_global_opts(
        title_opts=opts.TitleOpts(title="ZFNet Accuracy",
                                  subtitle="batch=8, pool=5"),
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
            name="准 确 率",
            is_show=True,
            is_scale=False,
            name_location='middle',
            name_gap=27,
            min_=0,
            max_=1,
            min_interval=0.1,
            max_interval=0.2,
            axislabel_opts=opts.LabelOpts(formatter="{value}",
                                          margin=5,
                                          color="#044161"),
            axisline_opts=opts.AxisLineOpts(is_show=True,
                                            is_on_zero=True,
                                            symbol=['none', 'arrow'])),
        legend_opts=opts.LegendOpts(type_='scroll',
                                    is_show=True,
                                    pos_right=400,
                                    pos_top=50),
        tooltip_opts=opts.TooltipOpts(
            trigger="axis", axis_pointer_type="cross")  # 苗点功能 cross shadow
    ).render("Pictures/zfnet.b8.p5.html")  #.overlap(line_mpi_p2)
)

c3 = (
    Line(init_opts=opts.InitOpts(theme=ThemeType.ROMA))
    # theme=ThemeType.LIGHT INFOGRAPHIC ROMA WESTEROS
    .add_xaxis(epoch2).add_yaxis(
        series_name="Loss(batch=8)",
        y_axis=alexnet_b8_loss,
        label_opts=opts.LabelOpts(font_size=15.0,
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
            series_name="Loss(batch=16)",
            y_axis=alexnet_b16_loss,
            is_smooth=True,
            #    color=Faker.rand_color(),
            label_opts=opts.LabelOpts(is_show=True,
                                      position="right",
                                      font_size=15.0,
                                      color="#00BFFF"),
            linestyle_opts=opts.LineStyleOpts(is_show=True,
                                              color="#00BFFF",
                                              opacity=1.0,
                                              width=0.5,
                                              curve=0.5,
                                              type_="solid"))
    #areastyle_opts=opts.AreaStyleOpts(opacity=0.7),curve弯曲度
    .set_colors(["#C71585", "#00BFFF"])  #"#111111"
    # .set_series_opts(
    #     axisline_opts=opts.AxisLineOpts(linestyle_opts=opts.LineStyleOpts(
    #         color=[[0.3, "#67e0e3"], [0.7, "#37a2da"], [1, "#fd666d"]],
    #         width=300)),
    #     label_opts=opts.LabelOpts(
    #         font_size=7.0, position="top", is_show=True))
    .set_global_opts(
        title_opts=opts.TitleOpts(title="AlexNet Loss", subtitle="batch=8/16"),
        xaxis_opts=opts.AxisOpts(
            type_="value",
            name="训 练 轮 数",
            is_show=True,
            is_scale=True,
            name_location='middle',
            name_gap=20,
            min_=0,
            max_=9,
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
            name="Loss",
            is_show=True,
            is_scale=True,
            name_location='middle',
            name_gap=40,
            min_=1500,
            max_=11500,
            min_interval=1000,
            max_interval=2000,
            axislabel_opts=opts.LabelOpts(formatter="{value}",
                                          margin=5,
                                          color="#044161"),
            axisline_opts=opts.AxisLineOpts(is_show=True,
                                            is_on_zero=True,
                                            symbol=['none', 'arrow'])),
        legend_opts=opts.LegendOpts(type_='scroll',
                                    is_show=True,
                                    pos_right=400,
                                    pos_top=50),
        tooltip_opts=opts.TooltipOpts(
            trigger="axis", axis_pointer_type="cross")  # 苗点功能 cross shadow
    ).render("Pictures/alexnet.loss.html")  #.overlap(line_mpi_p2)
)

c3 = (
    Line(init_opts=opts.InitOpts(theme=ThemeType.ROMA))
    # theme=ThemeType.LIGHT INFOGRAPHIC ROMA WESTEROS
    .add_xaxis(epoch).add_yaxis(series_name="Loss(batch=8)",
                                y_axis=zfnet_b8_loss,
                                label_opts=opts.LabelOpts(font_size=15.0,
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
                                    series_name="Loss(batch=16)",
                                    y_axis=zfnet_b16_loss,
                                    is_smooth=True,
                                    #    color=Faker.rand_color(),
                                    label_opts=opts.LabelOpts(is_show=True,
                                                              position="right",
                                                              font_size=15.0,
                                                              color="#00BFFF"),
                                    linestyle_opts=opts.LineStyleOpts(
                                        is_show=True,
                                        color="#00BFFF",
                                        opacity=1.0,
                                        width=0.5,
                                        curve=0.5,
                                        type_="solid"))
    #areastyle_opts=opts.AreaStyleOpts(opacity=0.7),curve弯曲度
    .set_colors(["#C71585", "#00BFFF"])  #"#111111"
    # .set_series_opts(
    #     axisline_opts=opts.AxisLineOpts(linestyle_opts=opts.LineStyleOpts(
    #         color=[[0.3, "#67e0e3"], [0.7, "#37a2da"], [1, "#fd666d"]],
    #         width=300)),
    #     label_opts=opts.LabelOpts(
    #         font_size=7.0, position="top", is_show=True))
    .set_global_opts(
        title_opts=opts.TitleOpts(title="ZFNet Loss", subtitle="batch=8/16"),
        xaxis_opts=opts.AxisOpts(
            type_="value",
            name="训 练 轮 数",
            is_show=True,
            is_scale=True,
            name_location='middle',
            name_gap=20,
            min_=0,
            max_=9,
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
            name="Loss",
            is_show=True,
            is_scale=False,
            name_location='middle',
            name_gap=40,
            min_=500,
            max_=11500,
            min_interval=1000,
            max_interval=2000,
            axislabel_opts=opts.LabelOpts(formatter="{value}",
                                          margin=5,
                                          color="#044161"),
            axisline_opts=opts.AxisLineOpts(is_show=True,
                                            is_on_zero=True,
                                            symbol=['none', 'arrow'])),
        legend_opts=opts.LegendOpts(type_='scroll',
                                    is_show=True,
                                    pos_right=400,
                                    pos_top=50),
        tooltip_opts=opts.TooltipOpts(
            trigger="axis", axis_pointer_type="cross")  # 苗点功能 cross shadow
    ).render("Pictures/zfnet.loss.html")  #.overlap(line_mpi_p2)
)