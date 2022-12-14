#include <QDebug>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "app/my_media_player.h"
#include "app/camera_info.h"

int main(int argc, char *argv[]) {
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QGuiApplication app(argc, argv);
  QString program_name(argv[0]);

  // QML engine
  QQmlApplicationEngine engine;

  // QML type registration
  CameraInfo::registerQmlType();

  // QML context
  MyMediaPlayer *player = new MyMediaPlayer();
  qRegisterMetaType<cv::Mat>("cv::Mat");

  engine.rootContext()->setContextProperty("player", player);

  // Load QML scene
  engine.addImportPath(QStringLiteral("qrc:/"));
  engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
  if (engine.rootObjects().isEmpty()) {
    return -1;
  }

  return app.exec();
}
