#include "mainwindow.h"
#include "settingsdialog.h"
#include "connectdialog.h"
#include "aboutdialog.h"
#include "settings.h"
#include "networkaccessmanagerfactory.h"
#include "authenticationdialog.h"

#include "qmlapplicationviewer.h"

#include "xbmc/xbmc.h"
#include "xbmc/videoplayer.h"
#include "xbmc/audioplayer.h"

#include <QMenuBar>
#include <QDeclarativeContext>
#include <QDeclarativeEngine>
#include <QProcess>
#include <QDebug>
#include <QApplication>
#include <QDesktopWidget>

#ifdef Q_WS_MAEMO_5

#include <QtGui/QX11Info>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusMessage>
#include <QtContacts/QContactDetailFilter>
#include <QtContacts/QContactPhoneNumber>
#include <QtContacts/QContactManager>
QTM_USE_NAMESPACE

#endif

MainWindow::MainWindow(Settings *settings, QWidget *parent) :
    QMainWindow(parent),
    m_settings(settings),
    m_videoPaused(false),
    m_audioPaused(false)
{

#ifdef Q_WS_MAEMO_5
    setAttribute(Qt::WA_Maemo5AutoOrientation, true);

    QDBusConnection::systemBus().connect(QString(), "/com/nokia/csd/call", "com.nokia.csd.Call", "Coming", this, SLOT(callEvent(QDBusObjectPath,QString)));
    QDBusConnection::systemBus().connect(QString(), "/com/nokia/csd/call", "com.nokia.csd.Call", "Created", this, SLOT(callEvent(QDBusObjectPath,QString)));

    grabZoomKeys(true);

//    connect(QApplication::desktop(), SIGNAL(resized(int)), this, SLOT(orientationChanged()));
#endif

    viewer = new QmlApplicationViewer;
    setCentralWidget(viewer);


    QMenuBar *menuBar = new QMenuBar();
    QMenu *menu = menuBar->addMenu("Xbmc");
    menu->addAction(tr("Connect..."), this, SLOT(openConnectDialog()));
    menu->addAction(tr("Settings"), this, SLOT(openSettingsDialog()));

    QAction *quitAction = menu->addAction("Quit xbmc", Xbmc::instance(), SLOT(quit()));
    Xbmc::instance()->connect(Xbmc::instance(), SIGNAL(connectedChanged(bool)), quitAction, SLOT(setEnabled(bool)));

    QMenu *helpMenu = menuBar->addMenu("Help");
    helpMenu->addAction(tr("About"), this, SLOT(openAboutDialog()));

    setMenuBar(menuBar);

    viewer->rootContext()->setContextProperty("settings", m_settings);
    viewer->rootContext()->setContextProperty("xbmc", Xbmc::instance());
    viewer->setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer->setMainQmlFile(QLatin1String("qml/fremantle/main.qml"));
    viewer->engine()->setNetworkAccessManagerFactory(new NetworkAccessManagerFactory());

    connect(Xbmc::instance(), SIGNAL(authenticationRequired(QString,QString)), SLOT(authenticationRequired(QString,QString)), Qt::QueuedConnection);

    // Load stored hosts
    bool connecting = false;
    foreach(const XbmcHost &host, settings->hostList()) {
        int index = Xbmc::instance()->hostModel()->insertOrUpdateHost(host);
        if(host.address() == settings->lastHost().address()) {
            qDebug() << "reconnecting to" << host.hostname() << host.address() << host.username() << host.password();
            Xbmc::instance()->hostModel()->connectToHost(index);
            connecting = true;
        }
    }
    connect(Xbmc::instance(), SIGNAL(connectedChanged(bool)), SLOT(connectionChanged(bool)));
    connect(Xbmc::instance()->hostModel(), SIGNAL(rowsRemoved(QModelIndex, int, int)), SLOT(hostRemoved()));

    if(!connecting) {
        openConnectDialog();
    }
}

MainWindow::~MainWindow()
{
#ifdef Q_WS_MAEMO_5
    grabZoomKeys(false);
#endif
}

