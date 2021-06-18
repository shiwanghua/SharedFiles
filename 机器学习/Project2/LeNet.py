import torch
import torch.nn as nn
import torch.optim as optim
import torch.nn.functional as F
import torchvision
import torchvision.transforms as transforms
print("LeNet.py")
# 对图片进行预处理
transform = transforms.Compose([
    transforms.ToTensor(),
    transforms.Normalize((0.5, 0.5, 0.5), (0.5, 0.5, 0.5))
])
train_data = torchvision.datasets.CIFAR10(root='cifar-10-batches-py', train=True,
                                          download=False, transform=transform)
test_data = torchvision.datasets.CIFAR10(root='cifar-10-batches-py', train=False,
                                         download=False, transform=transform)
train_loader = torch.utils.data.DataLoader(train_data, batch_size=20, shuffle=True)
test_loader = torch.utils.data.DataLoader(test_data, batch_size=20, shuffle=True)

train_len = len(train_data)
test_len = len(test_data)
print('train length:', train_len)
print('test_length:', test_len)


# 模型搭建
class LeNet(nn.Module):
    def __init__(self):
        super(LeNet, self).__init__()
        self.conv1 = nn.Conv2d(3, 6, 5)
        self.relu = nn.ReLU()
        self.maxpool1 = nn.MaxPool2d(2, 2)
        self.conv2 = nn.Conv2d(6, 16, 5)
        self.maxpool2 = nn.MaxPool2d(2, 2)

        self.fc1 = nn.Linear(16 * 5 * 5, 120)
        self.fc2 = nn.Linear(120, 84)
        self.fc3 = nn.Linear(84, 10)

    def forward(self, x):
        x = self.conv1(x)
        x = self.relu(x)
        x = self.maxpool1(x)
        x = self.conv2(x)
        x = self.relu(x)
        x = self.maxpool2(x)
        x = x.view(-1, 16 * 5 * 5)
        x = F.relu(self.fc1(x))
        x = F.relu(self.fc2(x))
        x = self.fc3(x)
        return x

if __name__ == '__main__':
    # print(torch.__version__)
    # print(torch.cuda.is_available())
    lenet = LeNet()
    # lenet = lenet.cuda()
    optimzer = optim.SGD(lenet.parameters(), lr=0.01)
    loss_function = nn.CrossEntropyLoss()

    # 训练模型
    for epoch in range(5):
        total_loss = 0
        train_correct = 0
        for batch in train_loader:
            images, labels = batch
            outputs = lenet(images)
            loss = loss_function(outputs, labels)
            # outputs = lenet(images.cuda())
            # loss = loss_function(outputs, labels.cuda())
            optimzer.zero_grad()
            loss.backward()
            optimzer.step()
            total_loss += loss.item()
            train_correct += outputs.argmax(dim=1).eq(labels).sum().item()
            # train_correct += outputs.argmax(dim=1).eq(labels.cuda()).sum().item()
        train_correct = train_correct / train_len
        test_correct=0.0
        for batch in test_loader:
            images.labels = batch
            out = lenet(images)
            # out = lenet(images.cuda())
            test_correct += out.argmax(dim=1).eq(labels).sum().item()
            # test_correct += out.argmax(dim=1).eq(labels.cuda()).sum().item()
        test_correct = test_correct / test_len
        print('total_loss:', total_loss, '  train_correct:', train_correct, '  test_correct:', test_correct)