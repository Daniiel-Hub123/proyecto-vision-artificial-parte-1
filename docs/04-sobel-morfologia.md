# Etapa 2 (Continuación): Detección de Bordes con Sobel y Operación Morfológica

El panel Morfología muestra la dilatación en cruz de 3 × 3 sobre Sobel. Pintura / Vitral aplica su propia erosión al fotograma BGR. Ver [Propuestas de filtros](05-filtros-propuesta.md).


## Objetivo y Orden del Pipeline

En este avance se completa la integración de los **cuatro filtros base** requeridos por la rúbrica del proyecto, partiendo de los ejemplos vistos en clase:

1. **Captura y Gris Ponderado** (`0.114*B + 0.587*G + 0.299*R`) — Preparación base de la imagen.
2. **Filtro 1: Ecualización de Histograma** (`equalizeHist`) — Realce de contraste global (Unidad 2 - Ejemplo 4).
3. **Filtro 2: Suavizado Gaussiano** (`GaussianBlur`) — Atenuación de ruido de alta frecuencia (Unidad 2 - Ejemplo 6).
4. **Filtro 3: Detección de Bordes con Sobel** (`Sobel` X e Y + combinación) — Estimación del gradiente espacial (Unidad 2 - Ejemplo 6).
5. **Filtro 4: Operación Morfológica** (`morphologyEx` con dilatación) — Ajuste y engrosamiento del trazo (Unidad 2 - Ejemplo 7).

El aporte original reunió seis vistas en dos filas de tres. El lienzo actual conserva esas filas y muestra las tres propuestas en una tercera.

---

## Explicación Detallada de Parámetros (Para Xavier)

### 1. Detección de Bordes con Sobel (Filtro 3)

El operador Sobel calcula las derivadas espaciales de primer orden de la imagen para encontrar cambios bruscos de intensidad (bordes). Se aplican dos convoluciones: una horizontal ($G_x$) y otra vertical ($G_y$).

Código utilizado (idéntico al Ejemplo 6 de Unidad 2):
```cpp
Sobel(suavizada, gX, CV_16S, 1, 0, 3, 1, 0);
Sobel(suavizada, gY, CV_16S, 0, 1, 3, 1, 0);
convertScaleAbs(gX, gXPos);
convertScaleAbs(gY, gYPos);
addWeighted(gXPos, 0.5, gYPos, 0.5, 0, bordesSobel);
```

#### Parámetros de `Sobel(src, dst, ddepth, dx, dy, ksize, scale, delta)`:
* **`suavizada` (src):** Imagen de entrada de 1 canal en escala de grises previamente filtrada con Gaussiano para evitar falsos bordes por ruido.
* **`gX` / `gY` (dst):** Matriz de destino que almacenará las derivadas calculadas.
* **`CV_16S` (ddepth):** Tipo de datos con signo de 16 bits (short con signo).
  * *¿Por qué no `CV_8U` directo?* Porque las transiciones de claro a oscuro producen derivadas con valores negativos (ej. de 255 a 0 da pendiente negativa). Si usáramos 8 bits sin signo (`uchar`), los valores negativos se truncarían a 0, perdiendo la mitad de los bordes.
* **`dx = 1, dy = 0` (en `gX`):** Orden de la derivada en X = 1, en Y = 0. Detecta bordes verticales resaltando cambios horizontales.
* **`dx = 0, dy = 1` (en `gY`):** Orden de la derivada en X = 0, en Y = 1. Detecta bordes horizontales resaltando cambios verticales.
* **`ksize = 3`:** Tamaño de la máscara o kernel de Sobel (matriz 3 × 3).
  * Para $G_x$: $\begin{bmatrix} -1 & 0 & 1 \\ -2 & 0 & 2 \\ -1 & 0 & 1 \end{bmatrix}$
  * Para $G_y$: $\begin{bmatrix} -1 & -2 & -1 \\ 0 & 0 & 0 \\ 1 & 2 & 1 \end{bmatrix}$
* **`scale = 1` y `delta = 0`:** Factor de escala opcional aplicado al resultado y valor constante sumado a la salida.

#### Parámetros de `convertScaleAbs` y `addWeighted`:
* **`convertScaleAbs(gX, gXPos)`:** Calcula el valor absoluto de cada pixel $|G_x|$ y lo convierte a `CV_8U` (rango 0 a 255 con saturación).
* **`addWeighted(gXPos, 0.5, gYPos, 0.5, 0, bordesSobel)`:** Realiza una combinación lineal ponderada:
  $$\text{bordesSobel} = 0.5 \times \operatorname{sat}_8(|G_x|) + 0.5 \times \operatorname{sat}_8(|G_y|)$$
  Es una combinación para visualizar bordes, con redondeo de la salida. `sat_8` limita los valores al intervalo 0 a 255 antes de combinarlos. No es la magnitud euclidiana exacta `sqrt(Gx*Gx + Gy*Gy)` ni conserva todo el rango de las derivadas originales.

---

### 2. Operación Morfológica (Filtro 4)

