#pragma once
#include <QObject>
#include <QMap>
#include <QPixmap>
#include <QPointF>
#include <QSize>

struct BackgroundImage {
    QString id;
    QString name;
    QPixmap pixmap;
    QPointF position;
    QSize size;
    double opacity = 0.8;
    bool hidden = false;
    int zOrder = 0;
};

class BackgroundImageManager : public QObject {
    Q_OBJECT
public:
    explicit BackgroundImageManager(QObject *parent = nullptr);

    QString addImage(const QString &filePath);
    void removeImage(const QString &id);
    void setImageProperty(const QString &id, const QString &prop, const QVariant &value);
    void moveImage(const QString &id, const QString &direction);

    BackgroundImage *image(const QString &id);
    QList<BackgroundImage *> allImages();

signals:
    void imageAdded(const QString &id);
    void imageRemoved(const QString &id);
    void imageModified(const QString &id);

private:
    QMap<QString, BackgroundImage> m_images;
    int m_nextZ = 0;
};
