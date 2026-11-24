# Propuestas de filtros visuales

Las tres propuestas trabajan sobre el mismo fotograma de la cámara. Comparten la secuencia gris → ecualización → Gaussiano de 5 × 5 → Sobel X/Y. Las dos respuestas de Sobel se combinan con `addWeighted`, con peso de 0.5 en cada eje. El panel **Morfología (Grosor)** muestra la dilatación en cruz de 3 × 3 sobre Sobel que incorporó Xavier. Cada efecto prepara su propia máscara con `inRange`.

## Pintura / Vitral

`aplicarVitral` erosiona el fotograma a color con una elipse de **9 × 9**. La erosión toma el mínimo de cada canal en la vecindad y expande las zonas oscuras. La máscara selecciona los valores de Sobel de **0 a 70**. `copyTo` conserva el color erosionado en esas zonas y deja negros los contornos fuertes.

## Glitch RGB

`aplicarGlitchRgb` separa los canales BGR con `split`. Usa `Rect` y `copyTo`, como en el lienzo, para mover el azul **12 píxeles a la izquierda** y el rojo **12 a la derecha**. El verde conserva su posición. Se rellenan con cero los espacios vacíos de cada canal y se unen los tres con `merge`.

La imagen habitual combina **25 % del original y 75 % de la imagen desplazada** con `addWeighted`. En los contornos, seleccionados con Sobel de **71 a 255** y dilatados con un kernel de **3 × 3**, se copia la imagen desplazada completa.

## Glitch de bloques

`aplicarGlitchBloques` usa una compresión JPEG de **calidad 6** para dar textura de compresión al fondo. `imencode` e `imdecode` hacen esa conversión en memoria. Si la conversión falla, el fotograma original sirve de fondo.

La máscara selecciona los valores de Sobel de **66 a 255** y se amplía con una dilatación rectangular de **15 × 15**. En esas zonas se copia una versión del fotograma procesada con apertura morfológica cuadrada de **11 × 11**. La apertura aplica una erosión seguida de una dilatación y reduce los detalles brillantes pequeños.

## Comparación

Con `make run`, la última fila muestra **Pintura / Vitral**, **Glitch RGB** y **Glitch de bloques**, de izquierda a derecha. ESC cierra el programa.
