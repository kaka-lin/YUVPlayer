#include "camera_info.h"

CameraInfo::CameraInfo(QQuickItem *parent)
    : QQuickItem(parent), QCameraInfo() {
  m_cameraDelegate = DeviceName;
}

CameraInfo::cameraDelegateType CameraInfo::cameraDelegate() {
    return m_cameraDelegate;
}

void CameraInfo::setCameraDelegate(CameraInfo::cameraDelegateType d) {
    m_cameraDelegate = d;
}

QString CameraInfo::defaultCamera() {
  // return QCameraInfo::defaultCamera();
}

QList<QString> CameraInfo::availableCameras() {
  QList<QString> r = QList<QString>();

  QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
  foreach (const QCameraInfo &cameraInfo, cameras) {
    QString rt = QString(cameraInfo.deviceName() + ":");
    if (DeviceName & m_cameraDelegate)
      rt += ':' + cameraInfo.deviceName();
    if (Orientation & m_cameraDelegate)
      rt += ':' + cameraInfo.orientation();
    if (Position & m_cameraDelegate)
      rt += ':' + cameraInfo.position();
    r.append(rt);
  }
  return r;
}
