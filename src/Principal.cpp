#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

using namespace std;
using namespace cv;

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
  Mat lienzo;
  Mat tmp;
  int resultado = 0;

  namedWindow("Lienzo", WINDOW_NORMAL);
  cout << "Camara abierta. Presiona ESC en el lienzo para salir." << endl;

  while (true) {
    video >> frame;

    // Comprobar antes de consultar o modificar los pixeles.
    if (frame.empty()) {
      cerr << "Error: no se pudo leer un fotograma de la camara." << endl;
      resultado = 1;
      break;
    }

    // Para girar la imagen de forma horizontal
    flip(frame, frame, 1);

    // Matriz de igual ancho y alto, con un unico canal de 8 bits.
    gris = Mat::zeros(frame.size(), CV_8UC1);

    for (int fila = 0; fila < frame.rows; fila++) {
      for (int columna = 0; columna < frame.cols; columna++) {
        Vec3b pixel = frame.at<Vec3b>(fila, columna);

        // OpenCV entrega el color en orden B, G, R.
        gris.at<uchar>(fila, columna) = static_cast<uchar>(
            0.114 * pixel[0] + 0.587 * pixel[1] + 0.299 * pixel[2]);
      }
    }

    // Redistribuye las intensidades del gris para realzar el contraste.
    equalizeHist(gris, ecualizada);

    // Suaviza la imagen ecualizada con una ventana de 5 x 5 pixeles.
    GaussianBlur(ecualizada, suavizada, Size(5, 5), 1.73, 1.73);

    // Cada cuadrante tiene el tamano de un fotograma y tres canales.
    lienzo.create(frame.rows * 2, frame.cols * 2, CV_8UC3);
    frame.copyTo(lienzo(Rect(0, 0, frame.cols, frame.rows)));

    // Iguala el numero de canales antes de copiar cada imagen al lienzo.
    cvtColor(gris, tmp, COLOR_GRAY2BGR);
    tmp.copyTo(lienzo(Rect(frame.cols, 0, frame.cols, frame.rows)));
    cvtColor(ecualizada, tmp, COLOR_GRAY2BGR);
    tmp.copyTo(lienzo(Rect(0, frame.rows, frame.cols, frame.rows)));
    cvtColor(suavizada, tmp, COLOR_GRAY2BGR);
    tmp.copyTo(lienzo(Rect(frame.cols, frame.rows, frame.cols, frame.rows)));

    putText(lienzo, "Original", Point(10, 30), FONT_HERSHEY_SIMPLEX, 0.7,
            Scalar(0, 255, 0), 2);
    putText(lienzo, "Gris", Point(frame.cols + 10, 30), FONT_HERSHEY_SIMPLEX,
            0.7, Scalar(0, 255, 0), 2);
    putText(lienzo, "Ecualizada", Point(10, frame.rows + 30),
            FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 255, 0), 2);
    putText(lienzo, "Gaussiano", Point(frame.cols + 10, frame.rows + 30),
            FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 255, 0), 2);

    imshow("Lienzo", lienzo);

    if (waitKey(23) == 27) { // 27 es el codigo de ESC.
      break;
    }
  }

  video.release();
  destroyAllWindows();
  return resultado;
}
