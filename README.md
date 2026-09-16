<p align="center">
  Preview: https://t.me/wxsdev/14
</p>

## Установка

### 1. Клонирование проекта

```bash
git clone https://github.com/finnalyy/stalzone-ui.git
cd stalzone-ui
```

### 2. Установка vcpkg

```bash
git clone https://github.com/microsoft/vcpkg.git cd C:/vcpkg
.\bootstrap-vcpkg.bat
```

### 3. Сборка проекта

Сгенерируйте проект Visual Studio:

```bash
cmake -S . -B build -G "Visual Studio 18 2026" -A x64 -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build --config Release
```

```.exe``` появится в:

```text
bin/Release/
```

## Devs

* Telegram: [@eleutria](https://t.me/eleutria)
* Telegram: [@wxsdev](https://t.me/wxsdev)

##
<p align="center"><b>Open Source • Eleutria</b></p>
