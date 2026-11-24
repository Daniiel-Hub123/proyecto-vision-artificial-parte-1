#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <vector>

using namespace std;
using namespace cv;

Mat aplicarVitral(const Mat &frame, const Mat &bordesSobel) {
  Mat pintura, mascara;
  Mat elipse = getStructuringElement(MORPH_ELLIPSE, Size(9, 9));
  erode(frame, pintura, elipse);
  inRange(bordesSobel, Scalar(0), Scalar(70), mascara);

  // Copia el color erosionado y deja negros los contornos.
  Mat vitral = Mat::zeros(pintura.size(), pintura.type());
  pintura.copyTo(vitral, mascara);
  return vitral;
}

Mat aplicarGlitchRgb(const Mat &frame, const Mat &bordesSobel) {
  const int d = 12; // Pixeles de desplazamiento

  // 1. Separar canales (0: Azul, 1: Verde, 2: Rojo)
  vector<Mat> canales;
  split(frame, canales);

  // 2. Copiar el azul a la izquierda y el rojo a la derecha.
  // Las partes que quedan fuera del fotograma se rellenan con cero.
  Mat azulMovido = Mat::zeros(frame.size(), CV_8UC1);
  Mat rojoMovido = Mat::zeros(frame.size(), CV_8UC1);
  int anchoCopia = frame.cols - d;
  if (anchoCopia > 0) {
    Rect izquierda(0, 0, anchoCopia, frame.rows);
    Rect derecha(d, 0, anchoCopia, frame.rows);
    canales[0](derecha).copyTo(azulMovido(izquierda));
    canales[2](izquierda).copyTo(rojoMovido(derecha));
  }
  canales[0] = azulMovido;
  canales[2] = rojoMovido;

  // 3. Unir canales y suavizar un poco el efecto mezclando con la original
  Mat colorSeparado, glitch;
  merge(canales, colorSeparado);
  addWeighted(frame, 0.25, colorSeparado, 0.75, 0, glitch);

  // 4. Aplicar el efecto al 100% sobre los bordes (Sobel + Morfología)
  Mat contornos;
  inRange(bordesSobel, Scalar(71), Scalar(255), contornos);
  dilate(contornos, contornos, Mat()); // Mat() usa un kernel 3x3 por defecto
  colorSeparado.copyTo(glitch, contornos);

  return glitch;
}

Mat aplicarGlitchBloques(const Mat &frame, const Mat &bordesSobel) {
  Mat zonaGlitch;
  inRange(bordesSobel, Scalar(66), Scalar(255), zonaGlitch);
  Mat kernelZona = getStructuringElement(MORPH_RECT, Size(15, 15));
  dilate(zonaGlitch, zonaGlitch, kernelZona);

  Mat bloques;
  Mat kernelBloques = getStructuringElement(MORPH_RECT, Size(11, 11));
  morphologyEx(frame, bloques, MORPH_OPEN, kernelBloques);

  // La compresion JPEG baja crea textura en el fondo sin guardar archivos.
  vector<uchar> buffer;
  vector<int> calidadJpeg = {IMWRITE_JPEG_QUALITY, 6};
  Mat glitch;
  if (imencode(".jpg", frame, buffer, calidadJpeg)) {
    glitch = imdecode(buffer, IMREAD_COLOR);
  }
  if (glitch.empty()) {
    glitch = frame.clone();
  }

  // Coloca la imagen con apertura solo cerca de los contornos.
  bloques.copyTo(glitch, zonaGlitch);
  return glitch;
}

