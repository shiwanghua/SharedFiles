# 引入必要的库
import numpy as np
import matplotlib.pyplot as plt
from itertools import cycle
from sklearn import svm, datasets
import torchvision.datasets as dataset
import torchvision.transforms as transforms
from sklearn.metrics import roc_curve, auc
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import label_binarize
from sklearn.multiclass import OneVsRestClassifier
from scipy import interp
from sklearn.manifold import TSNE
from PIL import Image as img
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.patheffects as pe
import seaborn as sb
from sklearn.decomposition import PCA


def rgb2gray(images):
    return np.expand_dims(np.dot(images, [0.2990, 0.5870, 0.1140]), axis=3)


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


transform = transforms.Compose([
    transforms.Resize(32),
    transforms.RandomHorizontalFlip(),
    transforms.CenterCrop(32),  # 32 从中心裁剪
    transforms.ToTensor(),
    transforms.Normalize([0.485, 0.465, 0.406], [0.229, 0.224, 0.225]),
])
train_data = dataset.SVHN(root='data',
                          split='train',
                          transform=transform,
                          download=False)
test_data = dataset.SVHN(root='data',
                         split='test',
                         transform=transform,
                         download=False)

n = 10000  #10000 5000
n_test = 5000  #5000 2000
n_classes = 10
path = "out/svm_pca/"

X = train_data.data
randIndices = [np.random.randint(0, X.shape[0]) for i in range(n)]
X = X[randIndices]
X = X.transpose((0, 2, 3, 1))
# X = rgb2gray(X).astype(np.float32)
X = X.reshape(X.shape[0], -1)
# X_mean = np.mean(X, axis=0)  # 第一维变化，对其他维每个位置上求均值和方差
# X_std = np.std(X, axis=0)
# X = (X - X_mean) / X_std
pca = PCA(n_components=0.9)
pca.fit(X)  #训练
X = pca.fit_transform(X)  #降维后的数据
d = X.shape[1]
y = train_data.labels
y = y[randIndices]
yb = label_binarize(y, classes=[0, 1, 2, 3, 4, 5, 6, 7, 8, 9])

X_test = test_data.data
randIndicesTest = [
    np.random.randint(0, X_test.shape[0]) for i in range(n_test)
]
X_test = X_test[randIndicesTest]
X_test = X_test.transpose((0, 2, 3, 1))
# X_test = rgb2gray(X_test).astype(np.float32)
X_test = X_test.reshape(X_test.shape[0], -1)
# X_test = (X_test - X_mean) / X_std
pca_test = PCA(n_components=d)
pca_test.fit(X_test)  #训练
X_test = pca_test.fit_transform(X_test)  #降维后的数据
y_test = test_data.labels
y_test = y_test[randIndicesTest]
yb_test = label_binarize(y_test, classes=[0, 1, 2, 3, 4, 5, 6, 7, 8, 9])

# X = np.vstack([X[y == i] for i in range(n_classes)])
# yb = np.vstack([yb[y == i] for i in range(n_classes)])
# y = np.hstack([y[y == i] for i in range(n_classes)])  # 最后才可以转y！！！！！

tsne_output = TSNE(n_components=2, perplexity=30, n_iter=5000).fit_transform(X)
plot2D(tsne_output, y, path + "svm_sample_tSNE.png")

# X_test = np.vstack([X_test[y_test == i] for i in range(n_classes)])
# yb_test = np.vstack([yb_test[y_test == i] for i in range(n_classes)])
# y_test = np.hstack([y_test[y_test == i]for i in range(n_classes)])  # 最后才可以转y！！！！！
tsne_output = TSNE(n_components=2, perplexity=30,
                   n_iter=5000).fit_transform(X_test)
plot2D(tsne_output, y_test, path + "svm_sample_test_tSNE.png")

# np.save('out/svm_sample.npy', y_train_predict, allow_pickle=False)
# np.save('out/svm_sample_test.npy', y_test_predict, allow_pickle=False)

print("pca contribution rate: ", pca.explained_variance_ratio_, "dimension: ",
      X.shape[1])
print("pca test contribution rate: ", pca_test.explained_variance_ratio_,
      "dimension: ", X_test.shape[1])

