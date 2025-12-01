#ifndef CAMERAWIDGET_H
#define CAMERAWIDGET_H

#include <QWidget>
#include <QImage>
#include <QPaintEvent>
#include <QPointF>
#include <QDebug>

#include <opencv2/dnn.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
//#include <opencv2/face.hpp>    // facemark

class cameraworker;

class camerawidget : public QWidget {
    Q_OBJECT
public:
    explicit camerawidget(int cameraDevice = 0, QWidget* parent = nullptr);
    explicit camerawidget(QWidget* parent);     // for Qt Designer
    ~camerawidget() override;

public slots:
    void start();
    void stop();
    void onFrame(const QImage& img);
    void nextGlasses();

protected:
    void paintEvent(QPaintEvent* event) override;
private slots:


private:
    // helpers
    QImage rotateImage(const QImage& img, qreal angle);

    // state
    QImage m_currentFrame;
    cameraworker* m_worker;
    QImage m_glasses;                 // original PNG, not pre-scaled

    // DNN + Facemark
    cv::dnn::Net m_net;
    //cv::Ptr<cv::face::Facemark> m_facemark;

    // detection results (camera coordinates)
    cv::Rect m_lastFaceBox;
    cv::Point2f m_leftEyeCamera;
    cv::Point2f m_rightEyeCamera;
    double m_faceAngleDeg = 0.0;
    //glasses pics
    QVector<QImage> m_glassesList;
    int m_glassesIndex = 0;
    // tuning
    int m_frameCounter = 0;
    int m_detectionInterval = 5;      // run face detection every N frames (reduce CPU)
};

#endif // CAMERAWIDGET_H
