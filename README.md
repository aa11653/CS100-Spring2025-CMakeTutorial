# CMake 简明教程

> 本教程是上海科技大学 CS100 2025 年学期教学材料的一部分。

PDF 文件发布到见 Release 界面。

## 例子：洛伦兹吸引子轨迹计算命令行程序

洛伦兹吸引子的命令行程序请见：`minimal/`

请使用以下命令编译

```
g++ .\LorenzField.cpp .\TraceQuery.cpp -o .\build\TraceQuery.exe
```

然后，按照以下方式输入初始坐标、选择输出格式

```
Input Initial Position (x, y, z):
2 1 1
Use Python array format? (0 for No, 1 for Yes): 1
```

程序会打印出 $t=0,0.01,0.02,\cdots,5$ 共五百个点的轨迹

```
[[-10, 53, -0.666667], ..., [12.2025, 22.7408, -0.918098]]
Final Position: (7.43099, 8.75662, 23.7125)
```

你可以将数组粘贴到 `plot.py` 后，运行脚本进行可视化。

**使用 CMake 管理后的版本请见：`minimal-cmake/`**

## 例子二：洛伦兹吸引子轨迹计算命令行程序

最后，一个简单的洛伦兹吸引子的可视化程序，使用 Magnum 作为第三方依赖，请见：`minimal-cmake/`

编译后，可以使用 `Up` / `Down` 键加快/减慢模拟速度，使用鼠标拖拽调整视角

## 文档

如果你有编译文档的需求，请使用带有 Excalidraw 和 Code Styler 插件的 Obsidian 打开本教程位于 `doc/` 目录下的源文件 `doc/CMake 简明教程.md`，随后导出为 PDF。