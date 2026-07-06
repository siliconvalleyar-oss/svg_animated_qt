#pragma once
#include <QObject>
#include <QGraphicsItem>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>
#include <QGraphicsDropShadowEffect>
#include <QMap>
#include <QVariant>
#include <QtMath>

struct AnimationPreset {
    QString id;
    QString name;
    QColor color;
    double defaultDuration;
    QString easing;
};

struct ElementConfig {
    QString presetId;
    QList<QString> extraPresets;
    double speed = 1.0;
    double delay = 0.0;
    QString iter = "infinite";
    QString dir = "normal";
    double ovalRx = 80, ovalRy = 40, ovalAngle = 0;
    double arcRx = 80, arcRy = 80;
    double directionAngle = 0;
    double pivotX = -1, pivotY = -1;
    QString trajectoryId;
};

class AnimationEngine : public QObject {
    Q_OBJECT
public:
    explicit AnimationEngine(QObject *parent = nullptr);

    static const QVector<AnimationPreset> &presets();
    static const AnimationPreset *presetById(const QString &id);
    static ElementConfig defaultConfig();

    void applyAnimation(QGraphicsItem *item, const ElementConfig &cfg,
                        const QRectF &boundingRect);
    void applyAllAnimations(const QMap<int, QGraphicsItem *> &items,
                            const QMap<int, ElementConfig> &configs,
                            const QMap<int, QRectF> &boundingRects);
    void stopAllAnimations(QGraphicsItem *item);
    void pauseAllAnimations(QGraphicsItem *item);
    void resumeAllAnimations(QGraphicsItem *item);
    void stopAll(const QMap<int, QGraphicsItem *> &items);

    bool isTranslateBased(const QString &id) const;
    bool isArcPreset(const QString &id) const;

    QList<QPropertyAnimation *> animationsForItem(QGraphicsItem *item) const;

signals:
    void animationApplied(int elementIndex);

private:
    QParallelAnimationGroup *buildRotationAnim(QGraphicsItem *item, const ElementConfig &cfg, const QRectF &rect);
    QParallelAnimationGroup *buildWheelAnim(QGraphicsItem *item, const ElementConfig &cfg, const QRectF &rect);
    QParallelAnimationGroup *buildPulseAnim(QGraphicsItem *item, const ElementConfig &cfg, const QRectF &rect);
    QParallelAnimationGroup *buildBounceAnim(QGraphicsItem *item, const ElementConfig &cfg, const QRectF &rect);
    QParallelAnimationGroup *buildGravityAnim(QGraphicsItem *item, const ElementConfig &cfg, const QRectF &rect);
    QParallelAnimationGroup *buildSlideAnim(QGraphicsItem *item, const ElementConfig &cfg, const QRectF &rect);
    QParallelAnimationGroup *buildOvalAnim(QGraphicsItem *item, const ElementConfig &cfg, const QRectF &rect);
    QParallelAnimationGroup *buildFadeAnim(QGraphicsItem *item, const ElementConfig &cfg, const QRectF &rect);
    QParallelAnimationGroup *buildDrawAnim(QGraphicsItem *item, const ElementConfig &cfg, const QRectF &rect);
    QParallelAnimationGroup *buildShakeAnim(QGraphicsItem *item, const ElementConfig &cfg, const QRectF &rect);
    QParallelAnimationGroup *buildFloatAnim(QGraphicsItem *item, const ElementConfig &cfg, const QRectF &rect);
    QParallelAnimationGroup *buildLevitateAnim(QGraphicsItem *item, const ElementConfig &cfg, const QRectF &rect);
    QParallelAnimationGroup *buildArcAnim(QGraphicsItem *item, const ElementConfig &cfg, const QRectF &rect);
    QParallelAnimationGroup *buildRadiateAnim(QGraphicsItem *item, const ElementConfig &cfg, const QRectF &rect);
    QParallelAnimationGroup *buildSpinAnim(QGraphicsItem *item, const ElementConfig &cfg, const QRectF &rect);
    QParallelAnimationGroup *buildGlowAnim(QGraphicsItem *item, const ElementConfig &cfg, const QRectF &rect);
    QParallelAnimationGroup *buildWaveSineAnim(QGraphicsItem *item, const ElementConfig &cfg, const QRectF &rect);
    QParallelAnimationGroup *buildWaveSquareAnim(QGraphicsItem *item, const ElementConfig &cfg, const QRectF &rect);
    QParallelAnimationGroup *buildWaveTriangleAnim(QGraphicsItem *item, const ElementConfig &cfg, const QRectF &rect);

    QMap<QGraphicsItem *, QParallelAnimationGroup *> m_animations;
    QMap<QGraphicsItem *, QGraphicsDropShadowEffect *> m_effects;
};
