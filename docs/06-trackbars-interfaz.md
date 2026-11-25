# Etapa 3 (Continuación): Controles Dinámicos de Kernels y Umbral

En esta etapa se añadieron controles deslizantes (**trackbars**) sobre la ventana `Lienzo` siguiendo el patrón de `createTrackbar`, `setTrackbarPos` y `getTrackbarPos` visto en Unidad 1 (Ejemplo 3) y Unidad 2 (Ejemplo 7). Los pasos intermedios ya se muestran simultáneamente en el lienzo 2 × 4.

## Controles implementados

| Trackbar | Rango | Valor inicial | Función |
| --- | --- | --- | --- |
| `Kernel Gauss` | 1 a 31 | 5 | Ajusta el tamaño impar del filtro Gaussiano ($k \times k$). Permite aumentar la carga computacional en tiempo real. |
| `Kernel Morf` | 1 a 21 | 3 | Ajusta el tamaño impar del elemento estructurante en cruz (`MORPH_CROSS`) para variar el grosor del trazo de los bordes. |
| `Umbral Borde` | 0 a 255 | 70 | Controla a partir de qué intensidad de borde el filtro térmico comienza a oscurecer hacia gris oscuro. |

## Nota sobre valores impares

Tanto `GaussianBlur` como `getStructuringElement` requieren dimensiones impares para tener un píxel central bien definido. La función auxiliar `tamanoImpar` garantiza que cualquier valor par o cero leído del trackbar se ajuste al impar siguiente ($\ge 1$).
