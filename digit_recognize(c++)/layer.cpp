#include <iostream>
#include "layer.h"
#include <random>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include "activation_function.h"

void ConvolutionalLayer::init(int num, int size, int stride, int padding, std::vector<std::vector<int>> convIndices)
{
	this->numKernel = num;
	this->KernelSize = size;
	this->stride = stride;
	this->padding = padding;

	// 初始化卷积核和偏置
	for (int i = 0; i < numKernel; ++i)
	{
		cv::Mat tmp(size,size, CV_64F);
		cv::randn(tmp, 0, 0.1);

		kernels.push_back(tmp);
	}
	bias = 0;

	this->convIndices = convIndices;
}
cv::Mat ConvolutionalLayer::Conv(cv::Mat feature_map, cv::Mat kernel, int stride, int padding)
{
	// 先对特征图填充
	cv::Mat padded_feature_map;
	cv::copyMakeBorder(feature_map, padded_feature_map, padding, padding, padding, padding, cv::BORDER_CONSTANT, 0);
	// 将填充后的特征图的数据类型转换为 double
	padded_feature_map.convertTo(padded_feature_map, CV_64F);

	

	// 卷积
	int row = (padded_feature_map.rows - kernel.rows) / stride + 1;
	int col = (padded_feature_map.cols - kernel.cols) / stride + 1;
	cv::Mat result(row, col, CV_64F);
	
	for(int i=0;i<row;++i)
	for(int j=0;j<col;++j)
	{
		// 计算卷积核在当前位置的
		cv::Mat roi = padded_feature_map(cv::Rect(j * stride, i * stride, kernel.cols, kernel.rows));

		// 计算结果
		double sum = 0;
		for(int m=0;m<kernel.rows;++m)
		for(int n=0;n<kernel.cols;++n)
		{
			sum += roi.at<double>(m, n) * kernel.at<double>(m, n);
		}

		result.at<double>(i, j) = sum;
	}

	return result;
}
std::vector<cv::Mat> ConvolutionalLayer::forward(const std::vector<cv::Mat>& input)
{
	// 先把输入层存入
	this->inputVector = input;

	//  第i个卷积核
	for(int i=0;i<numKernel;++i)
	{
		// 要卷积的特征图下标存在convIndices
		std::vector<int> select = convIndices[i];	
		cv::Mat Result;
		int num = select.size();
		for(int j=0;j<num;++j)
		{
			cv::Mat kernel = kernels[i];
			cv::Mat feature_map = input[select[j]];

			cv::Mat convResult = Conv(feature_map,kernel,stride,padding);
			

			if (j == 0)	Result = convResult;
			else Result = Result + convResult;
		}

		Result += bias;
		outputVector.push_back(Result);
	}

	return outputVector;
}
std::vector<cv::Mat> ConvolutionalLayer::get_outputVector()
{
	return outputVector;
}
std::vector<cv::Mat> ConvolutionalLayer::backward(const std::vector<cv::Mat>& sensitivity,std::vector<std::vector<int>> rt,int T)
{
	// 预处理,初始化最左上角对应的位置
	int output_size = sensitivity[0].rows;
	std::vector<int> pos;
	int v = 0;
	for (int i = 0; i < output_size; ++i)
	{
		pos.push_back(v);
		v += stride;
	}

	// 先算梯度
	// 第i个卷积核
	for(int i=0;i<numKernel;++i)
	{
		cv::Mat result = cv::Mat::zeros(KernelSize,KernelSize,CV_64F);
		std::vector<int> idx = convIndices[i];	// 第i个卷积核卷了的特征图的下标
		
		// 求卷积核(x,y)的梯度
		cv::Mat add = cv::Mat::zeros(output_size, output_size, CV_64F);
		for(int x=0;x<KernelSize;++x)
		for(int y=0;y<KernelSize;++y)
		{
			int x_bias = x, y_bias = y;
			// 将卷积特征图的求和到result中,分别对应下标m、n
			int idx_size = idx.size();
			for(int s = 0;s<idx_size;++s)
			{
				int k = idx[s];
				cv::Mat tmp;
				inputVector[k].convertTo(tmp, CV_64F);
				for(int m=0;m<output_size;++m)
				for(int n=0;n<output_size;++n)
				{
					int id_m = pos[m] + x_bias;
					int id_n = pos[n] + y_bias;

					add.at<double>(m, n) += tmp.at<double>(id_m,id_n);
				}
			}

			// (x,y)的值为add*output[i]
			result.at<double>(x, y) = cv::sum(add.mul(sensitivity[i]))[0];
		}

		gradients.push_back(result);
	}

	// 计算传递给上一层的梯度，6,等价于另外一个卷积
	// 第i个特征输入图,第j个卷积核
	std::vector<cv::Mat> Result;
	int num = inputVector.size();
	int input_size = inputVector[0].rows;
	for(int i=0;i<num;++i)
	{
		std::vector<int> idx = rt[i];
		cv::Mat result(input_size, input_size, CV_64F);
		
		int num_kernel = idx.size();
		for(int s=0;s<num_kernel;++s)
		{
			int idx_j = idx[s];

			// 创建卷积核的旋转180度
			cv::Mat flippedKernel;
			cv::flip(kernels[idx_j], flippedKernel, -1);
			cv::Mat sensitivityMap = sensitivity[idx_j];
			// 公式
			int padding = ( (KernelSize-1)*stride + input_size-output_size )/2;
			cv::Mat tmp = Conv(sensitivityMap,flippedKernel,1,padding);

			result += tmp;
		}

		Result.push_back(result);
	}

	return Result;
}
void ConvolutionalLayer::optim(double alpha)
{
	int num = KernelSize;
	for (int i = 0; i < num; ++i)
		kernels[i] = kernels[i] - alpha * gradients[i];
}
void ConvolutionalLayer::clear()
{
	gradients.clear(); // 清除向量中的所有元素
	inputVector.clear();
	outputVector.clear();
}



