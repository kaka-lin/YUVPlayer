#ifndef BACKEND_THREAD_H
#define BACKEND_THREAD_H

#include <QDebug>
#include <QMap>
#include <QObject>
#include <QThread>

#include "opencv2/opencv.hpp"

class BackendThread : public QObject {
 private:
  Q_OBJECT

 public:
  explicit BackendThread(QObject *parent = nullptr);
  ~BackendThread(){};

 public slots:
  void opencvStart(QString mode, QMap<QString, QVariant> config);
  void opencvStop();
  void opencvPause();

  void finished(QString thread_name);

 signals:
  void uiUnlock();
  void frameReady(cv::Mat frame);
  void threadFinishSig();

 private:
  QMap<QString, QThread *> threads;
  // QVariantMap == QMap<QString, QVariant>
  QMap<QString, QVariant> workers;
};

#endif  // BACKEND_THREAD_H
