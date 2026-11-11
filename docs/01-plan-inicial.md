# Paso 0: planificación antes del código

Estado: propuesta inicial. Todavía no se ha implementado ni probado el programa.

## Alcance de la Parte I

Crear un efecto artístico propio sobre vídeo, en C++ con OpenCV, partiendo de los ejemplos de Unidad 1 y Unidad 2. Avanzar mediante pequeñas versiones que Daniel y Xavier puedan explicar.

Según la guía local `../../Guia-Proyecto-Integrador-Visión-Artificial-G9_.pdf` (páginas 3 a 8), el resultado completo incluye:

- Al menos cuatro filtros vistos en clase y un filtro propio, con justificación matemática y manipulación de píxeles o máscaras.
- Procesamiento equivalente con `cv::Mat` para CPU y `cv::UMat` con OpenCL.
- Controles para modificar tamaño y peso de los kernels y consultar pasos intermedios.
- Comparación de tiempos en milisegundos, FPS, memoria y recursos del sistema en escritorio y móvil.
- Versión Android con C++ nativo/JNI.
- Salida procesada como textura en una escena 3D de escritorio con `cv::viz`/VTK.
- Informe con diagrama, pruebas, fuentes y explicación matemática; vídeo explicativo en inglés.

La escala de grises será preparación de la imagen: no la contaremos como uno de los cuatro filtros. La ventana 2D servirá durante el aprendizaje y las pruebas; la entrega de escritorio requiere la integración 3D. El reconocimiento de señas, FFT y bot pertenecen a la Parte II.

## Propuesta sencilla de efecto

Nombre provisional: **Grabado a dos tonos con bordes oscuros**.

Aspecto buscado: imagen similar a una impresión sobre papel, con zonas claras y oscuras en dos colores elegidos y contornos marcados.

Flujo inicial propuesto:

1. Capturar el fotograma y convertirlo a gris.
2. Ecualizar el gris para realzar el contraste.
3. Suavizar mediante un filtro Gaussiano.
4. Obtener los bordes mediante Sobel en X e Y y combinar sus magnitudes.
5. Aplicar una operación morfológica sobre los bordes para ajustar el grosor del trazo.
6. Aplicar el efecto propio: elegir el tono de cada píxel según su intensidad y oscurecerlo según la máscara de bordes. Parametrizar el umbral tonal, los colores y la intensidad del trazo.

Los cuatro filtros base serán ecualización, Gaussiano, Sobel y morfología. Sobel X e Y cuentan juntos como un filtro. La transformación artística final se documentará por separado; aún debemos definir su fórmula exacta y comprobar el resultado visual. Si hace falta una convolución propia adicional para justificar el diseño, se decidirá al trabajar esa etapa a partir del Ejemplo 5, sin añadirla ahora.

Es una propuesta para trabajar, no un efecto ya acordado ni una garantía de exclusividad. Antes de cerrarlo, comprobar con el docente que el diseño no coincide con el de otro grupo.

## Ejemplos que usaremos como base

Las rutas de esta tabla son relativas a la carpeta de la asignatura; cada ejemplo contiene una segunda carpeta del mismo nombre y el archivo `Principal.cpp`.

| Necesidad | Referencia local | Qué adaptar |
| --- | --- | --- |
| Captura y gris | Unidad 1 / Ejemplo-2-Conformación-Imágenes-Escala-Grises-Video-OpenCV | `VideoCapture`, `Mat`, conversión a gris, ventanas y salida con ESC. |
| Transformación de píxeles y controles | Unidad 1 / Ejemplo-3-Espacio-Color-HSV-Binarización-por-Umbral-de-Color-OpenCV | Recorridos por píxel, máscaras y trackbars. |
| Contraste | Unidad 1 o Unidad 2 / Ejemplo-4-Ecualización-Histograma-OpenCV | Ecualización del gris. |
| Convolución y explicación matemática | Unidad 1 o Unidad 2 / Ejemplo-5-Filtro-CLAHE-Convolución-Imágenes-OpenCV | Entender la matriz del filtro y sus pesos. |
| Suavizado y bordes | Unidad 1 o Unidad 2 / Ejemplo-6-Filtro-Gaussiano-Filtro-Sobel-Detección-Bordes-Suavizado-OpenCV | Gaussiano, Sobel y combinación de resultados; panel de pasos. |
| Grosor del trazo | Unidad 2 / Ejemplo-7-Operaciones-Morfológicas-OpenCV | Elemento estructurante y operación morfológica. |

No es necesario incorporar todos los ejemplos. Canny y ruido sal y pimienta quedan fuera del diseño inicial para mantenerlo pequeño.

## Repartición propuesta

Ser responsable significa preparar y explicar esa parte; ambos deben comprender el proyecto completo para la sustentación individual.