void AveragePoolingLayer::init(int size, int stride)
{
	poolingType = AVERAGE_POOLING;
	this->kernelSize = size;
	this->stride = stride;

	// 初始化平均池化卷积核
	this->kernel = cv::Mat::ones(kernelSize, kernelSize, CV_64F) / (kernelSize * kernelSize);

}
cv::Mat AveragePoolingLayer::AvgPool(cv::Mat feature_map, cv::Mat kernel, int stride)
{
	// 池化后尺寸
	int output_height = (feature_map.rows - kernel.rows) / stride + 1;
	int output_width = (feature_map.cols - kernel.cols) / stride + 1;
	cv::Mat pooled(output_height, output_width, feature_map.type());

	// for循环模拟池化操作
	for(int i=0;i<output_height;++i)
	for(int j=0;j<output_width;++j)
	{
		int row_start = i * stride;
		int col_start = j * stride;

		double sum = 0;
		for (int m = 0; m < kernel.rows; ++m)
			for (int n = 0; n < kernel.cols; ++n)
				sum += feature_map.at<double>(row_start + m, col_start + n);

		double avg_value = sum / (kernel.rows * kernel.cols);
		// 将平均值写入池化结果图像中
		pooled.at<double>(i, j) = avg_value;
	}


	return pooled;
}
std::vector<cv::Mat> AveragePoolingLayer::forward(const std::vector<cv::Mat>& input)
{
	std::vector<cv::Mat> ret;

	// 先把输入层存入
	this->inputVector = input;

	int num = input.size();
	for(int i=0;i<num;++i)
	{
		cv::Mat feature_map = input[i];
		cv::Mat Result = AvgPool(feature_map, kernel, stride);

		outputVector.push_back(Result);
		ret.push_back(Result);
	}

	return ret;
}
std::vector<cv::Mat> AveragePoolingLayer::backward(std::vector<cv::Mat> sensitivity, int pooledSize)
{

	std::vector<cv::Mat> result;
	int befSize = inputVector[0].rows;
	double avg = kernelSize * kernelSize;

	int num = sensitivity.size();
	for(int i=0;i<num;++i)
	{
		cv::Mat tmp = cv::Mat::zeros(befSize, befSize, CV_64F);
		// 填充,先选某一个偏导传递
		for(int x=0;x<pooledSize;++x)
		for(int y=0;y<pooledSize;++y)
		{
			// 对于池化后是(x,y)
			double value = sensitivity[i].at<double>(x, y);

			//  算出对应的卷积的左上角
			int a_end = x * kernelSize + kernelSize;
			int b_end = y * kernelSize + kernelSize;
			for(int a=x*kernelSize;a<a_end;++a)
			for(int b=y*kernelSize;b<b_end;++b)
			{
				tmp.at<double>(a, b) = value;
			}
		}

		tmp /= avg;
		result.push_back(tmp);
	}

	return result;
}
void AveragePoolingLayer::clear()
{
	inputVector.clear();
	outputVector.clear();
}



void FullyConnectedLayer::init(int inputSize, int outputSize)
{
	this->InputSize = inputSize, this->OutputSize = outputSize;
	
	w = cv::Mat(outputSize, inputSize, CV_64F);
	cv::randn(w, 0, 0.1);

	b = cv::Mat::zeros(outputSize, 1, CV_64F);
}
cv::Mat FullyConnectedLayer::forward(const cv::Mat& input)
{
	inputVector = input;
	
	outputVector = w * input + b;

	return outputVector;
}
cv::Mat FullyConnectedLayer::get_OutputVector()
{
	return outputVector;
}
cv::Mat FullyConnectedLayer::backward(const cv::Mat& sensitivity)
{
	gradient = cv::Mat(OutputSize,InputSize,CV_64F);
	// Wji
	for(int i=0;i<OutputSize;++i)
	for(int j=0;j<InputSize;++j)
	{
		gradient.at<double>(i,j) = sensitivity.at<double>(i, 0) * inputVector.at<double>(j, 0);
	}

	// 偏导传递
	cv::Mat ret = cv::Mat(InputSize,1,CV_64F);
	for(int i=0;i<InputSize;++i)
	{
		cv::Mat cols = w.col(i);
		cols = cols.mul(sensitivity);
		double value = cv::sum(cols)[0];
		ret.at<double>(i, 0) = value;
	}

	return ret;
}
void FullyConnectedLayer::optim(double alpha)
{
	w = w - alpha * gradient;
}
void FullyConnectedLayer::clear()
{

}
cv::Mat FullyConnectedLayer::get_w()
{
	return w;
}
cv::Mat FullyConnectedLayer::get_b()
{
	return b;
}