#pragma once

#include "Controller.h"

namespace mycontrol
{
// 单关节/多关节 PID 控制器骨架。
// 当前仅声明接口，具体实现待补充。
class PIDController : public IController
{
public:
    PIDController(double kp, double ki, double kd, std::size_t dof);

    void Reset() override;

    std::vector<double> Compute(const std::vector<double>& reference,
                                const std::vector<double>& feedback,
                                double dt) override;

private:
    double m_kp;
    double m_ki;
    double m_kd;
    std::size_t m_dof;

    std::vector<double> m_integral;
    std::vector<double> m_prevError;
};
}