La morfología matemática procesa las formas de la imagen basándose en la teoría de conjuntos y un **elemento estructurante** (kernel binario).

Código utilizado (adaptado de Unidad 2 - Ejemplo 7):
```cpp
elementoMorf = getStructuringElement(MORPH_CROSS, Size(3, 3), Point(-1, -1));
morphologyEx(bordesSobel, bordesMorf, MORPH_DILATE, elementoMorf);
```

#### Parámetros de `getStructuringElement(shape, ksize, anchor)`:
* **`MORPH_CROSS`:** Forma del elemento estructurante. Crea una cruz centrada:
  $$\begin{bmatrix} 0 & 1 & 0 \\ 1 & 1 & 1 \\ 0 & 1 & 0 \end{bmatrix}$$
  (Alternativas: `MORPH_RECT` para rectángulo o `MORPH_ELLIPSE` para círculo/elipse).
* **`Size(3, 3)`:** Tamaño de la vecindad espacial del elemento estructurante (ancho x alto).
* **`Point(-1, -1)`:** Posición del punto de anclaje (centro del elemento estructurante por defecto).

#### Parámetros de `morphologyEx(src, dst, op, kernel)`:
* **`bordesSobel` (src):** En este programa, imagen en escala de grises con los bordes detectados; no se ha aplicado binarización.
* **`bordesMorf` (dst):** Imagen resultante.
* **`MORPH_DILATE`:** Operación de dilatación. Asigna a cada píxel el valor máximo dentro de la vecindad definida por el elemento estructurante:
  $$(I \oplus B)(x, y) = \max_{(i, j) \in B} I(x - i, y - j)$$
  *Efecto visual:* Expande y engrosa los trazos blancos de los bordes, pudiendo conectar trazos próximos y reforzando la respuesta de los bordes.

---

## Sustentación Conjunta: Los 4 Filtros Base

| Filtro | Función OpenCV | Propósito en el Pipeline | Justificación Matemática |
|---|---|---|---|
| **1. Ecualización** | `equalizeHist` | Maximizar el contraste dinámico global de la escena antes de filtrar. | Transforma el histograma usando su Función de Distribución Acumulada (CDF) normalizada: $s_k = T(r_k) = (L-1) \sum_{j=0}^{k} p_r(r_j)$. |
| **2. Gaussiano** | `GaussianBlur` | Atenuar ruido de alta frecuencia e imperfecciones del sensor sin borrar bordes principales. | Convolución 2D con campana de Gauss normalizada: $G(x, y) = \frac{1}{2\pi\sigma^2} e^{-\frac{x^2+y^2}{2\sigma^2}}$. |
| **3. Sobel** | `Sobel` + `addWeighted` | Detectar contornos y transiciones de intensidad en direcciones ortogonales. | Aproximación discreta de las derivadas direccionales: $\nabla I = \left(\frac{\partial I}{\partial x}, \frac{\partial I}{\partial y}\right)$. |
| **4. Morfología** | `morphologyEx` (`MORPH_DILATE`) | Ajustar el grosor del trazo de los bordes para el estilo artístico final. | Operación de dilatación mediante elemento estructurante: $(I \oplus B)(x,y) = \sup_{b \in B} I(x-b)$. |

---

## Comprobación visual de los cuatro filtros base

Las dos primeras filas de la ventana `Lienzo` muestran:

```
+--------------------+--------------------+--------------------+
|      Original      |        Gris        |   1. Ecualizada    |
|       (Color)      |    (Ponderado)     |    (Contraste)     |
+--------------------+--------------------+--------------------+
|    2. Gaussiano    |  3. Sobel (Bordes) | 4. Morfologia      |
|    (Suavizado)     |   (Gradiente X+Y)  | (Grosor de trazo)  |
+--------------------+--------------------+--------------------+
```

### Qué observar durante la prueba:
1. **Gris vs Ecualizada:** En Ecualizada las zonas con iluminación pobre o sombras se redistribuyen ganando definición.
2. **Ecualizada vs Gaussiano:** El suavizado elimina el grano fino y ruido introducido por la ecualización.
3. **Gaussiano vs Sobel:** Sobel resalta en líneas blancas los contornos de objetos, rostro y manos sobre un fondo negro.
4. **Sobel vs Morfología:** La dilatación engrosa notablemente los bordes, haciendo que las líneas sean más densas y visibles.

## Revisión del avance

La integración de Sobel, dilatación y seis vistas es coherente con el flujo previsto. La dilatación toma máximos en la vecindad en cruz y engrosa los trazos claros; el ejemplo del profesor usa erosión sobre Laplaciano, por lo que aquí se adapta la operación al objetivo de engrosamiento sobre Sobel. No se requiere binarizar para esta dilatación en gris.

Comprobación realizada: `make` termina correctamente. Falta registrar una nueva comprobación visual del lienzo y de la salida con ESC para esta etapa. Los cuatro filtros base y las tres propuestas están integrados. Queda por elegir el efecto para la entrega.
