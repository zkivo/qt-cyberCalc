#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QDir>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qDebug() << "Current working dir:" << QDir::currentPath();

    QFile f("C:\\Users\\marco\\Documents\\qt-workspace\\qt-calculator\\cyberpunk_buttons.qss");
    if (!f.exists()) {
        qDebug() << "QSS file NOT found!";
    }
    a.setStyle(QStyleFactory::create("Fusion"));  // 🔴 add this line
    if (f.open(QFile::ReadOnly)) {
        a.setStyleSheet(QString::fromUtf8(f.readAll()));
    }

    MainWindow w;
    w.show();
    return a.exec();
}
