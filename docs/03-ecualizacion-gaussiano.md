# Etapa 2: ecualización y suavizado Gaussiano

## Objetivo y orden

El flujo es cámara → gris con `cvtColor` → ecualización → Gaussiano. En el lienzo actual, Original, Gris y Ecualizada están en la primera fila. Gaussiano está a la izquierda de la segunda.

La ecualización redistribuye intensidades usando la distribución acumulada del histograma. Puede mejorar el contraste, pero también realzar ruido. El Gaussiano posterior suaviza variaciones locales antes de obtener bordes en la siguiente etapa.

## Parámetros que debemos entender

- `equalizeHist(gris, ecualizada)` recibe una imagen de un canal y 8 bits y escribe una salida distinta, para conservar el gris de comparación.
- `GaussianBlur(ecualizada, suavizada, Size(5, 5), 0)` usa una ventana impar de 5 × 5. El valor cero pide a OpenCV calcular la desviación estándar según el tamaño de la ventana. Sus pesos siguen una distribución gaussiana y se normalizan para sumar uno.
- El peso de un vecino es proporcional a `exp(-(x*x + y*y)/(2*sigma*sigma))`: los vecinos más cercanos al centro tienen mayor influencia.
- Una ventana mayor suele suavizar más y aumenta el trabajo; por ahora los parámetros son fijos. No se añadieron controles todavía.
- Se añadió el módulo `imgproc` a CMake y su cabecera al C++.

## Compilar y ejecutar

Desde la carpeta `Proyecto Interciclo`, usar los comandos de `02-inicio-camara-gris.md`. Ya apuntan a `build-interciclo`, configurada y compilada correctamente desde la nueva ubicación.

Para volver a compilar y ejecutar en PowerShell:

```powershell
$env:Path = "C:/msys64/ucrt64/bin;" + $env:Path
cmake --build build-interciclo
if ($LASTEXITCODE -eq 0) { ./build-interciclo/vision_parte1.exe }
```

No reutilizar `build` ni `build-filtros`: sus cachés conservan la ruta antigua. Se mantienen como archivos locales ignorados por Git.

Validación realizada: configuración y compilación correctas con OpenCV 5.0.0 y GNU C++ 16.2.0. La comprobación visual de esta nueva etapa está pendiente en ambos equipos.

## Comprobación visual pendiente

1. Abrir la cámara con `make run` y confirmar que Original, Gris, Ecualizada y Gaussiano actualizan la misma escena.
2. Comparar Gris y Ecualizada con una escena de poco contraste. No todas las escenas mejoran con ecualización global.
3. Comparar Ecualizada y Gaussiano en textura fina: la última debe verse más suave.
4. Probar iluminación clara y tenue, mover la mano frente a la cámara y salir con ESC.
5. Xavier debe repetir esta prueba y registrar su resultado sin sustituir el registro anterior de cámara y gris.

## Reparto de la etapa

- Daniel: ecualización y Gaussiano implementados en este avance; revisar el resultado visual y comprender los parámetros.
- Xavier: implementó Sobel X/Y y morfología siguiendo los Ejemplos 6 y 7.
- Ambos: comparar las [tres propuestas](05-filtros-propuesta.md), elegir el efecto final y preparar su explicación matemática.

## Pendientes según la rúbrica

- Diseñar y justificar el filtro propio y la manipulación de píxeles del efecto final.
- Incorporar controles de tamaño y peso del kernel y consulta de pasos intermedios.
- Implementar procesamiento equivalente CPU (`Mat`) y OpenCL (`UMat`), verificando el dispositivo usado.
- Medir milisegundos, FPS, RAM y recursos del sistema en escritorio y móvil con entradas y parámetros comparables. No asumir una aceleración antes de medir.
- Preparar Android con JNI/C++ y comprobar pronto una aplicación mínima nativa; preparar también la disponibilidad de `viz`/VTK en escritorio.
- Integrar el resultado de escritorio en una escena 3D: estas ventanas 2D sirven para desarrollo y no completan ese requisito.
- Informe con diagrama, matemática, resultados, fuentes y asistencia de IA; vídeo explicativo en inglés y defensa individual.

Las carpetas Android, informe, vídeo y mediciones son espacios de trabajo preparados, no entregables completados. Esta etapa mantiene un único archivo C++ y no adelanta la Parte II de señas/FFT.

## Lienzo de comparación

Se utiliza una matriz BGR de tres veces el ancho y tres veces el alto del fotograma. `Rect(x, y, ancho, alto)` selecciona cada cuadrante y `copyTo` copia la imagen en esa región. Las imágenes de gris se convierten temporalmente a BGR para igualar los tres canales del lienzo, sin alterar las matrices procesadas. Los rótulos se dibujan solo en el lienzo.

`create` reutiliza la memoria si las dimensiones no cambian y la ajusta si cambia el tamaño del fotograma. La ventana se puede redimensionar y ESC conserva la salida del programa. Este panel 2D es de desarrollo: siguen pendientes Android y la visualización 3D de escritorio.
