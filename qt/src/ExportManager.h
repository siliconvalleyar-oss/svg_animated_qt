#pragma once
#include <QObject>
#include <QString>
#include <QMap>
#include "AnimationEngine.h"

class ExportManager : public QObject {
    Q_OBJECT
public:
    explicit ExportManager(QObject *parent = nullptr);

    QString exportSvg(const QString &originalSvg,
                      const QMap<int, ElementConfig> &configs);

    bool saveToFile(const QString &svgContent, const QString &filePath);

private:
    QString generateKeyframes(const QString &presetId, int elemIndex, double angle);
    QString baseKeyframeName(const QString &id);
    QString baseKeyframes(const QString &id);
};
