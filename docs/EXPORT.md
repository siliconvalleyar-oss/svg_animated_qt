# Exportar

## SVG Animado

El botón "Exportar SVG Animado" genera un archivo `.svg` autocontenido con las animaciones CSS embebidas para cada elemento.

## Qué se exporta

- El SVG original con todos sus elementos
- Un elemento `<style>` con los keyframes CSS de cada animación
- Cada elemento animado lleva un atributo `data-anim-index` para selectores CSS precisos
- Animaciones independientes por pieza, cada una con su configuración individual

## Estructura del SVG exportado

```xml
<?xml version="1.0" encoding="UTF-8"?>
<svg viewBox="0 0 200 200">
  <style>
    @keyframes svgRotate {
      from { transform: rotate(0deg); }
      to { transform: rotate(360deg); }
    }
    [data-anim-index="0"] {
      transform-origin: center center;
      transform-box: fill-box;
      animation: svgRotate 2s linear infinite normal;
    }
  </style>
  <circle data-anim-index="0" cx="100" cy="100" r="70"/>
</svg>
```

## Compatible con

- Navegadores web modernos
- Editores SVG (Inkscape, Illustrator)
- Embebido en HTML con `<img>` o `<object>`
- CSS inline en páginas web

## No exporta

- Posiciones del modo piezas (solo efecto en preview)
- Efectos que requieren JavaScript
- Fondos o imágenes de fondo

## Formatos de exportación

- **SVG animado**: Formato principal con CSS embebido
- **SVG estático**: Sin animaciones (frame actual)
- **Todos los elementos**: Incluye elementos no animados

## Opciones de exportación

- Incluir/ocultar elementos no animados
- Preservar metadatos del SVG original
- Optimizar tamaño del archivo
