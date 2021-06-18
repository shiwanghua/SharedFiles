# coding: utf-8
import scipy.io as io
from scipy import optimize
import numpy as np
import math
import matplotlib.pyplot as plt
from sklearn.metrics import roc_auc_score

plt.gray()
import matplotlib.patheffects as pe
import seaborn as sb
from sklearn.manifold import TSNE
# %matplotlib inline
# from IPython.display import display
infinity = float(-2**31)
# np.set_printoptions(suppress=True, threshold=np.nan)    #去除科学计数法


def rgb2gray(images):
    return np.expand_dims(np.dot(images, [0.2990, 0.5870, 0.1140]), axis=3)


def getUniformData(X, Y, n):
    n_per_class = int(n / 10)
    X_uniform = []
    Y_uniform = []
    for i in range(10):
        Xi = X[Y[:, 0] == i]
        randIndices = [
            np.random.randint(0, Xi.shape[0]) for i in range(n_per_class)
        ]
        X_uniform.append(Xi[randIndices])
        Yi = Y[(Y[:, 0] == i)]
        Y_uniform.append(Yi[randIndices])
    X_uniform = np.vstack(np.array(X_uniform))
    Y_uniform = np.vstack(np.array(Y_uniform))
    return X_uniform, Y_uniform


n = 20000
n_test = 4000

mat = io.loadmat('./data/train_32x32.mat')
X = mat['X']  # (32, 32, 3, 73257)
Y = mat['y']  # (73257, 1)
Y[Y == 10] = 0
X = X.transpose((3, 0, 1, 2))
X, Y = getUniformData(X, Y, n)
# randIndices = [np.random.randint(0, X.shape[0]) for i in range(n)]
# X = X[randIndices]
# Y = Y[randIndices]
X = rgb2gray(X).astype(np.float32)
X = X.reshape(n, -1)  # (73257,3072)
X_mean = np.mean(X, axis=0)  # 第一维变化，对其他维每个位置上求均值和方差
X_std = np.std(X, axis=0)
X = (X - X_mean) / X_std
# X = np.column_stack((np.full((n, 1), 1), X))  #  hstack
# X=X/255
# X=(X/255-0.5)*2

mat = io.loadmat('./data/test_32x32.mat')
X_test = mat['X']
Y_test = mat['y']
Y_test[Y_test == 10] = 0
X_test = X_test.transpose((3, 0, 1, 2))
X_test, Y_test = getUniformData(X_test, Y_test, n_test)
# randIndicesTest = [np.random.randint(0, X_test.shape[0]) for i in range(n_test)]
# X_test = X_test[randIndicesTest]
# Y_test = Y_test[randIndicesTest]
X_test = rgb2gray(X_test).astype(np.float32)
X_test = X_test.reshape(n_test, -1)
X_test = (X_test - X_mean) / X_std
# X_test = np.column_stack((np.full((n_test, 1), 1), X_test))
# X_test=(X_test/255-0.5)*2
# X_test=X_test/255

print(X.shape, Y.shape, X_test.shape, Y_test.shape)


