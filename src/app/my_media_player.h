#ifndef MY_MEDIA_PLAYER_H
#define MY_MEDIA_PLAYER_H

#include <QObject>
#include <QMap>
#include <QDebug>
#include <QAbstractVideoSurface>
#include <QVideoSurfaceFormat>
#include <QVideoFrame>

#include "src/threads/backend_thread.h"

class YUVBuffer : public QAbstractVideoBuffer {
 public:
  YUVBuffer(cv::Mat *mat) : QAbstractVideoBuffer(NoHandle), m_mode(NotMapped) {
    m_yuvMat.reset(mat);
  }

  MapMode mapMode() const Q_DECL_OVERRIDE { return m_mode; }
  uchar *map(MapMode mode, int *numBytes, int *bytesPerLine) Q_DECL_OVERRIDE {
    if (mode != NotMapped && m_mode == NotMapped) {
      if (numBytes)
        *numBytes = m_yuvMat->rows * m_yuvMat->cols;
      if (bytesPerLine)
        *bytesPerLine = m_yuvMat->cols;
      m_mode = mode;
      return m_yuvMat->data;
    }
    return 0;
  }
  void unmap() Q_DECL_OVERRIDE { m_mode = NotMapped; }
  QVariant handle() const Q_DECL_OVERRIDE { return 0; }

 private:
  MapMode m_mode;
  QScopedPointer<cv::Mat> m_yuvMat;
};

class MyMediaPlayer : public QObject {
 private:
  Q_OBJECT
  Q_PROPERTY(QAbstractVideoSurface *videoSurface READ videoSurface WRITE setVideoSurface NOTIFY surfaceChanged)

 public:
  Q_INVOKABLE void play(QString mode, QMap<QString, QVariant> config);
  Q_INVOKABLE void pause();
  Q_INVOKABLE void stop();

 public:
  explicit MyMediaPlayer(QObject *parent = nullptr);
  ~MyMediaPlayer() {};

  QAbstractVideoSurface *videoSurface() const;
  void showRGB(cv::Mat curr_frame);
  void showYUV(cv::Mat curr_frame);

 public slots:
  void setVideoSurface(QAbstractVideoSurface *surface);
  void onVideoFrameReady(cv::Mat curr_frame);

 signals:
  void surfaceChanged(QAbstractVideoSurface* surface);
  void threadFinishSig();

 private:
  void setFormat(int width, int height, QVideoFrame::PixelFormat frame_format);

  QAbstractVideoSurface *m_surface = NULL;
  QVideoSurfaceFormat m_format;
  BackendThread *m_backend;

  bool m_isFormatSet;
  QImage m_image;
  int count = 0;
};

#endif // MY_MEDIA_PLAYER_H

