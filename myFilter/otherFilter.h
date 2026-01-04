#pragma once
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <iostream>  
#include <array>
using namespace std;

//多点中值滤波器，可以用于去除信号中的尖峰噪声
template<int WindowSize>
class MedianFilter {
	// 静态断言：编译时检查
	static_assert(WindowSize > 0, "Window size must be positive.");
	static_assert(WindowSize % 2 != 0, "Window size must be odd.");
	static_assert(WindowSize >= 3, "Window size should be at least 3.");
public://构造函数与析构函数
	//构造函数，传入窗口大小
	MedianFilter() {
		buffer_.fill(0.0);		// 初始化缓冲区
		temp_.fill(0.0);			// 初始化临时数组
		midIndex_ = WindowSize / 2; // 计算中点位置,向上取整
		cout << "construct MedianFilter" << " midIndex_: " << midIndex_ << endl;
	}
	
	//析构函数
	~MedianFilter() = default;


public://公有成员函数
	double doFilter(double input);//执行滤波操作，传入新值，返回滤波后的值
	void reset();//重置滤波器状态
		
private://私有成员变量
	//int windowSize;
	std::array<double, WindowSize> buffer_;// 环形缓冲区
	std::array<double, WindowSize> temp_;   // 临时数组用于排序
	int pos_ = 0;     // 当前写入位置
	int count_ = 0;   // 当前有效数据数量
	int midIndex_; // 中点位置

private://私有成员函数
	
};
