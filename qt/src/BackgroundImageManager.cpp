#include "BackgroundImageManager.h"
#include <QFileInfo>

BackgroundImageManager::BackgroundImageManager(QObject *parent) : QObject(parent) {}

QString BackgroundImageManager::addImage(const QString &filePath) {
    QPixmap pix(filePath);
    if (pix.isNull()) return QString();

    QString id = QString("bg_%1").arg(QDateTime::currentMSecsSinceEpoch());
    BackgroundImage img;
    img.id = id;
    img.name = QFileInfo(filePath).fileName();
    img.pixmap = pix;
    img.position = QPointF(50, 50);
    img.size = pix.size() / 2;
    img.zOrder = m_nextZ++;

    m_images[id] = img;
    emit imageAdded(id);
    return id;
}

void BackgroundImageManager::removeImage(const QString &id) {
    if (!m_images.contains(id)) return;
    m_images.remove(id);
    emit imageRemoved(id);
}

void BackgroundImageManager::setImageProperty(const QString &id, const QString &prop, const QVariant &value) {
    if (!m_images.contains(id)) return;
    auto &img = m_images[id];
    if (prop == "opacity") img.opacity = value.toDouble();
    else if (prop == "hidden") img.hidden = value.toBool();
    else if (prop == "x") img.position.setX(value.toDouble());
    else if (prop == "y") img.position.setY(value.toDouble());
    else if (prop == "width") img.size.setWidth(value.toInt());
    else if (prop == "height") img.size.setHeight(value.toInt());
    else if (prop == "zOrder") img.zOrder = value.toInt();
    emit imageModified(id);
}

void BackgroundImageManager::moveImage(const QString &id, const QString &direction) {
    if (!m_images.contains(id)) return;
    auto &img = m_images[id];

    QList<BackgroundImage *> sorted;
    for (auto it = m_images.begin(); it != m_images.end(); ++it)
        sorted.append(&it.value());
    std::sort(sorted.begin(), sorted.end(), [](const BackgroundImage *a, const BackgroundImage *b) {
        return a->zOrder < b->zOrder;
    });

    int idx = -1;
    for (int i = 0; i < sorted.size(); i++) {
        if (sorted[i]->id == id) { idx = i; break; }
    }
    if (idx == -1) return;

    if (direction == "up" && idx < sorted.size() - 1) {
        std::swap(sorted[idx]->zOrder, sorted[idx + 1]->zOrder);
    } else if (direction == "down" && idx > 0) {
        std::swap(sorted[idx]->zOrder, sorted[idx - 1]->zOrder);
    } else if (direction == "front") {
        img.zOrder = m_nextZ++;
    } else if (direction == "back") {
        for (auto &s : sorted) if (s->id != id) s->zOrder++;
    }

    emit imageModified(id);
}

BackgroundImage *BackgroundImageManager::image(const QString &id) {
    return m_images.contains(id) ? &m_images[id] : nullptr;
}

QList<BackgroundImage *> BackgroundImageManager::allImages() {
    QList<BackgroundImage *> result;
    for (auto it = m_images.begin(); it != m_images.end(); ++it)
        result.append(&it.value());
    return result;
}
