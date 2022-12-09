#ifndef CAMERA_INFO_H
#define CAMERA_INFO_H

#include <QObject>
#include <QDebug>
#include <QList>
#include <QString>
#include <QQuickItem>
#include <QtMultimedia/QCameraInfo>

class CameraInfo : public QQuickItem, public QCameraInfo {
 private:
  Q_OBJECT
  Q_DISABLE_COPY(CameraInfo)
  Q_ENUMS(cameraDelegateType)
  Q_PROPERTY(QString defaultCamera READ defaultCamera)
  Q_PROPERTY(QList<QString> availableCameras READ availableCameras NOTIFY
                   availableCamerasChanged)
  Q_PROPERTY(cameraDelegateType cameraDelegate READ cameraDelegate WRITE setCameraDelegate)

 public:
  explicit CameraInfo(QQuickItem *parent = 0);
  ~CameraInfo() {};

  enum cameraDelegateType {
    DeviceName = 0x1,
    Orientation = 0x2,
    Position = 0x4,
  };

  static void registerQmlType() {
    qmlRegisterType<CameraInfo>("CameraInfo", 1, 0, "CameraInfo");
  }

  cameraDelegateType cameraDelegate();
  QString defaultCamera();
  QList<QString> availableCameras();
  void setCameraDelegate(cameraDelegateType d);

 signals:
  void availableCamerasChanged();

 private:
  cameraDelegateType m_cameraDelegate; // = DeviceName;
};

#endif // CAMERA_INFO_H

