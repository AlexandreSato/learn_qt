// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "analogclock.h"

#include <QPainter>
#include <QTime>
#include <QTimer>


AnalogClock::AnalogClock(QWidget *parent)
    : QWidget(parent)
{
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&AnalogClock::update)); // timeout update
    timer->start(1000);

    setWindowTitle(tr("My Analog Clock"));
    resize(800, 800);
}


void AnalogClock::paintEvent(QPaintEvent *)
{
    static const QPoint hourHand[3] = { // triangle
        QPoint(7, 8),
        QPoint(-7, 8),
        QPoint(0, -70)
    };
    static const QPoint minuteHand[3] = { // triangle
        QPoint(7, 8),
        QPoint(-7, 8),
        QPoint(0, -90)
    };

    QColor hourColor(127, 0, 127);
    QColor minuteColor(0, 127, 127, 191); // translucid

    int side = qMin(width(), height());

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); // smooth edges
    painter.translate(width() / 2, height() / 2); // center in the square
    painter.scale(side / 200.0, side / 200.0); // oval?


    painter.setPen(Qt::NoPen);
    painter.setBrush(hourColor);


    QTime time = QTime::currentTime();

// draw hour pointer
    painter.save(); // save matrix transformation
    painter.rotate(30.0 * ((time.hour() + time.minute() / 60.0)));
    painter.drawConvexPolygon(hourHand, 3);
    painter.restore(); // restores transformation matrix

// draw hour markers
    painter.setPen(hourColor);
    for (int i = 0; i < 12; ++i) {
        painter.drawLine(88, 0, 96, 0);
        painter.rotate(30.0);
    }

// draw minute pointer
    painter.setPen(Qt::NoPen);
    painter.setBrush(minuteColor);

    painter.save(); // save matrix
    painter.rotate(6.0 * (time.minute() + time.second() / 60.0));
    painter.drawConvexPolygon(minuteHand, 3);
    painter.restore(); // restore matrix transformation

// draw minutes markers
    painter.setPen(minuteColor);
    for (int j = 0; j < 60; ++j) {
        if ((j % 5) != 0)
            painter.drawLine(92, 0, 96, 0);
        painter.rotate(6.0);
    }
}
