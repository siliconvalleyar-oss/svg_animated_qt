# SVG Animator Qt - Documentación

Aplicación de escritorio para animar archivos SVG con una amplia gama de efectos predefinidos y controles avanzados. Desarrollada con Qt6/C++.

## Plataformas soportadas

- Windows (10/11)
- macOS
- Linux (Ubuntu, Debian, etc.)

## Características

- **13 animaciones preset**: Rotar, Rueda, Pulsar, Rebotar, Gravedad, Deslizar, Óvalo, Desvanecer, Dibujar, Temblar, Flotar, Girar, Brillar
- **Controles avanzados**: Velocidad, retraso, repetición, dirección
- **Modo piezas**: Seleccionar y mover elementos individuales del SVG
- **Generador de formas**: 12 formas predefinidas para generar SVGs
- **Importar SVG**: Selector de archivos o drag & drop
- **Exportar**: SVGs con animaciones CSS embebidas
- **Slideshow**: Crear presentaciones con transiciones
- **Undo/Redo**: Hasta 50 pasos
- **Gestión de workspaces**: Múltiples espacios de trabajo

## Arquitectura

```
qt/
├── CMakeLists.txt        # Configuración CMake
└── src/                  # Código fuente C++
    ├── main.cpp          # Punto de entrada
    ├── MainWindow.cpp/h  # Ventana principal
    ├── SvgView.cpp/h     # Widget de visualización SVG
    ├── AnimationEngine.cpp/h  # Motor de animaciones
    ├── ElementPanel.cpp/h     # Panel de elementos
    ├── ControlsWidget.cpp/h   # Controles de animación
    ├── ExportManager.cpp/h    # Gestor de exportación
    ├── WorkspaceManager.cpp/h # Gestor de workspaces
    ├── TrajectoryManager.cpp/h # Gestor de trayectorias
    ├── SlideshowManager.cpp/h  # Gestor de slideshow
    ├── Shapes.cpp/h           # Formas predefinidas
    ├── Theme.cpp/h            # Temas de color
    └── BackgroundImageManager.cpp/h # Gestor de fondo
```

## Compilación rápida

```bash
cd qt
mkdir build && cd build
cmake ..
cmake --build .
# Windows: .\Release\svg-animator.exe
# Linux/Mac: ./svg-animator
```

## Documentación

- [Instalación](INSTALLATION.md)
- [Animaciones](ANIMATIONS.md)
- [Controles](CONTROLS.md)
- [Modo Piezas](PIECES.md)
- [Exportar](EXPORT.md)
- [Desarrollo](DEVELOPMENT.md)
- [TODO](TODO.md)
