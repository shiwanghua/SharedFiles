import torch
import torch.nn as nn
import torch.optim as optim
import torch.nn.functional as F
import torchvision
import torchvision.transforms as transforms
import torchvision.datasets as dset
from sklearn.metrics import roc_auc_score
import numpy as np

transform = transforms.Compose([
    transforms.Resize((224, 224)),
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
train_len = len(train_data)
test_len = len(test_data)
print('train_len:', train_len)
print('test_len:', test_len)
batchSize = 8
numTrainSample = 5000
numTestSample = 1000
train_loader = torch.utils.data.DataLoader(train_data,
                                           batch_size=batchSize,
                                           shuffle=True)
test_loader = torch.utils.data.DataLoader(test_data,
                                          batch_size=batchSize,
                                          shuffle=True)


class ZFNet(nn.Module):
    def __init__(self):
        super(ZFNet, self).__init__()
        self.features = nn.Sequential(
            nn.Conv2d(3, 96, 7, 2, 1),  # 第一输入信道一定是3个
            nn.ReLU(),  # 7+2*109=225 < 226=224+1*2
            nn.MaxPool2d(3, 2, 1),  # 3+2*54<110+1*2=112
            nn.Conv2d(96, 128, 5, 2, 0),  # 5+2*25=55
            nn.ReLU(),
            nn.MaxPool2d(3, 2, 1),  # 3+2*12<28=26+1*2
            nn.Conv2d(128, 256, 3, 1, 1),  # 3+1*12=15=13+1*2
            nn.ReLU(),
            nn.Conv2d(256, 384, 3, 1, 1),  # 3+1*12=13+1*2
            nn.ReLU(),
            nn.Conv2d(384, 256, 3, 1, 1),  # 3+1*12=13+1*2
            nn.ReLU(),
            # nn.MaxPool2d(5, 2, 1),  # 5+2*5=13+1*2
            nn.MaxPool2d(3, 2),  # 3+2*5=15=13+1*2
        )

        # # 改了输入输出通道数，准确率不到50%
        # self.features = nn.Sequential(
        #     nn.Conv2d(3, 64, 7, 2, 1),  # 第一输入信道一定是3个
        #     nn.ReLU(),  # 7+2*109=225 < 226=224+1*2
        #     nn.MaxPool2d(3, 2, 1),  # 3+2*54<110+1*2=112
        #     nn.Conv2d(64, 128, 5, 2, 0),  # 5+2*25=55
        #     nn.ReLU(),
        #     nn.MaxPool2d(3, 2, 1),  # 3+2*12<28=26+1*2
        #     nn.Conv2d(128, 256, 3, 1, 1),  # 3+1*12=15=13+1*2
        #     nn.ReLU(),
        #     nn.Conv2d(256, 512, 3, 1, 1),  # 3+1*12=13+1*2
        #     nn.ReLU(),
        #     nn.Conv2d(512, 256, 3, 1, 1),  # 3+1*12=13+1*2
        #     nn.ReLU(),
        #     # nn.MaxPool2d(5, 2, 1),  # 5+2*5=13+1*2
        #     nn.MaxPool2d(3, 2),  # 3+2*5=15=13+1*2
        # )

        # self.features = nn.Sequential(
        #     nn.Conv2d(3, 96, 7, 3, 0),  # 第一输入信道一定是3个
        #     nn.ReLU(),  #  7+3*72<224
        #     nn.MaxPool2d(5, 2, 1),  # 5+2*35=73+1*2
        #     nn.Conv2d(96, 128, 3, 1, 0),  # 3+1*33=36
        #     nn.ReLU(),
        #     nn.MaxPool2d(3, 1, 1),  # 3+1*33=34+1*2
        #     nn.Conv2d(128, 256, 3, 1, 1),  # 3+1*33=34+1*2
        #     nn.ReLU(),
        #     nn.Conv2d(256, 384, 3, 1, 1),  # 3+1*33=34+1*2
        #     nn.ReLU(),
        #     nn.Conv2d(384, 96, 3, 1, 1),  # 3+3*11=34+1*2
        #     nn.ReLU(),
        #     nn.MaxPool2d(3, 1, 0),  # 3+1*9=12
        # )
        self.classifier = nn.Sequential(
            # nn.Linear(1572864, 4096),  # 为什么不是96*10*10？
            nn.Linear(256 * 6 * 6, 4096),
            nn.ReLU(),
            nn.Dropout(),
            # nn.Linear(4096, 4096),  #
            nn.Linear(4096, 1024),  # 
            nn.ReLU(),
            nn.Dropout(),
            nn.Linear(1024, 10),  # 
            # nn.Linear(4096, 10),
        )

    def forward(self, x):
        x = self.features(x)
        x = x.view(-1, 256 * 6 * 6)
        x = self.classifier(x)
        return x


if __name__ == '__main__':
    model = ZFNet()
    # model = model.cuda()
    optimzer = optim.SGD(model.parameters(), lr=0.01)

    def get_correct(out, label):
        return out.argmax(dim=1).eq(label).sum().item()

    print("batch_b16 4096->1024")
    for epoch in range(10):
        total_loss = 0
        train_c = 0
        test_c = 0
        # train_label=[]
        # pred_label=[]
        num = 0
        for batch in train_loader:
            images, labels = batch
            outs = model(images)
            # outs = model(images.cuda())
            loss = F.cross_entropy(outs, labels)
            # loss = F.cross_entropy(outs, labels.cuda())
            total_loss += loss.item()
            optimzer.zero_grad()
            loss.backward()
            optimzer.step()
            train_c += get_correct(outs, labels)
            # train_c += get_correct(outs, labels.cuda())
            # num += 1
            # if numTrainSample / batchSize == num:
            #     break
        num = 0
        for batch in test_loader:
            images, labels = batch
            out = model(images)
            # out = model(images.cuda())
            test_c += get_correct(out, labels)
            # test_c += get_correct(out, labels.cuda())
            if numTestSample / batchSize == num:
                break
        train_c = train_c / train_len
        test_c = test_c / test_len
        # train_c = train_c / numTrainSample
        # test_c = test_c / numTestSample
        print('process:', epoch + 1, ' loss:', total_loss, ' train_correct:',
              train_c, ' test_correct:', test_c)

        # auc = roc_auc_score(label_all, pred_all)

    out_output = []
    out_target = []
    out_data = []
    # cnt=0
    # print("save final output")
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
    np.save('out/ZFNet_b8_1024_output.npy', output_array, allow_pickle=False)
    np.save('out/ZFNet_b8_1024_target.npy', target_array, allow_pickle=False)
    # np.save('out/ZFNet_data.npy', data_array, allow_pickle=False)

    # test
    out_output = []
    out_target = []
    out_data = []
    # cnt=0
    # print("save final output")
    for batch in test_loader:

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
    np.save('out/ZFNet_b8_1024_test_output.npy',
            output_array,
            allow_pickle=False)
    np.save('out/ZFNet_b8_1024_test_target.npy',
            target_array,
            allow_pickle=False)
    # np.save('out/ZFNet_b8_test_data.npy', data_array, allow_pickle=False)