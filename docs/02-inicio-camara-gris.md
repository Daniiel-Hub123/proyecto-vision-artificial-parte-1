# Primer avance: compilación, cámara y gris

## Qué hace esta versión

Abre la cámara predeterminada y muestra dos ventanas: `Original` (color) y `Gris` (gris ponderado). Se termina presionando ESC con una ventana de vídeo activa. Si no se puede abrir la cámara o leer un fotograma, muestra un mensaje de error y devuelve el código 1; la salida normal devuelve 0.

Solo se han incorporado captura y conversión a gris. Los filtros del efecto se añadirán en avances posteriores.

## Archivos

- `CMakeLists.txt`: selecciona C++17, localiza OpenCV y crea el ejecutable `vision_parte1` a partir del archivo C++.
- `src/Principal.cpp`: contiene todo el programa de este avance, sin clases adicionales.

## Compilar en el equipo de Daniel (PowerShell)

Abrir la terminal dentro de `Proyecto Parte 1`. Estos comandos usan las herramientas que se encontraron y probaron en este equipo; no requieren instalar nada adicional:

```powershell
$env:Path = "C:/msys64/ucrt64/bin;" + $env:Path

cmake -S . -B build -G Ninja `
  "-DCMAKE_MAKE_PROGRAM=C:/Users/ASUS/miniconda3/envs/opencv_pytorch/Library/bin/ninja.exe" `
  "-DCMAKE_CXX_COMPILER=C:/msys64/ucrt64/bin/g++.exe" `
  "-DOpenCV_DIR=C:/msys64/ucrt64/lib/cmake/opencv5"

cmake --build build
if ($LASTEXITCODE -eq 0) { ./build/vision_parte1.exe }
```

El acento invertido al final de una línea de PowerShell continúa el mismo comando: no colocar espacios después de él. Ejecutar la compilación solo si la configuración terminó sin errores.

- `-S .`: usar el proyecto de la carpeta actual.
- `-B build`: guardar los archivos generados dentro de `build`.
- `-G Ninja` y `CMAKE_MAKE_PROGRAM`: seleccionar la herramienta que ejecuta las tareas de compilación.
- `CMAKE_CXX_COMPILER`: seleccionar el compilador C++ compatible con esta instalación de OpenCV.
- `OpenCV_DIR`: indicar la carpeta que contiene `OpenCVConfig.cmake`.
- La modificación de `Path` es temporal para esa terminal y permite encontrar las DLL de OpenCV al ejecutar.

Después de modificar únicamente `Principal.cpp`, basta con repetir `cmake --build build` y ejecutar el programa.

## Para Xavier

1. Descargar lo último del repositorio con `git pull origin main` dentro de su copia local.
2. Revisar el `Principal.cpp` del Ejemplo 2 de Unidad 1 y compararlo con el del proyecto.
3. Identificar qué compilador y qué instalación de OpenCV usa para los ejemplos. No copiar las rutas de Daniel si no existen en su equipo.
4. Configurar CMake con sus rutas, compilar y ejecutar. El CMake del repositorio no contiene rutas particulares de ninguno de los dos equipos.
5. Confirmar que ambas ventanas muestran vídeo continuo y que ESC las cierra. Registrar su entorno y el resultado.

No mezclar una biblioteca OpenCV compilada para Visual Studio con el compilador MinGW. En el equipo de Daniel se probó el compilador de MSYS2 UCRT64 con OpenCV de ese mismo entorno.

## Explicación del código para ambos

1. `VideoCapture video(0)` abre la cámara de índice 0. Si hay varias, se puede probar el índice 1 cambiando esa línea y recompilando.
2. `isOpened()` comprueba si la apertura tuvo éxito.
3. `Mat frame` representa el fotograma de color y `Mat gris` la imagen de un canal.
4. `video >> frame` obtiene el siguiente fotograma. `empty()` detecta una lectura sin imagen antes de acceder a sus píxeles.
5. `Mat::zeros(frame.size(), CV_8UC1)` crea el destino con las mismas dimensiones, un canal y valores de 8 bits.
6. Los dos bucles recorren filas y columnas. `Vec3b` permite leer los tres valores de color del píxel, en orden BGR.
7. Cada píxel de salida se calcula como `0.114 * B + 0.587 * G + 0.299 * R`. El verde tiene mayor peso; no es el promedio simple de los tres canales. `static_cast<uchar>` convierte el resultado al tipo de 8 bits del destino, descartando su parte decimal, como la asignación del ejemplo.
8. `imshow` muestra las matrices. `waitKey(23)` atiende los eventos de las ventanas y permite detectar ESC (27). Los 23 milisegundos no garantizan un FPS concreto.
9. `release()` libera la cámara y `destroyAllWindows()` cierra las ventanas.

Para mantener visible la matemática del ejemplo se usa el recorrido manual, en lugar de sustituirlo por una llamada a `cvtColor` en esta primera versión. El original conserva la orientación que entrega la cámara; no se añadió el efecto espejo del ejemplo.

## Pruebas realizadas en el equipo de Daniel

- Configuración y compilación completas: GNU C++ 16.2.0, OpenCV 5.0.0 de MSYS2 UCRT64 y Ninja.
- Ejecución de escritorio: cámara abierta y ventanas `Original` y `Gris` comprobadas visualmente.
- Cierre con ESC: proceso finalizado con código 0.
- En el entorno restringido de la herramienta, la apertura de cámara falló y el programa mostró el mensaje previsto y terminó con código 1. La prueba de cámara satisfactoria se realizó fuera de ese entorno restringido.
- El entorno de Xavier todavía no se ha probado.

Esta instalación puede imprimir advertencias de GStreamer al buscar un backend de captura antes de abrir la cámara con otro disponible. En la prueba de escritorio la apertura y el vídeo funcionaron. Si solo aparece el error de apertura, cerrar otras aplicaciones que ocupen la cámara y comprobar el índice seleccionado y los permisos de cámara de Windows.
