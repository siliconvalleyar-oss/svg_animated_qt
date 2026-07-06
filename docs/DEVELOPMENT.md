# Desarrollo

## Estructura del codigo

### styles.css
- Variables CSS para theming
- Layout (sidebar, preview, header)
- Componentes UI (botones, sliders, toggles)
- Animaciones CSS (keyframes y clases)

### app.js
- **Presets**: Array de animaciones disponibles
- **Shapes**: SVGs de formas predefinidas
- **State**: Estado global (currentSvg, elementAnimations, slides)
- **File browser**: Carga de SVGs del servidor
- **Animation engine**: Aplicacion y control de animaciones
- **Direction system**: Angulo y sentido para animaciones de translacion
- **Group system**: Agrupar elementos para aplicar misma animacion
- **Pieces mode**: Seleccion y movimiento de elementos
- **Export**: Generacion de SVG animado

### index.html
- Estructura semantica HTML5
- Referencia a CSS y JS externos
- Secciones del sidebar

## Agregar nueva animacion

1. Agregar preset en `app.js`:
```js
{ name: 'Mi Animacion', id: 'mi-anim', color: '#ff0000', duration: 1, easing: 'ease-in-out' }
```

2. Agregar clase CSS en `styles.css`:
```css
.anim-mi-anim { animation: svgMiAnim var(--dur) var(--easing) var(--iter) var(--dir); }
```

3. Agregar keyframes en `styles.css`:
```css
@keyframes svgMiAnim {
  0% { transform: ... }
  100% { transform: ... }
}
```

4. Si la animacion usa translacion y soporta direccion, agregarla en `isTranslateBased` en `applyOneAnimation` y en `ensureDirectionKeyframes`.

5. Agregar keyframe en export (seccion de keyframes individuales en `app.js`).

## Agregar nueva forma

Agregar en el array `shapes` en `app.js`:
```js
{ name: 'Mi Forma', svg: '<svg viewBox="0 0 200 200">...</svg>' }
```

## Servidor de desarrollo

```bash
./serve.sh 8080
```

Los cambios en CSS y JS se refrescan al recargar la pagina (no hay hot reload).

## Dependencias

Ninguna. Solo vanilla HTML/CSS/JS.