class LogsiticRegression(object):
    w = None  # 权重
    W = None
    n = 0  # 样本个数
    d = 0  # 维度个数
    learning_rate = 0.01
    loss_difference = 0.01  # loss 改变的百分比
    loss_bound = 0.3
    num_labels = 10

    #     plambda = 0.1 # 正则参数

    def __init__(self, *args, **kwargs):
        self.f = open('output.txt', "w", encoding='utf-8')

    def sigmoid(self, xb):  # xb 是列向量
        #         s=np.ones((len(xb),1))
        #         s=1.0/(1.0+np.exp(-xb))
        #         return s
        return 1.0 / (1.0 + np.exp(-xb))

    def loss(self, X, Y):
        Xw = self.sigmoid(np.dot(X, self.w))
        #         w2=self.w.copy()
        #         w2[0]=0
        Loss = -(np.dot(Y.transpose(), np.log(Xw)) + np.dot(
            np.transpose(1 - Y), np.log(1 - Xw))) / self.n  # Xw里有1导致log 0
        #+(plambda*np.dot(np.transpose(w2),w2))/(2*self.n)
        return Loss[0][0]

    # 课件1 P4 简化公式
    def loss2(self, X, Y):
        Xw = np.dot(X, self.w)
        #         w2=w.copy()
        #         w2[0]=0
        Loss = (np.sum(np.log(1 + np.exp(Xw))) -
                np.dot(Y.transpose(), Xw)) / self.n
        #+(plambda*np.dot(np.transpose(w2),w2))/(2*self.n)
        return Loss[0][0]

    # 逐个样本计算,去除异常值
    def loss3(self, X, Y):
        sum_err = 0.0
        Xw = self.sigmoid(np.dot(X, self.w))  #  Bug： 一直没加sigmoid！！！
        for i in range(self.n):
            if Xw[i][0] > 0 and Xw[i][0] < 1:
                sum_err -= (Y[i][0] * np.log(Xw[i][0]) +
                            (1 - Y[i][0]) * np.log(1 - Xw[i][0]))
        return sum_err / self.n

    def loss4(self, w, X, Y, initial_lambda):
        n = len(Y)
        h = 1.0 / (1.0 + np.exp(-np.dot(X, w)))
        #         theta1=w.copy()
        #         theta1[0]=0
        J = -(np.dot(np.transpose(Y), np.log(h)) + np.dot(
            np.transpose(1 - Y), np.log(1 - h)
        )) / n  #+(initial_lambda*np.dot(np.transpose(theta1),theta1))/(2*n)
        return J

    def gradient(self, X, Y):
        #         g=np.zeros((self.d))
        hx = self.sigmoid(np.dot(X, self.w))  #调用sigmoid函数
        #         w2=self.w.copy()
        #         w2[0]=0
        g = np.dot(np.transpose(X), hx - Y)  # / self.n
        #+lambdap*w2/self.n
        return g

    def gradient2(self, w, X, Y, initial_lambda):
        #         g=np.zeros((self.d))
        n = X.shape[0]
        hx = self.sigmoid(np.dot(X, w))
        #         w2=w.copy()
        #         w2[0]=0
        g = np.dot(np.transpose(X), hx - Y) / n
        #+initial_lambda*w2/n
        return g

    def train(self, X, Y):
        self.n = X.shape[0]
        self.d = X.shape[1]
        y_class = np.zeros((X.shape[0], 1))  # 第i个样本是不是数字j
        self.W = np.zeros((X.shape[1], self.num_labels))

        for i in range(self.num_labels):
            count = 1
            new_loss = 0
            self.learning_rate = 0.01
            self.w = np.full((self.d, 1), 0)  # np.zeros((X.shape[1],1))
            y_class[:, 0] = np.int32(Y == i).reshape(1, -1)  # 行向量赋给列向量
            acc = 0
            auc = 0.0
            while True:
                old_loss = new_loss
                self.w = self.w - self.learning_rate * self.gradient(
                    X, y_class)
                #                 self.w=self.w/np.max(self.w)
                new_loss = self.loss3(X, y_class)
                acc, auc = self.prediect_one_class(X, y_class)
                if (acc > 0.94 and count > 500) or count > 1000 or (
                        new_loss < self.loss_bound
                        and math.fabs(old_loss - new_loss) /
                        max(self.loss_bound, new_loss) < self.loss_difference
                ):  # or auc > 0.99
                    # if acc > 0.94 and count > 500:
                    #     print("11111")
                    # if auc > 0.98:
                    #     print("22222")
                    # if count > 1000:
                    #     print("33333")
                    # if new_loss < self.loss_bound and math.fabs(
                    #         old_loss - new_loss
                    # ) / max(self.loss_bound, new_loss) < self.loss_difference:
                    #     print("44444")
                    print("第" + str(i) + "个分类器训练完毕，迭代", count, "次，收敛到:",
                          new_loss, " 准确率：", acc, " auc: ", auc, "old_loss: ",
                          old_loss)
                    #                     self.f.write("第"+str(i)+"个分类器训练完毕\n收敛到:"+str(new_loss)+"\n")
                    break
                # if count%20==0:
                #     print("训练数字"+str(i)+" 迭代第",count,"次! loss=",new_loss)

                #                     print(self.w)
                #                 self.f.write("训练数字"+str(i)+" 迭代第"+str(count)+"次!\n"+str(old_loss)+'\n'+str(new_loss)+'\n')
                #                 print("损失减小:",(old_loss - new_loss))
                #                 print(old_loss)
                count += 1
                self.learning_rate = self.learning_rate * 0.99

            # print("数字"+str(i)+"训练完毕")
