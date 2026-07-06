---
name: svg-animator
description: Use when working on the SVG Animator project (svg_animated, svg_effects). A vanilla JS web app for animating SVGs with presets, pieces mode, slideshow, and export. Use for questions about animations, presets, pieces mode, slides, shapes, export, controls, CSS keyframes, or the codebase structure.
---

# SVG Animator — Skill Reference

App web 100% client-side para animar SVGs. Sin dependencias, sin build tools. Vanilla HTML/CSS/JS.

## Stack

| Capa | Tecnología |
|------|-----------|
| Frontend | Vanilla JS (ES6), HTML5, CSS3 |
| Layout | Flexbox, CSS Grid, CSS Custom Properties |
| Animaciones | CSS `@keyframes`, clases de animación |
| Servidor | Python 3 (`python3 -m http.server`) |
| Dependencias | **Ninguna** |

## Archivos principales

| Archivo | Propósito |
|---------|-----------|
| `index.html` | Estructura HTML semántica (sidebar izq, preview central, panel der) |
| `styles.css` | Tema oscuro, layout, keyframes CSS, transiciones slides |
| `app.js` | Toda la lógica: presets, shapes, motor de animación, modo piezas, export, slides |
| `serve.sh` | Script para arrancar servidor HTTP local |
| `VERSION` | `1.0.0` |
| `docs/` | Documentación completa en español |
| `files/` | SVGs de ejemplo servidos por el servidor |

## Estructura del HTML (`index.html`)

- **Sidebar izquierdo** (320px): Import (drag & drop + file input), Archivos del servidor, Generar SVG (12 shapes), Animaciones (presets + play/pause/stop), Controles (speed, delay, repeat, direction), Controles de óvalo, Modo Piezas, Exportar, Slides
- **Preview central**: Área de previsualización con estado vacío
- **Panel derecho** (220px): Miniaturas de elementos SVG del documento cargado

## Animaciones disponibles (13 implementadas en código)

| ID | Nombre | Easing default | Descripción |
|----|--------|---------------|-------------|
| `rotate` | Rotar | linear | Rotación 360° continua |
| `wheel` | Rueda | linear | Rotación con pasos de 90° |
| `pulse` | Pulsar | ease-in-out | Escala 1 → 1.15 |
| `bounce` | Rebotar | ease-in-out | TranslateY -20px |
| `gravity` | Gravedad | ease-out (cubic-bezier) | Caída con rebote amortiguado |
| `slide` | Deslizar | ease-in-out | TranslateX ±80px |
| `oval` | Óvalo | linear | Trayectoria elíptica configurable (rx, ry, angle) |
| `fade` | Desvanecer | ease-in-out | Opacidad 1 → 0.15 |
| `draw` | Dibujar | ease-in-out | stroke-dashoffset (requiere stroke) |
| `shake` | Temblar | ease-in-out | Vibración horizontal ±8px |
| `float` | Flotar | ease-in-out | TranslateY -15px |
| `spin` | Girar | ease-in-out | Rotación + escala 0.85 |
| `glow` | Brillar | ease-in-out | drop-shadow |

**Documentadas pero NO implementadas en código**: `spiral` (espiral), `elastic` (elástico), `wave` (wave).

## Formas predefinidas (12)

Círculo, Cuadrado, Triángulo, Estrella, Corazón, Hexágono, Rombo, Cruz, Onda, Flecha, Rayo, Luna.

## Controles

| Control | Rango | Default |
|---------|-------|---------|
| Velocidad | 0.2s – 5s | 1.0s |
| Retraso | 0s – 3s | 0s |
| Repetir | infinite / 1 / 3 | infinite |
| Dirección | normal / reverse / alternate | normal |

### Óvalo (solo visible con animación `oval`)

| Control | Rango | Default |
|---------|-------|---------|
| Ancho X | 10–150px | 80px |
| Alto Y | 10–150px | 40px |
| Ángulo | 0–360° | 0° |

### Atajos de teclado

| Tecla | Acción |
|-------|--------|
| `ESC` | Deseleccionar elemento en modo piezas |
| `Space` | Play/Pause animación |

