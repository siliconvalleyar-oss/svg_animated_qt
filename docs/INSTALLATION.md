# Instalación

## Requisitos

- Qt 5.x (Core, Gui, Widgets, Svg, Xml)
- CMake 3.16+
- Compilador C++17 (GCC, Clang, MSVC)

## Instalar dependencias

### Windows

1. **Instalar Qt 5.x**
   - Descargar desde: https://www.qt.io/download-qt-installer
   - Durante la instalación, seleccionar:
     - Qt 5.15.2 → MinGW 8.1.0 64-bit (recomendado) o MSVC 2019
     - Componentes: Core, Gui, Widgets, Svg, Xml

2. **Instalar CMake**
   - Descargar desde: https://cmake.org/download
   - Marcar "Add CMake to system PATH"

3. **Instalar compilador** (si usa MinGW)
   - Incluido con Qt installer
   - O instalar MSYS2: https://www.msys2.org/

4. **Agregar Qt al PATH**
   ```
   C:\Qt\5.15.2\mingw81_64\bin
   ```

### macOS

1. **Instalar Homebrew** (si no está instalado)
   ```bash
   /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
   ```

2. **Instalar Qt 5**
   ```bash
   brew install qt@5
   ```

3. **Agregar Qt al PATH**
   ```bash
   echo 'export PATH="/opt/homebrew/opt/qt@5/bin:$PATH"' >> ~/.zshrc
   source ~/.zshrc
   ```

4. **Configurar CMake**
   ```bash
   echo 'export CMAKE_PREFIX_PATH="/opt/homebrew/opt/qt@5"' >> ~/.zshrc
   source ~/.zshrc
   ```

### Linux (Ubuntu/Debian)

```bash
# Actualizar repositorios
sudo apt update

# Instalar Qt5 y dependencias
sudo apt install -y \
    qt5-default \
    qtbase5-dev \
    libqt5svg5-dev \
    qttools5-dev-tools

# Instalar CMake y compilador
sudo apt install -y cmake g++ build-essential
```

### Linux (Fedora/RHEL)

```bash
# Instalar Qt5
sudo dnf install -y \
    qt5-qtbase-devel \
    qt5-qtsvg-devel \
    cmake \
    gcc-c++
```

### Linux (Arch/Manjaro)

```bash
# Instalar Qt5
sudo pacman -S qt5-base qt5-svg cmake gcc
```

## Compilación

### Usando scripts (recomendado)

#### Windows
```cmd
cd qt
build.bat
```

#### macOS / Linux
```bash
cd qt
chmod +x build.sh
./build.sh
```

### Compilación manual

```bash
cd qt
mkdir build && cd build
cmake ..
cmake --build .
```

### Compilación con MSVC (Windows)

```cmd
cd qt
mkdir build && cd build
cmake -G "Visual Studio 16 2019" -A x64 ..
cmake --build . --config Release
```

## Ejecución

### Usando scripts

#### Windows
```cmd
cd qt
run.bat
```

#### macOS / Linux
```bash
cd qt
chmod +x run.sh
./run.sh
```

### Ejecución directa

#### Windows
```cmd
.\build\bin\Release\svg-animator.exe
```

#### macOS
```bash
./build/bin/svg-animator.app/Contents/MacOS/svg-animator
```

#### Linux
```bash
./build/bin/svg-animator
```

## Solución de problemas

### Error: Qt5 not found

**Windows:**
```cmd
set CMAKE_PREFIX_PATH=C:\Qt\5.15.2\mingw81_64
cmake ..
```

**macOS:**
```bash
export CMAKE_PREFIX_PATH="/opt/homebrew/opt/qt@5"
cmake ..
```

**Linux:**
```bash
export CMAKE_PREFIX_PATH="/usr/lib/x86_64-linux-gnu/cmake/Qt5"
cmake ..
```

### Error: CMake version

Se requiere CMake 3.16 o superior:
```bash
cmake --version
```

### Error: Compiler not found

**Windows:** Asegúrese de que MinGW o MSVC está en el PATH
**macOS:** `xcode-select --install`
**Linux:** `sudo apt install build-essential`

### Error: Qt5Svg not found

**Linux:**
```bash
sudo apt install libqt5svg5-dev
```

### macOS: "App is damaged"

```bash
xattr -cr ./build/bin/svg-animator.app
```

## Instalación del ícono (Linux)

Para agregar un ícono al escritorio:

```bash
# Crear archivo .desktop
cat > ~/.local/share/applications/svg-animator.desktop << EOF
[Desktop Entry]
Name=SVG Animator
Exec=/ruta/a/svg-animator
Icon=/ruta/a/icono.png
Type=Application
Categories=Graphics;2DGraphics;
EOF
```

## Desinstalación

Simplemente eliminar la carpeta `build`:
```bash
cd qt
rm -rf build
```
