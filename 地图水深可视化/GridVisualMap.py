# -*- coding: utf-8 -*-
# coding: utf-8
import folium
import pandas as pd
import os
from math import *
import seaborn as sns;
import geopy.distance

import re
def LngLatToNumber(s): 
    longitude_split = re.split(u"°|′|″", s)[:3]
    if len(longitude_split) == 3:
        x = [float(j) for j in longitude_split]
        return x[0] + x[1] / 60 + x[2] / 3600

data = pd.read_csv('./hebing.txt')
data.head()
# data.size
# data=data.iloc[:3000,0:3]
# 交换列
temp = data.loc[:, 'shuidigaocheng']
data = data.drop(labels=['shuidigaocheng'], axis=1)
data.insert(2, 'shuidigaocheng', temp)

r=int(data.size/3) # 3 列数据，r 是行数
for i in range(r):
    data.iloc[i,0],data.iloc[i,1],data.iloc[i,2]=LngLatToNumber(data.iloc[i,0]),LngLatToNumber(data.iloc[i,1]),data.iloc[i,2]
print('点数：'+str(len(data)))
data.head()

# 输入四个角上的点，纬度在前，经度在后, 深度不需要
top_left=[31.291057,121.60121,-1.544]
bottom_left=[31.29054,121.601035,-1.085]
top_right=[31.289920271111114,121.605971625,-1.995]
bottom_right=[31.28946,121.60580169805555,-1.626]
basePoint = bottom_left # 以左下角的点为基准点
UNKNOWN_DEEPTH = -100

deepColor='#0086cc'
lightColor='deepskyblue'
blackColor='black'
gmap = folium.Map(location=[bottom_right[0],bottom_right[1]], zoom_start=18) # 地图对象

def drawAPoint(p): # 画一个点
    folium.CircleMarker(radius = 0.1,location=[p[0],p[1]], color=lightColor,fill_color=blackColor,line_opacity = 1,fill_opacity=1,  tooltip=  str(p[0])+ ' ' + str(p[1])+' '+str(p[2])).add_to(gmap)

# 计算左边宽度
left_width = geopy.distance.geodesic(top_left[:2], bottom_left[:2]).km*1000
# 计算右边宽度（没用到）
right_width = geopy.distance.geodesic(top_right[:2], bottom_right[:2]).km*1000 
# 计算上面长度（没用到）
top_length = geopy.distance.geodesic(top_left[:2], top_right[:2]).km*1000
# 计算下面长度
bottom_length = geopy.distance.geodesic(bottom_left[:2], bottom_right[:2]).km*1000

print('四条边长：'+str(left_width)+', '+str(right_width)+', '+str(top_length)+', '+str(bottom_length))
# 后面以 left_width、top_length 为矩形的宽和长构建正方形网格

