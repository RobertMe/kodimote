#include "mainwindow.h"
#include "settingsdialog.h"
#include "connectdialog.h"
#include "aboutdialog.h"
#include "settings.h"
#include "networkaccessmanagerfactory.h"

#include "qmlapplicationviewer.h"

#include "xbmc/xbmc.h"
#include "xbmc/videoplayer.h"

#include <QMenuBar>
#include <QDeclarativeContext>
#include <QDeclarativeEngine>
#include <QProcess>
#include <QDebug>

#ifdef Q_WS_MAEMO_5
    #include <QtGui/QX11Info>
    #include <X11/Xlib.h>
    #include <X11/Xatom.h>
    #include <QtDBus/QDBusConnection>
#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{

#ifdef Q_WS_MAEMO_5
    setAttribute(Qt::WA_Maemo5AutoOrientation, true);

    QDBusConnection::systemBus().connect(QString(), "/com/nokia/csd/call", "com.nokia.csd.Call", "Coming", this, SLOT(callEvent(QDBusObjectPath,QString)));
    QDBusConnection::systemBus().connect(QString(), "/com/nokia/csd/call", "com.nokia.csd.Call", "Created", this, SLOT(callEvent(QDBusObjectPath,QString)));

    grabZoomKeys(true);
#endif

    viewer = new QmlApplicationViewer;
    setCentralWidget(viewer);


    QMenuBar *menuBar = new QMenuBar();
    QMenu *menu = menuBar->addMenu("Xbmc");
    menu->addAction("Connect...", this, SLOT(openConnectDialog()));
    menu->addAction("Settings", this, SLOT(openSettingsDialog()));

    QAction *quitAction = menu->addAction("Quit xbmc", Xbmc::instance(), SLOT(quit()));
    Xbmc::instance()->connect(Xbmc::instance(), SIGNAL(connectedChanged(bool)), quitAction, SLOT(setEnabled(bool)));

    QMenu *helpMenu = menuBar->addMenu("Help");
    helpMenu->addAction("About", this, SLOT(openAboutDialog()));

    setMenuBar(menuBar);

    m_settings = new Settings(this);
    viewer->rootContext()->setContextProperty("settings", m_settings);
    viewer->rootContext()->setContextProperty("xbmc", Xbmc::instance());
    viewer->setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer->setMainQmlFile(QLatin1String("qml/xbmcremote/fremantle/main.qml"));
    viewer->engine()->setNetworkAccessManagerFactory(new NetworkAccessManagerFactory());

}

MainWindow::~MainWindow()
{
#ifdef Q_WS_MAEMO_5
    grabZoomKeys(false);
#endif
}

void MainWindow::openSettingsDialog()
{
    SettingsDialog *settings = new SettingsDialog();
    settings->exec();
}

void MainWindow::openConnectDialog()
{
    ConnectDialog *settings = new ConnectDialog();
    settings->exec();
}

void MainWindow::openAboutDialog()
{
    AboutDialog *about = new AboutDialog();
    about->exec();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_F7:
        Xbmc::instance()->setVolume(Xbmc::instance()->volume() + 5);
        event->accept();
        break;

    case Qt::Key_F8:
        Xbmc::instance()->setVolume(Xbmc::instance()->volume() - 5);
        event->accept();
        break;
    }
    QWidget::keyPressEvent(event);
}

#ifdef Q_WS_MAEMO_5
void MainWindow::grabZoomKeys(bool grab) {
    if (!winId()) {
        qWarning("Can't grab keys unless we have a window id");
        return;
    }

    unsigned long val = (grab) ? 1 : 0;
    Atom atom = XInternAtom(QX11Info::display(), "_HILDON_ZOOM_KEY_ATOM", False);
    if (!atom) {
        qWarning("Unable to obtain _HILDON_ZOOM_KEY_ATOM. This example will only work "
                 "on a Maemo 5 device!");
        return;
    }

    XChangeProperty (QX11Info::display(),
             winId(),
             atom,
             XA_INTEGER,
             32,
             PropModeReplace,
             reinterpret_cast<unsigned char *>(&val),
             1);
}

void MainWindow::callEvent(const QDBusObjectPath &param1, const QString &param2)
{
    qDebug() << "phone call event" << param1.path() << param2;
    Settings settings;
    if(settings.changeVolumeOnCall()) {
        Xbmc::instance()->dimVolumeTo(settings.volumeOnCall());
    }

    if(settings.pauseOnCall() && Xbmc::instance()->videoPlayer()->state() == "playing") {
        Xbmc::instance()->videoPlayer()->playPause();
    }

    QDBusConnection::systemBus().connect(QString(), param1.path(), "com.nokia.csd.Call.Instance", "Terminated", this, SLOT(callTerminated()));

}

void MainWindow::callTerminated()
{
    Settings settings;
    if(settings.changeVolumeOnCall()) {
        Xbmc::instance()->restoreVolume();
    }

    if(settings.pauseOnCall() && Xbmc::instance()->videoPlayer()->state() != "playing") {
        Xbmc::instance()->videoPlayer()->playPause();
    }
}
#endif
