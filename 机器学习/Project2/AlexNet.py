import torch
import torch.nn as nn
import torch.optim as optim
import torch.nn.functional as F
import torchvision
import torchvision.datasets as dset
import torchvision.transforms as transforms
import numpy as np


class AlexNet(nn.Module):
    def __init__(self):
        super(AlexNet, self).__init__()
        self.features = nn.Sequential(
            nn.Conv2d(3, 96, 11, 4, 2),  # 11+4*54<228=224+2*2
            nn.ReLU(),
            nn.MaxPool2d(3, 2),  # 3+2*26=55
            nn.Conv2d(96, 192, 5, 1, 2),  # 5+26=31=27+2*2
            nn.ReLU(),
            nn.MaxPool2d(3, 2),  # 3+2*12=27
            nn.Conv2d(192, 384, 3, 1, 1),  #3+12=15=13+1*2
            nn.ReLU(),
            nn.Conv2d(384, 256, 3, 1, 1),  #3+12=15=13+1*2
            nn.ReLU(),
            # nn.Conv2d(256, 128, 3, 1, 1),  #3+12=15=13+1*2
            nn.Conv2d(256, 256, 3, 1, 1),  #3+12=15=13+1*2
            nn.ReLU(),
            # nn.MaxPool2d(3, 2),  # 3+2*5=13
            nn.MaxPool2d(5, 2, 1),  # 5+2*5=13+1*2
        )
        # self.features = nn.Sequential(
        #     nn.Conv2d(3, 64, 11, 4, 2),  # 11+4*54<228=224+2*2
        #     nn.ReLU(),
        #     nn.MaxPool2d(3, 2),  # 3+2*26=55
        #     nn.Conv2d(64, 128, 5, 1, 2),  # 5+26=31=27+2*2
        #     nn.ReLU(),
        #     nn.MaxPool2d(3, 2),  # 3+2*12=27
        #     nn.Conv2d(128, 256, 3, 1, 1),  #3+12=15=13+1*2
        #     nn.ReLU(),
        #     nn.Conv2d(256, 512, 3, 1, 1),  #3+12=15=13+1*2
        #     nn.ReLU(),
        #     nn.Conv2d(512, 128, 3, 1, 1),  #3+12=15=13+1*2
        #     # nn.Conv2d(256, 256, 3, 1, 1),  #3+12=15=13+1*2
        #     nn.ReLU(),
        #     nn.MaxPool2d(3, 2),  # 3+2*5=13
        #     # nn.MaxPool2d(5, 2, 1),  # 5+2*5=13+1*2
        # )
        self.classifier = nn.Sequential(
            nn.Dropout(),
            # nn.Linear(128 * 6 * 6, 4096),
            nn.Linear(256 * 6 * 6, 4096),
            nn.ReLU(),
            nn.Dropout(),
            # nn.Linear(4096, 4096),  # 1024
            nn.Linear(4096, 1024),
            nn.ReLU(),
            nn.Dropout(),
            nn.Linear(1024, 10))
        # nn.Linear(4096, 10))

    def forward(self, x):
        x = self.features(x)
        x = self.classifier(x.view(-1, 256 * 6 * 6))
        # x = self.classifier(x.view(-1, 128 * 6 * 6))
        return x


transform = transforms.Compose([
    transforms.Resize(224),
    transforms.RandomHorizontalFlip(),
    transforms.CenterCrop(224),
    transforms.ToTensor(),
    transforms.Normalize([0.485, 0.465, 0.406], [0.229, 0.224, 0.225]),
])

train_data = dset.CIFAR10(root='cifar-10-batches-py',
                          train=True,
                          transform=transform,
                          download=False)
test_data = dset.CIFAR10(root='cifar-10-batches-py',
                         train=False,
                         transform=transform,
                         download=False)
train_loader = torch.utils.data.DataLoader(train_data,
                                           batch_size=16,
                                           shuffle=True)
test_loader = torch.utils.data.DataLoader(test_data,
                                          batch_size=16,
                                          shuffle=True)
train_len = len(train_data)
test_len = len(test_data)
print("AlexNet")
print(train_len, test_len)

if __name__ == '__main__':

    model = AlexNet()
    # model = model.cuda()
    optimzer = optim.SGD(model.parameters(), lr=0.01)

    print("batch_b16 pool5 nn.Linear(256 * 6 * 6, 4096) 128改256")
    for epoch in range(10):
        total_loss = 0
        train_correct = 0
        test_correct = 0
        cnt = 0
        for batch in train_loader:
            # cnt += len(data)
            # print("processing: %d/%d" % (cnt, len(train_loader.dataset)))
            images, labels = batch
            outs = model(images)
            loss = F.cross_entropy(outs, labels)
            # outs = model(images.cuda())
            # loss = F.cross_entropy(outs, labels.cuda())
            total_loss += loss.item()
            optimzer.zero_grad()
            loss.backward()
            optimzer.step()
            # train_correct += outs.argmax(dim=1).eq(labels.cuda()).sum().item()
            train_correct += outs.argmax(dim=1).eq(labels).sum().item()

        for batch in test_loader:
            images, labels = batch
            out = model(images)
            test_correct += out.argmax(dim=1).eq(labels).sum().item()
            # out = model(images.cuda())
            # test_correct = out.argmax(dim=1).eq(labels.cuda()).sum().item()
        train_correct = train_correct / train_len
        test_correct = test_correct / test_len
        print(epoch, ' -- total_loss:', total_loss, '  train_correct:',
              train_correct, '  test_correct:', test_correct)

    out_output = []
    out_target = []
    out_data = []
    # cnt=0
    # print("save final train output")
    for batch in train_loader:
        # cnt += len(data)
        # print("processing: %d/%d" % (cnt, len(train_loader.dataset)))
        images, labels = batch
        outs = model(images)

        output_np = outs.data.numpy()
        target_np = labels.data.numpy()
        data_np = images.data.numpy()
        out_output.append(output_np)
        out_target.append(target_np[:, np.newaxis])  #(10,)变(10,1)
        out_data.append(np.squeeze(data_np))

    output_array = np.concatenate(out_output, axis=0)
    target_array = np.concatenate(out_target, axis=0)
    data_array = np.concatenate(out_data, axis=0)
    # 不允许序列化
    np.save('out/AlexNet_b16_p5_256_output.npy',
            output_array,
            allow_pickle=False)
    np.save('out/AlexNet_b16_p5_256_target.npy',
            target_array,
            allow_pickle=False)
    # np.save('out/AlexNet_b8_data.npy', data_array, allow_pickle=False)

    out_output = []
    out_target = []
    out_data = []
    # cnt=0
    # print("save final test output")
    for batch in test_loader:
        # cnt += len(data)
        # print("processing: %d/%d" % (cnt, len(train_loader.dataset)))
        images, labels = batch
        outs = model(images)

        output_np = outs.data.numpy()
        target_np = labels.data.numpy()
        data_np = images.data.numpy()
        out_output.append(output_np)
        out_target.append(target_np[:, np.newaxis])  #(10,)变(10,1)
        out_data.append(np.squeeze(data_np))

    output_array = np.concatenate(out_output, axis=0)
    target_array = np.concatenate(out_target, axis=0)
    data_array = np.concatenate(out_data, axis=0)
    # 不允许序列化
    np.save('out/AlexNet_b16_p5_256_test_output.npy',
            output_array,
            allow_pickle=False)
    np.save('out/AlexNet_b16_p5_256_test_target.npy',
            target_array,
            allow_pickle=False)
    # np.save('out/AlexNet_b8_test_data.npy', data_array, allow_pickle=False)
