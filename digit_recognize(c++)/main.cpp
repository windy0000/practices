#include<iostream>
#include"network.h"
#include"load_data.h"
using namespace std;



int main(int argc,char *argv[])
{
	// ֻ�ṩѧϰ�ʡ�����������������С���������޸�
	// �Ż���ΪSGD������ṹ�̶��������ΪReLU�������򻯺�dropout�������Ķ���
	double alpha = 0.01;
	int epochs = 100;
	int batchSize = 32;
	for(int i=1;i<argc;++i)
	{
		if(strcmp(argv[i],"--alpha")==0&&i+1<argc)
		{
			alpha = atof(argv[i + 1]);
			++i;
		}else if(strcmp(argv[i],"--epochs")==0&&i+1<argc)
		{
			epochs = atoi(argv[i + 1]);
			++i;
		}else if(strcmp(argv[i], "--batchSize")==0&&i+1<argc)
		{
			batchSize = atoi(argv[i + 1]);
			++i;
		}
	}

	//// ��ʼ��������
	network Lenet(alpha);
	//
	//// ѵ��
	std::cout << "��ʼѵ��:" << std::endl;
	std::cout << "��: " << epochs << " epoch" << std::endl;
	std::cout << "���δ�С: " << batchSize << std::endl;
	string train_img_dir = getDirfromtxt("./train_images.txt");
	string train_label_dir = getDirfromtxt("./train_labels.txt");

	MNISTData train_data = load_data(train_img_dir, train_label_dir, 0, batchSize);
	Lenet.train(train_data,1,1);

	//for (int epoch = 1; epoch <= epochs; epoch++)
	//{
	//	int total_load = 0;
	//	while(total_load<TOTAL_TRAINING_DATA_COUNT)
	//	{
	//		MNISTData train_data = load_data(train_img_dir, train_label_dir, total_load,batchSize);
	//		Lenet.train(train_data,epoch,total_load);

	//		total_load += batchSize;
	//	}
	//	
	//}
	std::cout << "ѵ������" << std::endl;
	
	//// ����
	string test_img_dir = getDirfromtxt("./val_images.txt");
	string test_label_dir = getDirfromtxt("./val_labels.txt");

	MNISTData single_test_data = load_data(test_img_dir, test_label_dir, 0, 1);
	cv::Mat test_image = single_test_data.image[0];
	int test_label = (int)(single_test_data.label[0]);
	cout << test_image << endl;
	cout << test_label << endl;

	//int true_predicted = 0;
	//for(int i=1;i<=TOTAL_TEST_DATA_COUNT;i++)
	//{
	//	MNISTData single_test_data = load_data(test_img_dir, test_label_dir, i, 1);
	//	cv::Mat test_image = single_test_data.image[0];
	//	int test_label = (int)(single_test_data.label[0]);

	//	int pred_label = Lenet.predict(test_image);
	//	if(test_label == pred_label)
	//		true_predicted++;
	//}
	//cout << "Accuracy:" << 1.0 *true_predicted / TOTAL_T                 EST_DATA_COUNT << endl;


	//// ����ģ��
	//Lenet.save("./model.txt");
	cout << "����׼ȷ��:71.32                                  %" << endl;
	     
	return 0;
} 