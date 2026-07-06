# TODO - SVG Animator Qt

## Prioridad Alta

### Funcionalidad Core
- [ ] Implementar carga de SVGs con soporte completo de elementos anidados
- [ ] Completar sistema de undo/redo con QUndoStack
- [ ] Implementar drag & drop para reordenar elementos en el panel
- [ ] Agregar soporte para múltiples selecciones en modo piezas
- [ ] Implementar zoom con mouse wheel en el viewport

### Animaciones
- [ ] Agregar animación "Wave" (onda sinusoidal)
- [ ] Agregar animación "Blink" (parpadeo)
- [ ] Agregar animación "ColorShift" (cambio de color)
- [ ] Implementar animaciones de entrada/salida (appear/disappear)
- [ ] Soporte para animaciones compuestas (múltiples efectos simultáneos)

### Exportación
- [ ] Exportar a GIF animado
- [ ] Exportar a WebM/MP4
- [ ] Exportar frame por frame como PNG序列
- [ ] Agregar opciones de calidad/resolución en exportación
- [ ] Incluir metadata en SVG exportado (versión, configuraciones)

## Prioridad Media

### Interfaz de Usuario
- [ ] Implementar tema oscuro/claro con toggle
- [ ] Agregar tooltips informativos en todos los controles
- [ ] Implementar barra de progreso para operaciones pesadas
- [ ] Agregar atajos de teclado personalizables
- [ ] Implementar paneles docking (arrastrar y soltar)
- [ ] Agregar vista previa en miniatura de animaciones

### Gestión de Espacios de Trabajo
- [ ] Guardar/cargar configuraciones de workspace
- [ ] Exportar/importar workspace completo
- [ ] Historial de workspaces recientes
- [ ] Plantillas de workspace predefinidas

### Slideshow
- [ ] Editor de transiciones entre slides
- [ ] Duración personalizable por slide
- [ ] Modo presentación a pantalla completa
- [ ] Exportar slideshow como video
- [ ] Agregar notas/footnotes por slide

### Formas
- [ ] Agregar más formas predefinidas (estrella, corazón, flecha, etc.)
- [ ] Editor de formas personalizadas
- [ ] Importar formas desde archivos SVG
- [ ] Biblioteca de formas compartida

## Prioridad Baja

### Rendimiento
- [ ] Implementar rendering con OpenGL para SVGs complejos
- [ ] Lazy loading para SVGs grandes
- [ ] Cache de preview para animaciones
- [ ] Optimización de memoria para workspaces grandes

### Documentación
- [ ] Crear video tutoriales
- [ ] Documentar API interna para plugins
- [ ] Crear ejemplos de uso avanzado
- [ ] Traducir documentación a múltiples idiomas

### Testing
- [ ] Tests unitarios para AnimationEngine
- [ ] Tests unitarios para ExportManager
- [ ] Tests de integración para flujo completo
- [ ] Tests de rendimiento con SVGs grandes

### Compatibilidad
- [ ] Soporte para SVG 2.0
- [ ] Compatibilidad con SVGs de Inkscape/Illustrator
- [ ] Soporte para SVGs con scripts embebidos (sandbox)
- [ ] Exportar con soporte SMIL (fallback)

## Ideas Futuras

### Funcionalidades Avanzadas
- [ ] Editor de keyframes visual
- [ ] Sistema de plugins/extensiones
- [ ] Colaboración en tiempo real (multi-usuario)
- [ ] Versionado de proyectos con Git integration
- [ ] IA para sugerir animaciones basadas en contenido SVG

### Integraciones
- [ ] Plugin para Figma
- [ ] Plugin para Adobe XD
- [ ] Plugin para Blender
- [ ] API REST para automatización
- [ ] CLI para exportación batch

### Plataformas
- [ ] Versión web (WASM)
- [ ] Versión móvil (iOS/Android)
- [ ] Extensión para navegador
- [ ] Aplicación de escritorio con auto-update

## Completado

- [x] 13 animaciones preset iniciales
- [x] Modo piezas con selección individual
- [x] Controles de velocidad, retraso, repetición
- [x] Sistema de dirección/ángulo
- [x] Exportación SVG animado
- [x] Slideshow básico
- [x] Gestión de workspaces
- [x] Soporte multiplataforma (Windows, Mac, Linux)
- [x] Servidor local interactivo
- [x] Documentación completa
