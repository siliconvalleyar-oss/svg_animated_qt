#pragma once
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsSvgItem>
#include <QSvgRenderer>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QTimer>
#include "AnimationEngine.h"

class SvgView : public QGraphicsView {
    Q_OBJECT
public:
    explicit SvgView(QWidget *parent = nullptr);

    bool loadSvgFile(const QString &filePath);
    bool loadSvgString(const QString &svgStr);
    QString currentSvgString() const { return m_svgString; }
    QString currentFilePath() const { return m_filePath; }

    double zoomLevel() const { return m_zoom; }
    void setZoom(double level);

    void applyAllAnimations(const QMap<int, ElementConfig> &configs);
    void stopAllAnimations();
    void pauseAnimations();
    void resumeAnimations();

    QList<QGraphicsItem *> svgElements() const { return m_elements; }
    QRectF svgBoundingRect() const { return m_svgBounds; }
    QString svgViewBox() const { return m_viewBox; }

    void setBoundaryVisible(bool visible);
    bool isBoundaryVisible() const { return m_boundaryVisible; }

    // Background images
    void setBackgroundImage(int index, const QPixmap &pixmap, const QRectF &rect, double opacity);
    void removeBackgroundImage(int index);
    void clearBackgroundImages();

    // Selection for pieces mode
    void setPiecesMode(bool enabled);
    bool isPiecesMode() const { return m_piecesMode; }
    void setSelectedElement(int index);
    int selectedElementIndex() const { return m_selectedIndex; }

signals:
    void zoomChanged(double level);
    void elementClicked(int index);
    void elementMoved(int index, QPointF delta);
    void svgLoaded(const QString &svgStr);

protected:
    void wheelEvent(QWheelEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void keyPressEvent(QKeyEvent *e) override;

private:
    void setupScene();
    void parseSvgElements();
    void updateBoundary();
    void updateBackgroundPositions();

    QGraphicsScene *m_scene;
    QGraphicsSvgItem *m_svgItem;
    QSvgRenderer *m_renderer;
    QGraphicsRectItem *m_boundaryRect;
    QList<QGraphicsItem *> m_elements;
    QMap<QGraphicsItem *, int> m_elementIndexMap;
    QRectF m_svgBounds;
    QString m_viewBox;
    QString m_svgString;
    QString m_filePath;

    double m_zoom = 1.0;
    bool m_boundaryVisible = false;
    bool m_piecesMode = false;
    int m_selectedIndex = -1;

    // Drag state for pieces mode
    bool m_dragging = false;
    QGraphicsItem *m_dragItem = nullptr;
    QPointF m_dragStart;
    QPointF m_dragItemStart;

    // Background images
    struct BgImage {
        QGraphicsPixmapItem *item = nullptr;
        int zOrder = 0;
    };
    QMap<int, BgImage> m_bgImages;
};
