#pragma once

namespace myfilter
{
	class KalmanFilter1D
	{
	public:
		KalmanFilter1D(double initialState, double initialCovariance, double processNoise, double measurementNoise);

		// Ô¤²â£ºx = x + u, P = P + Q
		double Predict(double u = 0.0);

		// ¸üÐÂ£ºK = P/(P+R), x = x + K(z-x), P = (1-K)P
		double Update(double z);

		double State() const noexcept;
		double Covariance() const noexcept;

	private:
		double m_x;
		double m_p;
		double m_q;
		double m_r;
	};
}