import scipy.io as io
import numpy as np
import math
# from IPython.display import display
infinity = float(-2**31)

mat = io.loadmat('./data/train_32x32.mat')
X = mat['X'] # (32, 32, 3, 73257)
Y = mat['y'] # (73257, 1)
n=X.shape[-1]
X=X.reshape(-1,n).transpose()  # (73257,3072)
print(X.shape)
X = np.column_stack((np.full((n,1),1),X)) #  hstack
X=(X/255-0.5)*2

mat = io.loadmat('./data/test_32x32.mat')
X_test = mat['X']
Y_test = mat['y']
n_test=X_test.shape[-1]
X_test=X_test.reshape(-1,n_test).transpose()
X_test = np.column_stack((np.full((n_test,1),1),X_test))
X_test=(X_test/255-0.5)*2


class LogsiticRegression(object):
    w = None # 权重
    W = None
    n = 0 # 样本个数
    d = 0 # 维度个数
    learning_rate = 0.1
    loss_difference = 0.5
    loss_bound = 0.5
    num_labels = 10
    #     plambda = 0.1 # 正则参数
    
    def __init__(self,*args, **kwargs):
        self.f=open('output.txt', "w",encoding='utf-8')
        
    def sigmoid(self,xb): # xb 是列向量
#         s=np.ones((len(xb),1))   
#         s=1.0/(1.0+np.exp(-xb))
#         return s
        return 1.0 / (1 + np.exp(-xb))

    def loss(self,X,Y):
        Xw=self.sigmoid(np.dot(X,self.w))
#         w2=self.w.copy()  
#         w2[0]=0           
        Loss=-(np.dot(Y.transpose(),np.log(Xw))+np.dot(np.transpose(1-Y),np.log(1-Xw)))/self.n # Xw里有1导致log 0
        #+(plambda*np.dot(np.transpose(w2),w2))/(2*self.n) 
        return Loss[0][0]
    
    # 课件1 P4 简化公式
    def loss2(self,X,Y):
        Xw=np.dot(X,self.w) 
#         w2=w.copy()  
#         w2[0]=0           
        Loss=(np.sum(np.log(1+np.exp(Xw)))-np.dot(Y.transpose(),Xw))/self.n
        #+(plambda*np.dot(np.transpose(w2),w2))/(2*self.n) 
        return Loss[0][0]
    
    # 逐个样本计算,去除异常值
    def loss3(self,X,Y):
        sum_err = 0.0
        Xw=self.sigmoid(np.dot(X,self.w))  #  Bug： 一直没加sigmoid！！！
        for i in range(self.n):
            if Xw[i][0] > 0 and Xw[i][0] < 1:
                sum_err -= (Y[i][0] * np.log(Xw[i][0]) + (1-Y[i][0]) * np.log(1-Xw[i][0]))
        return sum_err / self.n
    
    def gradient(self,X,Y):
        g=np.zeros((self.d))
        hx=self.sigmoid(np.dot(X,self.w))    #调用sigmoid函数
#         w2=self.w.copy()
#         w2[0]=0            
        g=np.dot(np.transpose(X),hx-Y)/self.n
        #+lambdap*w2/self.n
        return g

    def train(self,X,Y):

        self.n = X.shape[0]
        self.d = X.shape[1]
        y_class=np.zeros((X.shape[0],1))  # 第i个样本是不是数字j
        self.W = np.zeros((X.shape[1],self.num_labels))
        for i in range(1,self.num_labels+1):
            count = 1
            self.learning_rate=0.1
            self.w = np.full((X.shape[1],1),1.0/self.n) # np.zeros((X.shape[1],1))
            y_class[:,0]=np.int32(Y==i).reshape(1,-1) # 行向量赋给列向量
            while True:
                old_loss = self.loss3(X, y_class)
                self.w = self.w - self.learning_rate * self.gradient(X,y_class)
                new_loss = self.loss3(X, y_class)
                if new_loss < self.loss_bound and math.fabs(old_loss - new_loss) < self.loss_difference:
