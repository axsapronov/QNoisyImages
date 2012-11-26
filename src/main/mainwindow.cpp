/*
  The basis for creating an application
  (QMainwindow)

  Tray + translate + about dialog + mainwindow

  Author: Sapronov Alexander
  E-mail: sapronov.alexander92@gmail.com
  */

#include "ui_mainwindow.h" /// ui

#include "defines.h" /// defines
#include "mainwindow.h" ///
#include "about.h" /// aboutdialog

#include <QDesktopServices>
#include <QDesktopWidget> /// moved to center
#include <QUrl>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    about(new AboutDialog(this))
{
    ui->setupUi(this);

    createActions(); // create action for tray
    createTrayIcon(); // add actionts to tray menu
    createConnects(); // moved func
    trIcon->show();  //display tray

    /// moved to center desktop
        QRect rect = QApplication::desktop()->availableGeometry(this);
        this->move(rect.width() / 2 - this->width() / 2,
                   rect.height() / 2 - this->height() / 2);
    /// maximized
//    this->showMaximized();
}

MainWindow::~MainWindow()
{
    delete ui;
}
//---------------------------------------
void MainWindow::createConnects()
{
    //tray
    connect(trIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(showHide(QSystemTrayIcon::ActivationReason)));

    // menu file
    connect(ui->action_File_Close, SIGNAL(triggered()), this, SLOT(close()));

    // menu settings

        //menu settings -> lang
    connect(ui->action_Settings_Language_Russian, SIGNAL(triggered()), this, SLOT(setLangRu()) );
    connect(ui->action_Settings_Language_English, SIGNAL(triggered()), this, SLOT(setLangEn()) );
    connect(ui->action_Settings_Language_Deutsch, SIGNAL(triggered()), this, SLOT(setLangDe()) );
    connect(ui->action_Settings_Language_France, SIGNAL(triggered()), this, SLOT(setLangFr()) );

    // menu search


    // menu about
    connect(ui->action_About_About, SIGNAL(triggered()), about, SLOT(show()));
    connect(ui->action_About_About_Qt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(ui->action_About_Site, SIGNAL(triggered()), this, SLOT(aboutOpenSite()));
}
//------------------------------------------------------_
void MainWindow::showHide(QSystemTrayIcon::ActivationReason r)
{
    if (r == QSystemTrayIcon::Trigger)
    {
        if ( !this->isVisible() )
        {
            this->show();
        }
        else
        {
            this->hide();
        }
    }
}
//------------------------------------------------------_
void MainWindow::createTrayIcon()
{
    trIcon = new QSystemTrayIcon();  //init
    trIcon->setIcon(QIcon(":/images/img.png"));  //set ico

    trayIconMenu = new QMenu(this);  // create menu
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(maximizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trIcon->setContextMenu(trayIconMenu); //set menu
}
//-------------------------------------------------------
void MainWindow::createActions()
{
    minimizeAction = new QAction(tr("&Hide"), this);
    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

    maximizeAction = new QAction(tr("&Expand"), this);
    connect(maximizeAction, SIGNAL(triggered()), this, SLOT(showMaximized()));

    restoreAction = new QAction(tr("&Restore"), this);
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

    quitAction = new QAction(tr("Q&uit"), this);
    connect(quitAction, SIGNAL(triggered()), this, SLOT(close()));
}
//-------------------------------------------------------
void MainWindow::setLangEn()
{
    ui->action_Settings_Language_Deutsch ->setChecked(false);
    ui->action_Settings_Language_Russian ->setChecked(false);
    ui->action_Settings_Language_France ->setChecked(false);
    ui->action_Settings_Language_English ->setChecked(true);
}

//-------------------------------------------------
void MainWindow::setLangRu()
{
    ui->action_Settings_Language_Deutsch ->setChecked(false);
    ui->action_Settings_Language_Russian ->setChecked(true);
    ui->action_Settings_Language_France ->setChecked(false);
    ui->action_Settings_Language_English ->setChecked(false);
}
//-------------------------------------------------
void MainWindow::setLangDe()
{
    ui->action_Settings_Language_Deutsch ->setChecked(true);
    ui->action_Settings_Language_Russian ->setChecked(false);
    ui->action_Settings_Language_France ->setChecked(false);
    ui->action_Settings_Language_English ->setChecked(false);
}
//-------------------------------------------------
void MainWindow::setLangFr()
{
    ui->action_Settings_Language_Deutsch ->setChecked(false);
    ui->action_Settings_Language_Russian ->setChecked(false);
    ui->action_Settings_Language_France ->setChecked(true);
    ui->action_Settings_Language_English ->setChecked(false);
}
//-------------------------------------------------
void MainWindow::aboutOpenSite()
{
    QDesktopServices::openUrl(QUrl(GL_WEB_SITE));
}
//--------------------------------------------------
