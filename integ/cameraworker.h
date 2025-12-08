#ifndef CAMERAWORKER_H
#define CAMERAWORKER_H

#include<QObject>
#include <QThread>
#include <QImage>
#include <atomic>
#include <opencv2/opencv.hpp>
#include <QDebug>

class cameraworker : public QThread {
    Q_OBJECT
public:
    explicit cameraworker(int device = 0, QObject* parent = nullptr);
    ~cameraworker() override;

    void stop();
    // void nextGlasses();

signals:
    void frameReady(const QImage& image);

protected:
    void run() override;

private:
    int m_device;
    std::atomic<bool> m_running;
private slots:

};

#endif // CAMERAWORKER_H
