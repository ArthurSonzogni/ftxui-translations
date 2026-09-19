# FTXUIにおけるPOSIXパイプ入力

> [!WARNING]
> この機能はLinuxおよびmacOSでのみ動作します。WindowsおよびWebAssemblyではサポートされていません。

## POSIXパイプとは？

POSIXパイプは、2つの異なるプログラムが通信する方法です。一方のプログラムは、その出力を別のプログラムへの入力として直接送信します。データを運ぶ一方通行のチューブのようなものと考えてください。

**Example:**

- `ls`: ファイルを一覧表示します。
- `interactive_grep`: テキストをフィルタリングし、入力できるFTXUIアプリケーションです。

- `ls`: Lists files.
- `interactive_grep`: An FTXUI application that filters text and lets you type.

You can connect them with a pipe (`|`):

```bash
ls -l | interactive_grep
```

何が起こるかというと:
1. `ls -l` は詳細とともにファイルを一覧表示します。
2. `|` はこのリストを `interactive_grep` に直接送信します。
3. `interactive_grep` はリストを受け取り、表示します。FTXUIアプリケーションであるため、`ls` から初期データを受け取ったにもかかわらず、入力してリストをフィルタリングできます。

## FTXUIがパイプ入力を処理する方法

POSIXパイプとは何かを理解したところで、FTXUIがそれらをどのように使用するかを見てみましょう。

FTXUIは、アプリケーションが他のプログラム（パイプからのデータなど）からデータを読み取りながら、キーボードでの対話も可能にします。これは、データを処理する対話型コマンドラインツールに役立ちます。

通常、FTXUIアプリケーションはすべての入力を `stdin` から受け取ります。ただし、FTXUIが `stdin` がパイプの出力に接続されていることを検出すると（つまり、データがアプリケーションにパイプされていることを意味します）、自動的に `/dev/tty` から対話型キーボード入力を読み取るように切り替わります。これにより、アプリケーションはパイプされたデータを処理している間でもユーザー入力を受け取ることができます。

This feature is **turned on by default**.

FTXUIアプリケーションがパイプされたデータを読み取り、キーボード入力にも応答する必要がある場合、通常は特別なことをする必要はありません。

```cpp
auto screen = App::Fullscreen();
// screen.HandlePipedInput(true); // This is enabled by default
screen.Loop(component);
```


## Turning Off Piped Input

If you don't need this feature, or if it conflicts with your custom input handling, you can turn it off.

無効にするには、アプリケーションのメインループを開始する前に `HandlePipedInput(false)` を呼び出します。

```cpp
auto screen = App::Fullscreen();
screen.HandlePipedInput(false); // Turn off piped input handling
screen.Loop(component);
```