int main() {
  // 0 identifica la camara predeterminada. Probar 1 si hay otra camara.
  VideoCapture video(0);

  if (!video.isOpened()) {
    cerr << "Error: no se pudo abrir la camara." << endl;
    return 1;
  }

  Mat frame;
  Mat gris;
  Mat ecualizada;
  Mat suavizada;
  Mat gX, gY;
  Mat gXPos, gYPos;
  Mat bordesSobel;
  Mat bordesMorf;
  Mat lienzo;
  Mat tmp;
  int resultado = 0;

  Mat elementoMorf = getStructuringElement(MORPH_CROSS, Size(3, 3));

  namedWindow("Lienzo", WINDOW_NORMAL);
  cout << "Presiona ESC en el lienzo para salir." << endl;

  while (true) {
    video >> frame;

    // Comprobar antes de consultar o modificar los pixeles.
    if (frame.empty()) {
      cerr << "Error: no se pudo leer un fotograma de la camara." << endl;
      resultado = 1;
      break;
    }

    // Refleja horizontalmente el fotograma.
    // flip(frame, frame, 1);

    cvtColor(frame, gris, COLOR_BGR2GRAY);

    // 1. Redistribuye las intensidades del gris para realzar el contraste
    equalizeHist(gris, ecualizada);

    // 2. Suaviza la imagen ecualizada con una ventana de 5 x 5 pixeles
    GaussianBlur(ecualizada, suavizada, Size(5, 5), 0);

    // 3. Deteccion de bordes con Sobel en X e Y y combinacion
    Sobel(suavizada, gX, CV_16S, 1, 0, 3, 1, 0);
    Sobel(suavizada, gY, CV_16S, 0, 1, 3, 1, 0);
    convertScaleAbs(gX, gXPos);
    convertScaleAbs(gY, gYPos);
    addWeighted(gXPos, 0.5, gYPos, 0.5, 0, bordesSobel);

    // 4. La dilatacion en cruz engrosa los bordes de Sobel.
    morphologyEx(bordesSobel, bordesMorf, MORPH_DILATE, elementoMorf);

    Mat vitral = aplicarVitral(frame, bordesSobel);
    Mat glitchRgb = aplicarGlitchRgb(frame, bordesSobel);
    Mat glitchBloques = aplicarGlitchBloques(frame, bordesSobel);

    int ancho = frame.cols;
    int alto = frame.rows;
    lienzo.create(3 * alto, 3 * ancho, CV_8UC3);

    // Primera fila: original, gris y ecualizada.
    frame.copyTo(lienzo(Rect(0, 0, ancho, alto)));
    cvtColor(gris, tmp, COLOR_GRAY2BGR);
    tmp.copyTo(lienzo(Rect(ancho, 0, ancho, alto)));
    cvtColor(ecualizada, tmp, COLOR_GRAY2BGR);
    tmp.copyTo(lienzo(Rect(2 * ancho, 0, ancho, alto)));

    // Segunda fila: Gaussiano, Sobel y morfologia.
    cvtColor(suavizada, tmp, COLOR_GRAY2BGR);
    tmp.copyTo(lienzo(Rect(0, alto, ancho, alto)));
    cvtColor(bordesSobel, tmp, COLOR_GRAY2BGR);
    tmp.copyTo(lienzo(Rect(ancho, alto, ancho, alto)));
    cvtColor(bordesMorf, tmp, COLOR_GRAY2BGR);
    tmp.copyTo(lienzo(Rect(2 * ancho, alto, ancho, alto)));

    // Tercera fila: los tres efectos propios.
    vitral.copyTo(lienzo(Rect(0, 2 * alto, ancho, alto)));
    glitchRgb.copyTo(lienzo(Rect(ancho, 2 * alto, ancho, alto)));
    glitchBloques.copyTo(lienzo(Rect(2 * ancho, 2 * alto, ancho, alto)));

    Scalar verde(0, 255, 0);
    putText(lienzo, "Original", Point(10, 30), FONT_HERSHEY_SIMPLEX, 0.7, verde,
            2);
    putText(lienzo, "Gris", Point(ancho + 10, 30), FONT_HERSHEY_SIMPLEX, 0.7,
            verde, 2);
    putText(lienzo, "1. Ecualizada", Point(2 * ancho + 10, 30),
            FONT_HERSHEY_SIMPLEX, 0.7, verde, 2);
    putText(lienzo, "2. Gaussiano", Point(10, alto + 30), FONT_HERSHEY_SIMPLEX,
            0.7, verde, 2);
    putText(lienzo, "3. Sobel", Point(ancho + 10, alto + 30),
            FONT_HERSHEY_SIMPLEX, 0.7, verde, 2);
    putText(lienzo, "4. Morfologia (Grosor)", Point(2 * ancho + 10, alto + 30),
            FONT_HERSHEY_SIMPLEX, 0.7, verde, 2);
    putText(lienzo, "Pintura / Vitral", Point(10, 2 * alto + 30),
            FONT_HERSHEY_SIMPLEX, 0.7, verde, 2);
    putText(lienzo, "Glitch RGB", Point(ancho + 10, 2 * alto + 30),
            FONT_HERSHEY_SIMPLEX, 0.7, verde, 2);
    putText(lienzo, "Glitch de bloques", Point(2 * ancho + 10, 2 * alto + 30),
            FONT_HERSHEY_SIMPLEX, 0.7, verde, 2);

    imshow("Lienzo", lienzo);

    if (waitKey(23) == 27) { // 27 es el codigo de ESC.
      break;
    }
  }

  video.release();
  destroyAllWindows();
  return resultado;
}
