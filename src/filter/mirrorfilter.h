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

#ifndef MIRRORFILTER_H
#define MIRRORFILTER_H

#include "qtimagefilter.h"

class MirrorFilter : public QtImageFilter {
public:
    enum MirrorFilterOption {
        MirrorHorizontal = UserOption,
        MirrorVertical
    };

    MirrorFilter()
    {
        m_mirrorVertical = false;
        m_mirrorHorizontal = true;
    }

    QImage apply(const QImage &img, const QRect& clipRect /*= QRect()*/ ) const
    {
        Q_UNUSED(clipRect);
        return img.mirrored(m_mirrorHorizontal, m_mirrorVertical);
    }

    QString name() const { return QString::fromLocal8Bit("Зеркало"); }

    QVariant option(int filteroption) const
    {
        switch ((MirrorFilterOption)filteroption) {
        case MirrorVertical:
            return QVariant(m_mirrorVertical);
        break;
        case MirrorHorizontal:
            return QVariant(m_mirrorHorizontal);
        break;
        default:
            break;
        }
        return QVariant();
    }

    bool setOption(int filteroption, const QVariant &value)
    {
        switch ((MirrorFilterOption)filteroption) {
            case MirrorVertical:
                m_mirrorVertical = value.toBool();
            break;
            case MirrorHorizontal:
                m_mirrorHorizontal = value.toBool();
            break;
            default:
                break;
        }
        return true;
    }

    bool supportsOption(int option) const
    {
        bool supports = false;
        switch ((MirrorFilterOption)option) {
            case MirrorVertical:
            case MirrorHorizontal:
                supports = true;
            break;
            default:
                break;
        }
        return supports;
    }

    QString description() const { return QObject::tr("A simple mirroring filter."); }

private:
    bool m_mirrorVertical;
    bool m_mirrorHorizontal;
};
#endif  /* MIRRORFILTER_H */
