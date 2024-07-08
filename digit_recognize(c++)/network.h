#pragma once
#include <iostream>
#include "layer.h"
#include "load_data.h"
#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/core/utility.hpp>

// Lenet-5模型
class network
{
private:
	cv::Mat inputLayer;						// 输出层,28*28的图片得变成32*32的
	// 隐藏层,C1 S2 C3 S4 F5 F6 
	ConvolutionalLayer C1;
	AveragePoolingLayer S2;
	ConvolutionalLayer C3;
	AveragePoolingLayer S4;
	FullyConnectedLayer F5;
	FullyConnectedLayer F6;
	
	FullyConnectedLayer outputLayer;						// 输出层，全连接

	double learningrate;
public:
	void test(cv::Mat img,double label)
	{
		cv::Mat y_hat = forward(img);
		// 要先把标签转换成向量(独热编码)
		cv::Mat one_hot = cv::Mat::zeros(10, 1, CV_64F);
		one_hot.at<double>((int)label, 0) = 1.0;
		
		backward(one_hot, y_hat);
		optim(learningrate);
	}

	// 初始化网络结构和网络参数
	network(double learningRate);

	// 返回未softmax的输出值
 	cv::Mat forward(cv::Mat input);
	void backward(cv::Mat y_true,cv::Mat y_hat);
	void optim(double alpha);
	void clear();

	// 传入是一批图片，但是训练是一张一张来的
	void train(MNISTData &train_data,int epoch, int total_loaded);

 	int predict(cv::Mat input);

	void save(std::string path);

};
