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
#include "debughelper.h"
#include "filecommon.h"

#include <QDesktopServices>
#include <QDesktopWidget> /// moved to center
#include <QUrl>
#include <QFileDialog>
#include <QStringListModel>

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

    debug();
}
//------------------------------------------------------------------------------
MainWindow::~MainWindow()
{
    typeModel->deleteLater();
    delete ui;
}
//------------------------------------------------------------------------------
void MainWindow::debug()
{
    QString t_str;
    t_str = "/home/warmonger/Pictures/wallp";
    ui->LEInputFolder->setText(t_str);
    loadListFilesToTable();
}
//------------------------------------------------------------------------------
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

    // central widget
    connect(ui->pBGenerate, SIGNAL(clicked()), SLOT(generateImages()));
    connect(ui->pBInputFolder, SIGNAL(clicked()), SLOT(setInputFolder()));
    connect(ui->pBOutputFolder, SIGNAL(clicked()), SLOT(setOutputFolder()));

    // menu about
    connect(ui->action_About_About, SIGNAL(triggered()), about, SLOT(show()));
    connect(ui->action_About_About_Qt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(ui->action_About_Site, SIGNAL(triggered()), this, SLOT(aboutOpenSite()));
}
//------------------------------------------------------------------------------
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
//------------------------------------------------------------------------------
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
//------------------------------------------------------------------------------
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
//------------------------------------------------------------------------------
void MainWindow::setLangEn()
{
    ui->action_Settings_Language_Deutsch ->setChecked(false);
    ui->action_Settings_Language_Russian ->setChecked(false);
    ui->action_Settings_Language_France ->setChecked(false);
    ui->action_Settings_Language_English ->setChecked(true);
}

//------------------------------------------------------------------------------
void MainWindow::setLangRu()
{
    ui->action_Settings_Language_Deutsch ->setChecked(false);
    ui->action_Settings_Language_Russian ->setChecked(true);
    ui->action_Settings_Language_France ->setChecked(false);
    ui->action_Settings_Language_English ->setChecked(false);
}
//------------------------------------------------------------------------------
void MainWindow::setLangDe()
{
    ui->action_Settings_Language_Deutsch ->setChecked(true);
    ui->action_Settings_Language_Russian ->setChecked(false);
    ui->action_Settings_Language_France ->setChecked(false);
    ui->action_Settings_Language_English ->setChecked(false);
}
//------------------------------------------------------------------------------
void MainWindow::setLangFr()
{
    ui->action_Settings_Language_Deutsch ->setChecked(false);
    ui->action_Settings_Language_Russian ->setChecked(false);
    ui->action_Settings_Language_France ->setChecked(true);
    ui->action_Settings_Language_English ->setChecked(false);
}
//------------------------------------------------------------------------------
void MainWindow::aboutOpenSite()
{
    QDesktopServices::openUrl(QUrl(GL_WEB_SITE));
}
//------------------------------------------------------------------------------
void MainWindow::generateImages()
{
    myDebug() << "begin generate";
    for (int i = 0; i < typeModel->rowCount(); i++)
    {
        myDebug() << typeModel->data(typeModel->index(i, 0), 0).toString();
    }
}
//------------------------------------------------------------------------------
void MainWindow::setInputFolder()
{
    QFileDialog::Options options = QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly;
    QString directory = QFileDialog::getExistingDirectory(this
                                                          , tr("Select input folder")
                                                          , ""
                                                          , options);
    if (!directory.isEmpty())
    {
        ui->LEInputFolder->setText(directory);
        ui->LEOutputFolder->setText(directory + "/output/");
        loadListFilesToTable();
    }
}
//------------------------------------------------------------------------------
void MainWindow::setOutputFolder()
{
    QFileDialog::Options options = QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly;
    QString directory = QFileDialog::getExistingDirectory(this
                                                          , tr("Select output folder")
                                                          , ""
                                                          , options);
    if (!directory.isEmpty())
        ui->LEInputFolder->setText(directory);
}
//------------------------------------------------------------------------------
void MainWindow::loadListFilesToTable()
{
    QStringList t_list = recursiveFind(ui->LEInputFolder->text());
    QStringList items;
    for (int i = 0; i < t_list.size(); i++)
    {
        items << QString(t_list.at(i).split("/").last());
    }
    typeModel = new QStringListModel(items, this);
    ui->listFiles->setModel(typeModel);
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
