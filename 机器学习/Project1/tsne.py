# coding: utf-8
import scipy.io as io
import numpy as np
import math
import torch
import torchvision
import torchvision.transforms as transforms
import matplotlib.pyplot as plt

plt.gray()
import matplotlib.patheffects as pe
import seaborn as sb
from sklearn.manifold import TSNE
from PIL import Image as img
from mpl_toolkits.mplot3d import Axes3D  # 进行3D图像绘制
# %matplotlib inline
# from IPython.display import display
# infinity = float(-2**31)

# transform = transforms.Compose([
#     transforms.ToTensor(),
#     transforms.Normalize((0.5, 0.5, 0.5), (0.5, 0.5, 0.5))
# ])
# train_data = torchvision.datasets.SVHN(root='data',
#                                        split='train',
#                                        transform=transform,
#                                        download=False)
# test_data = torchvision.datasets.SVHN(root='data',
#                                       split='test',
#                                       transform=transform,
#                                       download=False)
# train_loader = torch.utils.data.DataLoader(train_data,
#                                            batch_size=50000,
#                                            shuffle=True)
# test_loader = torch.utils.data.DataLoader(test_data,
#                                           batch_size=10000,
#                                           shuffle=True)

n_classes = 10


def plot2D(x, colors, path_name):
    # 生成10种颜色出来
    palette = np.array(sb.color_palette("hls", n_classes))
    #     print(palette.shape)

    f = plt.figure(figsize=(32, 32))
    ax = plt.subplot(aspect='equal')
    # 将第一维和第二维作为xy坐标
    # lw: linewidth

    # set lable
    ax.set_xlabel('X', fontsize=10)
    ax.set_ylabel('Y', fontsize=10)

    sc = ax.scatter(x[:, 0],
                    x[:, 1],
                    lw=0,
                    s=40,
                    cmap="jet",
                    marker="o",
                    c=palette[colors.astype(np.int32)])
    txts = []
    for i in range(n_classes):
        # Position of each label.
        xtext, ytext = np.median(x[colors == i, :], axis=0)
        txt = ax.text(xtext, ytext, str(i), fontsize=26)
        txt.set_path_effects(
            [pe.Stroke(linewidth=7, foreground="w"),
             pe.Normal()])
        txts.append(txt)
    plt.title(path_name, fontsize=38, color='black', backgroundcolor='white')
    plt.savefig(path_name)
    #     plt.axis('on')
    #     plt.show()
    return f, ax, txts


def plot3D(x, colors, path_name):
    # 生成10种颜色出来
    palette = np.array(sb.color_palette("hls", n_classes))
    #     print(palette.shape)

    f = plt.figure(figsize=(32, 32))
    #     ax = plt.subplot(aspect='equal')
    ax = Axes3D(f)
    # 将第一维和第二维作为xy坐标
    # lw: linewidth

    # set lable
    ax.set_xlabel('X', fontsize=10)
    ax.set_ylabel('Y', fontsize=10)
    ax.set_zlabel('Z', fontsize=10)

    sc = ax.scatter(x[:, 0],
                    x[:, 1],
                    x[:, 2],
                    lw=0,
                    s=40,
                    cmap="jet",
                    marker="o",
                    c=palette[colors.astype(np.int32)])
    txts = []
    for i in range(n_classes):
        # Position of each label.
        xtext, ytext, ztext = np.median(x[colors == i, :], axis=0)
        txt = ax.text(xtext, ytext, ztext, str(i), fontsize=32)
        txt.set_path_effects(
            [pe.Stroke(linewidth=5, foreground="w"),
             pe.Normal()])
        txts.append(txt)
    plt.title('Cifar10-tSNE',
              fontsize=38,
              color='black',
              backgroundcolor='white')
    plt.savefig(path_name)
    #     plt.axis('on')
    #     plt.show()
    return f, ax, txts


# for batch in test_loader:
#     X,Y=batch
#     X=X.reshape(X.shape[0],-1)
#     print(X.shape)
#     print(Y.shape)
# #     print(X)
# #     print(Y)

#     X = np.vstack([X[(Y==i)] for i in range(n_classes)])
#     Y = np.hstack([Y[(Y==i)] for i in range(n_classes)])

