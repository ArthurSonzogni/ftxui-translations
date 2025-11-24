@page installation_cmake CMake

@tableofcontents

本頁說明如何使用 [CMake](https://cmake.org) 依賴 FTXUI。

# 整合方法

## 使用 FetchContent

這種方法會在配置時下載 FTXUI，不需要全系統安裝。

```cmake
include(FetchContent)

FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI
  GIT_TAG v6.1.9  # 請替換為版本、標籤或提交雜湊
)

FetchContent_MakeAvailable(ftxui)

add_executable(main main.cpp)
target_link_libraries(main
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
)
```

這確保了可重現的構建和輕鬆的依賴管理。

## 使用 find_package

如果 FTXUI 已在系統範圍內安裝或透過套件管理器（例如 vcpkg 或 Conan）安裝，您可以使用：

```cmake
find_package(ftxui REQUIRED)

add_executable(main main.cpp)
target_link_libraries(main
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
)
```

請確保該套件在您的 `CMAKE_PREFIX_PATH` 中可見。

## 使用 git submodule

您也可以將 FTXUI 作為 [Git 子模組](https://git-scm.com/book/en/v2/Git-Tools-Submodules) 添加，使其成為您儲存庫的一部分：

```cmake
git submodule add https://github.com/ArthurSonzogni/FTXUI external/ftxui
git submodule update --init --recursive
```

當克隆已包含 FTXUI 作為子模組的儲存庫時，請確保使用以下命令獲取子模組：

```
git clone --recurse-submodules <your-repo>
# 或者，如果已克隆：
git submodule update --init --recursive
```

然後在您的 `CMakeLists.txt` 中：

```cmake
add_subdirectory(external/ftxui)

add_executable(main main.cpp)
target_link_libraries(main
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
)
```

如果您想在自己的儲存庫中供應 FTXUI，這種方法效果很好。

# 可選的 CMake 旗標

FTXUI 支援以下 CMake 選項：

| 選項                            | 描述                   | 預設值 |
| --------------------------------- | ----------------------------- | ------- |
| FTXUI_BUILD_EXAMPLES              | 構建捆綁的範例        | OFF     |
| FTXUI_BUILD_DOCS                  | 構建文件       | OFF     |
| FTXUI_BUILD_TESTS                 | 啟用測試                  | OFF     |
| FTXUI_ENABLE_INSTALL              | 生成安裝目標      | ON      |
| FTXUI_MICROSOFT_TERMINAL_FALLBACK | 改善 Windows 相容性 | ON/OFF  |


要啟用選項：

```
cmake -DFTXUI_BUILD_EXAMPLES=ON ..
```

# 驗證整合

要確認設置正常，請構建並運行一個最小範例。
如果您需要完整的範本，請參閱：[ftxui-starter](https://github.com/ArthurSonzogni/ftxui-starter)

<div class="section_buttons">
 
| 上一個          |
|:------------------|
| [入門](getting-started.html) |
 
</div>