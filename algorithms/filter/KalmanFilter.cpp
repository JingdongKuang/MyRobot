#include "KalmanFilter.h"

namespace myfilter
{
	KalmanFilter1D::KalmanFilter1D(double initialState, double initialCovariance, double processNoise, double measurementNoise)
		: m_x(initialState)
		, m_p(initialCovariance)
		, m_q(processNoise)
		, m_r(measurementNoise)
	{
	}

	double KalmanFilter1D::Predict(double u)
	{
		m_x += u;
		m_p += m_q;
		return m_x;
	}

	double KalmanFilter1D::Update(double z)
	{
		const double k = m_p / (m_p + m_r);
		m_x = m_x + k * (z - m_x);
		m_p = (1.0 - k) * m_p;
		return m_x;
	}

	double KalmanFilter1D::State() const noexcept
	{
		return m_x;
	}

	double KalmanFilter1D::Covariance() const noexcept
	{
		return m_p;
	}
}