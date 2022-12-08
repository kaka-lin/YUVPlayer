#ifndef OPENCV_THREAD_H
#define OPENCV_THREAD_H

#include <QDebug>
#include <QMap>
#include <QObject>
#include <iostream>
#include <unistd.h>
#include <string>

#include <opencv2/opencv.hpp>

class OpenCVThread : public QObject {
 private:
  Q_OBJECT

 public:
  explicit OpenCVThread(QMap<QString, QVariant> config, QObject *parent = nullptr);
  ~OpenCVThread(){};

 public slots:
  void start();
  void pause();
  void stop();

 signals:
  void frameReady(cv::Mat frame);
  void finished(QString thread_name);

 private:
  cv::VideoCapture cap;
  cv::Mat frame;

  std::string file;
  std::string width;
  std::string height;
  std::string framerate;
  std::string format;

  bool isRunning = false;
  bool isPausing = false;
};
Q_DECLARE_METATYPE(OpenCVThread*)

#endif  // OPENCV_THREAD_H
