# Instalación

## Requisitos

- Qt 5.x (Core, Gui, Widgets, Svg, Xml)
- CMake 3.16+
- Compilador C++17 (GCC, Clang, MSVC)

## Plataformas soportadas

- Windows (10/11)
- macOS
- Linux (Ubuntu, Debian, etc.)

## Instalar dependencias

### Windows

1. Instalar Qt 5.x desde https://www.qt.io/download-qt-installer
2. Durante la instalación, seleccionar:
   - Qt 5.x.x → MinGW 64-bit o MSVC 2017/2019
   - Componentes: Core, Gui, Widgets, Svg, Xml
3. Instalar CMake desde https://cmake.org/download
4. Instalar Visual Studio 2017+ con soporte C++ (si usa MSVC)

### macOS

```bash
brew install qt@5 cmake
```

### Linux (Ubuntu/Debian)

```bash
sudo apt install qt5-default qtbase5-dev libqt5svg5-dev cmake g++
```

## Compilación

```bash
cd qt
mkdir build && cd build
cmake ..
cmake --build .
```

## Ejecución

```bash
# Windows
.\Release\svg-animator.exe

# Linux/Mac
./svg-animator
```

## Solución de problemas

### Error: Qt5 not found

Asegúrese de que Qt5 está en el PATH o especifique la ruta:

```bash
# Windows
cmake -DCMAKE_PREFIX_PATH="C:/Qt/5.15.2/mingw81_64" ..

# Linux
cmake -DCMAKE_PREFIX_PATH="/usr/lib/x86_64-linux-gnu/cmake/Qt5" ..
```

### Error: CMake version

Se requiere CMake 3.16 o superior:

```bash
cmake --version
```
