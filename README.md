# Qt6 图片浏览器

基于 Qt6 的桌面图片浏览器，支持目录树浏览、图片预览、幻灯片播放和背景音乐。

## 功能

- 目录树浏览本地文件夹
- 缩略图列表 + 大图预览
- 幻灯片自动播放
- 背景音乐播放 (Qt6 Multimedia)
- QSS 样式美化

## 依赖

- Qt 6.11.1 MinGW 64-bit
- Qt6 Widgets / Multimedia

## 工程结构

```
├── main.cpp / mainwindow.*    # 主窗口
├── protree.*                   # 目录树
├── protreeitem.*               # 目录树项
├── protreethread.*             # 目录树异步加载
├── protreewidget.*             # 目录树控件
├── picshow.*                   # 图片预览
├── picanimation.*              # 图片动画
├── picbutton.*                 # 图片按钮
├── prelistwid.*                # 预览列表
├── style/style.qss             # QSS 样式
└── res/                        # UI 图标资源
```

## 编译

- Qt Creator + MinGW 64-bit
- 打开 `Album.pro`，执行 qmake 后构建
