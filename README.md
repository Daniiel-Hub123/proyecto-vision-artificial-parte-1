# Proyecto Interciclo - Visión Artificial

Proyecto integrador de interciclo: efecto visual propio en vídeo con OpenCV y C++.

## Integrantes

- Daniel Santiago Barros Pallango
- Xavier Siguachi

## Base académica

El procesamiento se desarrollará a partir de los códigos de ejemplo de las carpetas Unidad 1 y Unidad 2 de la asignatura. Cada adaptación debe identificar su origen y ser comprendida por ambos integrantes.

La guía del proyecto permanece en la carpeta superior. Este repositorio contiene los archivos del Proyecto Interciclo.

## Estructura

- src/: implementación C++.
- include/: cabeceras del proyecto.
- docs/: diseño del efecto, explicación matemática, referencias e informe.
- assets/: recursos pequeños de prueba que puedan compartirse.
- results/: resultados seleccionados y comparaciones de rendimiento.

## Orden de desarrollo

1. Definir el efecto artístico propio y sus etapas.
2. Capturar vídeo y visualizar original y escala de grises con cv::Mat.
3. Probar e integrar al menos cuatro filtros vistos en clase y el filtro propio.
4. Incorporar controles y visualización de pasos intermedios.
5. Implementar el procesamiento equivalente con cv::UMat y OpenCL.
6. Medir tiempos, FPS y memoria en CPU y OpenCL.
7. Adaptar el procesamiento a Android con C++/JNI.
8. Integrar la salida de escritorio en una escena 3D con cv::viz/VTK.
9. Preparar el informe y el vídeo explicativo en inglés.

La firma de forma y Fourier corresponden a la Parte II.

## Estado inicial

Primer avance implementado: captura de cámara y conversión manual a gris ponderado, basado en el Ejemplo 2 de Unidad 1. Compilación, visualización y cierre con ESC comprobados en el equipo de Daniel; Xavier también registró una prueba satisfactoria de cámara, gris y ESC.

Las instrucciones de compilación, explicación del código y pruebas están en [Inicio: cámara y gris](docs/02-inicio-camara-gris.md).

La propuesta de efecto, repartición entre Daniel y Xavier y alcance del primer avance están en [el plan inicial](docs/01-plan-inicial.md).

## Avance actual

Captura de cámara, conversión a gris y cuatro filtros base: ecualización, Gaussiano, Sobel X/Y y dilatación en cruz. Hay tres propuestas para comparar antes de elegir el efecto final: Pintura / Vitral, Glitch RGB y Glitch de bloques.

Quedan pendientes los controles, CPU/OpenCL, mediciones, Android y VTK.

Consultar [Ecualización y Gaussiano](docs/03-ecualizacion-gaussiano.md), [Sobel y morfología](docs/04-sobel-morfologia.md) y [Propuestas de filtros](docs/05-filtros-propuesta.md).

## Próximos entregables

- `android/`: futura aplicación móvil; aún sin implementar.
- `entregables/informe/`: informe final, matemática, diagrama y referencias.
- `entregables/video/`: guion y material del vídeo en inglés.
- `results/mediciones/`: futuras tablas de tiempos, FPS y memoria.

La carpeta local se llama `Proyecto Interciclo`. El nombre del repositorio remoto y el ejecutable `vision_parte1` se conservan para mantener las instrucciones de ejecución existentes.

## Comandos rápidos

Desde la carpeta del proyecto:

```powershell
make
make run
```

`make` configura y compila; `make run` compila primero y luego abre la aplicación. ESC cierra el lienzo. Se requieren Make, CMake y Ninja en el PATH.

El Makefile usa `CMakeLists.txt` para compilar. CMake enlaza `imgcodecs` para la compresión JPEG en memoria de Glitch de bloques. La salida se guarda en `build-make-ninja/`, ignorada por Git.

En Windows se usa por defecto OpenCV 5 y el compilador de MSYS2 UCRT64 instalados en `C:/msys64/ucrt64`. Para otro equipo se pueden cambiar las rutas sin editar el archivo:

```powershell
make OPENCV_ROOT=D:/msys64/ucrt64
make run OPENCV_ROOT=D:/msys64/ucrt64
```

También se pueden ajustar `OPENCV_DIR`, `BUILD_DIR`, `APP` y pasar opciones adicionales con `CMAKE_ARGS`. Si se cambia de compilador o instalación de OpenCV, usar una carpeta nueva con `BUILD_DIR` para evitar reutilizar la caché anterior. Se usa Ninja para evitar el problema detectado con MinGW Make y los acentos de la ruta del proyecto.

Validación del Makefile: compilación completa correcta y comprobación de los comandos de `make run` mediante `make -n run`; no se realizó una nueva prueba de cámara en este ajuste.

## Visualización de desarrollo

El lienzo reúne el original, el gris y los cuatro filtros base en las dos primeras filas. El panel Morfología muestra la dilatación en cruz de 3 × 3 aplicada a Sobel. La última fila muestra los tres efectos, cada uno implementado en su propia función.

- **Pintura / Vitral** combina la erosión a color con contornos negros derivados de Sobel.
- **Glitch RGB** desplaza azul y rojo en sentidos opuestos y refuerza el desfase cerca de los contornos.
- **Glitch de bloques** usa JPEG de calidad 6 como fondo. Cerca de los contornos dilatados aplica una apertura cuadrada de 11 × 11. La ecualización, el Gaussiano y Sobel alimentan el mapa de contornos.

Las máscaras se usan dentro de las funciones. Ejecutar `make run` y salir con ESC.
