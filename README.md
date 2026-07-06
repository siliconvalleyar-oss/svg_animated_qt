# SVG Animator Qt

Aplicación de escritorio para animar archivos SVG con efectos predefinidos y controles avanzados. Desarrollada con Qt6/C++.

## Plataformas soportadas

- Windows (10/11)
- macOS
- Linux (Ubuntu, Debian, etc.)

## Características

- **13 animaciones preset**: Rotar, Rueda, Pulsar, Rebotar, Gravedad, Deslizar, Óvalo, Desvanecer, Dibujar, Temblar, Flotar, Girar, Brillar
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

- Qt 6.x (Core, Gui, Widgets, Svg, Xml)
- CMake 3.20+
- Compilador C++17

## Compilación

```bash
cd qt
mkdir build && cd build
cmake ..
cmake --build .
```

## Ejecución

```bash
./build/svg-animator
```

## Estructura del proyecto

```
svg_animated_qt/
├── qt/                    # Proyecto Qt C++
│   ├── CMakeLists.txt    # Configuración CMake
│   └── src/              # Código fuente C++
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
├── assets/               # Recursos SVG de ejemplo
├── docs/                 # Documentación
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
