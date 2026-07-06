#pragma once
#include <QString>
#include <QVector>
#include <QPair>

namespace Shapes {
    struct Shape {
        QString name;
        QString svg;
    };

    const QVector<Shape> &all();
    QString svgByName(const QString &name);
}