## Modo Piezas

1. Cargar SVG → click "Mover piezas por separado"
2. Click en elemento del SVG para seleccionar (borde punteado violeta)
3. Arrastrar para mover (usa `transform: translate()`)
4. ESC para deseleccionar
- Las animaciones se pausan al activar el modo
- Los movimientos NO se guardan al exportar

## Slides

- Agregar SVG actual como slide
- 6 transiciones: fade, slide-h, slide-v, zoom, flip, blur
- Duración de slide: 1–10s
- Velocidad de transición: 0.2–2s
- Drag & drop para reordenar
- Reproducción automática con Play

## Export

- Genera SVG autocontenido con `<style>` + `@keyframes` embebidos + `animation` CSS
- No exporta posiciones del modo piezas ni config de transform-origin
- Compatible con navegadores, editores SVG, `<img>`, `<object>`

## Características implementadas recientemente

### Animaciones independientes por pieza
- Cada elemento del SVG tiene su propio preset de animación independiente.
- Al hacer clic en una miniatura del panel derecho, se cargan sus controles en la barra izquierda.
- El `elementAnimations` map (por índice) almacena la configuración por pieza.
- `selectedElementIndex` indica qué elemento está siendo editado.
- `selectPreset(id)` asigna la animación solo al elemento seleccionado.
- `applyOneAnimation(index)` aplica la animación a un solo elemento vía `style.animation`.
- `applyAllAnimations()` itera todos los elementos con animación asignada.

### Sentido / Ángulo de movimiento
- Control deslizante de ángulo (0–360°) para animaciones basadas en translación.
- Botones de dirección rápida: → ↗ ↑ ↖ ← ↙ ↓ ↘.
- Genera keyframes dinámicos con la dirección aplicada (inyectados en `<style id="dir-keyframes">`).
- Flecha de trayectoria superpuesta en el preview que muestra la dirección actual.
- Animaciones afectadas: slide, bounce, shake, float, gravity.

### Preview expandido
- `overflow: visible` en `.preview` y `#preview-area` para que piezas movidas no se corten.

### Playback multi-pieza
- `setAllAnimationsPlayState(state)` controla todas las animaciones simultáneamente.
- Play/Pause/Stop ahora operan sobre todos los elementos animados.
- Atajo de teclado `Space` para Play/Pause (solo fuera de inputs).

### Export multi-pieza
- Exporta cada elemento con su animación y keyframes individuales.
- Genera reglas `tag:nth-child(n)` para cada pieza.

## Cómo agregar una animación nueva

1. Agregar preset en `app.js`:
   ```js
   { name: 'Nombre', id: 'nombre-id', color: '#hex', duration: 1, easing: 'ease-in-out' }
   ```
2. Agregar clase CSS en `styles.css`:
   ```css
   .anim-nombre-id { animation: svgNombreId var(--dur) var(--easing) var(--iter) var(--dir); }
   ```
3. Agregar `@keyframes svgNombreId { ... }` en `styles.css`
4. Agregar keyframe en export (sección de keyframes individuales en `app.js`) y en `ensureDirectionKeyframes` si aplica.
5. Si la animación usa translación y debe soportar dirección, agregarla en `isTranslateBased` en `applyOneAnimation`.

## Cómo agregar una forma nueva

Agregar en el array `shapes` en `app.js`:
```js
{ name: 'Nombre', svg: '<svg viewBox="0 0 200 200">...</svg>' }
```

## Servidor de desarrollo

```bash
./serve.sh 8080
# o puerto personalizado: ./serve.sh 3000
```

Los cambios se reflejan recargando la página (sin hot reload).

## Referencia de documentación

- `docs/README.md` — Resumen general, características, arquitectura
- `docs/INSTALLATION.md` — Instalación y requisitos
- `docs/ANIMATIONS.md` — Lista completa de animaciones con easings
- `docs/CONTROLS.md` — Todos los controles disponibles
- `docs/PIECES.md` — Modo piezas detallado
- `docs/EXPORT.md` — Formato de exportación
- `docs/DEVELOPMENT.md` — Guía para desarrolladores
