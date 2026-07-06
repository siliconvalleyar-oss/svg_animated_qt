# Instalación

## Requisitos

- Qt 6.x (Core, Gui, Widgets, Svg, Xml)
- CMake 3.20+
- Compilador C++17 (GCC, Clang, MSVC)

## Plataformas soportadas

- Windows (10/11)
- macOS
- Linux (Ubuntu, Debian, etc.)

## Instalar dependencias

### Windows

1. Instalar Qt 6 desde https://www.qt.io/download
2. Instalar CMake desde https://cmake.org/download
3. Instalar Visual Studio 2019+ con soporte C++

### macOS

```bash
brew install qt@6 cmake
```

### Linux (Ubuntu/Debian)

```bash
sudo apt install qt6-base-dev qt6-svg-dev cmake g++
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
./svg-animator
```

## Estructura de archivos

Los archivos SVG se cargan mediante el selector de archivos o drag & drop directamente en la aplicación.

## Solución de problemas

### Error: Qt6 not found

Asegúrese de que Qt6 está en el PATH o especifique la ruta:

```bash
cmake -DCMAKE_PREFIX_PATH=/path/to/qt6 ..
```

### Error: CMake version

Se requiere CMake 3.20 o superior:

```bash
cmake --version
```
