#pragma once
#include <iostream>
#include "layer.h"
#include "load_data.h"
#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/core/utility.hpp>

// Lenet-5ģ��
class network
{
private:
	cv::Mat inputLayer;						// �����,28*28��ͼƬ�ñ��32*32��
	// ���ز�,C1 S2 C3 S4 F5 F6 
	ConvolutionalLayer C1;
	AveragePoolingLayer S2;
	ConvolutionalLayer C3;
	AveragePoolingLayer S4;
	FullyConnectedLayer F5;
	FullyConnectedLayer F6;
	
	FullyConnectedLayer outputLayer;						// ����㣬ȫ����

	double learningrate;
public:
	void test(cv::Mat img,double label)
	{
		cv::Mat y_hat = forward(img);
		// Ҫ�Ȱѱ�ǩת��������(���ȱ���)
		cv::Mat one_hot = cv::Mat::zeros(10, 1, CV_64F);
		one_hot.at<double>((int)label, 0) = 1.0;
		
		backward(one_hot, y_hat);
		optim(learningrate);
	}

	// ��ʼ������ṹ���������
	network(double learningRate);

	// ����δsoftmax�����ֵ
 	cv::Mat forward(cv::Mat input);
	void backward(cv::Mat y_true,cv::Mat y_hat);
	void optim(double alpha);
	void clear();

	// ������һ��ͼƬ������ѵ����һ��һ������
	void train(MNISTData &train_data,int epoch, int total_loaded);

 	int predict(cv::Mat input);

	void save(std::string path);

};
