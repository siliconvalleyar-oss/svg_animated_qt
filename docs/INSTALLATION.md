# Instalacion

## Requisitos

- Navegador web moderno (Chrome, Firefox, Safari, Edge)
- Python 3 (para el servidor local)

## Plataformas soportadas

- Windows (7/8/10/11)
- macOS
- Linux (Ubuntu, Debian, etc.)
- Raspberry Pi (Raspberry OS)

## Uso sin servidor

Abrir directamente `index.html` en el navegador. Limitaciones:
- No funciona el navegador de archivos del servidor
- Solo importar SVGs por drag & drop o selector de archivos

## Uso con servidor

### Mac / Linux / Raspberry Pi

```bash
cd svg_effects
python3 serve.py
```

O con el script bash (solo Mac/Linux):

```bash
./serve.sh 8080
```

### Windows

```cmd
cd svg_effects
python serve.py
```

O simplemente hacer doble clic en `serve.bat`.

### Puerto personalizado

```bash
# Mac / Linux / Raspberry Pi
python3 serve.py 3000

# Windows
python serve.py 3000
```

### Detener servidor

Presiona `Ctrl+C` en la terminal.

## Estructura de archivos

Coloca archivos SVG en el directorio `files/` para que aparezcan en el navegador de archivos del servidor.

```bash
cp mi-archivo.svg files/
```
