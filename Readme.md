# 🎮 GT Engine

[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](https://opensource.org/licenses/Apache-2.0)

> **GT Engine** (Game Toolkit) 是一个基于 **[The Cherno 的 Hazel 引擎](https://github.com/TheCherno/Hazel)** 进行修改与重构的 **学习研究型项目**。


## 📖 项目介绍

该项目基于 **[The Cherno 的 Hazel 引擎](https://github.com/TheCherno/Hazel)** 。

目前新增 3D模型渲染，粒子系统，以及 资源热重载（用于实时更新 纹理，着色器） 功能 

![GT Engine Editor](docs/images/editor.png)

## 项目规划

1. 添加内置文本编辑器，以实现shader与script实时编辑


## 项目架构

GT/src        GT引擎位置

GT/vendor     GT引擎第三方依赖库

GTEditor      GT编辑器位置

GTScriptCore  GT脚本位置

scripts       存放方案生成脚本


### 构建与运行

Windows：
    1. 双击 "GT\scripts\Win-GenerateProject.bat" 生成 vs solution 文件。
    2. 在 "GT\GT\vendor" 中 复制 assimp-vc143-mt.dll，mono-2.0-sgen.dll 到 "bin"文件夹下程序实际运行位置。
