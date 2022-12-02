#include "my_media_player.h"

#include <unistd.h>

#include <iostream>

#define MAXPATHLEN 256

using namespace std;

MyMediaPlayer::MyMediaPlayer(QObject *parent)
    : QObject(parent), m_isFormatSet(false) {
  m_backend = new BackendThread(this);
  connect(m_backend, &BackendThread::frameReady, this, &MyMediaPlayer::onVideoFrameReady);
}

QAbstractVideoSurface *MyMediaPlayer::videoSurface() const {
  return m_surface;
}

void MyMediaPlayer::setVideoSurface(QAbstractVideoSurface *surface) {
  if (m_surface == surface) return;

  if (m_surface && m_surface != surface && m_surface->isActive()) {
    m_surface->stop();
  }

  m_surface = surface;
  emit surfaceChanged(m_surface);

  // m_backend->start();
  if (m_surface && m_format.isValid()) {
    m_format = m_surface->nearestFormat(m_format);
    m_surface->start(m_format);
  }
}

void MyMediaPlayer::setFormat(int width, int heigth,
                              QVideoFrame::PixelFormat frame_format) {
  QSize size(width, heigth);
  QVideoSurfaceFormat format(size, frame_format);
  m_format = format;

  if (m_surface) {
    if (m_surface->isActive()) {
      m_surface->stop();
    }
    m_format = m_surface->nearestFormat(m_format);
    m_surface->start(m_format);
  }
}

void MyMediaPlayer::onVideoFrameReady(cv::Mat curr_frame) {
  QVideoFrame::PixelFormat frame_format;

  if (!m_surface || curr_frame.empty()) return;

#ifdef A8
  frame_format = QVideoFrame::Format_UYVY;
  cv::Mat* m = new cv::Mat(curr_frame);
#else
  frame_format = QVideoFrame::Format_YUV420P;
  cv::Mat* m = new cv::Mat;
  // cv::COLOR_RGBA2YUV_I420 or cv::COLOR_BGR2YUV_YV12
  cvtColor(curr_frame, *m, cv::COLOR_RGBA2YUV_I420);
#endif

  if (!m_isFormatSet) {
    setFormat(curr_frame.cols, curr_frame.rows,
              frame_format);
    m_isFormatSet = true;
  }

  m_surface->present(QVideoFrame(new YUVBuffer(m), QSize(curr_frame.cols, curr_frame.rows), frame_format));
}

void MyMediaPlayer::play() { m_backend->opencvStart(); }

void MyMediaPlayer::pause() {}

void MyMediaPlayer::stop() { m_backend->opencvStop(); }
