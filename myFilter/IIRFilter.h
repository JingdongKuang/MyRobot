#pragma once

#include <Eigen/Dense>
#include <vector>
#include <iostream>
#include <stdexcept>

using namespace Eigen;

class SosFilter {
public:
	//构造函数，通过输入Eigen库的MatrixXd指定SOSMatrix，VectorXd 指定ScaleValues。
	SosFilter(const Eigen::MatrixXd& sosMatrix, const Eigen::VectorXd& scaleValues, const int dimension);

	~SosFilter() = default;

public:
	double doFilter(double input);//处理单个样本
	VectorXd doFilter(const VectorXd& inputVec);//处理向量样本
	void reset();

private://私有函数
	

private://私有变量
	const MatrixXd sosMatrix_;
	const VectorXd scaleValues_;
	std::vector<MatrixXd> stateBuffer_;// 每通道一个 [numSections x 2] 状态矩阵
	int numSections_;
	int dimension_; // 1: 标量模式；>1: 向量模式
};
