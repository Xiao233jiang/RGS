# GhostEscape 项目说明

本项目是一个基于 SDL3 和 GLM 的幽灵逃生小游戏，跟随 B 站 up 主 [ZiyuGameDev](https://space.bilibili.com/34930837) 的教程实现。  
参考资料：
- [B 站视频教程](https://www.bilibili.com/video/BV1jf9XYQEhW?spm_id_from=333.788.videopod.sections&vd_source=a8a554eb57c061d8bc9b0b077e956f85)
- [原项目 GitHub](https://github.com/WispSnow/GhostEscape)

---

## 目录结构
```
GhostEscape-Xiaojiang/ 
├── 3rdlib/ # 第三方库（SDL3、GLM等） 
├── Assets/ # 游戏资源（图片、音效等） 
├── Game/ # 游戏主程序源码 
│ ├── src/ # 主要源码 
│ ├── main.cpp # main程序
│ └── CMakeLists.txt # 次级 CMake 配置 
├── CMakeLists.txt # 顶层 CMake 配置 
└── README.md # 项目说明
```
---

## 构建与运行

### 依赖环境

- CMake ≥ 3.10
- C++17 编译器
- SDL3、GLM（已包含在 3rdlib 目录）

### 构建步骤

1. **克隆项目**
```sh
git clone <仓库地址>
cd GhostEscape-Xiaojiang
```
2. 创建构建目录并生成工程文件
```sh
mkdir build
cd build
cmake ..
```
3. 编译
```sh
cmake --build .
```
4. 运行游戏
    可执行文件和依赖的 DLL、资源会自动复制到输出目录，直接运行即可。

---

## CMake 配置说明
### 顶层 CMakeLists.txt
* 设置 C++17 标准
* 支持多平台（Windows、Linux、Android、Darwin、Emscripten）
* 添加 Game 子目录

### Game/CMakeLists.txt
* 配置头文件和库文件路径
* 指定所有源码文件
* 链接 SDL3、GLM 及相关依赖
* 自动复制 DLL 和资源文件到输出目录

---

## 主要功能模块
* 核心系统：游戏主循环、场景管理、对象管理
* 角色与敌人：玩家、敌人、武器、技能等
* UI 系统：状态栏、技能栏、文本标签、按钮等
* 特效与动画：精灵动画、特效、碰撞检测等

---

## 致谢
感谢 ZiyuGameDev 的教程与开源项目，为本项目提供了宝贵的学习资源！
