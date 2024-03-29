/*!
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *
 *    Authors: Alexander Sapronov/WarmongeR
 *    E-Mail: warmonger72@gmail.com
 *
 *    Andrey Grienko
 *    skype: grin1010
 *
 *    Years: 2010-2011
 */


#include <QtGui/QImageReader>
#include <QtGui/QFileDialog>
#include <QtGui/QImage>
#include <QtGui/QMessageBox>

#include "imagetoolwidget.h"
#include "ui_imagetoolwidget.h"

#include "qtimagefilter.h"
#include "mirrorfilter.h"

#include "debughelper.h"

ImageToolWidget::ImageToolWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageToolWidget)
{
    ui->setupUi(this);

    // Register our mirror filter.
    qtRegisterImageFilter<MirrorFilter>(QLatin1String("Фильтры"));

    // Iterate through all registered filters and append them to the vector
    QStringList filters = QtImageFilterFactory::imageFilterList();
    for (int i = 0; i < filters.count(); ++i) {
        m_imageFilters += QtImageFilterFactory::createImageFilter(filters[i]);
    }

    for (int i = 0; i < m_imageFilters.count(); ++i) {
        ui->FiltersCombo->addItem(m_imageFilters[i]->name());
    }

    ui->FiltersCombo->insertItem(0, QString(tr("Выберите фильтр")));

    m_imageFilters.prepend((QtImageFilter*)0);

    //connect(ui->LoadButton, SIGNAL(clicked()), this, SLOT(loadImage()));
    connect(ui->ReloadButton, SIGNAL(clicked()), this, SLOT(reloadImage()));
    connect(ui->FilterButton, SIGNAL(clicked()), this, SLOT(filterImage()));
    connect(ui->FiltersCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(filterIndexChanged(int)));

    //    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(acept()));
    //    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(close()));


    ui->FiltersCombo->setCurrentIndex(0);
    m_currentFilename = QLatin1String("images/qtlogo.png");
    reloadImage();
    //    setWindowTitle(tr("Фильтры - %1").arg(APP_NAME_AND_VER));
    setWindowTitle("Filters");


}
//------------------------------------------------------------------------------
ImageToolWidget::~ImageToolWidget()
{
    delete ui;
}
//------------------------------------------------------------------------------
void ImageToolWidget::loadImage()
{
    QList<QByteArray> formats = QImageReader::supportedImageFormats();
    QString strFormats;
    int i;
    for (i = 0; i < formats.size(); i++)
    {
        if (i != 0) strFormats+=" ";
        strFormats += QString("*.") + formats.at(i);
    }
    strFormats = "Images (" + strFormats + ")";
    QString s = QFileDialog::getOpenFileName
            (
                this,
                "Choose a file",
                QString(),
                strFormats
                );
    QImage img;
    if(img.load(s))
    {
        m_currentFilename = s;
        QPixmap pixmap = QPixmap::fromImage(img);
        setPixmapAndResize(pixmap);
    }
}
//------------------------------------------------------------------------------
void ImageToolWidget::reloadImage()
{
    /*  QImage img;
    if(img.load(m_currentFilename)) {
        QPixmap pixmap = QPixmap::fromImage(img);
        setPixmapAndResize(pixmap);
    }*/

    setPixmapAndResize(m_backuppixmap);
}
//------------------------------------------------------------------------------
void ImageToolWidget::filterIndexChanged(int index)
{
    if (index == 0 || index >= m_imageFilters.count())
    {
        ui->FilterButton->setToolTip(QLatin1String("No image filter chosen"));
        ui->FilterButton->setEnabled(false);
        ui->gbBorderPolicy->setVisible( false );
        ui->gbChannels->setVisible( false );
        ui->gbMirror->setVisible( false );
    }
    else
    {
        QtImageFilter *filter = m_imageFilters[index];
        ui->FilterButton->setToolTip(filter->description());
        ui->FilterButton->setEnabled(true);
        ui->gbBorderPolicy->setVisible( filter->supportsOption(QtImageFilter::FilterBorderPolicy)  );
        ui->gbChannels->setVisible( filter->supportsOption(QtImageFilter::FilterChannels)  );
        ui->gbMirror->setVisible( filter->supportsOption(MirrorFilter::MirrorHorizontal) || filter->supportsOption(MirrorFilter::MirrorVertical) );
    }
}
//------------------------------------------------------------------------------
void ImageToolWidget::filterImage()
{
    if (ui->PixmapLabel->pixmap() == 0)
    {
        QMessageBox::information(this, "QImageTool", "Sorry, you must load an image first\n");
    }
    else
    {
        setCursor(Qt::WaitCursor);
        // QImage imgToFilter = ui->PixmapLabel->pixmap()->toImage();
        imgToFilter = ui->PixmapLabel->pixmap()->toImage();
        QtImageFilter *filter = m_imageFilters[ui->FiltersCombo->currentIndex()];
        if (filter->name() == "Punch")
        {
            filter->setOption(QtImageFilter::Radius, qMin(imgToFilter.width(), imgToFilter.height())/2);
            filter->setOption(QtImageFilter::Center, QPointF(imgToFilter.width()/2.0,imgToFilter.height()/2.0));
            filter->setOption(QtImageFilter::Force, 0.5);
        }
        else if (filter->name() == "ConvolutionFilter")
        {
            // A simple mean filter just to demonstrate that we can add our own kernels.
            static int kernelElements[9] =
            {    1,  1,  1,
                 1,  1,  1,
                 1,  1,  1 };
            QtConvolutionKernelMatrix kernel(kernelElements, 3, 3);
            QVariant value;
            qVariantSetValue<QtConvolutionKernelMatrix>(value, kernel);
            filter->setOption(QtImageFilter::ConvolutionKernelMatrix, value);

        }
        if (filter->supportsOption(MirrorFilter::MirrorHorizontal))
            filter->setOption(MirrorFilter::MirrorHorizontal, ui->ckHorizontal->isChecked() );
        if (filter->supportsOption(MirrorFilter::MirrorVertical))
            filter->setOption(MirrorFilter::MirrorVertical, ui->ckVertical->isChecked());

        if (filter->supportsOption(QtImageFilter::FilterChannels))
        {
            QString rgba = ui->ckRed->isChecked() ? "r" : "";
            rgba+= ui->ckGreen->isChecked() ? "g" : "";
            rgba+= ui->ckBlue->isChecked() ? "b" : "";
            rgba+= ui->ckAlpha->isChecked() ? "a" : "";
            filter->setOption(QtImageFilter::FilterChannels, rgba);
        }

        if (filter->supportsOption(QtImageFilter::FilterBorderPolicy))
        {
            QString borderPolicy;
            if (ui->rbExtend->isChecked()) borderPolicy = "Extend";
            else if (ui->rbMirror->isChecked()) borderPolicy = "Mirror";
            else borderPolicy = "Wrap";
            filter->setOption(QtImageFilter::FilterBorderPolicy, borderPolicy);
        }

        imgToFilter = filter->apply(imgToFilter);
        setCursor(Qt::ArrowCursor);
        setPixmapAndResize(QPixmap::fromImage(imgToFilter));
        // setPixmapAndResize(QPixmap::fromImage(paintArea->theImage));
    }
}
//------------------------------------------------------------------------------
void ImageToolWidget::setPixmapAndResize(const QPixmap &pixmap)
{
    ui->PixmapLabel->setPixmap(pixmap);
    ui->PixmapLabel->resize(pixmap.size());
    m_pixmap = pixmap;
    m_backuppixmap = pixmap;
}
//------------------------------------------------------------------------------
void ImageToolWidget::acept()
{
    close();
}
//------------------------------------------------------------------------------
void ImageToolWidget::filterListImages(QStringList *listImages, QString *outputPath)
{
//    myDebug() << *outputPath << listImages->size();

    QDir dir(*outputPath);
    dir.mkpath(*outputPath);

    QProgressDialog trainProgress("", "It's not Cancel", 0, 100);
    trainProgress.setValue(0);
    trainProgress.setGeometry(750, 300, 400, 170);
    trainProgress.show();

    QLabel overallLabel(&trainProgress);
    overallLabel.setGeometry(11, 10, 378, 20);
    overallLabel.setText("Operation...");
    overallLabel.show();


    for(int i = 0; i < listImages->size(); i++)
    {

        trainProgress.setValue(100 * i / listImages->size());
        QApplication::processEvents();

        QString t_last = listImages->at(i).split("/").last();
        QPixmap pixmap;
        pixmap.load(listImages->at(i));

        setPixmapAndResize(pixmap);
        filterImage();

        //        myDebug() << *outputPath + t_last;
        m_pixmap.save(*outputPath + t_last);
    }
}
//------------------------------------------------------------------------------
