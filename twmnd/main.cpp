#include <QApplication>
#include <QTextCodec>
#include <QtGlobal>
#include <QLocale>
#include <QLibraryInfo>
#include <QTranslator>
#include <QDesktopWidget>

#include "widget.h"
#include "xcb/xcb.h"
#include "dbusinterface.h"

#include <iostream>
#include <memory>

#ifndef VERSION
#define VERSION "1.2"
#endif

void logOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
#ifdef QT_NO_DEBUG_OUTPUT
    Q_UNUSED(type);
    Q_UNUSED(msg);
    Q_UNUSED(context);
    return;
#else
    std::cout << "[" << QTime::currentTime().toString("hh:mm:ss").toStdString() << "]";
    switch (type){
    case QtDebugMsg:
        std::cout << " " << msg;
        break;
    case QtWarningMsg:
        std::cout << "[warning][" << context.file << '-' << context.line << "] " << msg.toStdString();
        break;
    case QtCriticalMsg:
        std::cout << "[critical][" << context.file << '-' << context.line << "] " << msg.toStdString();
        break;
    case QtFatalMsg:
        std::cout << "[fatal][" << context.file << '-' << context.line << "] " <<  msg.toStdString();
        break;
    }
    std::cout << std::endl;
#endif
}

int spawn_per_screen(const char *wmname, DBusInterface &dbus, QApplication &a) {
    std::vector<std::unique_ptr<Widget>> widgets;

    auto no_of_screens = QDesktopWidget().screenCount();

    for (int screen = 0; screen < no_of_screens; ++screen) {
        widgets.push_back(std::make_unique<Widget>(wmname, screen));
    }

    for (auto &w : widgets) w->connectToDBus(dbus);

    return a.exec();
}

int main(int argc, char *argv[])
{
    std::cout << "Starting twmnd version " << VERSION << std::endl;
    qInstallMessageHandler(logOutput);
    QApplication a(argc, argv);
    QApplication::setQuitOnLastWindowClosed(true);
    QApplication::setApplicationName("twmn");
    QPalette p = a.palette();
    p.setBrush(QPalette::Link, QBrush(QColor("black")));
    a.setPalette(p);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    const char* wname = "twmn";
    if (argc > 1) {
      wname = argv[1];
    }

    DBusInterface dbus(&a);

    if (strcmp(wname, "--per-screen") == 0) {
        return spawn_per_screen("twmn", dbus, a);
    }

    Widget w(wname);
    w.connectToDBus(dbus);
    return a.exec();
}
