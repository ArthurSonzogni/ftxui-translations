# FTXUI 中的 POSIX 管道输入

> [!WARNING]
> 此功能仅适用于 Linux 和 macOS。Windows 和 WebAssembly 不支持此功能。

## 什么是 POSIX 管道？

POSIX 管道是两个独立程序之间通信的一种方式。一个程序将其输出直接作为另一个程序的输入。可以把它想象成一个单向的数据传输管道。

**示例：**

假设您想列出文件然后以交互方式过滤它们。

- `ls`：列出文件。
- `interactive_grep`：一个 FTXUI 应用程序，用于过滤文本并允许您键入。

您可以使用管道 (`|`) 连接它们：

```bash
ls -l | interactive_grep
```

以下是发生的情况：
1. `ls -l` 列出文件详细信息。
2. `|` 将此列表直接发送到 `interactive_grep`。
3. `interactive_grep` 接收列表并显示它。因为这是一个 FTXUI 应用程序，所以您可以键入以过滤列表，即使它从 `ls` 接收了初始数据。

## FTXUI 如何处理管道输入

现在您了解了什么是 POSIX 管道，接下来让我们看看 FTXUI 如何使用它们。

FTXUI 允许您的应用程序从其他程序（例如来自管道）读取数据，同时仍允许您使用键盘进行交互。这对于处理数据的交互式命令行工具非常有用。

通常，FTXUI 应用程序从 `stdin` 接收所有输入。但是，当 FTXUI 检测到 `stdin` 连接到管道的输出时（意味着数据正在通过管道输入到您的应用程序中），它会自动切换到从 `/dev/tty` 读取交互式键盘输入。这可确保您的应用程序即使在处理管道数据时仍能接收用户输入。

此功能 **默认开启**。

如果您的 FTXUI 应用程序需要读取管道数据并响应键盘输入，您通常不需要做任何特殊的事情：

```cpp
auto screen = ScreenInteractive::Fullscreen();
// screen.HandlePipedInput(true); // 此功能默认开启
screen.Loop(component);
```


## 关闭管道输入

如果您不需要此功能，或者它与您的自定义输入处理冲突，则可以将其关闭。

要禁用它，请在启动应用程序的主循环之前调用 `HandlePipedInput(false)`：

```cpp
auto screen = ScreenInteractive::Fullscreen();
screen.HandlePipedInput(false); // 关闭管道输入处理
screen.Loop(component);
```
