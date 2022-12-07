#include <QVideoFrame>
#include <string>

#include "opencv_thread.h"

#define MAXPATHLEN 256

using namespace std;

OpenCVThread::OpenCVThread(QMap<QString, QVariant> config, QObject *parent)
    : QObject(parent), isRunning(false), isPausing(false) {

  this->file = config["file"].toString().toStdString();
  this->width = config["width"].toString().toStdString();
  this->height = config["height"].toString().toStdString();
  this->framerate = config["framerate"].toString().toStdString();
  this->format = config["format"].toString().toStdString();
}

void OpenCVThread::start() {
  char cwd[MAXPATHLEN];
  string path;
  size_t pos;
  string data_path, gst_str;
  stringstream gst_pipline;

  // if (getcwd(cwd, sizeof(cwd)) != NULL) {
  //   printf("Current working dir: %s\n", cwd);
  //   path = string(cwd);
  //   pos = path.find("/build");
  //   data_path = path.substr(0, pos) + "/images/test.yuv";
  // } else {
  //   qDebug() << "Please replace the path with your video path";
  // }

  // GStreamer Pipeline
  gst_pipline << "filesrc location = " << this->file
              << " ! videoparse format=GST_VIDEO_FORMAT_UYVY"
              << " width=" << this->width << " height=" << this->height
              << " framerate=" << this->framerate << " ! videoconvert ! appsink";

  gst_str = gst_pipline.str();
  cout << gst_str;
  cap.open(gst_str, cv::CAP_GSTREAMER);
  if (!cap.isOpened()) {
    cout << ">>>> ERROR: Unable to open camera/video:" << gst_str << endl;
    emit finished("OpenCV");
    return;
  }

  this->isRunning = true;
  while (this->isRunning) {
    cap.read(this->frame);
    if (this->frame.empty()) {
      qDebug() << "ERROR! blank frame grabbed\n";
      break;
    } else {
      emit frameReady(this->frame);
      usleep(30000);  // 30ms (30fps, 0.03s)
    }
  }

  this->cap.release();
  cv::destroyAllWindows();
  emit finished("OpenCV");
}

void OpenCVThread::stop() { this->isRunning = false; }
