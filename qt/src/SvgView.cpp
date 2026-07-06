#include "SvgView.h"
#include <QFile>
#include <QXmlStreamReader>
#include <QScrollBar>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>

SvgView::SvgView(QWidget *parent)
    : QGraphicsView(parent)
    , m_scene(new QGraphicsScene(this))
    , m_svgItem(nullptr)
    , m_renderer(new QSvgRenderer(this))
    , m_boundaryRect(nullptr)
{
    setScene(m_scene);
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    setDragMode(NoDrag);
    setViewportUpdateMode(FullViewportUpdate);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setStyleSheet("background: transparent; border: none;");
    setAcceptDrops(true);

    setupScene();
}

void SvgView::setupScene() {
    m_scene->setBackgroundBrush(QColor(15, 17, 23));
    // Grid pattern background
    QPixmap gridPixmap(24, 24);
    gridPixmap.fill(Qt::transparent);
    QPainter painter(&gridPixmap);
    painter.setPen(QPen(QColor(46, 50, 69), 1));
    painter.drawPoint(0, 0);
    m_scene->setBackgroundBrush(QBrush(gridPixmap));
}

bool SvgView::loadSvgFile(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return false;
    QString content = file.readAll();
    file.close();
    m_filePath = filePath;
    return loadSvgString(content);
}

bool SvgView::loadSvgString(const QString &svgStr) {
    stopAllAnimations();
    m_scene->clear();
    m_elements.clear();
    m_elementIndexMap.clear();
    m_boundaryRect = nullptr;
    m_svgItem = nullptr;
    m_selectedIndex = -1;

    if (!m_renderer->load(svgStr.toUtf8())) return false;

    m_svgString = svgStr;
    m_svgItem = new QGraphicsSvgItem;
    m_svgItem->setSharedRenderer(m_renderer);
    m_svgItem->setFlags(QGraphicsItem::ItemClipsToShape);
    m_scene->addItem(m_svgItem);

    m_svgBounds = m_renderer->viewBoxF();
    m_viewBox = QString("%1 %2 %3 %4").arg(m_svgBounds.x()).arg(m_svgBounds.y())
                                       .arg(m_svgBounds.width()).arg(m_svgBounds.height());

    parseSvgElements();

    // Fit in view
    fitInView(m_svgItem, Qt::KeepAspectRatio);
    m_zoom = transform().m11();
    emit zoomChanged(m_zoom);

    if (m_boundaryVisible) updateBoundary();

    emit svgLoaded(svgStr);
    return true;
}

void SvgView::parseSvgElements() {
    m_elements.clear();
    m_elementIndexMap.clear();

    QXmlStreamReader xml(m_svgString);
    QList<QGraphicsItem *> stack;
    int elemIdx = 0;

    // We need to find graphical elements in the SVG
    // Since QGraphicsSvgItem renders the whole SVG, we need to identify elements by traversing the XML
    // and mapping to rendered items. For simplicity, we'll parse and create invisible markers.
    
    // Alternative: iterate through scene items
    for (auto *item : m_scene->items()) {
        if (item == m_svgItem || item == m_boundaryRect) continue;
        // Background items
        bool isBg = false;
        for (auto it = m_bgImages.constBegin(); it != m_bgImages.constEnd(); ++it) {
            if (it.value().item == item) { isBg = true; break; }
        }
        if (isBg) continue;

        m_elementIndexMap[item] = elemIdx;
        m_elements.append(item);
        elemIdx++;
    }

    // If no child items found (common with QGraphicsSvgItem), 
    // create virtual elements by parsing XML
    if (m_elements.isEmpty()) {
        QXmlStreamReader xml(m_svgString);
        int idx = 0;
        while (!xml.atEnd()) {
            xml.readNext();
            if (xml.isStartElement()) {
                QString tag = xml.name().toString().toLower();
                if (tag == "circle" || tag == "rect" || tag == "ellipse" ||
                    tag == "path" || tag == "line" || tag == "polyline" ||
                    tag == "polygon" || tag == "g" || tag == "text") {
                    // Create a proxy graphics item for this element
                    auto *proxy = m_scene->addRect(0, 0, 1, 1, QPen(Qt::transparent), QBrush(Qt::transparent));
                    proxy->setData(0, idx);
                    proxy->setFlag(QGraphicsItem::ItemHasNoContents, true);
                    m_elements.append(proxy);
                    m_elementIndexMap[proxy] = idx;
                    idx++;
                }
            }
        }
    }
}

void SvgView::setZoom(double level) {
    m_zoom = qBound(0.2, level, 5.0);
    QTransform t;
    t.scale(m_zoom, m_zoom);
    setTransform(t);
    emit zoomChanged(m_zoom);
    if (m_boundaryVisible) updateBoundary();
}

void SvgView::wheelEvent(QWheelEvent *e) {
    double factor = (e->angleDelta().y() > 0) ? 1.1 : 1.0 / 1.1;
    setZoom(m_zoom * factor);
}

