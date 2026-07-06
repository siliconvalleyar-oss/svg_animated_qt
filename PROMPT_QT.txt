==============================================================================
PROMPT_QT.txt
SOLICITUD PARA GENERAR UNA APLICACION Qt NATIVA
A PARTIR DE LA APLICACION WEB SVG ANIMATOR
==============================================================================

FECHA: 2026-07-06
PROYECTO: SVG Animator -> SVG Animator Qt
ESTADO: Solicitud de generacion de prompt

==============================================================================
SECCION 1: CONTEXTO Y OBJETIVO
==============================================================================

1.1 SITUACION ACTUAL
--------------------
Se dispone de una aplicacion web funcional llamada "SVG Animator" desarrollada
con tecnologias vanilla (HTML5, CSS3, JavaScript ES6). La aplicacion es 100%
client-side, no requiere servidor, y se ejecuta directamente en el navegador
abriendo el archivo index.html.

La aplicacion permite:
- Importar y visualizar archivos SVG
- Aplicar 19 animaciones preset diferentes a elementos SVG
- Controlar animaciones individuales por pieza
- Modo piezas para mover elementos individualmente
- Sistema de slides/presentaciones con transiciones
- Exportar SVGs con animaciones CSS embebidas
- Sistema de espacios de trabajo (workspaces) multiples
- Historial de acciones (undo/redo)
- Trayectorias personalizadas
- Imagenes de fondo de referencia
- Zoom, rotacion, y controles de vista

1.2 DOCUMENTACION EXISTENTE
----------------------------
El proyecto cuenta con documentacion completa en la carpeta docs/:

- docs/README.md: Resumen general, arquitectura, plataformas soportadas
- docs/INSTALLATION.md: Guia de instalacion y requisitos
- docs/ANIMATIONS.md: Lista completa de 19 animaciones con easings
- docs/CONTROLS.md: Todos los controles disponibles y sus rangos
- docs/PIECES.md: Modo piezas detallado con limitaciones
- docs/EXPORT.md: Formato de exportacion SVG animado
- docs/DEVELOPMENT.md: Guia para desarrolladores
- docs/RULES.md: Reglas de versionado del proyecto

Ademas existe un archivo de skill en:
- .opencode/skills/svg-animator/SKILL.md: Referencia tecnica completa

1.3 OBJETIVO DE LA MIGRACION
-----------------------------
Generar una solicitud detallada (prompt) para que un asistente de IA desarrolle
una aplicacion nativa de escritorio usando el framework Qt que replique todas
las funcionalidades de la aplicacion web SVG Animator, con las ventajas de una
aplicacion nativa (rendimiento, integracion con el sistema, sin dependencia de
navegador).

==============================================================================
SECCION 2: REQUISITOS FUNCIONALES DETALLADOS
==============================================================================

2.1 CARGA Y GESTION DE ARCHIVOS
--------------------------------

[2.1.1 Importar SVG]
- Boton "Importar SVG" en barra de herramientas
- Atajo de teclado: Ctrl+O
- Dialogo nativo del sistema para seleccionar archivo
- Soporte para drag & drop (arrastrar SVG desde explorador)
- Validacion de que el archivo sea SVG valido
- Recordar ultima carpeta abierta (persistencia)
- Soporte para SVGs con namespace personalizado

[2.1.2 Navegador de archivos del servidor]
- Panel lateral que muestra archivos SVG en una carpeta predefinida
- Carga automatica de archivos al iniciar
- Click en archivo para cargarlo en el preview
- Indicador de archivo activo (resaltado)
- Soporte para subcarpetas

