// Base: Unidad 1, Ejemplo 2, Principal.cpp.
// Adaptacion: solo camara y gris ponderado, con comprobaciones de lectura.
// Asistencia de Codex documentada en docs/02-inicio-camara-gris.md.

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
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
    int resultado = 0;

    namedWindow("Original", WINDOW_AUTOSIZE);
    namedWindow("Gris", WINDOW_AUTOSIZE);
    cout << "Camara abierta. Presiona ESC en una ventana de video para salir." << endl;

    while (true) {
        video >> frame;

        // Comprobar antes de consultar o modificar los pixeles.
        if (frame.empty()) {
            cerr << "Error: no se pudo leer un fotograma de la camara." << endl;
            resultado = 1;
            break;
        }

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

        imshow("Original", frame);
        imshow("Gris", gris);

        if (waitKey(23) == 27) { // 27 es el codigo de ESC.
            break;
        }
    }

    video.release();
    destroyAllWindows();
    return resultado;
}
