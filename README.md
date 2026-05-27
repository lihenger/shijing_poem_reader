# shijing_poem_reader
Shijing Poem Reader

一个基于 C++ 和 Qt 框架开发的《诗经》阅读器桌面应用，提供简洁直观的界面，方便用户阅读和赏析中国最早的诗歌总集——《诗经》。

《诗经》是中国古代诗歌的开端，最早的一部诗歌总集，收集了西周初年至春秋中叶（前11世纪至前6世纪）的诗歌，共 311 篇（其中 6 篇为笙诗，只有标题没有内容），反映了周初至周晚期约五百年间的社会面貌。

本应用旨在以现代化的方式呈现这部经典作品，让读者能够方便地浏览、检索和学习《诗经》中的诗歌。

功能特性

· 诗歌浏览：清晰展示诗歌原文，支持现代文翻译对照
· 全文搜索：可按诗歌标题、原文内容或翻译进行检索
· 随机一首：随机跳转至任意一篇诗歌，适合随手翻阅品味
· 跨平台支持：基于 Qt 框架开发，支持 Windows、Linux、macOS 主流操作系统

技术栈

· 开发语言：C++
· GUI 框架：Qt Widgets（Qt5/Qt6）
· 数据处理：JSON 格式存储诗歌数据，使用 json.hpp 进行解析
· 设计模式：单例模式（PoemManager 管理全局诗歌数据）
· 构建工具：Visual Studio / qmake / CMake

项目结构

```
shijing_poem_reader/
├── QtWidgetsApplication/          # 主项目目录
│   ├── main.cpp                   # 程序入口
│   ├── mainwindow.h/.cpp/.ui      # 主窗口界面
│   ├── poemManager.h/.cpp         # 诗歌数据管理（单例模式）
│   ├── searchwidget.h/.cpp/.ui    # 搜索功能组件
│   ├── randompoemwidget.h/.cpp/.ui# 随机诗歌组件
│   ├── aboutdialog.h/.cpp/.ui     # 关于对话框
│   ├── shijing.json               # 《诗经》数据文件
│   └── resources.qrc              # Qt 资源文件
├── README.md                      # 项目说明文档
├── .gitattributes                 # Git 属性配置
└── .gitignore                     # Git 忽略文件配置
```

构建与运行

环境要求

· C++11 及以上标准的编译器
· Qt 5.12 或更高版本（包含 Qt Widgets 模块）
· CMake 3.10+ 或 Visual Studio 2019/2022

Windows (Visual Studio)

1. 克隆仓库：
   ```bash
   git clone https://github.com/lihenger/shijing_poem_reader.git
   ```
2. 使用 Visual Studio 打开 QtWidgetsApplication.slnx 解决方案文件
3. 确保已正确安装 Qt Visual Studio Tools 插件并配置 Qt 版本路径
4. 编译并运行项目

Linux / macOS (CMake)

1. 克隆仓库：
   ```bash
   git clone https://github.com/lihenger/shijing_poem_reader.git
   cd shijing_poem_reader
   ```
2. 使用 CMake 构建：
   ```bash
   mkdir build && cd build
   cmake ..
   make
   ```
3. 运行可执行文件：
   ```bash
   ./QtWidgetsApplication
   ```

数据来源

项目使用的《诗经》数据以 JSON 格式存储在 shijing.json 中，包含诗歌的：

· 原文（汉字全文）
· 现代汉语翻译
· 诗歌标题及所属分类（风、雅、颂）

贡献指南

欢迎提交 Pull Request 或 Issue 来改进项目。

如果你希望为本项目做出贡献，可以从以下几个方面入手：

· 完善或校对诗歌翻译
· 增加诗歌注释或赏析内容
· 改进 UI 交互体验
· 添加更多功能（如朗读、收藏、笔记等）

开源协议

本项目采用 MIT License 开源协议，详情请参见 LICENSE 文件。这意味着你可以自由地使用、修改和分发本项目的代码。

致谢

· 感谢 Qt 团队提供优秀的跨平台 GUI 开发框架
· 感谢 json.hpp 库的作者 nlohmann 提供简洁易用的 JSON 解析器

---

愿古老的诗歌在现代的屏幕中依然焕发光彩 🌾

————
⚠️ 说明：本 README 中的技术架构信息基于项目源码分析整理，部分内容（如构建方式、数据范围等）建议根据实际代码情况进行补充和调整。
