# Filtro térmico de falso color

La función `aplicarTermico` recibe el gris ecualizado y suavizado, y los bordes de Sobel después de la dilatación en cruz de 3 × 3 de Xavier. El color indica intensidad de imagen. Para medir temperatura se necesita una cámara térmica.

Cada píxel gris se lee con `at<uchar>` y se convierte a BGR con `Vec3b`, como en los recorridos por píxel de clase:

| Intensidad | Transición |
| --- | --- |
| 0 a 85 | Azul a cian |
| 86 a 170 | Cian a amarillo |
| 171 a 255 | Amarillo a rojo |

El factor 3 permite recorrer 255 niveles de un canal a lo largo de cada tramo de 85. Si el borde dilatado supera 70, el píxel se pinta gris oscuro con `Vec3b(30, 30, 30)`.

Ecualización y Gaussiano preparan las intensidades de la paleta. Sobel y morfología producen los contornos que la complementan. La asignación manual es la transformación propia y se hace con bucles y condiciones.

Ejecutar `make run`. El efecto aparece abajo a la derecha en el lienzo. ESC cierra el programa.
