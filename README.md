<p align="center">
  <img src="https://cdn4.telesco.pe/file/Y6Jd3RGInCf5QBANBvr_-L1xkffHb5WFPhjmCmZ82IN704EGTxW_HEkmFcYanHigt1873gs_S2sVFAD5zRyXAzI6zys0spA5WW7An2z_8k0GR_CnIkzIJiKBFVBKGYliU4QGtrgkYv6rUwHeFK_E7p5pRpipfRrALmW8nB-uUpf28mfDGKxQe1MkwKRnzhF6gmTmm0hWadceBXpkihwSFge97y-TYxwMCIz2X8UhsguzLP_PXkS3Ziudg-Jn1L-Bpj3vGmLfBA0XTEYX-b8xcRwvDeJL3WpzrmiNcmWnmrDPhRTus7_9L8RXjhV1alKFTbTCKiUU_P5iIKZOusXU8Q.jpg" alt="Eleutria Preview">
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
