# Modo Piezas

El modo piezas permite interactuar con elementos individuales dentro de un SVG.

## Activar

1. Cargar un SVG (importar o generar)
2. Hacer clic en "Mover piezas por separado" en el panel de elementos
3. El modo se activa y las animaciones se pausan

## Funcionamiento

### Seleccionar elemento
- Hacer clic en cualquier elemento del SVG en la vista previa
- El elemento seleccionado se resalta con un borde punteado

### Mover elemento
- Arrastrar el elemento seleccionado con el mouse
- El movimiento se escala automáticamente al espacio de coordenadas del SVG

### Deseleccionar
- Presionar `Escape`
- Hacer clic en otro elemento (selecciona el nuevo)

## Comportamiento

- Las animaciones se pausan automáticamente al entrar al modo piezas
- Las animaciones se reanudan al salir del modo
- Los movimientos se aplican via `transform: translate()` en el elemento
- Los movimientos son relativos a la posición original del elemento

## Panel de elementos

El panel lateral muestra todos los elementos del SVG:
- Lista de elementos con nombre y tipo
- Selección múltiple con `Ctrl+click`
- Aplicar animación a elemento seleccionado
- Aplicar animación a todos los elementos

## Limitaciones

- Los movimientos no se guardan al exportar (solo afectan el preview)
- Para exportar con posiciones personalizadas, editar el SVG manualmente
