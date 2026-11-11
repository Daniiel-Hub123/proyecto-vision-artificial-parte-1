# Proyecto Parte 1 - Visión Artificial

Proyecto integrador de interciclo: efecto visual propio en vídeo con OpenCV y C++.

## Integrantes

- Daniel Santiago Barros Pallango
- Xavier Siguachi

## Base académica

El procesamiento se desarrollará a partir de los códigos de ejemplo de las carpetas Unidad 1 y Unidad 2 de la asignatura. Cada adaptación debe identificar su origen y ser comprendida por ambos integrantes.

La guía del proyecto permanece en la carpeta superior. Este repositorio contiene exclusivamente los archivos de Proyecto Parte 1.

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

Primer avance implementado: captura de cámara y conversión manual a gris ponderado, basado en el Ejemplo 2 de Unidad 1. Compilación, visualización y cierre con ESC comprobados en el equipo de Daniel; pendiente de probar en el de Xavier.

Las instrucciones de compilación, explicación del código y pruebas están en [Inicio: cámara y gris](docs/02-inicio-camara-gris.md).

La propuesta de efecto, repartición entre Daniel y Xavier y alcance del primer avance están en [el plan inicial](docs/01-plan-inicial.md).

## Fuentes y uso de IA

Documentar en docs/ las fuentes de los ejemplos, el código de terceros y la asistencia de IA utilizada, explicando las adaptaciones realizadas.
