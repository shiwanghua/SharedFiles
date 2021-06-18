import torch
import torch.nn as nn
import torch.nn.functional as F
import torchvision
import torchvision.datasets as dset
import torch.optim as optim
import torchvision.transforms as transforms

root='cifar-10-batches-py'
transformfun = transforms.Compose([
    transforms.ToTensor(),
])
train = dset.CIFAR10(root, train=True, transform=transformfun, target_transform=None, download=True)
test = dset.CIFAR10(root, train=False, transform=transformfun, target_transform=None, download=False)
train_loader = torch.utils.data.DataLoader(dataset=train, batch_size=8, shuffle=True)
test_loader = torch.utils.data.DataLoader(dataset=test, batch_size=8, shuffle=False)


class ResBlock(nn.Module):
    def __init__(self, in_channels, out_channels, stride=1):
        super(ResBlock, self).__init__()
        self.conv1 = nn.Conv2d(in_channels, out_channels, kernel_size=3, stride=stride, padding=1)
        self.bn1 = nn.BatchNorm2d(out_channels)
        self.relu = nn.ReLU(inplace=True)
        self.conv2 = nn.Conv2d(out_channels, out_channels, kernel_size=3, stride=1, padding=1)
        self.bn2 = nn.BatchNorm2d(out_channels)
        if in_channels != out_channels:
            self.downsample = nn.Sequential(nn.Conv2d(in_channels, out_channels, kernel_size=1, stride=2),
                                            nn.BatchNorm2d(out_channels))
        else:
            self.downsample = None

    def forward(self, x):
        identity = x
        out = self.conv1(x)
        out = self.bn1(out)
        out = self.relu(out)
        out = self.conv2(out)
        out = self.bn2(out)
        if self.downsample is not None:
            identity = self.downsample(x)

        out += identity
        out = self.relu(out)
        return out


class ResNet34(nn.Module):
    def __init__(self, block):
        super(ResNet34, self).__init__()
        self.first = nn.Sequential(nn.Conv2d(3, 64, 7, 2, 3), nn.BatchNorm2d(64),
                                   nn.ReLU(inplace=True), nn.MaxPool2d(3, 1, 1))
        self.layer1 = self.make_layer(block, 64, 64, 3, 1)
        self.layer2 = self.make_layer(block, 64, 128, 4, 2)
        self.layer3 = self.make_layer(block, 128, 256, 6, 2)
        self.layer4 = self.make_layer(block, 256, 512, 3, 2)
        self.avg_pool = nn.AvgPool2d(2)
        self.fc = nn.Linear(512, 10)

    def make_layer(self, block, in_channels, out_channels, block_num, stride):
        layers = []
        layers.append(block(in_channels, out_channels, stride))
        for i in range(block_num - 1):
            layers.append(block(out_channels, out_channels, 1))
        return nn.Sequential(*layers)

    def forward(self, x):
        x = self.first(x)
        x = self.layer1(x)
        x = self.layer2(x)
        x = self.layer3(x)
        x = self.layer4(x)
        x = self.avg_pool(x)
        x = x.view(x.size()[0], -1)
        x = self.fc(x)
        return x

if __name__ == '__main__':
    model = ResNet34(ResBlock)
    # model = model.cuda()
    optimizer = optim.SGD(model.parameters(), lr=0.01)
    for epoch in range(10):
        total_loss = 0
        for batch in train_loader:
            images, labels = batch
            out = model(images)
            loss = F.cross_entropy(out, labels)
            # out = model(images.cuda())
            # loss = F.cross_entropy(out, labels.cuda())
            optimizer.zero_grad()
            loss.backward()
            optimizer.step()
            total_loss += loss.item()
        correct = 0
        total = 0
        for images, labels in test_loader:
            # outputs = model(images.cuda())
            outputs = model(images)
            numbers, predicted = torch.max(outputs.data, 1)
            correct += (predicted == labels).sum().item()
            # correct += (predicted == labels.cuda()).sum().item()
            total += labels.size(0)

        print("process:", epoch + 1, "  loss:", total_loss, 'correct:', (100 * correct / total), '%')