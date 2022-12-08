#include <QVideoFrame>
#include <string>

#include "opencv_thread.h"
#include "src/app/video_format.h"

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
  string gst_str;
  stringstream gst_pipline;

  // GStreamer Pipeline
  int gst_format = 0;
  std::unordered_map<std::string, int>::const_iterator format_map = VideoFormat.find(this->format);
  if (format_map == VideoFormat.end()) {
    cout << ">>>> ERROR: This format: " << format  << " is not supported!"<< endl;
    emit finished("OpenCV");
    return;
  } else {
    gst_format = format_map->second;
  }

  gst_pipline << "filesrc location = " << this->file
              << " ! videoparse format=" << gst_format
              << " width=" << this->width << " height=" << this->height
              << " framerate=" << this->framerate << " ! videoconvert ! appsink";

  gst_str = gst_pipline.str();
  cap.open(gst_str, cv::CAP_GSTREAMER);
  if (!cap.isOpened()) {
    cout << ">>>> ERROR: Unable to open camera/video:" << gst_str << endl;
    emit finished("OpenCV");
    return;
  }

  this->isRunning = true;
  while (this->isRunning) {
    if (!this->isPausing) {
      cap.read(this->frame);
      if (this->frame.empty()) {
        // qDebug() << "ERROR! blank frame grabbed\n";
        break;
      } else {
        emit frameReady(this->frame);
        usleep(30000);  // 30ms (30fps, 0.03s)
      }
    }
  }

  this->cap.release();
  cv::destroyAllWindows();
  emit finished("OpenCV");
}

void OpenCVThread::pause() { this->isPausing = !this->isPausing; }

void OpenCVThread::stop() { this->isRunning = false; }
