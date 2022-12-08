#include <unistd.h>
#include <iostream>

#include "my_media_player.h"

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
  if (!m_surface || curr_frame.empty()) return;

  if (curr_frame.type() == CV_8UC3 || curr_frame.type() == CV_8UC4)
    showRGB(curr_frame);
  else
    showYUV(curr_frame);
}

void MyMediaPlayer::showRGB(cv::Mat curr_frame) {
  cv::Mat continuousFrame;
  if (!curr_frame.isContinuous())
    continuousFrame = curr_frame.clone();
  else
    continuousFrame = curr_frame;

  if (!m_isFormatSet) {
    setFormat(continuousFrame.cols,
              continuousFrame.rows,
              QVideoFrame::Format_RGB32);
    m_isFormatSet = true;
  }

  m_image =
      QImage(continuousFrame.data,
             continuousFrame.cols,
             continuousFrame.rows,
             continuousFrame.step,
             QImage::Format_RGB888);

  m_image = m_image.rgbSwapped();
  m_image = m_image.convertToFormat(
      QVideoFrame::imageFormatFromPixelFormat(QVideoFrame::Format_RGB32));

  m_surface->present(m_image);
}

void MyMediaPlayer::showYUV(cv::Mat curr_frame) {
  QVideoFrame::PixelFormat frame_format;
  frame_format = QVideoFrame::Format_YUV420P;
  cv::Mat *m = new cv::Mat(curr_frame);
  cvtColor(curr_frame, *m, cv::COLOR_BGR2YUV_I420);

  if (!m_isFormatSet) {
    setFormat(curr_frame.cols, curr_frame.rows, frame_format);
    m_isFormatSet = true;
  }

  m_surface->present(QVideoFrame(
      new YUVBuffer(m), QSize(curr_frame.cols, curr_frame.rows), frame_format));
}

void MyMediaPlayer::play(QMap<QString, QVariant> config) {
  m_backend->opencvStart(config);
}

void MyMediaPlayer::pause() { m_backend->opencvPause(); }

void MyMediaPlayer::stop() { m_backend->opencvStop(); }
