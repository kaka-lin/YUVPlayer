#include <QVideoFrame>
#include <string>

#include "opencv_thread.h"

#define MAXPATHLEN 256

using namespace std;

OpenCVThread::OpenCVThread(QObject *parent)
    : QObject(parent), isRunning(false), isPausing(false) {
  cv::VideoCapture cap;
}

void OpenCVThread::start() {
  char cwd[MAXPATHLEN];
  string path;
  size_t pos;
  string data_path, gst_str;
  stringstream gst_pipline;

  if (getcwd(cwd, sizeof(cwd)) != NULL) {
    printf("Current working dir: %s\n", cwd);
    path = string(cwd);
    pos = path.find("/build");
    data_path = path.substr(0, pos) + "/data/test.yuv";
  } else {
    qDebug() << "Please replace the path with your video path";
  }

  // GStreamer Pipeline
  gst_pipline << "filesrc location = " << data_path
              << " ! videoparse format=GST_VIDEO_FORMAT_UYVY"
              << " width=" << 1280 << " height=" << 720
              << " framerate=" << 30 / 1 << " ! videoconvert ! appsink";

  gst_str = gst_pipline.str();
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