[2.1.3 Generador de formas]
- Grid de 12 formas SVG predefinidas que se pueden generar:
  1. Circulo (stroke #6c5ce7)
  2. Cuadrado (stroke #e74c3c)
  3. Triangulo (stroke #2ecc71)
  4. Estrella (stroke #f39c12)
  5. Corazon (stroke #e74c3c)
  6. Hexagono (stroke #1abc9c)
  7. Rombo (stroke #9b59b6)
  8. Cruz (stroke #3498db)
  9. Onda (stroke #e67e22)
  10. Flecha (stroke #6c5ce7)
  11. Rayo (stroke #f1c40f)
  12. Luna (stroke #8e44ad)
- Click en forma para generar SVG con viewBox 200x200
- Cada forma es un SVG de una sola linea con stroke

[2.1.4 Sistema de espacios de trabajo]
- Pestañas en la parte superior del panel izquierdo
- Cada workspace mantiene su propio estado independiente:
  * SVG cargado
  * Animaciones asignadas
  * Grupos creados
  * Trayectorias
  * Imagenes de fondo
  * Nivel de zoom
- Boton "+" para crear nuevo workspace
- Boton "x" en cada pestana para cerrar
- Click en pestana para cambiar de workspace
- Confirmacion al cerrar workspace con contenido
- Nombre editable via doble-click

2.2 ANIMACIONES (19 PRESETS)
-----------------------------

[2.2.1 Lista completa de animaciones]

| ID            | Nombre     | Easing Default    | Descripcion                           |
|---------------|------------|-------------------|---------------------------------------|
| rotate        | Rotar      | linear            | Rotacion continua 360 grados          |
| wheel         | Rueda      | linear            | Rotacion con pasos de 90 grados       |
| pulse         | Pulsar     | ease-in-out       | Escala 1 -> 1.15                      |
| bounce        | Rebotar    | ease-in-out       | TranslateY -20px                      |
| gravity       | Gravedad   | ease-out          | Caida con rebote amortiguado          |
| slide         | Deslizar   | ease-in-out       | TranslateX +/-80px                    |
| oval          | Ovalo      | linear            | Trayectoria eliptica configurable     |
| fade          | Desvanecer | ease-in-out       | Opacidad 1 -> 0.15                    |
| draw          | Dibujar    | ease-in-out       | stroke-dashoffset (requiere stroke)   |
| shake         | Temblar    | ease-in-out       | Vibracion horizontal +/-8px           |
| float         | Flotar     | ease-in-out       | TranslateY -15px                      |
| levitate      | Levitar    | ease-in-out       | TranslateY con escala sutil           |
| arc           | Arco       | ease-in-out       | Movimiento en arco configurable       |
| radiate       | Radiar     | ease-in-out       | Arco con efecto de brillo             |
| spin          | Girar      | ease-in-out       | Rotacion + escala 0.85                |
| glow          | Brillar    | ease-in-out       | Efecto drop-shadow pulsante           |
| wave-sine     | Senoidal   | ease-in-out       | Movimiento sinusoidal                 |
| wave-square   | Cuadrada   | step-end          | Movimiento en escalera                |
| wave-triangle | Triangular | linear            | Movimiento triangular                 |

[2.2.2 Sistema de animaciones por pieza]
- Cada elemento SVG puede tener su propia animacion independiente
- Al hacer click en un elemento del panel derecho, se cargan sus controles
- El mapa elementAnimations almacena configuracion por indice
- selectPreset(id) asigna animacion solo al elemento seleccionado
- applyOneAnimation(index) aplica animacion a un solo elemento
- applyAllAnimations() aplica animaciones a todos los elementos

[2.2.3 Sistema de grupo de animaciones]
- Cada animacion tiene un color asociado para identificacion visual
- Los colores disponibles son: #6c5ce7, #e74c3c, #2ecc71, #f39c12, #1abc9c, #9b59b6, #3498db, #e67e22
- Los botones de preset muestran un dot del color de la animacion
- El panel derecho muestra el dot del color al lado de cada elemento

2.3 CONTROLES DE ANIMACION
---------------------------

[2.3.1 Control de velocidad]
- Slider: rango 0.2s a 16s (configurable via input numerico)
- Valor por defecto: 1.0s
- Display: muestra el valor con 1 decimal + "s"
- Paso: 0.1s
- Sincronizacion con grupos: al cambiar uno, cambian todos los del grupo

[2.3.2 Control de retraso]
- Slider: rango 0s a 3s
- Valor por defecto: 0s
- Display: muestra el valor con 1 decimal + "s"
- Paso: 0.1s

[2.3.3 Control de repeticiones]
- Botones toggle: infinito, 1, 3, random
- Boton "random" selecciona aleatoriamente entre: 1, 2, 3, 5, 10, infinite
- Input numerico oculto para valor personalizado (1-999)
- Se muestra solo cuando el valor no esta en las opciones predefinidas

[2.3.4 Control de direccion]
- Botones toggle: Normal, Reversa, Alterno
- Mapeo CSS: normal, reverse, alternate

[2.3.5 Controles de ovalo (solo para animacion "oval")]
- Ancho X: slider 10-150px, default 80px
- Alto Y: slider 10-150px, default 40px
- Angulo: slider 0-360 grados, default 0 grados
- Se ocultan cuando no hay animacion oval activa

[2.3.6 Controles de arco (para animaciones "arc" y "radiate")]
- Eje X: slider 10-200px, default 80px
- Eje Y: slider 10-200px, default 80px
- Se ocultan cuando no hay animacion de arco activa

[2.3.7 Sentido/Angulo de movimiento]
- Slider: rango 0-360 grados, paso 5 grados
- 8 botones de direccion rapida: flechas cardinales
- Flecha de trayectoria superpuesta en el preview
- Animaciones afectadas: slide, bounce, shake, float, gravity, levitate, arc, radiate
- Generacion dinamica de keyframes con la direccion aplicada

[2.3.8 Sistema de trayectorias]
- Boton "Nueva trayectoria" para crear
- Boton "Editar trayectorias" para entrar/salir del modo edicion
- Modo lazo para agregar puntos haciendo click
- Arrastrable: cada punto se puede mover
- Click en linea para insertar punto entre dos existentes
- Click derecho en punto para eliminar
- Maximo 8 colores para trayectorias: #f39c12, #e74c3c, #2ecc71, #3498db, #9b59b6, #1abc9c, #e67e22, #6c5ce7
- Asignacion de trayectoria a elemento via:
  * Dropdown en controles
  * Menu contextual derecho
- Las trayectorias generan keyframes CSS dinamicos

2.4 MODO PIEZAS
----------------

[2.4.1 Activacion]
- Boton "Mover piezas por separado"
- Al activar:
  * Las animaciones CSS se pausan
  * Los elementos se vuelven seleccionables
  * Aparece cursor pointer al hacer hover
  * Borde punteado violeta en elemento seleccionado

[2.4.2 Interaccion]
- Click en elemento para seleccionarlo
- Arrastrar elemento para moverlo (transform: translate)
- El movimiento se escala al espacio de coordenadas del SVG
- ESC para deseleccionar
- Click en otro elemento selecciona el nuevo

[2.4.3 Marcador de pivote]
- Visible solo en modo piezas con animaciones arc/radiate
- Icono cruz naranja (#f39c12)
- Arrastrable para reposicionar el pivote
- Actualiza las variables --arc-offset-x y --arc-offset-y

[2.4.4 Salir del modo]
- Boton "Salir del modo piezas"
- Las animaciones se reanudan
- Los movimientos NO se guardan (solo efecto en preview)

2.5 SISTEMA DE GRUPOS
----------------------

[2.5.1 Crear grupo]
- Seleccionar multiples elementos (Ctrl+Click o modo seleccion)
- Boton "Grupo +" o menu contextual "Agrupar"
- Dialogo para nombre del grupo (default: "Grupo N")
- Cada grupo tiene color unico (rotativo entre 8 colores)
- El grupo guarda la configuracion de animacion del primer elemento

[2.5.2 Gestion de grupos]
- Panel de grupos en la parte inferior del panel derecho
- Click en grupo para seleccionarlo
- Boton "x" para eliminar grupo
- Los elementos del grupo comparten la misma animacion
- Al cambiar animacion de un elemento del grupo, cambia para todos

[2.5.3 Desagrupar]
- Menu contextual "Desagrupar"
- Elimina el grupo pero mantiene los elementos
- Si el grupo queda con menos de 2 elementos, se elimina automaticamente

2.6 SISTEMA DE SLIDES/PRESENTACIONES
--------------------------------------

[2.6.1 Agregar slide]
- Boton "Agregar SVG actual"
- Guarda el SVG actual como slide
- Nombre automatico: "Slide N"

[2.6.2 Lista de slides]
- Panel en la parte inferior del sidebar izquierdo
- Cada slide muestra: numero, nombre, boton eliminar
- Drag & drop para reordenar slides
- Click en slide para ir a el

[2.6.3 Transiciones (6 tipos)]
- fade: Desvanecer
- slide-h: Deslizar horizontal
- slide-v: Deslizar vertical
- zoom: Zoom in/out
- flip: Rotar en Y
- blur: Desenfocar

[2.6.4 Controles de slide]
- Duracion de slide: slider 1-10s, default 3s
- Velocidad de transicion: slider 0.2-2s, default 0.6s
- Botones: Anterior, Play/Pause, Siguiente
- Reproduccion automatica con intervalo configurable

[2.6.5 Animaciones de transicion]
- CSS classes para cada tipo de transicion
- Estados: enter (entrar) y exit (salir)
- Direccion: left, right, up, down
- Ejemplo: exit-left, enter-right, exit-fade, enter-fade

2.7 EXPORTAR SVG ANIMADO
--------------------------

[2.7.1 Boton de exportacion]
- Boton "Descargar SVG Animado"
- Genera archivo .svg autocontenido

[2.7.2 Contenido del SVG exportado]
- XML declaration con encoding UTF-8
- SVG original con todos sus elementos
- Elemento <style> con:
  * @keyframes de cada animacion usada
  * Reglas CSS para cada elemento animado
  * Variables CSS para configuracion
- Atributo data-anim-index en cada elemento animado
- Selector [data-anim-index="N"] para cada pieza

[2.7.3 Animaciones exportadas]
- Cada elemento puede tener multiples animaciones (comma-separated)
- Keyframes individuales por angulo de direccion
- Keyframes de trayectoria personalizada
- Configuracion de transform-origin por elemento

[2.7.4 No exporta]
- Posiciones del modo piezas
- Imagenes de fondo
- Configuracion de workspace

2.8 SISTEMA DE HISTORIAL (UNDO/REDO)
--------------------------------------

[2.8.1 Almacenamiento]
- Pila de estados (maximo 50)
- Cada estado contiene: elementAnimations y elementGroups
- Index actual en la pila

[2.8.2 Operaciones]
- Undo: Ctrl+Z o boton "Deshacer"
- Redo: Ctrl+Shift+Z o boton "Rehacer"
- Los botones se deshabilitan cuando no hay accion disponible

[2.8.3 Reset]
- Boton "Resetear todo"
- Confirma antes de resetear
- Restaura el SVG original sin animaciones

2.9 CONTROLES DE VISTA
-----------------------

[2.9.1 Zoom]
- Barra de zoom flotante en el preview
- Botones +/- para zoom in/out
- Rango: 20% a 500%
- Paso: dividir/multiplicar por 1.3
- Display: porcentaje actual
- Draggable: se puede reposicionar la barra
- Posiciones guardadas: br, bl, tr, tl (persistencia)

[2.9.2 Marco de limites]
- Boton "Mostrar limite"
- Muestra rectangulo punteado blanco del viewBox
- Handles en las 4 esquinas para redimensionar
- Modifica el viewBox del SVG
- Sincroniza con overlay de trayectorias

[2.9.3 Imagenes de fondo] (Fondo de referencia)
- Boton "Agregar imagen"
- Soporte para PNG y JPEG
- Cada imagen tiene:
  * Posicion (x, y)
  * Dimensiones (width, height)
  * Opacidad (0-100%)
  * Visibilidad (mostrar/ocultar)
  * Z-index (orden de apilamiento)
- Arrastrable para mover
- Resize handles en las 4 esquinas
- Panel de control con:
  * Miniatura
  * Nombre del archivo
  * Slider de opacidad
  * Botones de orden: frente, atras, subir, bajar
  * Boton restablecer posicion
  * Boton eliminar

2.10 PANEL DERECHO (ELEMENTOS SVG)
-----------------------------------

[2.10.1 Lista de elementos]
- Muestra todos los elementos del SVG cargado
- Miniatura de cada elemento (normalizada a 200x200)
- Nombre del elemento (id, clase, o "tag N")
- Tipo: nombre de la animacion asignada o "sin animacion"
- Dot del color de la animacion

[2.10.2 Interaccion]
- Click para seleccionar elemento
- Click en elemento ya seleccionado para deseleccionar
- Boton ojo para mostrar/ocultar elemento
- Ctrl+Click para seleccion multiple

[2.10.3 Indicadores visuales]
- Borde violeta para elemento seleccionado
- Borde verde para seleccion multiple
- Borde izquierdo coloreado para elementos en grupo
- Badge con nombre del grupo

[2.10.4 Controles de grupo]
- Header con botones: Seleccionar, Grupo +, Limpiar
- Seccion de grupos existentes
- Click en grupo para seleccionar todos sus elementos

2.11 MENU CONTEXTUAL
---------------------

[2.11.1 Opciones]
- Agrupar: crear grupo con elementos seleccionados
- Desagrupar: eliminar grupo
- Ver: mostrar elemento
- Ocultar: ocultar elemento
- Asignar trayectoria: submenu con todas las trayectorias
- Quitar trayectoria: remover asignacion
- Eliminar: borrar elemento del SVG

2.12 CONTROLES DE REPRODUCCION
-------------------------------

[2.12.1 Botones]
- Play (>): iniciar/reanudar animacion
- Pause (||): pausar animacion
- Stop ([]): detener y resetear

[2.12.2 Comportamiento]
- Play aplica todas las animaciones y las pone en estado "running"
- Pause cambia animationPlayState a "paused"
- Stop elimina las animaciones, luego las re-aplica en siguiente frame
- Los botones se resaltan con estado activo

2.13 ATAJOS DE TECLADO
------------------------

| Tecla         | Accion                              |
|---------------|-------------------------------------|
| Ctrl+O        | Abrir archivo SVG                   |
| Ctrl+Z        | Deshacer                            |
| Ctrl+Shift+Z  | Rehacer                             |
| Space         | Play/Pause (fuera de inputs)        |
| ESC           | Deseleccionar (modo piezas)         |
| Flecha Izq    | Slide anterior (en modo slides)     |
| Flecha Der    | Slide siguiente (en modo slides)    |

2.14 SOPORTE RIVE (OPCIONAL)
-----------------------------

[2.14.1 Carga de archivos .riv]
- Deteccion de archivos Rive por extension
- Carga usando libreria @rive-app/canvas
- Canvas dedicado para renderizado
- Controles de reproduccion: Play, Pause, Stop
- Se ocultan controles SVG cuando se carga Rive

==============================================================================
SECCION 3: REQUISITOS NO FUNCIONALES
==============================================================================

3.1 DEPENDENCIA DEL SERVIDOR
-----------------------------
- La aplicacion Qt NO debe depender de ningun servidor
- NO debe hacer peticiones HTTP a endpoints
- Todos los recursos SVG de ejemplo deben estar empaquetados
- El servidor de archivos locales debe ser opcional

3.2 RENDIMIENTO
----------------
- Renderizado de SVGs grandes (100+ elementos) sin lag
- Animaciones a 60fps
- Carga instantanea de SVGs de hasta 1MB
- Uso de memoria optimizado para workspaces multiples

3.3 PORTABILIDAD
-----------------
- Windows 10/11 (prioridad)
- Linux (Ubuntu, Debian)
- macOS
- Single executable o paquete de instalacion

3.4 PERSISTENCIA
-----------------
- Configuracion de la ventana (posicion, tamano)
- Ultima carpeta abierta
- Tema seleccionado (claro/oscuro)
- Posicion de la barra de zoom
- Workspaces anteriores (opcional)

3.5 TEMA VISUAL
----------------
- Tema oscuro por defecto (similar a la web)
- Variables de color:
  * --bg: #0f1117
  * --surface: #1a1d27
  * --surface2: #242734
  * --border: #2e3245
  * --text: #e4e6f0
  * --text-dim: #8b8fa7
  * --accent: #6c5ce7
  * --accent-hover: #7c6ef7
  * --danger: #e74c3c
  * --success: #2ecc71
- Fuentes: Segoe UI, system-ui, sans-serif
- Bordes redondeados: 8px

3.6 ACCESIBILIDAD
-------------------
- Navegacion completa por teclado
- Tooltips en todos los botones
- Contraste de color suficiente
- Tamano de fuente legible (minimo 10px)

==============================================================================
SECCION 4: ANALISIS DE DOCUMENTACION EXISTENTE
==============================================================================

4.1 ARCHIVOS A ANALIZAR
------------------------

La IA que genere el prompt DEBE leer y considerar:

[Archivos de documentacion]
- docs/README.md: Arquitectura general, estructura de archivos
- docs/INSTALLATION.md: Requisitos, plataformas, servidor
- docs/ANIMATIONS.md: Detalle de cada animacion y sus propiedades
- docs/CONTROLS.md: Todos los controles y sus rangos
- docs/PIECES.md: Modo piezas, activacion, comportamiento, limitaciones
- docs/EXPORT.md: Formato de exportacion, que se exporta y que no
- docs/DEVELOPMENT.md: Guia para desarrolladores, como agregar animaciones
- docs/RULES.md: Reglas de versionado (NO modificar este archivo)

[Archivos de codigo fuente]
- index.html: Estructura HTML semantica, secciones del sidebar
- styles.css: Variables CSS, layout, keyframes, animaciones
- app.js: Toda la logica: presets, shapes, motor, modo piezas, export
- serve.py: Servidor multiplataforma (referencia para funcionalidad)

[Archivos de skill]
- .opencode/skills/svg-animator/SKILL.md: Referencia tecnica completa

4.2 REPORTES DE TESTING EXISTENTES
------------------------------------

Se han identificado los siguientes problemas en el codigo actual:

[Bug Critico]
- Variable "i" no definida en funcion getExportKeyframe() (linea 2637)
- Afecta el export de animaciones con direccion personalizada

[Bug Medio]
- Selector CSS duplicado en 15+ ubicaciones
- Memory leak en event listeners del modo piezas
- Variables CSS no se limpian al cambiar animacion

[Bug Bajo]
- Estado inconsistente al eliminar elementos (indices desincronizados)

[Problemas de Escalabilidad]
- Archivo monolitico de 3000+ lineas
- 20+ variables globales mutables
- DOM queries repetidas sin cache
- Sin virtualizacion para listas grandes

[Problemas de Seguridad]
- XSS potencial en nombres de archivos
- Carga de scripts externos sin integrity
- Sin validacion de SVGs importados

4.3 SKILLS DOCUMENTADOS
-------------------------

[Skill: svg-animator]
- Descripcion: App vanilla JS para animar SVGs
- Tecnologias: HTML5, CSS3, JavaScript ES6
- Sin dependencias externas
- Servidor: Python 3
- Caracteristicas: presets, modo piezas, slideshow, export

==============================================================================
SECCION 5: INSTRUCCIONES PARA GENERACION DEL PROMPT FINAL
==============================================================================

5.1 QUE DEBE HACER LA IA
--------------------------

1. LEER los archivos .md en docs/ para entender la aplicacion completa
2. LEER el codigo fuente (index.html, styles.css, app.js) para detallar
3. ANALIZAR los reportes de testing para evitar problemas conocidos
4. GENERAR un prompt que solicite una app Qt con TODAS las funcionalidades
5. INCLUIR cada boton, cada interaccion, cada control
6. ESPECIFICAR que es sin servidor y 100% local
7. MENCIONAR las ventajas de app nativa vs web

5.2 FORMATO DEL PROMPT A GENERAR
---------------------------------

El prompt generado debe:
- Ser auto-contenido (no asumir conocimiento previo)
- Estar estructurado con secciones claras
- Usar formato ASCII (sin emojis)
- Incluir listas numeradas y con viñetas
- Ser copiable y pegable directamente
- Tener entre 500 y 1500 lineas

5.3 NIVEL DE DETALLE REQUERIDO
--------------------------------

Para CADA funcionalidad se debe especificar:
- Nombre de la funcionalidad
- Ubicacion en la interfaz (barra, menu, panel)
- Comportamiento al activarla
- Estados posibles (activo/inactivo)
- Atajo de teclado (si aplica)
- Restricciones o validaciones
- Comportamiento en error

5.4 PREGUNTAS QUE EL PROMPT DEBE RESPONDER
--------------------------------------------

- Como se cargan los archivos SVG?
- Como se navega entre multiples SVGs?
- Como se controlan las animaciones?
- Como funciona el modo piezas?
- Como se exporta el resultado?
- Como se guardan las preferencias?
- Como funciona el undo/redo?
- Que pasa con archivos SVG muy grandes?
- Como se manejan los errores?

5.5 EJEMPLO DE SECCION DEL PROMPT GENERADO
--------------------------------------------

La IA debe generar algo como:

"SECCION: Barra de herramientas principal

La barra de herramientas debe contener los siguientes botones
de izquierda a derecha:

1. Boton Abrir (icono de carpeta)
   - Atajo: Ctrl+O
   - Abre dialogo nativo de archivos
   - Filtro: archivos SVG (*.svg)
   - Recuerda ultima carpeta

2. Separador vertical

3. Botones de navegacion:
   - Anterior (icono flecha izquierda)
   - Siguiente (icono flecha derecha)
   - Indicador: '3/15'

[continuar con cada boton]"

==============================================================================
SECCION 6: CRITERIOS DE ACEPTACION
==============================================================================

6.1 CRITERIOS OBLIGATORIOS
-----------------------------

El prompt generado DEBE cumplir TODOS estos criterios:

[ ] Contiene todas las 19 animaciones con sus propiedades
[ ] Describe cada control (velocidad, retraso, repeticion, direccion)
[ ] Incluye el modo piezas con todas sus interacciones
[ ] Detalla el sistema de grupos y trayectorias
[ ] Describe las 6 transiciones de slides
[ ] Especifica el formato de exportacion SVG
[ ] Menciona el sistema de workspaces
[ ] Incluye los atajos de teclado
[ ] Describe el panel derecho de elementos
[ ] Especifica el tema visual (colores, fuentes)
[ ] Indica que es sin servidor y 100% local
[ ] Menciona la documentacion existente en docs/
[ ] Incluye los reportes de testing conocidos
[ ] Especifica portabilidad (Windows, Linux, macOS)
[ ] Describe persistencia de configuracion

6.2 CRITERIOS DE CALIDAD
--------------------------

- Sin ambiguedades en las descripciones
- Terminologia consistente
- Estructura logica y navegable
- Sin suposiciones sobre conocimiento previo
- Referencias a la documentacion existente
- Formato ASCII limpio y legible

6.3 VERIFICACION FINAL
-----------------------

Antes de entregar el prompt, verificar que:

[ ] NO contiene codigo de programacion
[ ] Describe TODAS las funcionalidades de la web
[ ] Menciona la carpeta docs/ y sus archivos
[ ] Especifica que la app Qt debe ser sin servidor
[ ] Esta en formato ASCII sin graficos
[ ] Es auto-contenido y claro
[ ] Puede ser copiado y pegado directamente
[ ] Incluye criterios de aceptacion
[ ] Toma en cuenta los bugs reportados
[ ] Propone soluciones a problemas de escalabilidad

==============================================================================
SECCION 7: PREGUNTAS ADICIONALES PARA CONSIDERAR
==============================================================================

7.1 PREGUNTAS DE AMPLIACION
------------------------------

La IA debe considerar si incluir en el prompt:

- Soporte para otros formatos (PNG, JPG, GIF animados)?
- Vista en miniatura de archivos en el explorador?
- Modo presentacion a pantalla completa?
- Impresion de SVGs?
- Historial de archivos recientes?
- Buscador de animaciones?
- Exportar como video (GIF, MP4)?
- Soporte para plugins o extensiones?
- Sincronizacion entre dispositivos?
- Modo oscuro/claro con cambio automatico?

7.2 PREGUNTAS DE RENDIMIENTO
------------------------------

- Cual es el tamano maximo de SVG soportado?
- Cuantos workspaces simultaneos se permiten?
- Se debe usar hilo separado para renderizado?
- Cache de SVGs recientes?

7.3 PREGUNTAS DE DISTRIBUCION
-------------------------------

- Instalador (.msi, .deb, .dmg)?
- Portable (sin instalacion)?
- AppImage para Linux?
- Code signing para Windows?

==============================================================================
FIN DEL PROMPT_QT.txt
==============================================================================

Este documento es una SOLICITUD para generar un prompt de desarrollo Qt.
NO contiene codigo de programacion.
Es un conjunto de instrucciones detalladas para ser procesado por una IA.

==============================================================================