void SvgView::mousePressEvent(QMouseEvent *e) {
    if (m_piecesMode && e->button() == Qt::LeftButton) {
        QGraphicsItem *item = itemAt(e->pos());
        if (item && m_elementIndexMap.contains(item)) {
            int idx = m_elementIndexMap[item];
            setSelectedElement(idx);
            m_dragging = true;
            m_dragItem = item;
            m_dragStart = e->pos();
            m_dragItemStart = item->pos();
            e->accept();
            return;
        }
    }
    QGraphicsView::mousePressEvent(e);
}

void SvgView::mouseMoveEvent(QMouseEvent *e) {
    if (m_dragging && m_dragItem) {
        QPointF delta = mapToScene(e->pos()) - mapToScene(m_dragStart.toPoint());
        m_dragItem->setPos(m_dragItemStart + delta);
        e->accept();
        return;
    }
    QGraphicsView::mouseMoveEvent(e);
}

void SvgView::mouseReleaseEvent(QMouseEvent *e) {
    if (m_dragging) {
        m_dragging = false;
        if (m_dragItem && m_elementIndexMap.contains(m_dragItem)) {
            int idx = m_elementIndexMap[m_dragItem];
            QPointF delta = m_dragItem->pos() - m_dragItemStart;
            emit elementMoved(idx, delta);
        }
        m_dragItem = nullptr;
        e->accept();
        return;
    }
    QGraphicsView::mouseReleaseEvent(e);
}

void SvgView::keyPressEvent(QKeyEvent *e) {
    if (e->key() == Qt::Key_Escape && m_piecesMode) {
        setSelectedElement(-1);
        e->accept();
        return;
    }
    QGraphicsView::keyPressEvent(e);
}

void SvgView::applyAllAnimations(const QMap<int, ElementConfig> &configs) {
    QMap<int, QRectF> bounds;
    for (auto it = m_elementIndexMap.constBegin(); it != m_elementIndexMap.constEnd(); ++it) {
        bounds[it.value()] = it.key()->boundingRect();
    }
    AnimationEngine engine;
    engine.applyAllAnimations(m_elementIndexMap.keys().isEmpty() ? QMap<int, QGraphicsItem *>() :
        [&]() { QMap<int, QGraphicsItem *> r; for (auto it = m_elementIndexMap.constBegin(); it != m_elementIndexMap.constEnd(); ++it) r[it.value()] = it.key(); return r; }(),
        configs, bounds);
}

void SvgView::stopAllAnimations() {
    for (auto it = m_elementIndexMap.constBegin(); it != m_elementIndexMap.constEnd(); ++it) {
        it.key()->setGraphicsEffect(nullptr);
    }
}

void SvgView::pauseAnimations() {}
void SvgView::resumeAnimations() {}

void SvgView::setBoundaryVisible(bool visible) {
    m_boundaryVisible = visible;
    updateBoundary();
}

void SvgView::updateBoundary() {
    if (m_boundaryRect) {
        m_scene->removeItem(m_boundaryRect);
        delete m_boundaryRect;
        m_boundaryRect = nullptr;
    }
    if (!m_boundaryVisible || !m_svgItem) return;

    QRectF bounds = m_svgItem->boundingRect();
    m_boundaryRect = m_scene->addRect(bounds, QPen(QColor(255, 255, 255, 128), 1, Qt::DashLine), QBrush(Qt::transparent));
    m_boundaryRect->setZValue(100);
}

void SvgView::setBackgroundImage(int index, const QPixmap &pixmap, const QRectF &rect, double opacity) {
    if (m_bgImages.contains(index)) {
        auto &bg = m_bgImages[index];
        bg.item->setPixmap(pixmap);
        bg.item->setPos(rect.topLeft());
        bg.item->setScale(1.0);
        bg.item->setOpacity(opacity);
        return;
    }
    auto *pi = m_scene->addPixmap(pixmap);
    pi->setPos(rect.topLeft());
    pi->setOpacity(opacity);
    pi->setZValue(-1);
    m_bgImages[index] = { pi, index };
}

void SvgView::removeBackgroundImage(int index) {
    auto it = m_bgImages.find(index);
    if (it != m_bgImages.end()) {
        m_scene->removeItem(it.value().item);
        delete it.value().item;
        m_bgImages.erase(it);
    }
}

void SvgView::clearBackgroundImages() {
    for (auto it = m_bgImages.constBegin(); it != m_bgImages.constEnd(); ++it) {
        m_scene->removeItem(it.value().item);
        delete it.value().item;
    }
    m_bgImages.clear();
}

void SvgView::setPiecesMode(bool enabled) {
    m_piecesMode = enabled;
    if (!enabled) {
        setSelectedElement(-1);
        setDragMode(NoDrag);
    } else {
        setDragMode(NoDrag);
    }
}

void SvgView::setSelectedElement(int index) {
    m_selectedIndex = index;
    // Visual feedback - iterate through elements
    for (auto it = m_elementIndexMap.constBegin(); it != m_elementIndexMap.constEnd(); ++it) {
        if (it.value() == index) {
            it.key()->setOpacity(1.0);
        } else if (index >= 0) {
            it.key()->setOpacity(0.3);
        } else {
            it.key()->setOpacity(1.0);
        }
    }
    emit elementClicked(index);
}

void SvgView::updateBackgroundPositions() {
    for (auto it = m_bgImages.constBegin(); it != m_bgImages.constEnd(); ++it) {
        Q_UNUSED(it)
    }
}
