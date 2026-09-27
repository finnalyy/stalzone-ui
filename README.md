## Preview

<p align="center">
  <img src="https://i.imgur.com/8A3kv0k.png" alt="STALZONE UI Preview">
</p>

## Features

* Native Windows application
* ImGui-based interface
* DirectX 11 rendering
* HTTP requests via cURL
* JSON processing via nlohmann/json
* x64 build support

## Tech Stack

| Technology    | Purpose                      |
| ------------- | ---------------------------- |
| C++17         | Core application             |
| CMake         | Build system                 |
| DirectX 11    | Rendering                    |
| ImGui         | User interface               |
| cURL          | HTTP requests                |
| nlohmann/json | JSON serialization / parsing |
| vcpkg         | Dependency management        |

## Requirements

* Windows 10/11
* Visual Studio with C++ desktop development tools
* CMake 3.16+
* Git
* vcpkg

## Build

```bash
git clone https://github.com/finnalyy/stalzone-ui.git
cd stalzone-ui
```

Configure:

```bash
cmake -S . -B build -A x64
```

Build:

```bash
cmake --build build --config Release
```

## Project Structure

```text
stalzone-ui/
├── src/
│   ├── SDK/
│   │   ├── DirectX/
│   │   └── ImGui/
│   ├── UI/
│   └── Utils/
├── script/
├── CMakeLists.txt
├── vcpkg.json
├── .gitmodules
└── README.md
```

## Contact

* Telegram: [@eleutria](https://t.me/eleutria)
* Telegram: [@wxsdev](https://t.me/wxsdev)
