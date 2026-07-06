# Desarrollo

## Estructura del código

### Módulos principales

- **MainWindow**: Ventana principal y orquestación de componentes
- **SvgView**: Widget Qt para visualización y manipulación de SVGs
- **AnimationEngine**: Motor de animaciones CSS con soporte para 13 presets
- **ElementPanel**: Panel lateral para selección de elementos del SVG
- **ControlsWidget**: Controles de animación (velocidad, dirección, etc.)
- **ExportManager**: Generación de SVGs animados autocontenidos
- **WorkspaceManager**: Gestión de múltiples espacios de trabajo
- **TrajectoryManager**: Visualización de trayectorias de movimiento
- **SlideshowManager**: Creación y reproducción de presentaciones
- **Shapes**: Biblioteca de 12 formas predefinidas
- **Theme**: Sistema de temas (claro/oscuro)
- **BackgroundImageManager**: Gestión de imagen de fondo

### Patrones de diseño

- **Signals/Slots**: Comunicación entre componentes Qt
- **MVC implícito**: Separación de lógica y presentación
- **Undo/Redo**: Pila de estados para operaciones reversibles

## Agregar nueva animación

1. Agregar preset en `AnimationEngine.cpp`:

```cpp
Preset{"Mi Animación", "mi-anim", QColor("#ff0000"), 1.0, "ease-in-out"}
```

2. Agregar generación de keyframes en `AnimationEngine::generateKeyframes()`:

```cpp
if (id == "mi-anim") {
    keyframes = "@keyframes svgMiAnim { 0% { transform: ... } 100% { transform: ... } }";
}
```

3. Actualizar `ExportManager` si la animación requiere lógica especial

## Agregar nueva forma

Agregar en `Shapes.cpp`:

```cpp
{"Mi Forma", "<svg viewBox=\"0 0 200 200\">...</svg>"}
```

## Sistema de temas

Los temas se definen en `Theme.cpp` con soporte para:
- Colores de fondo
- Colores de texto
- Colores de acento
- Bordes y sombras

## Compilación en desarrollo

```bash
cd qt/build
cmake --build .
./svg-animator
```

## Testing

```bash
cd qt/build
ctest --output-on-failure
```

## Dependencias

- Qt 5.x (Core, Gui, Widgets, Svg, Xml)
- CMake 3.16+
- Estándar C++17
