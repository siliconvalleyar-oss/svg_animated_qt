# Animaciones

## Lista de animaciones

| ID | Nombre | Descripcion | Easing por defecto |
|----|--------|-------------|-------------------|
| `rotate` | Rotar | Rotacion continua 360 | linear |
| `wheel` | Rueda | Rotacion continua con pasos de 90 | linear |
| `pulse` | Pulsar | Escala arriba/abajo | ease-in-out |
| `bounce` | Rebotar | Rebote vertical | ease-in-out |
| `gravity` | Gravedad | Caida con rebote realista | ease-out |
| `slide` | Deslizar | Movimiento horizontal | ease-in-out |
| `oval` | Ovalo | Trayectoria eliptica configurable | linear |
| `fade` | Desvanecer | Oscurecimiento opacidad | ease-in-out |
| `draw` | Dibujar | Dibujado de trazos (stroke) | ease-in-out |
| `shake` | Temblar | Vibracion horizontal | ease-in-out |
| `float` | Flotar | Flotacion suave vertical | ease-in-out |
| `spin` | Girar | Giro con escala | ease-in-out |
| `glow` | Brillar | Brillo con drop-shadow | ease-in-out |

## Sentido / Angulo de movimiento

Las animaciones basadas en translacion (slide, bounce, shake, float, gravity) soportan control de direccion mediante angulo (0-360°) con previsualizacion de trayectoria.

## Efectos con controles especiales

### Ovalo
- **Ancho (X)**: Radio horizontal de la elipse (10-150px)
- **Alto (Y)**: Radio vertical de la elipse (10-150px)
- **Angulo**: Rotacion de la trayectoria eliptica (0-360grados)

### Gravedad
- Simula caida libre con rebote amortiguado
- Usa easing cubic-bezier para realismo

### Dibujar
- Animacion de stroke-dashoffset
- Funciona mejor con SVGs que tengan trazos (stroke)

## Easing functions disponibles

| Valor CSS | Descripcion |
|-----------|-------------|
| `linear` | Velocidad constante |
| `ease` | Lento inicio y fin (default CSS) |
| `ease-in` | Lento inicio |
| `ease-out` | Lento fin |
| `ease-in-out` | Lento inicio y fin |
| `cubic-bezier(x1,y1,x2,y2)` | Curva personalizada |
