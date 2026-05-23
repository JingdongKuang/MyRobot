#pragma once

#include <vector>

namespace mycontrol
{
// 通用控制器抽象。Compute() 接收参考值与反馈值，输出控制量。
class IController
{
public:
    virtual ~IController() = default;

    // 复位控制器内部状态（积分、上一次误差等）。
    virtual void Reset() = 0;

    // 计算控制输出。
    // reference: 期望值；feedback: 实际反馈值；dt: 调用周期（秒）。
    virtual std::vector<double> Compute(const std::vector<double>& reference,
                                        const std::vector<double>& feedback,
                                        double dt) = 0;
};
}