w = 1.0 # 正方形网格的边长，单位是米
nx = int(top_length//w)  # 横向网格数（Integer）
ny = int(left_width//w)  # 纵向网格数（Integer）
print('横向网格数：'+str(nx)+', 纵向网格数：'+str(ny))

grid=[[[]]*ny for i in range(nx)] # nx行，ny列的网格，每个网格用一个ArrayList表示
for i in range(nx):
    for j in range(ny):
        grid[i][j]=[]
gridDeepth = [[0]*ny for i in range(nx)] # nx行，ny列的二维数组，每个值表示该网格的深度

# 粗过滤，过滤出经度在 (min(top_left[1],bottom_left[1]), max(top_right[1],bottom_right[1])) 范围内、
# 维度在 (min(bottom_left[0],bottom_right[0]),max(top_left[0],top_right[0])) 范围内的点
data=data[(min(top_left[1],bottom_left[1])<=data['lon'])&(data['lon']<= max(top_right[1],bottom_right[1])) &
          (min(bottom_left[0],bottom_right[0])<=data['lat'])&(data['lat']<=max(top_left[0],top_right[0])) ].reset_index(drop=True)
# data=data[min(top_left[1],bottom_left[1])<=data['lon']].reset_index(drop=True)
# data=data[max(top_right[1],bottom_right[1])>=data['lon']].reset_index(drop=True)
# data=data[min(bottom_left[0],bottom_right[0])<=data['lat']].reset_index(drop=True)
# data=data[max(top_left[0],top_right[0])>=data['lat']].reset_index(drop=True)
print('简单过滤后剩下'+str(len(data))+'个可能位于网格内的点要处理')

# 公式，浮点数除法
dx1,dy1 = (bottom_right[1]-bottom_left[1])/nx, (bottom_right[0]-bottom_left[0])/nx
dx2,dy2 = (top_left[1]-bottom_left[1])/ny, (top_left[0]-bottom_left[0])/ny
print('dx1='+str(dx1)+', dy1='+str(dy1))
print('dx2='+str(dx2)+', dy2='+str(dy2))
p1 = [31.29053, 121.6055] # 随便一个用于测试的点
p2 = [p1[0]+dy1+dy2,p1[1]+dx1+dx2]
print('网格的对角线长：'+str(geopy.distance.geodesic(p1[:2], p2[:2]).km*1000)+'米') # 测试网格的对角线距离计算

xit,yit=230,20 # 测试这个网格（Unused）
# print('处理前网格有'+str(len(grid[xit][yit]))+'个点')
for i,v in enumerate(data.iloc[:, 0: 3].values): # 遍历每一个GPS点 v(纬度，经度，深度)
    # 公式，浮点数除法，最后向下取整
    # 当前点位于 xi 行 yi 列网格中
    yi=int((v[0]-dy1/dx1*v[1]-basePoint[0]+basePoint[1]*dy1/dx1)//(dy2-dx2*dy1/dx1)) # // 表示向下取整
    xi=int((v[1]-yi*dx2-basePoint[1])//dx1)
    # print(v)
    # print('第'+str(xi)+'行，第'+str(yi)+'列')
    if xi<0 or xi>=nx or yi<0 or yi>=ny:
        # print("Not in grid area") # 细过滤，不在网格内
        continue
    grid[xi][yi].append(v[2]) # 在网格内，加入网格数组ArrayList

# print('处理后网格有'+str(len(grid[xit][yit]))+'个点')
# print(grid[xit][yit])

for i in range(nx):
    for j in range(ny):
        # i,j=xit,yit
        if len(grid[i][j])>0: # 网格内有点
            gridDeepth[i][j]=max(grid[i][j]) #/len(grid[i][j]) # 取最大值作为这一点的深度
        else:
            gridDeepth[i][j]=UNKNOWN_DEEPTH
        # print('第'+str(i)+'行，第'+str(j)+'列网格深度为 '+str(gridDeepth[i][j]))

# 插 值 
direction = [[-1,-1],[0,-1],[1,-1],[-1,0],[1,0],[-1,1],[0,1],[1,1]] # [dy,dx] 八个方向
gni=0
for i in range(nx):
    for j in range(ny):
        if gridDeepth[i][j]!=UNKNOWN_DEEPTH: # 深度已知的不需要插值
            continue
        # BFS
        bigSum,bigCnt=0.0,0 # 大于-1的深度的和、个数
        smallSum,smallCnt=0.0,0 # 小于等于-1的深度的和、个数
        for k in range(8):
            xi=i+direction[k][1]
            yi=j+direction[k][0]
            if 0<=xi<nx and 0<=yi<ny and gridDeepth[xi][yi]!=UNKNOWN_DEEPTH:
                if gridDeepth[xi][yi]>-1:
                    bigSum+=gridDeepth[xi][yi]
                    bigCnt+=1
                else:
                    smallSum+=gridDeepth[xi][yi]
                    smallCnt+=1
        if bigCnt==0:
            if smallCnt==0:
                print('No KNOWN GRID：',i,j) # 很少出现这种情况，只需保证左下角附近有GPS点
                gridDeepth[i][j]=-1
            else:
                gridDeepth[i][j]=smallSum/smallCnt
        else:
            if smallCnt==0:
                gridDeepth[i][j]=bigSum/bigCnt
            else:
                gridDeepth[i][j]=(bigSum/bigCnt+smallSum/smallCnt)/2
        gni+=1
        # print('第'+str(gni)+'个网格 ('+str(i)+','+str(j)+') 被插值为 '+str(gridDeepth[i][j]))
        
# 染 色
for i in range(nx):
    for j in range(ny):
        # i,j=xit,yit
        if gridDeepth[i][j]>-1: # 对深度大于-1的网格染色
            p_bottom_left = [basePoint[0]+dy1*i+dy2*j, basePoint[1]+dx1*i+dx2*j] # 网格左下角纬经度
            p_top_right = [p_bottom_left[0]+dy1+dy2, p_bottom_left[1]+dx1+dx2]   # 网格右上角纬经度
            folium.Rectangle(bounds = [p_bottom_left, p_top_right],color=deepColor,fill_color=deepColor, line_opacity = 1,fill_opacity=1, tooltip=  str(p_bottom_left[0])+ ' ' + str(p_bottom_left[1]) + ' ' + str(gridDeepth[i][j])).add_to(gmap)
        # else:
        #     p_bottom_left = [basePoint[0]+dy1*i+dy2*j, basePoint[1]+dx1*i+dx2*j,gridDeepth[i][j]]
        #     p_top_right = [p_bottom_left[0]+dy1+dy2, p_bottom_left[1]+dx1+dx2,gridDeepth[i][j]]
        #     drawAPoint(p_bottom_left)
        #     folium.Rectangle(bounds = [p_bottom_left, p_top_right],color=lightColor,fill_color=lightColor, line_opacity = 1,fill_opacity=1, tooltip=  str(p_bottom_left[0])+ ' ' + str(p_bottom_left[1]) + ' ' + str(gridDeepth[i][j])).add_to(gmap)

# 画四个角上的点
folium.CircleMarker(radius = 0.1,location=[top_left[0],top_left[1]], color=blackColor,fill_color=blackColor,line_opacity = 1,fill_opacity=1,  tooltip=  str(top_left[0])+ ' ' + str(top_left[1])).add_to(gmap)
folium.CircleMarker(radius = 0.1,location=[bottom_left[0],bottom_left[1]], color=blackColor,fill_color=blackColor,line_opacity = 1,fill_opacity=1,  tooltip=  str(bottom_left[0])+ ' ' + str(bottom_left[1])).add_to(gmap)
folium.CircleMarker(radius = 0.1,location=[top_right[0],top_right[1]], color=blackColor,fill_color=blackColor,line_opacity = 1,fill_opacity=1,  tooltip=  str(top_right[0])+ ' ' + str(top_right[1])).add_to(gmap)
folium.CircleMarker(radius = 0.1,location=[p4[0],p4[1]], color=blackColor,fill_color=blackColor,line_opacity = 1,fill_opacity=1,  tooltip=  str(bottom_right[0])+ ' ' + str(bottom_right[1])).add_to(gmap)

# 四条边
gridColor = blackColor
folium.PolyLine(locations = [top_left[:2],bottom_left[:2]],color=gridColor,fill_color=gridColor, line_opacity = 1).add_to(gmap)
folium.PolyLine(locations = [top_left[:2],top_right[:2]],color=gridColor,fill_color=gridColor, line_opacity = 1).add_to(gmap)
folium.PolyLine(locations = [bottom_left[:2],bottom_right[:2]],color=gridColor,fill_color=gridColor, line_opacity = 1).add_to(gmap)
folium.PolyLine(locations = [top_right[:2],bottom_right[:2]],color=gridColor,fill_color=gridColor, line_opacity = 1).add_to(gmap)

gmap.save('水深.html')
print('DONE.')