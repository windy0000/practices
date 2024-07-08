#include "network.h"
#include "load_data.h"
#include "layer.h"
#include "loss_function.h"
#include "activation_function.h"
#include "utils.h"
#include <fstream>

network::network(double learningRate)
{
	this->learningrate = learningRate;

	// ��������ṹ
	std::vector<std::vector<int>> d1 = {
		{0},{0},{0},{0},{0},{0}
	};
	C1.init(6, 5, 1, 0,d1);

	S2.init(2, 2);

	std::vector<std::vector<int>> d2 = {
		{0,1,2},
		{1,2,3},
		{2,3,4},
		{3,4,5},
		{4,5,0},
		{5,0,1},
		{0,1,2,3},
		{1,2,3,4},
		{2,3,4,5},
		{3,4,5,0},
		{4,5,0,1},
		{5,0,1,2},
		{0,1,3,4},
		{1,2,4,5},
		{0,2,3,5},
		{0,1,2,3,4,5}
	};
	C3.init(16,5,1, 0,d2);

	S4.init(2,2);

	
	F5.init(16 * 5 * 5, 120);	// ��ȫ���Ӳ�֮ǰ�ǵõð�������ƽ
	F6.init(120, 84);

	

	// �����Ҳ��ȫ���Ӳ�
	outputLayer.init(84,10);
}

cv::Mat network::forward(cv::Mat input)
{
	// �Ƚ�ͼ�������32*32
	cv::Mat padded_image = input;
	cv::copyMakeBorder(padded_image, padded_image, 2, 2, 2, 2, cv::BORDER_CONSTANT, cv::Scalar(0));

	
	std::vector<cv::Mat> output;	// ��ʱ�洢���
	output.push_back(padded_image);
	// ���ز�
	output = C1.forward(output);
	output = get_sigmoid(output);
	
	output = S2.forward(output);
	
	output = C3.forward(output);
	output = get_sigmoid(output);

	output = S4.forward(output);
	
	cv::Mat flattened_output = flattenFeatureMaps(output);	// �ǵ�����һά��
	flattened_output = F5.forward(flattened_output);
	flattened_output = sigmoid(flattened_output);
	

	flattened_output = F6.forward(flattened_output);
	flattened_output = sigmoid(flattened_output);

	// �����
	flattened_output = outputLayer.forward(flattened_output);
	
	return flattened_output;
}

void network::backward(cv::Mat y_true, cv::Mat y_pred)
{
	std::vector<cv::Mat> tmp;		// ��ʱ�洢ÿһ������ж�(���ڼ���ֵƫ��)
	cv::Mat oneDimension_tmp;		// һά�Ĵ洢ÿһ������ж�(���ڼ���ֵƫ��)

	// �����ķ��򴫲�
	// �����sofmmax���ֵ����loss�ĵ�
	// Ȼ����softmax�Ķ��ڼ���ֵ��(����ǰ��һ��)
	// �����㱾��ĵ�
	cv::Mat y_hat = softmax(y_pred);
	oneDimension_tmp = cross_entropy_softmax_backward(y_hat,y_true);	// ����loss�������һ�㼤��ֵ��ƫ��
	oneDimension_tmp =  outputLayer.backward(oneDimension_tmp);
	
	
	// ���ز㷴�򴫲�
	cv::Mat output = F6.get_OutputVector();
	cv::Mat sigmoid_tmp = sigmoid_gradient(output);
	oneDimension_tmp = oneDimension_tmp.mul(sigmoid_tmp);
	oneDimension_tmp = F6.backward(oneDimension_tmp);
	
	output = F5.get_OutputVector();
	sigmoid_tmp = sigmoid_gradient(output);
	oneDimension_tmp = oneDimension_tmp.mul(sigmoid_tmp);
	oneDimension_tmp = F5.backward(oneDimension_tmp);
	

	tmp = ReshapeFlattened(oneDimension_tmp, 16, 5, 5);// ������������
	tmp = S4.backward(tmp,5);

	std::vector<cv::Mat> outputs = C3.get_outputVector();
	outputs = get_sigmoid_gradient(outputs);
	tmp = mul(tmp, outputs);
	std::vector<std::vector<int>> rd2 = {
		{0,4,5,6,9,10,11,12,14,15},
		{1,2,5,6,7,10,11,12,13,15},
		{0,1,2,6,7,8,11,13,14,15},
		{1,2,3,6,7,8,9,12,14,15},
		{2,3,4,7,8,9,10,12,13,15},
		{3,4,5,8,9,10,11,13,14,15}
	};
	tmp = C3.backward(tmp,rd2,0);

	tmp = S2.backward(tmp, 14);
	
	outputs = C1.get_outputVector();
	outputs = get_sigmoid_gradient(outputs);
	tmp = mul(tmp, outputs);
	std::vector<std::vector<int>> rd1 = {
		{0,1,2,3,4}
	};
	tmp = C1.backward(tmp, rd1,1);

}

void network::optim(double alpha)
{	
	outputLayer.optim(alpha);
	F6.optim(alpha);
	F5.optim(alpha);
	C3.optim(alpha);
	C1.optim(alpha);
}

void network::clear()
{
	outputLayer.clear();
	F6.clear();
	F5.clear();
	S4.clear();
	C3.clear();
	S2.clear();
	C1.clear();
}

void network::train(MNISTData& train_data,int epoch,int total_loaded)
{
	int num = train_data.image.size();
	// һ��һ��ͼƬ��ѵ��
	for(int i=0;i<num;++i)
	{
		cv::Mat img = train_data.image[i];
		double label = train_data.label[i];

		// Ԥ��
		// Ҫ�Ȱѱ�ǩת��������(���ȱ���)
		cv::Mat y_pred = forward(img);
		cv::Mat y_hat = softmax(y_pred);
		cv::Mat one_hot = cv::Mat::zeros(10, 1, CV_64F);
		one_hot.at<double>((int)label, 0) = 1.0;

		// ���򴫲��Ż�
		backward(one_hot,y_hat);
		optim(learningrate);
		clear();

		if ((i+total_loaded) % 50 == 0)
			train_progress_bar(epoch,TOTAL_TRAINING_DATA_COUNT,total_loaded,i);
	}
}

int network::predict(cv::Mat input)
{
	cv::Mat y_pred = forward(input);
	cv::Mat y_hat = softmax(y_pred);
	clear();

	int idx = 0;
	for (int i = 0; i < 9; i++)
		if (y_pred.at<double>(i, 0) > y_pred.at<double>(idx, 0))
			idx = i;

	return idx;
}

void network::save(std::string path)
{
	std::ofstream file(path, std::ios::trunc);
	if (!file.is_open()) {
		std::cerr << "Error: Failed to open file " << path << std::endl;
		return;
	}

	file << "outputLayer:w b" << std::endl;
	file << outputLayer.get_w() << std::endl;
	file << outputLayer.get_b() << std::endl;
}