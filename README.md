# SVG Effects

Aplicación web para animar archivos SVG con efectos predefinidos y controles avanzados.

## Plataformas soportadas

- Windows (7/8/10/11)
- macOS
- Linux (Ubuntu, Debian, etc.)
- Raspberry Pi (Raspberry OS)

## Características

- **13 animaciones preset**: Rotar, Rueda, Pulsar, Rebotar, Gravedad, Deslizar, Óvalo, Desvanecer, Dibujar, Temblar, Flotar, Girar, Brillar
- **Animaciones independientes por pieza**: Cada elemento del SVG puede tener su propio efecto
- **Sentido / Ángulo**: Control de dirección con previsualización de trayectoria para animaciones de translación
- **Controles avanzados**: Velocidad, retraso, repetición, dirección
- **Modo piezas**: Seleccionar y mover elementos individuales del SVG
- **Generador de formas**: 12 formas predefinidas para generar SVGs
- **Importar SVG**: Drag & drop o selección de archivos
- **Exportar**: Descarga SVGs con animaciones CSS embebidas
- **Slideshow**: Crear presentaciones con transiciones
- **Servidor local**: Multiplataforma con menú interactivo

## Requisitos

- Navegador web moderno (Chrome, Firefox, Safari, Edge)
- Python 3 (para el servidor local)

## Instalación rápida

### Mac / Linux / Raspberry Pi

```bash
git clone https://github.com/siliconvalleyar-oss/svg_effects.git
cd svg_effects
python3 serve.py
```

### Windows

```cmd
git clone https://github.com/siliconvalleyar-oss/svg_effects.git
cd svg_effects
python serve.py
```

Abrir `http://localhost:8080` en el navegador.

### Uso directo (sin servidor)

Abrir `index.html` directamente en el navegador. Limitaciones:
- No funciona el navegador de archivos del servidor
- Solo importar SVGs por drag & drop

## Estructura del proyecto

```
svg_effects/
├── index.html          # Estructura HTML
├── styles.css          # Estilos y keyframes CSS
├── app.js              # Lógica JavaScript
├── serve.py            # Servidor multiplataforma (recomendado)
├── serve.sh            # Script de servidor (Mac/Linux)
├── serve.bat           # Launcher para Windows
├── VERSION             # Versión actual
├── files/              # Directorio de SVGs del servidor
│   └── sample.svg      # SVG de ejemplo
├── docs/               # Documentación completa
│   ├── README.md       # Resumen general
│   ├── INSTALLATION.md # Guía de instalación
│   ├── ANIMATIONS.md   # Lista de animaciones
│   ├── CONTROLS.md     # Controles disponibles
│   ├── PIECES.md       # Modo piezas
│   ├── EXPORT.md       # Formato de exportación
│   └── DEVELOPMENT.md  # Guía para desarrolladores
└── config/             # Configuración del servidor
    └── port.cfg        # Puerto guardado
```

## Animaciones disponibles

| ID | Nombre | Descripción |
|----|--------|-------------|
| `rotate` | Rotar | Rotación 360° continua |
| `wheel` | Rueda | Rotación con pasos de 90° |
| `pulse` | Pulsar | Escala 1 → 1.15 |
| `bounce` | Rebotar | TranslateY -20px |
| `gravity` | Gravedad | Caída con rebote amortiguado |
| `slide` | Deslizar | TranslateX ±80px |
| `oval` | Óvalo | Trayectoria elíptica configurable |
| `fade` | Desvanecer | Opacidad 1 → 0.15 |
| `draw` | Dibujar | Animación de trazo (requiere stroke) |
| `shake` | Temblar | Vibración horizontal ±8px |
| `float` | Flotar | TranslateY -15px |
| `spin` | Girar | Rotación + escala 0.85 |
| `glow` | Brillar | Efecto de brillo (drop-shadow) |

## Controles

| Control | Rango | Default |
|---------|-------|---------|
| Velocidad | 0.2s – 5s | 1.0s |
| Retraso | 0s – 3s | 0s |
| Repetir | infinite / 1 / 3 | infinite |
| Dirección | normal / reverse / alternate | normal |

## Atajos de teclado

| Tecla | Acción |
|-------|--------|
| `ESC` | Deseleccionar elemento en modo piezas |
| `Space` | Play/Pause animación |

## Uso del servidor

```bash
# Puerto por defecto (8080)
python3 serve.py

# Puerto personalizado
python3 serve.py 3000

# Mac/Linux también pueden usar
./serve.sh 8080
```

El servidor muestra un menú interactivo para:
- Iniciar servidor en el puerto actual
- Cambiar el puerto
- Seleccionar puertos rápidos predefinidos

## Agregar una animación nueva

1. Agregar preset en `app.js`:
```javascript
{ name: 'Nombre', id: 'nombre-id', color: '#hex', duration: 1, easing: 'ease-in-out' }
```

2. Agregar clase CSS en `styles.css`:
```css
.anim-nombre-id { animation: svgNombreId var(--dur) var(--easing) var(--iter) var(--dir); }
```

3. Agregar `@keyframes svgNombreId` en `styles.css`

## Documentación completa

- [Instalación](docs/INSTALLATION.md)
- [Animaciones](docs/ANIMATIONS.md)
- [Controles](docs/CONTROLS.md)
- [Modo Piezas](docs/PIECES.md)
- [Exportar](docs/EXPORT.md)
- [Desarrollo](docs/DEVELOPMENT.md)

## Licencia

Proyecto de código abierto.
