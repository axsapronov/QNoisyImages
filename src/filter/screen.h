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

#ifndef SCREEN_H
#define SCREEN_H

#include <QWidget>
#include <QLabel>
#include <QMouseEvent>

class PaintArea;

class screen : public QWidget
{

    Q_OBJECT

public:
    screen();

private:
    QLabel *label;
    PaintArea *paintArea;

protected:
    virtual void mousePressEvent(QMouseEvent *event);

signals:
    void colorSelected(QColor color);
    void colorSelectedBack(QColor color);
};

#endif // SCREEN_H
