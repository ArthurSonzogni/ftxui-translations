@page installation_conan Conan
@tableofcontents

FTXUI peut être facilement obtenu et intégré à votre projet à l'aide du gestionnaire de paquets Conan.

## Prérequis

Tout d'abord, assurez-vous que Conan est installé sur votre système. Si ce n'est pas le cas, vous pouvez l'installer via pip :

```powershell
pip install conan
```
Conan fonctionne souvent en tandem avec CMake, vous devrez donc également avoir CMake installé. Une fois que vous avez confirmé que Conan et CMake sont installés, créez un répertoire de projet, par exemple, `ftxui-demo` :

```powershell
mkdir C:\ftxui-demo
cd C:\ftxui-demo
```

## Configuration

Après vous être assuré que votre environnement est correctement configuré, créez un fichier de configuration Conan `conanfile.txt`. Ce fichier est utilisé pour déclarer les dépendances de votre projet. Le paquet FTXUI maintenu par la communauté peut être trouvé sur [Conan Center](https://conan.io/center/recipes/ftxui).

> [!note]
> Il s'agit d'un script de construction non officiel. Cela signifie qu'il n'est pas maintenu par l'équipe FTXUI
> mais par la communauté. Le mainteneur du paquet semble le mettre à jour activement
> aux dernières versions. Un grand merci au mainteneur pour son travail !

@todo Si vous êtes familier avec le processus, veuillez envisager d'ajouter un script de construction "officiel" à Conan Center.
Il pourrait s'agir d'une action GitHub qui met à jour automatiquement Conan Center lors de nouvelles versions.

```ini
[requires]
ftxui/6.0.2

[generators]
CMakeDeps
CMakeToolchain

[layout]
cmake_layout
```

## Installer les dépendances et construire

Une fois configuré, exécutez la commande suivante pour installer FTXUI et ses dépendances :

```powershell
conan install . --output-folder=build --build=missing
```

Cela téléchargera et installera `ftxui/6.0.2` ainsi que toutes ses dépendances à partir des référentiels distants de Conan.

Après l'installation terminée, vous pouvez la tester en créant un fichier `demo.cpp` dans votre répertoire de projet :

```cpp
#include <ftxui/screen/screen.hpp>
#include <ftxui/dom/elements.hpp>
#include <iostream>

int main() {
    using namespace ftxui;
    auto document = hbox({
        text(" Hello "),
        text("FTXUI ") | bold | color(Color::Red),
        text(" world! ")
    });
    auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(document));
    Render(screen, document);
    std::cout << screen.ToString() << std::endl;
    return 0;
}
```

Si le test est réussi, vous pouvez alors créer un fichier `CMakeLists.txt` dans le répertoire du projet :

```cmake
cmake_minimum_required(VERSION 3.20)
project(ftxui-demo)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)

# Find the FTXUI package installed via Conan
find_package(ftxui CONFIG REQUIRED)

# Create the executable
add_executable(demo demo.cpp)

# Link the executable to the FTXUI library
target_link_libraries(demo PRIVATE ftxui::component)
```

@todo 考虑到中国多数地区使用Conan很有可能遇到各种网络问题，我想做一个定制的版本说明，但是我对conan的了解有限再加上没有找到合适的资料，因此这个计划短暂的被搁置了，如果您知道方法，欢迎在[中文版本](xiaoditx.girhub.io/public/docs/ftxui%E4%B8%AD%E6%96%87%E7%BF%BB%E8%AF%91/installation/conan/)的下方留下评论以提醒我

---

<div class="section_buttons">

| Précédent          |
|:------------------|
| [Premiers pas](getting-started.html) |

</div>