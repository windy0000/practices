#include <iostream>
#include <fstream>
#include <random>
#include "load_data.h"

std::string getDirfromtxt(std::string path)
{
	std::ifstream file(path);
	if(!file.is_open())
	{
		std::cerr << "Failed to open this file:" << path << std::endl;
	}
	std::string data_path;
	file >> data_path;
	file.close();
	
	return data_path;
}

MNISTData load_data(std::string imageFile, std::string labelFile, int loaded, int batchsize)
{
	MNISTData mnistData;
	// 打开图片和标签文件
	std::ifstream imgFileStream(imageFile, std::ios::binary);
	if(!imgFileStream.is_open())
	{
		std::cerr << "Failed to open the imgDataFile:" << imageFile << std::endl;
	}
	std::ifstream labelFileStream(labelFile, std::ios::binary);
	if(!labelFileStream.is_open())
	{
		std::cerr << "Failed to open the imgDataFile:" << labelFile << std::endl;
	}

	// 跳过文件头部信息和已读取信息，直接读取数据
	imgFileStream.seekg(16);
	labelFileStream.seekg(8);

	imgFileStream.seekg(loaded * WIDTH_AND_HEIGHT * WIDTH_AND_HEIGHT,std::ios::cur);
	labelFileStream.seekg(loaded,std::ios::cur);

	// 开始读取
	int k = 0;
	for(int i=1;i<=batchsize;++i)
	{
		// 如果读到文件末尾，退出
		if (imgFileStream.eof() || labelFileStream.eof())
			break;

		// 加载图片
		cv::Mat digit_image(WIDTH_AND_HEIGHT, WIDTH_AND_HEIGHT, CV_8UC1);
		for(int x=0;x<WIDTH_AND_HEIGHT;++x)
		for(int y=0;y<WIDTH_AND_HEIGHT;++y)
		{
			unsigned char pixel = 0;
			imgFileStream.read((char*)&pixel, sizeof(pixel));
			digit_image.at<uchar>(x,y) = pixel;
		}



		mnistData.image.push_back(digit_image);

		
		// 一个标签只有8位，因为是二进制，不用看大小端
		unsigned char digit_label = 0;
		labelFileStream.read((char*)&digit_label, sizeof(digit_label));
		mnistData.label.push_back((unsigned int)digit_label);

	}

	imgFileStream.close();
	labelFileStream.close();
	return mnistData;
}
