#include "IIRFilter.h"

//构造函数
SosFilter::SosFilter(const Eigen::MatrixXd& sosMatrix, const Eigen::VectorXd& scaleValues, const int dimension)
	: sosMatrix_(sosMatrix), scaleValues_(scaleValues), dimension_(dimension)
{
	if (dimension_ <= 0) {
		throw std::invalid_argument("channels (dimension) must be >= 1");
	}
	numSections_ = static_cast<int>(sosMatrix_.rows());
	stateBuffer_.assign(dimension_, MatrixXd::Zero(numSections_, 2));//初始化每个通道的状态矩阵
	std::cout << "SosFilter initialized with " << dimension_ << " channels and " << numSections_ << " sections." << std::endl;
}


//处理单个样本
double SosFilter::doFilter(double input) {
	double output = input * scaleValues_(0);
	MatrixXd& state = stateBuffer_[0];//标量情况下只有一个通道
	for (int i = 0; i < numSections_; ++i) {
		double b0 = sosMatrix_(i, 0);
		double b1 = sosMatrix_(i, 1);
		double b2 = sosMatrix_(i, 2);
		double a1 = sosMatrix_(i, 4);
		double a2 = sosMatrix_(i, 5);
		
		double w1 = state(i, 0);
		double w2 = state(i, 1);
		double w0 = output - a1 * w1 - a2 * w2;
		output = b0 * w0 + b1 * w1 + b2 * w2;
		output *= scaleValues_(i+1);

		state(i, 0) = w0;
		state(i, 1) = w1;
	}
	return output;
}


// 处理向量样本：inputVec 代表多通道同一时刻的样本，长度必须等于 dimension_
VectorXd SosFilter::doFilter(const VectorXd& inputVec) {
	if (inputVec.size() != dimension_) {
		throw std::invalid_argument("SosFilter::doFilter(VectorXd): input size must equal channel count.");
	}
	
	VectorXd outputVec(dimension_);
	for (int ch = 0; ch < dimension_; ++ch) {
		double y = inputVec(ch) * scaleValues_(0);
		MatrixXd& state = stateBuffer_[ch];

		for (int i = 0; i < numSections_; ++i) {
			const double b0 = sosMatrix_(i, 0);
			const double b1 = sosMatrix_(i, 1);
			const double b2 = sosMatrix_(i, 2);
			const double a1 = sosMatrix_(i, 4);
			const double a2 = sosMatrix_(i, 5);

			const double w1 = state(i, 0);
			const double w2 = state(i, 1);
			const double w0 = y - a1 * w1 - a2 * w2;
			y = b0 * w0 + b1 * w1 + b2 * w2;
			y *= scaleValues_(i + 1);

			state(i, 0) = w0;
			state(i, 1) = w1;
		}
		outputVec(ch) = y;
	}
	return outputVec;
}

// 重置滤波器状态
void SosFilter::reset() {
	for (auto& state : stateBuffer_) {
		state.setZero();
	}
}