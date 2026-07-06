# Modo Piezas

El modo piezas permite interactuar con elementos individuales dentro de un SVG.

## Activar

1. Cargar un SVG (importar, generar, o seleccionar del servidor)
2. Hacer clic en "Mover piezas por separado"
3. El modo se activa y las animaciones se pausan

## Funcionamiento

### Seleccionar elemento
- Hacer clic en cualquier elemento del SVG
- El elemento seleccionado se resalta con un borde punteado violeta

### Mover elemento
- Arrastrar el elemento seleccionado con el mouse
- El movimiento se escala automaticamente al espacio de coordenadas del SVG

### Deseleccionar
- Presionar `ESC`
- Hacer clic en otro elemento (selecciona el nuevo)

## Comportamiento

- Las animaciones CSS se pausan automaticamente al entrar al modo piezas
- Las animaciones se reanudan al salir del modo
- Los movimientos se aplican via `transform: translate()` en el elemento
- Los movimientos son relativos a la posicion original del elemento

## Limitaciones

- Los movimientos no se guardan al exportar (solo afectan el preview)
- Para exportar con posiciones personalizadas, editar el SVG manualmente
