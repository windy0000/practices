# -*- coding: UTF-8 -*-
"""
@version: 1.0
@PackageName: 图像分类 - model.py
@author: wenyi X
@Description: 
@since 2023/09/17 14:15
"""
# 使用CIFAR10数据集，数据尺寸3*32*32

import torch.nn as nn
import torch.nn.functional as F


class LeNet(nn.Module):  # 定义一个类，名为LeNet，继承于nn.Module这个父类
    def __init__(self):  # 初始化函数
        super(LeNet, self).__init__()
        # [3,32,32]-->[16,28,28]
        self.conv1 = nn.Conv2d(3, 16, 5)  # 定义第一个卷积层 (in,out,size）
        # [16,28,28]-->[16,14,14]
        self.pool1 = nn.MaxPool2d(2, 2)  # 定义第一个池化层 (size,stride)
        # [16,14,14]-->[32,10,10]
        self.conv2 = nn.Conv2d(16, 32, 5)
        # [32,10,10]-->[32,5,5]
        self.pool2 = nn.MaxPool2d(2, 2)

        self.fc1 = nn.Linear(32 * 5 * 5, 120)  # 第一个全连接层，全连接层的输入是一个一维向量，所以需要先展平
        self.fc2 = nn.Linear(120, 84)
        self.fc3 = nn.Linear(84, 10)

    def forward(self, x):  # 定义正向传播过程，x是输入数据[batch,channel,hight,width]
        x = F.relu(self.conv1(x))  # [3,32,32]-->[16,28,28]
        x = self.pool1(x)  # [16,28,28]-->[16,14,14]
        x = F.relu(self.conv2(x))  # [16,14,14]-->[32,10,10]
        x = self.pool2(x)  # [32,10,10]-->[32,5,5]
        x = x.view(-1, 32 * 5 * 5)  # [32,5,5]-->[32*5*5]  展平后再连接全连接层
        x = F.relu(self.fc1(x))  # [32*5*5]-->[120]
        x = F.relu(self.fc2(x))  # [120]-->[84]
        x = self.fc3(x)  # [84]-->[10]
        return x

# import torch
#
# input1 = torch.rand([32, 3, 32, 32])
# model1 = LeNet()
# print(model1)
# output = model1(input1)