| Etapa | Daniel | Xavier | Revisión conjunta |
| --- | --- | --- | --- |
| Primer avance | Preparar compilación de escritorio, captura, original y gris. | Revisar el Ejemplo 2 y preparar pruebas de cámara y una explicación de la conversión a gris. | Ejecutar en los dos equipos y explicar el bucle de captura. |
| Filtros base | Ecualización y Gaussiano. | Sobel y morfología. | Probar cada filtro por separado y explicar parámetros. |
| Efecto e interfaz | Transformación propia de tonos y combinación con bordes. | Trackbars y visualización de pasos intermedios. | Ajustar el diseño visual y escribir su explicación matemática. |
| Comparación CPU/OpenCL | Versión equivalente con `UMat` y comprobación de OpenCL. | Medición de tiempos, FPS y memoria; tablas y gráficas. | Usar el mismo vídeo, resolución y parámetros, y comprobar que los resultados coincidan. |
| Plataformas | Integración de escritorio con `cv::viz`/VTK. | Integración Android con JNI y C++ nativo. | Compartir el procesamiento y comprobarlo en ambas plataformas. |
| Entrega | Redactar diseño del efecto y resultados de escritorio. | Redactar integración móvil y comparaciones. | Informe, citas, diagrama, vídeo en inglés y ensayo de defensa. |

En la primera semana, Xavier puede comprobar el entorno Android y ejecutar una prueba mínima de JNI, como recomienda la guía. Esa comprobación de herramientas no implica adelantar todos los filtros móviles. También conviene comprobar pronto que existe una instalación de OpenCV con `viz` disponible para escritorio.

## Primer avance de código: solo cámara y gris

Archivos previstos para la próxima etapa:

- `CMakeLists.txt`: configuración mínima, basada en los ejemplos y adaptada al equipo.
- `src/Principal.cpp`: captura de cámara y visualización del original y del gris.

No copiar directamente la ruta de OpenCV de los CMake del profesor: contienen una ruta de su equipo. Primero se comprobarán el compilador y la instalación local. No se crearán clases ni varias cabeceras para esta primera versión.

Pasos de esa etapa:

1. Comprobar con qué herramientas se compilan actualmente los ejemplos en cada equipo.
2. Abrir la cámara con `VideoCapture` y comprobar que se abrió correctamente.
3. Leer un fotograma y comprobar que no esté vacío antes de procesarlo.
4. Convertirlo a gris y mostrar original y gris en dos ventanas.
5. Salir con ESC, liberar la cámara y cerrar las ventanas.
6. Explicar la conversión ponderada que aparece en el Ejemplo 2: gris = 0.114 B + 0.587 G + 0.299 R.

Criterio para cerrar el avance: compila, muestra vídeo continuo en ambas ventanas, sale con ESC y presenta un mensaje claro si no puede abrir la cámara. Ambos integrantes pueden explicar `Mat`, `VideoCapture`, el bucle, la conversión y `waitKey`. Registrar qué equipo se probó; no dar por probado el de Xavier desde el de Daniel.

## Orden de los siguientes avances

1. Cámara y gris.
2. Cuatro filtros base, incorporados uno por uno.
3. Efecto propio y su justificación matemática.
4. Controles y pasos intermedios.
5. Versión equivalente con OpenCL y mediciones.
6. Integración Android y VTK, verificadas por separado.
7. Pruebas finales, informe y vídeo.

No asumir que usar `UMat` convierte automáticamente cualquier recorrido manual de píxeles en trabajo de GPU. Al llegar a OpenCL se revisará cómo expresar el efecto propio sin perder su equivalencia. Tampoco se prometerá una aceleración concreta antes de medirla.

## Trabajo en el repositorio

- Mantener todos los archivos del proyecto dentro de `Proyecto Parte 1`.
- Descargar los cambios compartidos antes de trabajar y coordinar quién modifica cada archivo; al inicio habrá un único `Principal.cpp`.
- Guardar avances pequeños que compilen y tengan una descripción clara. La documentación puede guardarse antes de que exista código.
- Registrar el ejemplo de origen de cada adaptación, qué se cambió y por qué.
- Anotar la asistencia de IA y revisar cada línea antes de usarla en la entrega.
- No subir ejecutables, carpetas de compilación ni vídeos grandes; el `.gitignore` actual ya contempla estos casos habituales.

## Fuentes de esta planificación

- Guía local del proyecto: Parte I y rúbricas, páginas 3 a 8; recomendaciones de inicio de Android, páginas 14 y 15.
- Ejemplos locales de Unidad 1 y Unidad 2 citados en la tabla.
- README actual del repositorio.
- Mensajes recientes accesibles del chat «Revisión Parte I», relativos a la creación del repositorio y su sincronización. La lectura disponible no expuso toda la conversación previa; esta propuesta no presupone que allí ya se haya acordado un efecto específico.
- Asistencia de Codex en la revisión de archivos y organización de esta propuesta. No se generó código C++ en este paso.
