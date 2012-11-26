#include <QtGui/QApplication>
#include "mainwindow.h"

#include <QTextCodec>
#include <QTranslator>
#include <QLocale>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec * codec = QTextCodec::codecForName("UTF-8"); // set encoding for progs
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForTr(codec);

    QTranslator translator;
    translator.load(""+QLocale::system().name(),"lang");
    a.installTranslator(&translator);

    MainWindow w;
    w.show();

    return a.exec();
}
