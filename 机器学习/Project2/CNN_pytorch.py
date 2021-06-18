import torch
import torchvision
import torchvision.transforms as transforms
import matplotlib.pyplot as plt
import numpy as np
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim

BATCH_SIZE = 4 #每批取的训练数据
EPOCH = 2

print("CNN")
#torchvision模块载入CIFAR10数据集，并且通过transform归一化到[0,1]
transform = transforms.Compose([transforms.ToTensor(), # 输出为[0,1]
                                            transforms.Normalize((0.5,0.5,0.5),(0.5,0.5,0.5))]) # 将[0,1]的RGB归一化为[-1,1]
trainset = torchvision.datasets.CIFAR10(root='./cifar-10-batches-py',train = True, # 训练集
                                        download=True, transform=transform) # 归一化
trainloader = torch.utils.data.DataLoader(trainset,batch_size = BATCH_SIZE,
                                          shuffle = True, num_workers=2) # 在if name == ‘main’:中用

testset = torchvision.datasets.CIFAR10(root='./cifar-10-batches-py',train = False, # 测试集
                                        download=True, transform=transform)
testloader = torch.utils.data.DataLoader(testset,batch_size = BATCH_SIZE,
                                          shuffle = False, num_workers=2)
classes = ('plane', 'car', 'bird', 'cat', 'deer', 'dog', 'frog', 'horse', 'ship', 'truck')

def imshow(img):
    img = img / 2 + 0.5     # unnormalize
    npimg = img.numpy()
    plt.imshow(np.transpose(npimg, (1, 2, 0)))

plt.imshow(trainset.data[86]) #trainset.data中储存了原始数据，并且是array格式
plt.show()

# https://blog.csdn.net/yanxueotft/article/details/97977754
# https://github.com/zergtant/pytorch-handbook/blob/master/chapter1/4_cifar10_tutorial.ipynb
class Net(nn.Module):
    def __init__(self):
        super(Net, self).__init__()
        self.conv1 = nn.Conv2d(3, 6, 5)
        self.pool = nn.MaxPool2d(2, 2)
        self.conv2 = nn.Conv2d(6, 16, 5)
        self.fc1 = nn.Linear(16 * 5 * 5, 120)
        self.fc2 = nn.Linear(120, 84)
        self.fc3 = nn.Linear(84, 10)

    def forward(self, x):
        x = self.pool(F.relu(self.conv1(x)))
        x = self.pool(F.relu(self.conv2(x)))
        x = x.view(-1, 16 * 5 * 5)
        x = F.relu(self.fc1(x))
        x = F.relu(self.fc2(x))
        x = self.fc3(x)
        return x

if __name__ == '__main__':
    dataiter = iter(trainloader)
    images, labels = dataiter.next()
    images_comb = torchvision.utils.make_grid(images)
    images_comb_unnor = (images_comb*0.5+0.5).numpy()
    plt.imshow(np.transpose(images_comb_unnor, (1, 2, 0)))
    plt.show()

    # 展示图像
    imshow(torchvision.utils.make_grid(images))
    # 显示图像标签
    print(' '.join('%5s' % classes[labels[j]] for j in range(4)))

    net = Net()
    criterion = nn.CrossEntropyLoss()
    optimizer = optim.SGD(net.parameters(), lr=0.001, momentum=0.9)


    for epoch in range(2):  # 多批次循环

        running_loss = 0.0
        for i, data in enumerate(trainloader, 0):
            # 获取输入
            inputs, labels = data

            # 梯度置0
            optimizer.zero_grad()

            # 正向传播，反向传播，优化
            outputs = net(inputs)
            loss = criterion(outputs, labels)
            loss.backward()
            optimizer.step()

            # 打印状态信息
            running_loss += loss.item()
            if i % 2000 == 1999:    # 每2000批次打印一次
                print('[%d, %5d] loss: %.3f' %
                      (epoch + 1, i + 1, running_loss / 2000))
                running_loss = 0.0

    print('Finished Training')


    dataiter = iter(testloader)
    images, labels = dataiter.next()

    # 显示图片
    imshow(torchvision.utils.make_grid(images))
    print('GroundTruth: ', ' '.join('%5s' % classes[labels[j]] for j in range(4)))
    outputs = net(images)

    _, predicted = torch.max(outputs, 1)

    print('Predicted: ', ' '.join('%5s' % classes[predicted[j]]
                                  for j in range(4)))

    correct = 0
    total = 0
    with torch.no_grad():
        for data in testloader:
            images, labels = data
            outputs = net(images)
            _, predicted = torch.max(outputs.data, 1)
            total += labels.size(0)
            correct += (predicted == labels).sum().item()

    print('Accuracy of the network on the 10000 test images: %d %%' % (
        100 * correct / total))

    class_correct = list(0. for i in range(10))
    class_total = list(0. for i in range(10))
    with torch.no_grad():
        for data in testloader:
            images, labels = data
            outputs = net(images)
            _, predicted = torch.max(outputs, 1)
            c = (predicted == labels).squeeze()
            for i in range(4):
                label = labels[i]
                class_correct[label] += c[i].item()
                class_total[label] += 1


    for i in range(10):
        print('Accuracy of %5s : %2d %%' % (
            classes[i], 100 * class_correct[i] / class_total[i]))