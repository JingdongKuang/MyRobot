#include "PIDController.h"

#include <algorithm>
#include <stdexcept>

namespace mycontrol
{
PIDController::PIDController(double kp, double ki, double kd, std::size_t dof)
    : m_kp(kp), m_ki(ki), m_kd(kd), m_dof(dof),
      m_integral(dof, 0.0), m_prevError(dof, 0.0)
{
}

void PIDController::Reset()
{
    std::fill(m_integral.begin(), m_integral.end(), 0.0);
    std::fill(m_prevError.begin(), m_prevError.end(), 0.0);
}

std::vector<double> PIDController::Compute(const std::vector<double>& reference,
                                           const std::vector<double>& feedback,
                                           double /*dt*/)
{
    if (reference.size() != m_dof || feedback.size() != m_dof)
    {
        throw std::invalid_argument("reference/feedback size mismatch with dof");
    }

    // TODO: 完整 PID 公式 (含 dt 的积分/微分项) 在此处实现。
    std::vector<double> output(m_dof, 0.0);
    return output;
}
}
