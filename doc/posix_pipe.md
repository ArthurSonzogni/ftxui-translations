# FTXUI 中的 POSIX 管道輸入

> [!WARNING]
> 此功能僅適用於 Linux 和 macOS。不支援 Windows 和 WebAssembly。

## 什麼是 POSIX 管道？

POSIX 管道是兩個獨立程式之間通訊的一種方式。一個程式將其輸出直接作為輸入發送給另一個程式。將其想像成一條單向的資料管道。

**範例：**

想像您想要列出檔案然後以互動方式過濾它們。

- `ls`：列出檔案。
- `interactive_grep`：一個用於過濾文字並讓您輸入的 FTXUI 應用程式。

您可以使用管道 (`|`) 將它們連接起來：

```bash
ls -l | interactive_grep
```

以下是發生的情況：
1. `ls -l` 列出帶有詳細資訊的檔案。
2. `|` 將此列表直接發送給 `interactive_grep`。
3. `interactive_grep` 接收列表並顯示它。因為它是一個 FTXUI 應用程式，即使它從 `ls` 接收到初始資料，您仍然可以輸入以過濾列表。

## FTXUI 如何處理管道輸入

現在您已經了解了什麼是 POSIX 管道，讓我們看看 FTXUI 如何使用它們。

FTXUI 允許您的應用程式從其他程式（例如從管道）讀取資料，同時仍然允許您使用鍵盤進行互動。這對於處理資料的互動式命令列工具非常有用。

通常，FTXUI 應用程式從 `stdin` 接收所有輸入。但是，當 FTXUI 檢測到 `stdin` 連接到管道的輸出時（表示資料正在透過管道傳輸到您的應用程式中），它會自動切換為從 `/dev/tty` 讀取互動式鍵盤輸入。這確保了您的應用程式即使在處理管道資料時，仍然可以接收使用者輸入。

此功能**預設為開啟**。

如果您的 FTXUI 應用程式需要讀取管道資料並回應鍵盤輸入，您通常不需要做任何特殊操作：

```cpp
auto screen = ScreenInteractive::Fullscreen();
// screen.HandlePipedInput(true); // 這預設是啟用的
screen.Loop(component);
```


## 關閉管道輸入

如果您不需要此功能，或者它與您的自訂輸入處理衝突，您可以將其關閉。

要禁用它，請在啟動應用程式的主循環之前呼叫 `HandlePipedInput(false)`：

```cpp
auto screen = ScreenInteractive::Fullscreen();
screen.HandlePipedInput(false); // 關閉管道輸入處理
screen.Loop(component);
```