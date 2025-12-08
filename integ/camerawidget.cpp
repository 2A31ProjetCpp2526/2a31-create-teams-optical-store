#include "camerawidget.h"
#include "cameraworker.h"
#include <QPainter>
#include <QVBoxLayout>
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>

camerawidget::camerawidget(int cameraDevice, QWidget* parent)
    : QWidget(parent),
    m_currentFrame(),
    m_worker(new cameraworker(cameraDevice, this))
{
    setMinimumSize(640, 480);
    connect(m_worker, &cameraworker::frameReady, this, &camerawidget::onFrame);



    // Load glasses image
    // Load multiple glasses
    QString base = "C:/Users/Baraa/Desktop/integarduino/integ(1)/integ/images/";

    m_glassesList = {
        QImage(base + "rectang1.png"),
        QImage(base + "round.png"),
        QImage(base + "tall.png")
    };

    for (auto &img : m_glassesList) {
        img = img.scaled(231, 231, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }

    m_glasses = m_glassesList[0]; // Start with the first one

    qDebug() << "Loaded" << m_glassesList.size() << "glasses models.";




    // Load DNN face detector once
    m_net = cv::dnn::readNetFromCaffe(
        "C:/Users/Baraa/Desktop/integarduino/integ(1)/integ/models/deploy.prototxt",
        "C:/Users/Baraa/Desktop/integarduino/integ(1)/integ/models/res10_300x300_ssd_iter_140000.caffemodel"
        );
}

camerawidget::camerawidget(QWidget* parent)
    : camerawidget(0, parent) {}

camerawidget::~camerawidget() {
    stop();
}

void camerawidget::start() {
    if (!m_worker->isRunning()) {
        m_worker->start();
    }
}

void camerawidget::stop() {
    if (m_worker->isRunning()) {
        m_worker->stop();
        m_worker->wait();
    }
}
void camerawidget::onFrame(const QImage& img) {
    m_currentFrame = img;
    update();

    // Convert to cv::Mat
    QImage rgb = img.convertToFormat(QImage::Format_RGB888);
    cv::Mat frame(rgb.height(), rgb.width(), CV_8UC3, (uchar*)rgb.bits(), rgb.bytesPerLine());

    // Increment counter
    m_frameCounter++;
    /*if (m_frameCounter % m_detectionInterval == 0) {
        // Run DNN
        cv::Mat blob = cv::dnn::blobFromImage(frame, 1.0, cv::Size(300,300), cv::Scalar(104,177,123));
        m_net.setInput(blob);
        cv::Mat detections = m_net.forward();

        // TODO: parse detections and update m_lastFaceBox
        // Example: m_lastFaceBox = cv::Rect(x, y, w, h);
    }*/
    if (m_frameCounter % m_detectionInterval == 0) {

        cv::Mat blob = cv::dnn::blobFromImage(frame, 1.0, cv::Size(300,300),
                                              cv::Scalar(104,177,123));
        m_net.setInput(blob);
        cv::Mat detections = m_net.forward();

        // detections is [1,1,N,7]
        float* data = (float*)detections.data;

        float bestConfidence = 0.0f;
        cv::Rect bestBox;

        for (int i = 0; i < detections.size[2]; i++) {
            float confidence = data[i * 7 + 2];
            if (confidence < 0.5) continue;  // minimum confidence

            int x1 = data[i * 7 + 3] * frame.cols;
            int y1 = data[i * 7 + 4] * frame.rows;
            int x2 = data[i * 7 + 5] * frame.cols;
            int y2 = data[i * 7 + 6] * frame.rows;

            cv::Rect box(x1, y1, x2 - x1, y2 - y1);

            if (confidence > bestConfidence) {
                bestConfidence = confidence;
                bestBox = box;
            }
            qDebug() << "Confidence:" << confidence;
            qDebug() << "Box:" << x1 << y1 << x2 << y2;

        }

        m_lastFaceBox = bestBox;  // save for paintEvent()

        qDebug() << "Detections count:" << detections.size[2];

    }

    qDebug() << "OnFrame called";


    // PaintEvent will use m_lastFaceBox for glasses position
}

/*void camerawidget::onFrame(const QImage& img)
{
    // 1) Display frame
    m_currentFrame = img;
    update();

    // 2) Convert QImage → BGR cv::Mat
    QImage rgb = img.convertToFormat(QImage::Format_RGB888);
    cv::Mat frame(rgb.height(), rgb.width(), CV_8UC3, (uchar*)rgb.bits(), rgb.bytesPerLine());
    cv::cvtColor(frame, frame, cv::COLOR_RGB2BGR);

    // 3) Create DNN blob
    cv::Mat blob = cv::dnn::blobFromImage(
        frame,
        1.0,
        cv::Size(300, 300),
        cv::Scalar(104, 177, 123),
        false,  // don't swap RB
        false   // don't crop
        );

    // 4) Run face detector
    m_net.setInput(blob);
    cv::Mat detections = m_net.forward();

    // TODO: parse detections for real bounding boxes
}
void camerawidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.fillRect(rect(), Qt::black);

    if (m_currentFrame.isNull())
        return;

    // 1) Scale image to fit widget
    QImage scaled = m_currentFrame.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPoint center = rect().center() - QPoint(scaled.width()/2, scaled.height()/2);
    painter.drawImage(center, scaled);

    // 2) If we have a detected face, convert coordinates
    if (!m_lastFaceBox.empty()) {

        float scaleX = scaled.width()  / float(m_currentFrame.width());
        float scaleY = scaled.height() / float(m_currentFrame.height());

        int x = int(m_lastFaceBox.x * scaleX + center.x());
        int y = int(m_lastFaceBox.y * scaleY + center.y());
        int w = int(m_lastFaceBox.width  * scaleX);
        int h = int(m_lastFaceBox.height * scaleY);

        QRect faceBox(x, y, w, h);

        // Draw green rectangle
        painter.setPen(QPen(Qt::green, 2));
        painter.drawRect(faceBox);

        // Draw glasses
        if ( !m_glasses.isNull()) {

            int glassesWidth  = m_lastFaceBox.width() * 0.9;
            int glassesHeight = m_lastFaceBox.height() * 0.35;

            int glassesX = m_lastFaceBox.x() + (m_lastFaceBox.width() - glassesWidth) / 2;
            int glassesY = m_lastFaceBox.y() + m_lastFaceBox.height() * 0.28;

            QRect glassesRect(glassesX, glassesY, glassesWidth, glassesHeight);

            painter.drawImage(glassesRect, m_glasses);
        }

    }
}*/
void camerawidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.fillRect(rect(), Qt::black);

    if (m_currentFrame.isNull())
        return;

    // --- Scale and center the webcam image ---
    QImage scaled = m_currentFrame.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPoint center = rect().center() - QPoint(scaled.width()/2, scaled.height()/2);
    painter.drawImage(center, scaled);

    // --- Draw face box and glasses in SCALED coordinates ---
    if (m_lastFaceBox.width > 0 && m_lastFaceBox.height > 0) {

        // Compute scale between camera frame and widget
        float scaleX = float(scaled.width())  / m_currentFrame.width();
        float scaleY = float(scaled.height()) / m_currentFrame.height();

        // Map face box to widget coordinates
        int x = int(m_lastFaceBox.x * scaleX + center.x());
        int y = int(m_lastFaceBox.y * scaleY + center.y());
        int w = int(m_lastFaceBox.width * scaleX);
        int h = int(m_lastFaceBox.height * scaleY);

        QRect faceBox(x, y, w, h);

        // Draw green rectangle
        painter.setPen(QPen(Qt::green, 2));
        painter.drawRect(faceBox);

        // Draw glasses
        if (!m_glasses.isNull()) {
            // Compute glasses width relative to face box
            int glassesWidth = int(w * 1);

            // Maintain original aspect ratio
            double aspect = double(m_glasses.height()) / double(m_glasses.width());
            int glassesHeight = int(glassesWidth * aspect);

            int glassesX = x + (w - glassesWidth) / 2;
            int glassesY = y + int(h * 0.32); // approximate eye level

            QRect glassesRect(glassesX, glassesY, glassesWidth, glassesHeight);
            painter.drawImage(glassesRect, m_glasses);

        }
    }

}
void camerawidget::nextGlasses() {
    if (m_glassesList.isEmpty()) return;

    m_glassesIndex = (m_glassesIndex + 1) % m_glassesList.size();
    m_glasses = m_glassesList[m_glassesIndex];

    qDebug() << "Switched to glasses model:" << m_glassesIndex;
}
