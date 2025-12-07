#include "cameraworker.h"
#include <chrono>
#include <thread>

using namespace std;

cameraworker::cameraworker(int device, QObject* parent)
    : QThread(parent), m_device(device), m_running(false) {}

cameraworker::~cameraworker() {
    stop();
    wait();
}

void cameraworker::stop() {
    m_running = false;
}

static QImage matToQImage(const cv::Mat& mat) {
    if (mat.empty()) return {};
    if (mat.type() == CV_8UC3) {
        cv::Mat rgb;
        cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB);
        return QImage(rgb.data, rgb.cols, rgb.rows, (int)rgb.step, QImage::Format_RGB888).copy();
    } else if (mat.type() == CV_8UC1) {
        return QImage(mat.data, mat.cols, mat.rows, (int)mat.step, QImage::Format_Grayscale8).copy();
    } else if (mat.type() == CV_8UC4) {
        return QImage(mat.data, mat.cols, mat.rows, (int)mat.step, QImage::Format_ARGB32).copy();
    }
    return {};
}
/*void cameraworker::nextGlasses()
{
    // TODO: change glasses index, update frame overlay, etc.
    qDebug() << "Next glasses clicked!";
}*/

void cameraworker::run() {
    cv::VideoCapture cap;
    if (!cap.open(m_device, cv::CAP_ANY)) return;

    cap.set(cv::CAP_PROP_FRAME_WIDTH, 1280);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 720);

    m_running = true;
    const int sleepMs = 33; // ~30 fps
    cv::Mat frame;

    while (m_running) {
        if (!cap.read(frame)) {
            this_thread::sleep_for(chrono::milliseconds(50));
            continue;
        }
        QImage img = matToQImage(frame);
        if (!img.isNull()) emit frameReady(img);
        this_thread::sleep_for(chrono::milliseconds(sleepMs));
    }

    cap.release();
}
