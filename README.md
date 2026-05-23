# MyRobot Docker Compose 方案说明

本文档用于说明将 `MyRobot` 项目改造成 **基于 Docker Compose 的 ROS2 仿真项目** 的整体设计思路，目标是让使用者通过一次 `docker compose up` 即可启动完整仿真环境。

## 目标

- 使用 Docker Compose 编排整个仿真系统
- 以 ROS2 作为容器间通信基础
- 将现有 C++ 模块接入 ROS2 仿真工作空间
- 支持后续接入 Gazebo、rviz、ros2_control 等组件
- 保留当前工程结构，尽量减少对现有库代码的侵入

## 整体架构

系统拆分为多个容器服务，各服务通过 ROS2 DDS 在同一网络内通信：

- `gazebo`：负责仿真世界、机器人模型加载、物理引擎运行
- `robot_node`：负责机器人主逻辑，包括规划、控制、滤波等能力
- `ros2_control`：负责控制器管理和关节状态发布
- `rviz`：负责可视化展示，可按需启用

开发者只需要执行：

```bash
docker compose up --build
```

即可启动完整仿真环境。

## 建议的目录结构

```text
MyRobot/
├── docker/
│   ├── Dockerfile.base
│   ├── Dockerfile.robot
│   └── Dockerfile.gazebo
├── ros2_ws/
│   └── src/
│       └── myrobot_ros2/
│           ├── package.xml
│           ├── CMakeLists.txt
│           ├── launch/
│           │   ├── sim.launch.py
│           │   └── robot.launch.py
│           ├── urdf/
│           │   └── myrobot.urdf
│           ├── config/
│           │   └── controllers.yaml
│           └── src/
│               ├── RobotNode.cpp
│               ├── SimServoAdapter.cpp
│               ├── SimServoAdapter.h
│               ├── SimForceSensorAdapter.cpp
│               └── SimForceSensorAdapter.h
├── docker-compose.yml
├── docker-compose.dev.yml
├── .env
└── README.md
```

## Docker 镜像分层设计

### 1. `docker/Dockerfile.base`

作为基础镜像，建议包含以下内容：

- `ros:humble-ros-base`
- CMake、GCC/G++、Make 等基础编译工具
- Eigen3
- `ros2_control` 相关包
- `joint_trajectory_controller` 等控制相关依赖

该镜像的职责是提供统一、可复用的 ROS2 构建环境。

### 2. `docker/Dockerfile.robot`

基于 `Dockerfile.base` 构建，主要职责：

- 拷贝整个项目源码
- 编译当前仓库中的 C++ 模块
- 编译 ROS2 工作空间
- 启动 `RobotNode`

建议入口为：

```bash
ros2 launch myrobot_ros2 robot.launch.py
```

### 3. `docker/Dockerfile.gazebo`

用于启动 Gazebo 仿真环境，建议包含：

- `osrf/ros:humble-desktop`
- `gazebo_ros_pkgs`
- `gazebo_ros2_control`
- 机器人 URDF 和世界文件

建议入口为：

```bash
ros2 launch myrobot_ros2 sim.launch.py
```

## Docker Compose 服务设计

### `gazebo`

职责：

- 加载仿真世界
- 加载机器人 URDF
- 提供仿真时钟、关节状态和物理环境

### `robot_node`

职责：

- 连接现有算法模块
- 接收目标指令
- 发布控制命令
- 与仿真伺服、仿真传感器适配层交互

### `ros2_control`

职责：

- 启动 `controller_manager`
- 管理控制器
- 发布标准关节状态

### `rviz`

职责：

- 展示机器人模型
- 可视化话题与 TF
- 用于调试控制和状态流

## `docker-compose.yml` 的关键配置点

建议包含以下设计：

- 所有服务处于同一个 Docker 网络中
- 所有服务使用相同的 `ROS_DOMAIN_ID`
- `robot_node`、`ros2_control` 依赖 `gazebo` 启动完成后再启动
- 挂载日志目录和录包目录，便于调试和数据保留

示例中的关键环境变量建议通过 `.env` 管理：

```env
ROS_DOMAIN_ID=42
ROBOT_NAME=myrobot
GAZEBO_HEADLESS=true
```

## 开发模式设计

建议提供一个 `docker-compose.dev.yml` 作为开发覆盖文件，用于：

- 挂载宿主机源码目录
- 保留构建缓存
- 进入容器调试
- 避免每次开发都重新 build 镜像

开发模式示例：

```bash
docker compose -f docker-compose.yml -f docker-compose.dev.yml up
```

生产或标准运行模式示例：

```bash
docker compose up
```

## 与当前仓库结构的衔接

当前仓库已经具备较清晰的模块划分，可直接作为 ROS2 节点内部能力来源：

- `math/`：基础数学能力
- `robot_model/`：机器人机械模型
- `algorithms/`：滤波、规划、控制等算法模块
- `external/`：伺服、传感器等外部接口相关模块

因此推荐的改造方式不是重写现有代码，而是：

1. 保留现有 C++ 库结构
2. 在 `ros2_ws/src/myrobot_ros2/` 中新增 ROS2 包作为集成层
3. 通过适配器把现有模块接入 ROS2 topic / service / action
4. 通过 Docker Compose 编排整个运行环境

## 对现有工程的建议改动

为适配 Docker Compose + ROS2 方案，建议后续补充：

- 根目录 `docker-compose.yml`
- 根目录 `docker-compose.dev.yml`
- 根目录 `.env`
- `docker/` 目录下的 Dockerfile
- `ros2_ws/` 工作空间
- `.gitignore` 中增加 ROS2 构建产物和仿真数据忽略项

如有需要，还可以在顶层 `CMakeLists.txt` 中增加单独的 ROS2 相关构建开关，但应尽量避免影响现有非 ROS2 构建流程。

## 启动流程示意

```bash
git clone <repository-url>
cd MyRobot
docker compose up --build
```

启动后，可以通过进入容器或通过 ROS2 命令向机器人发送目标指令，观察 Gazebo 中的运动效果。

## 推荐实施顺序

1. 先完成基础镜像 `Dockerfile.base`
2. 再完成机器人节点镜像 `Dockerfile.robot`
3. 再完成 Gazebo 镜像 `Dockerfile.gazebo`
4. 编写 `docker-compose.yml`
5. 增加 `docker-compose.dev.yml`
6. 构建 ROS2 工作空间与适配层
7. 补充 URDF、launch、controller 配置

## 总结

这个方案的核心目标是：**将现有 MyRobot 项目包装为一个可通过 Docker Compose 一键启动的 ROS2 仿真系统**。

这样做的好处包括：

- 环境统一
- 部署简单
- 后续便于扩展 Gazebo、rviz、控制器和传感器
- 便于本地开发和未来 CI/CD 集成

后续如果正式开始实施，可以在此 README 基础上继续补充实际文件、启动命令和部署说明。
