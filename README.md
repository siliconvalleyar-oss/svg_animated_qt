# SVG Animator Qt

Aplicación de escritorio para animar archivos SVG con efectos predefinidos y controles avanzados. Desarrollada con Qt5/C++.

## Plataformas soportadas

- **Windows** (10/11)
- **macOS** (10.14+)
- **Linux** (Ubuntu 18.04+, Debian 10+, Fedora 30+)

## Características

- **19 animaciones preset**: Rotar, Rueda, Pulsar, Rebotar, Gravedad, Deslizar, Óvalo, Desvanecer, Dibujar, Temblar, Flotar, Levitar, Arco, Radiar, Girar, Brillar, Senoidal, Cuadrada, Triangular
- **Animaciones independientes por pieza**: Cada elemento del SVG puede tener su propio efecto
- **Sentido / Ángulo**: Control de dirección con previsualización de trayectoria
- **Controles avanzados**: Velocidad, retraso, repetición, dirección
- **Modo piezas**: Seleccionar y mover elementos individuales del SVG
- **Generador de formas**: 12 formas predefinidas para generar SVGs
- **Importar SVG**: Selector de archivos o drag & drop
- **Exportar**: SVGs con animaciones CSS embebidas
- **Slideshow**: Crear presentaciones con transiciones
- **Undo/Redo**: Hasta 50 pasos
- **Gestión de workspaces**: Múltiples espacios de trabajo

## Requisitos

- Qt 5.x (Core, Gui, Widgets, Svg, Xml)
- CMake 3.16+
- Compilador C++17

## Compilación rápida

### Windows

```cmd
cd qt
build.bat
```

### macOS / Linux

```bash
cd qt
chmod +x build.sh
./build.sh
```

### Manual (cualquier plataforma)

```bash
cd qt
mkdir build && cd build
cmake ..
cmake --build .
```

## Ejecución

### Windows

```cmd
cd qt
run.bat
```

### macOS / Linux

```bash
cd qt
chmod +x run.sh
./run.sh
```

### Directo desde build

```bash
# Windows
.\build\bin\Release\svg-animator.exe

# macOS
./build/bin/svg-animator.app/Contents/MacOS/svg-animator

# Linux
./build/bin/svg-animator
```

## Estructura del proyecto

```
svg_animated_qt/
├── qt/                        # Proyecto Qt C++
│   ├── CMakeLists.txt         # Configuración CMake
│   ├── build.bat              # Script de compilación (Windows)
│   ├── build.sh               # Script de compilación (Mac/Linux)
│   ├── run.bat                # Script de ejecución (Windows)
│   ├── run.sh                 # Script de ejecución (Mac/Linux)
│   └── src/                   # Código fuente C++
│       ├── main.cpp
│       ├── MainWindow.cpp/h
│       ├── SvgView.cpp/h
│       ├── AnimationEngine.cpp/h
│       ├── ElementPanel.cpp/h
│       ├── ControlsWidget.cpp/h
│       ├── ExportManager.cpp/h
│       ├── WorkspaceManager.cpp/h
│       ├── TrajectoryManager.cpp/h
│       ├── SlideshowManager.cpp/h
│       ├── Shapes.cpp/h
│       ├── Theme.cpp/h
│       └── BackgroundImageManager.cpp/h
├── assets/                    # Recursos SVG de ejemplo
├── docs/                      # Documentación
└── README.md
```

## Documentación

- [Instalación](docs/INSTALLATION.md)
- [Animaciones](docs/ANIMATIONS.md)
- [Controles](docs/CONTROLS.md)
- [Modo Piezas](docs/PIECES.md)
- [Exportar](docs/EXPORT.md)
- [Desarrollo](docs/DEVELOPMENT.md)
- [TODO](docs/TODO.md)

## Licencia

Proyecto de código abierto.