# mat = io.loadmat('./data/train_32x32.mat')
# X = mat['X']  # (32, 32, 3, 73257)
# # X = X.reshape(-1, X.shape[-1]).transpose()  # (73257,3072)

# X_gray = np.zeros((X.shape[-1], X.shape[0] * X.shape[1]))
# for i in range(X.shape[-1]):
#     X_gray[i] = np.array(img.fromarray(X[:, :, :, i],
#                                        "RGB").convert('L')).reshape(1, -1)
# X = X_gray

# X_mean = np.mean(X, axis=0)  # 第3维变化，对其他维每个位置上求均值和方差
# X_std = np.std(X, axis=0)
# X = (X - X_mean) / X_std
# print(X.shape)

# Y = mat['y']  # (73257, 1)
# Y = Y - 1
# tsne_output = TSNE(n_components=2, perplexity=30, n_iter=5000).fit_transform(X)
# plot2D(tsne_output, Y[:, 0],
#        "out/tSNE_original_gray_normalization_output_30_5000_73257.png")

# mat = io.loadmat('./data/test_32x32.mat')
# X_test = mat['X']
# # X_test = X_test.reshape(-1, X_test.shape[-1]).transpose()

# X_test_gray = np.zeros((X_test.shape[-1], X_test.shape[0] * X_test.shape[1]))
# for i in range(X_test.shape[-1]):
#     X_test_gray[i] = np.array(
#         img.fromarray(X_test[:, :, :, i], "RGB").convert('L')).reshape(1, -1)
# X_test = X_test_gray

# X_test = (X_test - X_mean) / X_std
# print(X_test.shape)

# Y_test = mat['y']
# Y_test = Y_test - 1
# tsne_output = TSNE(n_components=2, perplexity=30,
#                    n_iter=5000).fit_transform(X_test)
# plot2D(tsne_output, Y_test[:, 0],
#        "out/tSNE_original_gray_normalization_test_output_30_5000_26032.png")

#     tsne_output = TSNE(n_components=3,perplexity=10,n_iter=3000).fit_transform(X)
#     plot3D(tsne_output,Y)
# plot2D(tsne_output[:,0:2],Y)

# X=X.reshape(X.shape[0],-1)
# print(X.shape)
# print(type(Y))
# X=X[0:1000,:]
# Y=Y[0:1000,:]
# print('target shape: ', X.shape)
# print('output shape: ', Y.shape)
#     print(X)
#     print(Y)

X = np.load('out/AlexNet_b16_output_10000.npy').astype(np.float64)
Y = np.load('out/AlexNet_b16_target_10000.npy')
X = np.vstack([X[(Y == i)[:, 0]] for i in range(n_classes)])
Y = np.vstack([Y[(Y == i)[:, 0]] for i in range(n_classes)])
print('target shape: ', X.shape)
print('output shape: ', Y.shape)
tsne_output = TSNE(n_components=2, perplexity=30, n_iter=5000).fit_transform(X)
# print(tsne_output.shape)
# print(tsne_output)
plot2D(tsne_output, Y[:, 0], "out/tSNE_AlexNet_b16_output_30_5000_10000.png")
# plot3D(tsne_output, Y[:, 0],
#        "out/tSNE_AlexNet_b16_output_30_5000_73257_3D.png")

X_test = np.load('out/AlexNet_b16_test_output_1000.npy').astype(np.float64)
Y_test = np.load('out/AlexNet_b16_test_target_1000.npy')
X_test = np.vstack([X_test[(Y_test == i)[:, 0]] for i in range(n_classes)])
Y_test = np.vstack([Y_test[(Y_test == i)[:, 0]] for i in range(n_classes)])
print('target shape: ', X_test.shape)
print('output shape: ', Y_test.shape)
tsne_output = TSNE(n_components=2, perplexity=30,
                   n_iter=5000).fit_transform(X_test)
# print(tsne_output.shape)
# print(tsne_output)
plot2D(tsne_output, Y_test[:, 0],
       "out/tSNE_AlexNet_b16_test_output_30_5000_1000.png")
# plot3D(tsne_output, Y[:, 0],
#        "out/tSNE_AlexNet_b16_test_output_30_5000_26032_3D.png")