void MainWindow::orientationChanged(){
#ifdef Q_WS_MAEMO_5
    QRect screenGeometry = QApplication::desktop()->screenGeometry();
    if (screenGeometry.width() > screenGeometry.height()) {
        qDebug() << "In Landscape Mode";
        showFullScreen();
    } else {
        qDebug() << "In Portrait Mode";
        showNormal();
    }
//    if(m_itemDialog) {
//        m_itemDialog->rotate(m_orientation);
//    }
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

#ifdef Q_WS_MAEMO_5
void MainWindow::callEvent(const QDBusObjectPath &param1, const QString &param2)
{
    qDebug() << "phone call event" << param1.path() << param2;
    Settings settings;

    if(settings.showCallNotifications()) {

        QDBusMessage msg = QDBusMessage::createMethodCall("com.nokia.csd", param1.path(), "com.nokia.csd.Call.Instance", "GetStatus");
        QDBusMessage reply = QDBusConnection::systemBus().call(msg);
        qDebug() << "call status:" << reply.arguments();

        if(reply.arguments().first().toInt() == 3) {

            QContactDetailFilter phoneFilter;
            phoneFilter.setDetailDefinitionName(QContactPhoneNumber::DefinitionName, QContactPhoneNumber::FieldNumber);
            phoneFilter.setValue(param2.right(6));
            phoneFilter.setMatchFlags(QContactFilter::MatchContains);
            qDebug() << "search contact";
            QContactManager contactManager;

            QString caller;
            if(contactManager.contacts(phoneFilter).count() > 0) {
                caller = contactManager.contacts(phoneFilter).first().displayLabel();
            } else {
                caller = param2;
            }

            qDebug() << "got contact" << caller;
            Xbmc::instance()->sendNotification(tr("Incoming call"), caller);
        }
    }

    if(settings.changeVolumeOnCall()) {
        Xbmc::instance()->dimVolumeTo(settings.volumeOnCall());
        m_videoPaused = true;
    }

    if(settings.pauseVideoOnCall() && Xbmc::instance()->videoPlayer()->state() == "playing") {
        Xbmc::instance()->videoPlayer()->playPause();
        m_audioPaused = true;
    }

    if(settings.pauseMusicOnCall() && Xbmc::instance()->audioPlayer()->state() == "playing") {
        Xbmc::instance()->audioPlayer()->playPause();
    }

    QDBusConnection::systemBus().connect(QString(), param1.path(), "com.nokia.csd.Call.Instance", "Terminated", this, SLOT(callTerminated()));

}
#endif

void MainWindow::callTerminated()
{
    Settings settings;
    if(settings.changeVolumeOnCall()) {
        Xbmc::instance()->restoreVolume();
    }

    if(m_videoPaused && Xbmc::instance()->videoPlayer()->state() != "playing") {
        Xbmc::instance()->videoPlayer()->playPause();
    }
    if(m_audioPaused && Xbmc::instance()->audioPlayer()->state() != "playing") {
        Xbmc::instance()->audioPlayer()->playPause();
    }
}

#endif

void MainWindow::authenticationRequired(const QString &hostname, const QString &address)
{
    Q_UNUSED(address);
    AuthenticationDialog *dialog = new AuthenticationDialog(hostname, this);
    dialog->exec();
}

void MainWindow::connectionChanged(bool connected)
{
    if(connected) {
        m_settings->addHost(*Xbmc::instance()->connectedHost());
        m_settings->setLastHost(*Xbmc::instance()->connectedHost());
    }

}

void MainWindow::hostRemoved()
{
    // We need to check if all our stored hosts are still in hostList
    for(int i = 0; i < m_settings->hostList().count();) {
        bool found = false;
        for(int j = 0; j < Xbmc::instance()->hostModel()->rowCount(QModelIndex()); ++j) {
            if(m_settings->hostList().at(i).address() == Xbmc::instance()->hostModel()->get(j, "address").toString()) {
                found = true;
                break;
            }
        }
        if(!found) {
            m_settings->removeHost(m_settings->hostList().at(i));
            qDebug() << "removed host" << i;
        } else {
            ++i;
        }
    }
}
