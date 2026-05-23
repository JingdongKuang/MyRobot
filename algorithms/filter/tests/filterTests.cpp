#include <cassert>
#include <cmath>
#include "KalmanFilter.h"
#include <iostream>

int main()
{
	using myfilter::KalmanFilter1D;

	KalmanFilter1D kf(0.0, 1.0, 0.01, 0.1);

	// ‘§≤‚≤‚ ‘
	kf.Predict(2.0);
	assert(std::abs(kf.State() - 2.0) < 1e-9);

	//  ’¡≤≤‚ ‘
	for (int i = 0; i < 20; ++i)
	{
		kf.Update(1.0);
	}
	assert(std::abs(kf.State() - 1.0) < 0.1);
	std::cout << "filterTests passed!" << std::endl;
	return 0;
}
