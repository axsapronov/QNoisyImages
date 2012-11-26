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

#include <QtGui>
#include <QtCore/QVector>
#include "qtimagefilter.h"

#include "ui_filterdialog.h"

class ImageToolWidget : public QWidget {
    Q_OBJECT
public:
    ImageToolWidget(QWidget *parent = 0);
    virtual ~ImageToolWidget();
    void setPixmapAndResize(const QPixmap &pixmap);
    QImage imgToFilter;

    QPixmap mypixmap;

public slots:
    void loadImage();
    void reloadImage();
    void filterImage();
    void acept();
    void filterIndexChanged(int index);

private:
    QString m_currentFilename;
    QVector<QtImageFilter*>  m_imageFilters;
    Ui::Form ui;

};