#                     print("第"+str(i)+"个分类器训练完毕")
#                     print("收敛到:",new_loss)
                    self.f.write("第"+str(i)+"个分类器训练完毕\n收敛到:"+str(new_loss)+"\n")
                    break
#                 if count%20==0:
#                 print("训练数字"+str(i)+" 迭代第",count,"次!")
                self.f.write("训练数字"+str(i)+" 迭代第"+str(count)+"次!\n"+str(old_loss)+'\n'+str(new_loss)+'\n')
#                 print("损失减小:",(old_loss - new_loss))
#                 print(old_loss)
#                 print(new_loss)
                count += 1
                self.learning_rate=self.learning_rate*0.99
#             print("数字"+str(i)+"训练完毕")
            self.f.write("数字"+str(i)+"训练完毕\n")
            self.W[:,i-1]=self.w[:,0]
    
    def train_sgd1(self,X,Y):
        self.n = X.shape[0]
        self.d = X.shape[1]
        y_class=np.zeros((X.shape[0],1))  # 第i个样本是不是数字j
        self.W = np.zeros((X.shape[1],self.num_labels))
        for i in range(1,self.num_labels+1):
            count = 1
            self.learning_rate=0.1
            
            self.w = np.full((X.shape[1],1),1.0/self.n) # np.zeros((X.shape[1],1))
            y_class[:,0]=np.int32(Y==i).reshape(1,-1) # 行向量赋给列向量
            while True:
                old_loss = self.loss3(X, y_class)
        
                sample_i=np.random.randint(self.n)
                hx=self.sigmoid(np.dot(X[sample_i],self.w))  
                self.w = self.w - self.learning_rate * np.dot(np.transpose(X[sample_i]).reshape(-1,1),(hx-Y[sample_i]).reshape(-1,1))
        
                new_loss = self.loss3(X, y_class)
                if new_loss < self.loss_bound and math.fabs(old_loss - new_loss) < self.loss_difference:
#                     print("第"+str(i)+"个分类器训练完毕")
#                     print("收敛到:",new_loss)
                    self.f.write("第"+str(i)+"个分类器训练完毕\n收敛到:"+str(new_loss)+"\n")
                    break
#                 if count%20==0:
#                 print("训练数字"+str(i)+" 迭代第",count,"次!")
#                 print("损失减小:",(old_loss - new_loss))
#                 print(old_loss)
#                 print(new_loss)
                    self.f.write("训练数字"+str(i)+" 迭代第"+str(count)+"次!\n"+str(old_loss)+'\n'+str(new_loss)+'\n')
                count += 1
                self.learning_rate=self.learning_rate*0.99
#             print("数字"+str(i)+"训练完毕")
            self.f.write("数字"+str(i)+"训练完毕\n")
            self.W[:,i-1]=self.w[:,0]
    
    def predict(self,X_test,y_test):
        y_predict = self.sigmoid(np.dot(X_test,self.W))
        y_class = np.zeros((X_test.shape[0],1))
        for i in range(X_test.shape[0]):
            max_class=0
            for j in range(1,self.num_labels):
                if y_predict[i][j] > y_predict[i][max_class]:
                    max_class=j
            y_class[i][0]=max_class+1
        result = (y_test==y_class)
#         print("准确率："+str(np.sum(result)/len(result)))
        self.f.write("准确率："+str(np.sum(result)/len(result))+'\n')
        return np.sum(result)/len(result)
    def save_W(self):
        with open('W.npy', 'wb') as f:
            np.save(f, self.W)
    def load_W(self):
        with open('W.npy', 'rb') as f:
            self.W = np.load(f)
        np.savez('./model.npz', L0_W = session.run(L0_W), L0_B = session.run(L0_B))
lr = LogsiticRegression()
lr.train(X,Y)
lr.predict(X,Y)
lr.predict(X_test,Y_test)
lr.save_W()
lr.f.close()