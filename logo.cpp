/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     rekols <rekols@foxmail.com>
 *
 * Maintainer: rekols <rekols@foxmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "logo.h"

#include <QPaintEvent>
#include <QPainter>
#include <QDebug>

#include <unistd.h>

Logo::Logo(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(90, 90);
    setFilePath(":/logo.png");
}

void Logo::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);

    QPainterPath painterPath;
    painterPath.addEllipse(QRect(0, 0, width(), height()));

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setClipPath(painterPath);

    painter.drawPixmap(e->rect(), m_appLogoPixmap);
}

void Logo::setFilePath(const QString &filePath)
{
    const auto ratio = devicePixelRatioF();

    QString logoPath = filePath;

    QUrl url(filePath);
    if (!QFile(url.toLocalFile()).exists()) {
        url = QUrl(filePath);
    }

    m_appLogoPixmap = QPixmap(url.toLocalFile()).scaled(size() * ratio, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    m_appLogoPixmap.setDevicePixelRatio(ratio);

    update();
}