names = ['poly', 'rbf', 'sigmoid', 'linear']
for kernel_name in names:

    classifier = OneVsRestClassifier(
        svm.SVC(kernel=kernel_name,
                probability=True,
                random_state=np.random.RandomState(
                    0)))  #一对多 linear poly rbf sigmod
    y_score = classifier.fit(X, yb).decision_function(X_test)

    print(
        kernel_name + ' kernel accuracy in trainset: ' +
        str(classifier.score(X, yb))
    )  # yb Classification metrics can't handle a mix of multiclass and multilabel-indicator targets
    print(kernel_name + ' kernel accuracy in testset: ' +
          str(classifier.score(X_test, yb_test)))  # yb_test

    # 计算每一类的ROC
    fpr = dict()  # 假正例率
    tpr = dict()  # 真正例率
    roc_auc = dict()
    for i in range(n_classes):
        fpr[i], tpr[i], _ = roc_curve(yb_test[:, i], y_score[:, i])
        roc_auc[i] = auc(fpr[i], tpr[i])

    # Compute micro-average ROC curve and ROC area（方法二）
    fpr["micro"], tpr["micro"], _ = roc_curve(yb_test.ravel(), y_score.ravel())
    roc_auc["micro"] = auc(fpr["micro"], tpr["micro"])
    # print(fpr)
    # print(tpr)
    # print(roc_auc)

    # Compute macro-average ROC curve and ROC area（方法一）
    # First aggregate all false positive rates
    all_fpr = np.unique(np.concatenate([fpr[i] for i in range(n_classes)]))
    # Then interpolate all ROC curves at this points
    mean_tpr = np.zeros_like(all_fpr)
    for i in range(n_classes):
        mean_tpr += np.interp(all_fpr, fpr[i], tpr[i])
    # Finally average it and compute AUC
    mean_tpr /= n_classes
    fpr["macro"] = all_fpr
    tpr["macro"] = mean_tpr
    roc_auc["macro"] = auc(fpr["macro"], tpr["macro"])

    # print(all_fpr)
    # print(mean_tpr)
    # print(roc_auc)

    plt.rcParams['figure.figsize'] = (14, 8)  # 调整展示时的图片区域大小
    # Plot all ROC curves
    lw = 2
    plt.figure()
    plt.plot(fpr["micro"],
             tpr["micro"],
             label='micro-average (area = {0:0.2f})'
             ''.format(roc_auc["micro"]),
             color='#DC143C',
             linestyle=':',
             linewidth=4)

    plt.plot(fpr["macro"],
             tpr["macro"],
             label='macro-average (area = {0:0.2f})'
             ''.format(roc_auc["macro"]),
             color='#9400D3',
             linestyle=':',
             linewidth=4)

    colors = cycle([
        '#1E90FF', '#87CEEB', '#00FFFF', '#00CED1', '#00FA9A', '#00FF00',
        '#BDB76B', '#FF8C00', '#FF4500', '#8B0000'
    ])
    for i, c in zip(range(n_classes), colors):
        plt.plot(fpr[i],
                 tpr[i],
                 color=c,
                 lw=lw,
                 label='class {0} (area = {1:0.2f})'
                 ''.format(i, roc_auc[i]))

    plt.plot([0, 1], [0, 1], '--', lw=lw)
    plt.xlim([0.0, 1.0])
    plt.ylim([0.0, 1.05])
    plt.xlabel('False Positive Rate')
    plt.ylabel('True Positive Rate')
    plt.title(' ROC curve - SVM with ' + kernel_name + ' kernel')
    plt.legend(loc="lower right")
    # plt.show()
    plt.savefig(path + "svm_" + kernel_name + "_roc.png")

    y_train_predict = classifier.predict_proba(X)
    # y_train_predict = np.vstack(
    #     [y_train_predict[(y == i)[:, 0]] for i in range(n_classes)])
    # y = np.vstack([y[(y == i)[:, 0]] for i in range(n_classes)])
    tsne_output = TSNE(n_components=2, perplexity=30,
                       n_iter=5000).fit_transform(y_train_predict)
    plot2D(tsne_output, y, path + "svm_" + kernel_name + "_tSNE.png")

    # X_test = np.load('out/AlexNet_b16_test_output_1000.npy').astype(np.float64)
    # Y_test = np.load('out/AlexNet_b16_test_target_1000.npy')
    y_test_predict = classifier.predict_proba(X_test)
    # y_test_predict = np.vstack(
    #     [y_test_predict[(y_test == i)[:, 0]] for i in range(n_classes)])
    # y_test = np.vstack([y_test[(y_test == i)[:, 0]] for i in range(n_classes)])
    tsne_output = TSNE(n_components=2, perplexity=30,
                       n_iter=5000).fit_transform(y_test_predict)
    plot2D(tsne_output, y_test, path + "svm_" + kernel_name + "_test_tSNE.png")

    # np.save('out/svm_' + kernel_name + '.npy',
    #         y_train_predict,
    #         allow_pickle=False)
    # np.save('out/svm_' + kernel_name + '_test.npy',
    #         y_test_predict,
    #         allow_pickle=False)
