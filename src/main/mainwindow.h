#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include "imagetoolwidget.h"

namespace Ui {
    class MainWindow;
}

QT_BEGIN_NAMESPACE
class AboutDialog;
class QAction;
class QMenu;
class QStringListModel;
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    //tray
    void showHide(QSystemTrayIcon::ActivationReason);

    // language
    void setLangRu();
    void setLangFr();
    void setLangDe();
    void setLangEn();

    void aboutOpenSite();

    /**
     * @brief generateImages
     */
    void generateImages();
    /**
     * @brief setInputFolder
     */
    void setInputFolder();
    /**
     * @brief setOutputFolder
     */
    void setOutputFolder();

    void loadImageForPreviewFilter(QModelIndex ind);

private:
    Ui::MainWindow *ui;

    //about
    AboutDialog *about;    // about dialog

    QStringList m_listFiles;

    //tray
    QSystemTrayIcon *trIcon;
    QMenu *trayIconMenu;
    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;
    QAction *quitAction;

    ImageToolWidget *GUI_Filters;

    void createConnects();
    void createTrayIcon();
    void createActions();

    void loadListFilesToTable();

    void debug();

    QStringListModel *typeModel;

};

#endif // MAINWINDOW_H