#             self.f.write("数字"+str(i)+"训练完毕\n")
            self.W[:, i] = self.w[:, 0]
#             self.W[:,i]=self.W[:,i-1]/np.max(self.W[:,i-1])

# 随机取一个样本

    def train_sgd1(self, X, Y):
        self.n = X.shape[0]
        self.d = X.shape[1]
        y_class = np.zeros((X.shape[0], 1))  # 第i个样本是不是数字j
        self.W = np.zeros((X.shape[1], self.num_labels))
        for i in range(1, self.num_labels + 1):
            count = 1
            self.learning_rate = 0.1

            self.w = np.full((X.shape[1], 1), 0)  # np.zeros((X.shape[1],1))
            y_class[:, 0] = np.int32(Y == i).reshape(1, -1)  # 行向量赋给列向量
            while True:
                old_loss = new_loss

                sample_i = np.random.randint(self.n)
                hx = self.sigmoid(np.dot(X[sample_i], self.w))
                self.w = self.w - self.learning_rate * np.dot(
                    np.transpose(X[sample_i]).reshape(-1, 1),
                    (hx - Y[sample_i]).reshape(-1, 1))

                new_loss = self.loss3(X, y_class)
                if new_loss < self.loss_bound and math.fabs(
                        old_loss - new_loss) < self.loss_difference:
                    print("第" + str(i) + "个分类器训练完毕")
                    print("收敛到:", new_loss)
                    #                     self.f.write("第"+str(i)+"个分类器训练完毕\n收敛到:"+str(new_loss)+"\n")
                    break
#                 if count%20==0:
                print("训练数字" + str(i) + " 迭代第", count, "次!")
                #                 print("损失减小:",(old_loss - new_loss))
                print(old_loss)
                print(new_loss)
                #                     self.f.write("训练数字"+str(i)+" 迭代第"+str(count)+"次!\n"+str(old_loss)+'\n'+str(new_loss)+'\n')
                count += 1
                self.learning_rate = self.learning_rate  #*0.999
            print("数字" + str(i) + "训练完毕")
            #             self.f.write("数字"+str(i)+"训练完毕\n")
            self.W[:, i - 1] = self.w[:, 0]

    # 没写好
    def train_sgd_batch(self, X, Y):
        batch_size = 100
        self.n = X.shape[0]
        self.d = X.shape[1]
        y_class = np.zeros((X.shape[0], 1))  # 第i个样本是不是数字j
        self.W = np.zeros((X.shape[1], self.num_labels))
        for i in range(1, self.num_labels + 1):
            count = 1
            self.learning_rate = 0.1

            self.w = np.full((X.shape[1], 1),
                             1.0 / self.n)  # np.zeros((X.shape[1],1))
            y_class[:, 0] = np.int32(Y == i).reshape(1, -1)  # 行向量赋给列向量
            while True:
                old_loss = new_loss

                sample_i = np.random.randint(self.n)
                hx = self.sigmoid(np.dot(X[sample_i], self.w))
                self.w = self.w - self.learning_rate * np.dot(
                    np.transpose(X[sample_i]).reshape(-1, 1),
                    (hx - Y[sample_i]).reshape(-1, 1))

                new_loss = self.loss3(X, y_class)
                if new_loss < self.loss_bound and math.fabs(
                        old_loss - new_loss) < self.loss_difference:
                    print("第" + str(i) + "个分类器训练完毕")
                    print("收敛到:", new_loss)
                    #                     self.f.write("第"+str(i)+"个分类器训练完毕\n收敛到:"+str(new_loss)+"\n")
                    break


