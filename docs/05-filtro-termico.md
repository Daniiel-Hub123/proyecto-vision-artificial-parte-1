# Filtro térmico de falso color

La función `aplicarTermico` recibe el gris ecualizado y suavizado, y los bordes de Sobel después de la dilatación en cruz de 3 × 3 de Xavier. El color indica intensidad de imagen. Para medir temperatura se necesita una cámara térmica.

## Kernel de realce

Se aplica `filter2D` al gris suavizado antes de asignar los colores.

```text
     0  -1   0
K = -1  12  -1  / 8
     0  -1   0
```

La salida de cada píxel es `(12 × centro − norte − sur − este − oeste) / 8`. Los pesos suman `(12 − 4) / 8 = 1`, así que una zona uniforme conserva su brillo. En las transiciones, un centro más claro que sus vecinos se aclara y uno más oscuro se oscurece. En el código los pesos ya están divididos: el centro vale 1.5 y cada vecino ortogonal vale −0.125. La salida conserva el tipo de 8 bits y se limita a 0–255.

## Paleta térmica

Cada píxel realzado se lee con `at<uchar>` y se convierte a BGR con `Vec3b`, como en los recorridos por píxel de clase.

| Intensidad | Transición |
| --- | --- |
| 0 a 85 | Azul a cian |
| 86 a 170 | Cian a amarillo |
| 171 a 255 | Amarillo a rojo |

El factor 3 permite recorrer 255 niveles de un canal a lo largo de cada tramo de 85.

## Contornos con mezcla ponderada

Hasta un valor de borde de 70 se conserva el color de la paleta. Por encima se calcula el peso del gris oscuro con `peso = (borde - 70) / 185.0`. El 185 es la diferencia entre 255 y 70, de modo que el peso llega a 1 en un borde máximo.

En cada canal se calcula `salida = (1 - peso) * color + peso * 30`. Se aplica la misma idea de mezcla ponderada del Ejemplo 6, pero aquí el peso depende de cada píxel. Los bordes moderados conservan parte del color y los más fuertes se acercan al gris oscuro.

Ecualización y Gaussiano preparan las intensidades. El kernel propio realza el contraste local para la paleta manual. Sobel y la morfología se calculan sobre el gris suavizado y aportan los contornos.

Ejecutar `make run`. El efecto aparece abajo a la derecha en el lienzo. ESC cierra el programa.
