# SVG Animator

Aplicacion web para animar archivos SVG con una amplia gama de efectos predefinidos y controles avanzados.

## Plataformas soportadas

- Windows (7/8/10/11)
- macOS
- Linux (Ubuntu, Debian, etc.)
- Raspberry Pi (Raspberry OS)

## Caracteristicas

- **13 animaciones preset**: Rotar, Rueda, Pulsar, Rebotar, Gravedad, Deslizar, Ovalo, Desvanecer, Dibujar, Temblar, Flotar, Girar, Brillar
- **Controles avanzados**: Velocidad, retraso, repeticion, direccion
- **Modo piezas**: Seleccionar y mover elementos individuales del SVG
- **Generador de formas**: 12 formas predefinidas para generar SVGs
- **Importar SVG**: Drag & drop o seleccion de archivos
- **Exportar**: Descarga SVGs con animaciones CSS embebidas
- **Servidor local**: Script multiplataforma para servir la app en un puerto configurable

## Arquitectura

```
svg_effects/
  index.html       # Estructura HTML
  styles.css       # Estilos y keyframes CSS
  app.js           # Logica JavaScript
  serve.py         # Servidor multiplataforma (Mac/Linux/Windows/Raspberry Pi)
  serve.sh         # Script de servidor (solo Mac/Linux)
  serve.bat        # Launcher para Windows
  sample.svg       # SVG de ejemplo
  files/           # Directorio de SVGs del servidor
  docs/            # Documentacion
```

## Rapido inicio

### Mac / Linux / Raspberry Pi

```bash
python3 serve.py
# o
./serve.sh 8080
# Abrir http://localhost:8080
```

### Windows

```cmd
python serve.py
# o hacer doble clic en serve.bat
# Abrir http://localhost:8080
```

## Documentacion

- [Instalacion](INSTALLATION.md)
- [Animaciones](ANIMATIONS.md)
- [Controles](CONTROLS.md)
- [Modo Piezas](PIECES.md)
- [Exportar](EXPORT.md)
- [Desarrollo](DEVELOPMENT.md)