#                 if count%20==0:
                print("训练数字" + str(i) + " 迭代第", count, "次!")
                #                 print("损失减小:",(old_loss - new_loss))
                print(old_loss)
                print(new_loss)
                #                     self.f.write("训练数字"+str(i)+" 迭代第"+str(count)+"次!\n"+str(old_loss)+'\n'+str(new_loss)+'\n')
                count += 1
                self.learning_rate = self.learning_rate * 0.999
            print("数字" + str(i) + "训练完毕")
            #             self.f.write("数字"+str(i)+"训练完毕\n")
            self.W[:, i - 1] = self.w[:, 0]

    # 多分类 同时预测每个类的准确率，选最大值
    def predict(self, X_test, y_test):
        y_predict = self.sigmoid(np.dot(X_test, self.W))
        y_class = np.zeros((X_test.shape[0], 1))
        for i in range(X_test.shape[0]):
            max_class = 0
            for j in range(1, self.num_labels):
                if y_predict[i][j] > y_predict[i][max_class]:
                    max_class = j
                    break
            y_class[i][0] = max_class
        result = (y_test == y_class)
        print("最大式多分类总和准确率：" + str(np.sum(result) / len(result)))
        #         print("auc: ", roc_auc_score(y_test, y_predict,average='macro'))#multi_class='ovo',average='weighted'

        #         self.f.write("准确率："+str(np.sum(result)/len(result))+'\n')
        return np.sum(result) / len(result)

    # 二分类 输入为向量
    def prediect_one_class(self, X, Y):
        y_predict = self.sigmoid(np.dot(X, self.w))
        y_class = np.zeros((X.shape[0], 1))
        for i in range(X.shape[0]):
            if y_predict[i][0] > 0.5:
                y_class[i][0] = 1
        result = (Y == y_class)
        acc = np.sum(result) / len(result)
        auc = roc_auc_score(Y, y_predict)
        # print("单个类准确率："+str(acc))
        # print("auc: ",auc)
        return acc, auc

    # 二分类 逐个预测每个类的准确率
    def predict2(self, X, Y):
        y_class = np.zeros((X.shape[0], 1))  # 预测出是不是类i
        Y_class = np.zeros((X.shape[0], 1))  # 真实类是不是类i
        acc = []
        auc = []
        for i in range(self.num_labels):
            y_class = np.zeros((X.shape[0], 1))
            y_predict = self.sigmoid(np.dot(X, self.W[:, i]))[:, np.newaxis]
            for j in range(X.shape[0]):
                if y_predict[j][0] > 0.5:
                    y_class[j][0] = 1
            Y_class[:, 0] = np.int32(Y == i).reshape(1, -1)  # 行向量赋给列向量
            result = (Y_class == y_class)
            acc.append(np.sum(result) / len(result))
            auc.append(roc_auc_score(Y_class, y_class))
        print("每个类上的准确率：", acc)
        print("每个类上的auc: ", auc)

    # 多分类 同时预测每个类的准确率, 迭代判断
    def predict3(self, X, Y):
        y_predict = self.sigmoid(np.dot(X, self.W))
        y_class = np.zeros((X.shape[0], 1))
        for i in range(X.shape[0]):
            max_class = 1
            for j in range(self.num_labels):
                if y_predict[i][j] > 0.5:
                    max_class = j
                    break
            y_class[i][0] = max_class
        result = (Y == y_class)
        print("迭代式多分类总和准确率：" + str(np.sum(result) / len(result)))
        #         print("auc: ",roc_auc_score(Y,y_predict,multi_class='ovo'))
        #         self.f.write("准确率："+str(np.sum(result)/len(result))+'\n')
        return np.sum(result) / len(result)

    # 有bug
    def train_opt(self, X, Y, initial_lambda):

        self.n = X.shape[0]
        self.d = X.shape[1]
        y_class = np.zeros((X.shape[0], 1))  # 第i个样本是不是数字j
        self.W = np.zeros((X.shape[1], self.num_labels))
        initial_w = np.zeros((self.d, 1))  #设置初始的theta值。全为0

        for i in range(1, self.num_labels):
            y_class[:, 0] = np.int32(Y == i).reshape(1, -1)
            result = optimize.fmin_bfgs(self.loss4,
                                        initial_w,
                                        fprime=self.gradient2,
                                        args=(X, y_class, initial_lambda))
            self.W[:, i] = result.reshape(1, -1)

    def save_W(self):
        with open('W.npy', 'wb') as f:
            np.save(f, self.W)
            f.close()

    def load_W(self):
        with open('W.npy', 'rb') as f:
            self.W = np.load(f)
            f.close()
        np.savez('./model.npz', L0_W=session.run(L0_W), L0_B=session.run(L0_B))
lr = LogsiticRegression()
# lr.train_sgd1(X,Y)
# lr.train_opt(X,Y,0.1)
lr.train(X, Y)
# lr.auc(X,Y)
print("训练集")
lr.predict(X, Y)
lr.predict2(X, Y)
lr.predict3(X, Y)
print("测试集")
lr.predict(X_test, Y_test)
lr.predict2(X_test, Y_test)
lr.predict3(X_test, Y_test)
# lr.save_W()
# lr.f.close()