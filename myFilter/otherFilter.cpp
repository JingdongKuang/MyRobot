#include "otherFilter.h"

template class MedianFilter<5>;
template class MedianFilter<3>;



// 执行滤波操作
template<int WindowSize>
double MedianFilter<WindowSize>::doFilter(double input) {

    // 更新缓冲区
    buffer_[pos_] = input;
    pos_ = (pos_ + 1) % WindowSize;
    if (count_ < WindowSize) count_++;

    // 如果数据不足，直接返回输入
    if (count_ < WindowSize) {
        return input;
    }
	cout << "buffer_: " << buffer_[0] << "," << buffer_[1] << "," << buffer_[2] << endl;
    // 复制数据并找中值
    temp_ = buffer_;
    std::nth_element(temp_.begin(), temp_.begin() + midIndex_, temp_.end());

	return temp_[midIndex_];
}


template<int WindowSize>
void MedianFilter<WindowSize>::reset() {
    buffer_.fill(0.0);
    pos_ = 0;
    count_ = 0;
}