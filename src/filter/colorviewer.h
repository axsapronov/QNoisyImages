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

#ifndef COLORVIEWER_H
#define COLORVIEWER_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>

class PaintArea;

class ColorViewer : public QWidget
{
    Q_OBJECT

public:
    ColorViewer(QWidget *parent = 0);
    ~ColorViewer();


    void setPen(const QPen &pen);
    QPen pen() const;

    void setColor(const QColor &color);
    QColor color() const;



    QColor actualColor;


public slots:
    void changeColor(const QColor &color);

protected:
    void paintEvent(QPaintEvent *event);
        PaintArea *paintArea;

private:

    QPen actualPen;
    QBrush actualBrush;





};

#endif // COLORVIEWER_H
