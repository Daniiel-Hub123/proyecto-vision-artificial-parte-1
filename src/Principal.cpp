#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

using namespace std;
using namespace cv;

Mat aplicarTermico(const Mat &suavizada, const Mat &bordesMorf) {
  // Arreglo de 3 filas y 3 columnas. Los pesos ya suman 1.
  float pesos[3][3] = {
      {0.0f, -0.125f, 0.0f},
      {-0.125f, 1.5f, -0.125f},
      {0.0f, -0.125f, 0.0f}};
  // CV_32F guarda los pesos como float de 32 bits, incluidos los decimales.
  Mat kernel(3, 3, CV_32F, pesos);

  Mat realzada;
  // Aplica el kernel a suavizada y guarda la salida en realzada.
  // El -1 conserva la profundidad de entrada: gris de 8 bits.
  filter2D(suavizada, realzada, -1, kernel);
  // Crea la salida del mismo tamano, con 8 bits por canal y tres canales BGR.
  Mat termico(realzada.size(), CV_8UC3);

  for (int fila = 0; fila < realzada.rows; fila++) {
    for (int columna = 0; columna < realzada.cols; columna++) {
      int gris = realzada.at<uchar>(fila, columna);
      Vec3b color;

      // Cambia de azul a cian, amarillo y rojo en orden BGR.
      if (gris <= 85) {
        color = Vec3b(255, 3 * gris, 0);
      } else if (gris <= 170) {
        int cambio = 3 * (gris - 85);
        color = Vec3b(255 - cambio, 255, cambio);
      } else {
        int cambio = 3 * (gris - 170);
        color = Vec3b(0, 255 - cambio, 255);
      }

      // Oscurece los contornos reforzados por la dilatacion.
      if (bordesMorf.at<uchar>(fila, columna) > 70) {
        color = Vec3b(30, 30, 30);
      }
      termico.at<Vec3b>(fila, columna) = color;
    }
  }

  return termico;
}

int main() {
  // 0 identifica la camara predeterminada. Probar 1 si hay otra camara.
  VideoCapture video(0);

  if (!video.isOpened()) {
    cerr << "Error: no se pudo abrir la camara." << endl;
    return 1;
  }

  Mat frame, gris, ecualizada, suavizada;
  Mat gX, gY, gXPos, gYPos, bordesSobel;
  Mat bordesMorf;
  Mat lienzo, vistaBgr;
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

    Mat termico = aplicarTermico(suavizada, bordesMorf);

    int ancho = frame.cols;
    int alto = frame.rows;
    int margen = ancho / 2;
    if (lienzo.rows != 2 * alto || lienzo.cols != 4 * ancho) {
      lienzo = Mat::zeros(Size(4 * ancho, 2 * alto), CV_8UC3);
    }

    // Primera fila: original, gris y ecualizada.
    frame.copyTo(lienzo(Rect(margen, 0, ancho, alto)));
    cvtColor(gris, vistaBgr, COLOR_GRAY2BGR);
    vistaBgr.copyTo(lienzo(Rect(margen + ancho, 0, ancho, alto)));
    cvtColor(ecualizada, vistaBgr, COLOR_GRAY2BGR);
    vistaBgr.copyTo(lienzo(Rect(margen + 2 * ancho, 0, ancho, alto)));

    // Segunda fila: Gaussiano, Sobel, morfologia y termico.
    cvtColor(suavizada, vistaBgr, COLOR_GRAY2BGR);
    vistaBgr.copyTo(lienzo(Rect(0, alto, ancho, alto)));
    cvtColor(bordesSobel, vistaBgr, COLOR_GRAY2BGR);
    vistaBgr.copyTo(lienzo(Rect(ancho, alto, ancho, alto)));
    cvtColor(bordesMorf, vistaBgr, COLOR_GRAY2BGR);
    vistaBgr.copyTo(lienzo(Rect(2 * ancho, alto, ancho, alto)));
    termico.copyTo(lienzo(Rect(3 * ancho, alto, ancho, alto)));

    Scalar verde(0, 255, 0);
    putText(lienzo, "Original", Point(margen + 10, 30), FONT_HERSHEY_SIMPLEX,
            0.7, verde, 2);
    putText(lienzo, "Gris", Point(margen + ancho + 10, 30),
            FONT_HERSHEY_SIMPLEX, 0.7, verde, 2);
    putText(lienzo, "1. Ecualizada", Point(margen + 2 * ancho + 10, 30),
            FONT_HERSHEY_SIMPLEX, 0.7, verde, 2);
    putText(lienzo, "2. Gaussiano", Point(10, alto + 30), FONT_HERSHEY_SIMPLEX,
            0.7, verde, 2);
    putText(lienzo, "3. Sobel", Point(ancho + 10, alto + 30),
            FONT_HERSHEY_SIMPLEX, 0.7, verde, 2);
    putText(lienzo, "4. Morfologia (Grosor)", Point(2 * ancho + 10, alto + 30),
            FONT_HERSHEY_SIMPLEX, 0.7, verde, 2);
    putText(lienzo, "5. Termico", Point(3 * ancho + 10, alto + 30),
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
