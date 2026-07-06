#include "AnimationEngine.h"
#include <QGraphicsOpacityEffect>
#include <QEasingCurve>

static const QVector<AnimationPreset> PRESETS = {
    { "rotate",       "Rotar",       QColor("#6c5ce7"), 2.0,   "linear" },
    { "wheel",        "Rueda",       QColor("#e74c3c"), 3.0,   "linear" },
    { "pulse",        "Pulsar",      QColor("#e67e22"), 1.5,   "ease-in-out" },
    { "bounce",       "Rebotar",     QColor("#2ecc71"), 0.8,   "ease-in-out" },
    { "gravity",      "Gravedad",    QColor("#1abc9c"), 1.5,   "ease-out" },
    { "slide",        "Deslizar",    QColor("#f39c12"), 2.0,   "ease-in-out" },
    { "oval",         "Ovalo",       QColor("#9b59b6"), 3.0,   "linear" },
    { "fade",         "Desvanecer",  QColor("#e74c3c"), 2.0,   "ease-in-out" },
    { "draw",         "Dibujar",     QColor("#1abc9c"), 2.0,   "ease-in-out" },
    { "shake",        "Temblar",     QColor("#e67e22"), 0.5,   "ease-in-out" },
    { "float",        "Flotar",      QColor("#9b59b6"), 3.0,   "ease-in-out" },
    { "levitate",     "Levitar",     QColor("#1abc9c"), 3.5,   "ease-in-out" },
    { "arc",          "Arco",        QColor("#f39c12"), 4.0,   "ease-in-out" },
    { "radiate",      "Radiar",      QColor("#e67e22"), 4.0,   "ease-in-out" },
    { "spin",         "Girar",       QColor("#3498db"), 1.2,   "ease-in-out" },
    { "glow",         "Brillar",     QColor("#e74c3c"), 2.0,   "ease-in-out" },
    { "wave-sine",    "Senoidal",    QColor("#1abc9c"), 3.0,   "ease-in-out" },
    { "wave-square",  "Cuadrada",    QColor("#e74c3c"), 1.5,   "step-end" },
    { "wave-triangle","Triangular",  QColor("#9b59b6"), 2.0,   "linear" },
};

AnimationEngine::AnimationEngine(QObject *parent) : QObject(parent) {}

const QVector<AnimationPreset> &AnimationEngine::presets() { return PRESETS; }

const AnimationPreset *AnimationEngine::presetById(const QString &id) {
    for (const auto &p : PRESETS) {
        if (p.id == id) return &p;
    }
    return nullptr;
}

ElementConfig AnimationEngine::defaultConfig() {
    return ElementConfig{};
}

bool AnimationEngine::isTranslateBased(const QString &id) const {
    static const QStringList ids = { "slide","bounce","shake","float","gravity","levitate","arc","radiate","wave-sine","wave-square","wave-triangle" };
    return ids.contains(id);
}

bool AnimationEngine::isArcPreset(const QString &id) const {
    return id == "arc" || id == "radiate";
}

QList<QPropertyAnimation *> AnimationEngine::animationsForItem(QGraphicsItem *item) const {
    QList<QPropertyAnimation *> result;
    auto it = m_animations.constFind(item);
    if (it != m_animations.constEnd()) {
        for (auto *anim : it.value()->children()) {
            if (auto *pa = qobject_cast<QPropertyAnimation *>(anim))
                result.append(pa);
        }
    }
    return result;
}

static QEasingCurve parseEasing(const QString &name) {
    if (name == "linear") return QEasingCurve::Linear;
    if (name == "ease-in") return QEasingCurve::InQuad;
    if (name == "ease-out") return QEasingCurve::OutQuad;
    if (name == "ease-in-out") return QEasingCurve::InOutQuad;
    if (name == "step-end") return QEasingCurve::Steps(1, QEasingCurve::StepEnd);
    return QEasingCurve::InOutQuad;
}

static QPropertyAnimation *makeAnim(QObject *target, const QByteArray &prop,
                                     int durationMs, const QEasingCurve &easing,
                                     const QVariant &start, const QVariant &end,
                                     int loopCount) {
    auto *a = new QPropertyAnimation(target, prop);
    a->setDuration(durationMs);
    a->setEasingCurve(easing);
    a->setStartValue(start);
    a->setKeyValueAt(0.5, end);
    a->setEndValue(start);
    a->setLoopCount(loopCount);
    return a;
}

static int loopCount(const QString &iter) {
    if (iter == "infinite") return -1;
    bool ok;
    int v = iter.toInt(&ok);
    return ok && v > 0 ? v : 1;
}

static int durationMs(double seconds) { return static_cast<int>(seconds * 1000); }

// Helper: animate QGraphicsItem position via x() and y() properties
static QPropertyAnimation *makeTranslateXAnim(QGraphicsItem *item, int dur, const QEasingCurve &e, double fromPx, double toPx, int loops) {
    auto *a = new QPropertyAnimation(item, "x");
    a->setDuration(dur);
    a->setEasingCurve(e);
    a->setStartValue(item->x() + fromPx);
    a->setKeyValueAt(0.5, item->x() + toPx);
    a->setEndValue(item->x() + fromPx);
    a->setLoopCount(loops);
    return a;
}

static QPropertyAnimation *makeTranslateYAnim(QGraphicsItem *item, int dur, const QEasingCurve &e, double fromPx, double toPx, int loops) {
    auto *a = new QPropertyAnimation(item, "y");
    a->setDuration(dur);
    a->setEasingCurve(e);
    a->setStartValue(item->y() + fromPx);
    a->setKeyValueAt(0.5, item->y() + toPx);
    a->setEndValue(item->y() + fromPx);
    a->setLoopCount(loops);
    return a;
}

// Build rotation animation around item center
QParallelAnimationGroup *AnimationEngine::buildRotationAnim(QGraphicsItem *item, const ElementConfig &cfg, const QRectF &rect) {
    Q_UNUSED(rect)
    auto *group = new QParallelAnimationGroup(this);
    int dur = durationMs(cfg.speed);
    int loops = loopCount(cfg.iter);
    auto ec = parseEasing("linear");

    // Use rotation property
    auto *a = new QPropertyAnimation(item, "rotation");
    a->setDuration(dur);
    a->setEasingCurve(ec);
    a->setStartValue(0.0);
    a->setEndValue(360.0);
    a->setLoopCount(loops);
    group->addAnimation(a);
    return group;
}

QParallelAnimationGroup *AnimationEngine::buildWheelAnim(QGraphicsItem *item, const ElementConfig &cfg, const QRectF &rect) {
    Q_UNUSED(rect)
    auto *group = new QParallelAnimationGroup(this);
    int dur = durationMs(cfg.speed);
    int loops = loopCount(cfg.iter);

    auto *a = new QPropertyAnimation(item, "rotation");
    a->setDuration(dur);
    a->setEasingCurve(QEasingCurve::Linear);
    a->setStartValue(0.0);
    a->setKeyValueAt(0.25, 90.0);
    a->setKeyValueAt(0.5, 180.0);
    a->setKeyValueAt(0.75, 270.0);
    a->setEndValue(360.0);
    a->setLoopCount(loops);
    group->addAnimation(a);
    return group;
}

QParallelAnimationGroup *AnimationEngine::buildPulseAnim(QGraphicsItem *item, const ElementConfig &cfg, const QRectF &rect) {
    Q_UNUSED(rect)
    auto *group = new QParallelAnimationGroup(this);
    int dur = durationMs(cfg.speed);
    int loops = loopCount(cfg.iter);
    auto ec = parseEasing(cfg.easing.isEmpty() ? "ease-in-out" : cfg.easing);

    auto *a = new QPropertyAnimation(item, "scale");
    a->setDuration(dur);
    a->setEasingCurve(ec);
    a->setStartValue(1.0);
    a->setKeyValueAt(0.5, 1.15);
    a->setEndValue(1.0);
    a->setLoopCount(loops);
    group->addAnimation(a);
    return group;
}

QParallelAnimationGroup *AnimationEngine::buildBounceAnim(QGraphicsItem *item, const ElementConfig &cfg, const QRectF &rect) {
    Q_UNUSED(rect)
    auto *group = new QParallelAnimationGroup(this);
    int dur = durationMs(cfg.speed);
    int loops = loopCount(cfg.iter);
    auto ec = parseEasing(cfg.easing.isEmpty() ? "ease-in-out" : cfg.easing);

    double baseY = item->y();
    double angle = cfg.directionAngle * M_PI / 180.0;
    double dx = 20.0 * qCos(angle);
    double dy = 20.0 * qSin(angle);

    auto *ax = new QPropertyAnimation(item, "x");
    ax->setDuration(dur); ax->setEasingCurve(ec); ax->setLoopCount(loops);
    ax->setStartValue(baseY != -1 ? item->x() : item->x());
    ax->setStartValue(item->x());
    ax->setKeyValueAt(0.5, item->x() + dx);
    ax->setEndValue(item->x());
    group->addAnimation(ax);

    auto *ay = new QPropertyAnimation(item, "y");
    ay->setDuration(dur); ay->setEasingCurve(ec); ay->setLoopCount(loops);
    ay->setStartValue(item->y());
    ay->setKeyValueAt(0.5, item->y() + dy);
    ay->setEndValue(item->y());
    group->addAnimation(ay);

    return group;
}

QParallelAnimationGroup *AnimationEngine::buildGravityAnim(QGraphicsItem *item, const ElementConfig &cfg, const QRectF &rect) {
    Q_UNUSED(rect)
    auto *group = new QParallelAnimationGroup(this);
    int dur = durationMs(cfg.speed);
    int loops = loopCount(cfg.iter);
    double angle = cfg.directionAngle * M_PI / 180.0;
    double cosA = qCos(angle), sinA = qSin(angle);

    auto *ay = new QPropertyAnimation(item, "y");
    ay->setDuration(dur);
    ay->setEasingCurve(QEasingCurve::OutQuad);
    ay->setLoopCount(loops);
    ay->setStartValue(item->y() + (-100.0 * sinA));
    ay->setKeyValueAt(0.3, item->y() + (80.0 * sinA));
    ay->setKeyValueAt(0.5, item->y() + (-40.0 * sinA));
    ay->setKeyValueAt(0.7, item->y() + (30.0 * sinA));
    ay->setKeyValueAt(0.85, item->y() + (-10.0 * sinA));
    ay->setEndValue(item->y());
    group->addAnimation(ay);

    auto *ax = new QPropertyAnimation(item, "x");
    ax->setDuration(dur);
    ax->setEasingCurve(QEasingCurve::OutQuad);
    ax->setLoopCount(loops);
    ax->setStartValue(item->x() + (-100.0 * cosA));
    ax->setKeyValueAt(0.3, item->x() + (80.0 * cosA));
    ax->setKeyValueAt(0.5, item->x() + (-40.0 * cosA));
    ax->setKeyValueAt(0.7, item->x() + (30.0 * cosA));
    ax->setKeyValueAt(0.85, item->x() + (-10.0 * cosA));
    ax->setEndValue(item->x());
    group->addAnimation(ax);

    return group;
}

QParallelAnimationGroup *AnimationEngine::buildSlideAnim(QGraphicsItem *item, const ElementConfig &cfg, const QRectF &rect) {
    Q_UNUSED(rect)
    auto *group = new QParallelAnimationGroup(this);
    int dur = durationMs(cfg.speed);
    int loops = loopCount(cfg.iter);
    auto ec = parseEasing(cfg.easing.isEmpty() ? "ease-in-out" : cfg.easing);
    double angle = cfg.directionAngle * M_PI / 180.0;
    double cosA = qCos(angle), sinA = qSin(angle);

    auto *ax = new QPropertyAnimation(item, "x");
    ax->setDuration(dur); ax->setEasingCurve(ec); ax->setLoopCount(loops);
    ax->setStartValue(item->x() + (-80.0 * cosA));
    ax->setKeyValueAt(0.5, item->x() + (80.0 * cosA));
    ax->setEndValue(item->x() + (-80.0 * cosA));
    group->addAnimation(ax);

    auto *ay = new QPropertyAnimation(item, "y");
    ay->setDuration(dur); ay->setEasingCurve(ec); ay->setLoopCount(loops);
    ay->setStartValue(item->y() + (-80.0 * sinA));
    ay->setKeyValueAt(0.5, item->y() + (80.0 * sinA));
    ay->setEndValue(item->y() + (-80.0 * sinA));
    group->addAnimation(ay);

    return group;
}

QParallelAnimationGroup *AnimationEngine::buildOvalAnim(QGraphicsItem *item, const ElementConfig &cfg, const QRectF &rect) {
    Q_UNUSED(rect)
    auto *group = new QParallelAnimationGroup(this);
    int dur = durationMs(cfg.speed);
    int loops = loopCount(cfg.iter);

    double rx = cfg.ovalRx;
    double ry = cfg.ovalRy;
    double baseX = item->x();
    double baseY = item->y();

    auto *ax = new QPropertyAnimation(item, "x");
    ax->setDuration(dur); ax->setEasingCurve(QEasingCurve::Linear); ax->setLoopCount(loops);
    ax->setStartValue(baseX);
    ax->setKeyValueAt(0.25, baseX + rx);
    ax->setKeyValueAt(0.5, baseX);
    ax->setKeyValueAt(0.75, baseX - rx);
    ax->setEndValue(baseX);
    group->addAnimation(ax);

    auto *ay = new QPropertyAnimation(item, "y");
    ay->setDuration(dur); ay->setEasingCurve(QEasingCurve::Linear); ay->setLoopCount(loops);
    ay->setStartValue(baseY);
    ay->setKeyValueAt(0.25, baseY);
    ay->setKeyValueAt(0.5, baseY + ry);
    ay->setKeyValueAt(0.75, baseY);
    ay->setEndValue(baseY);
    group->addAnimation(ay);

    return group;
}

QParallelAnimationGroup *AnimationEngine::buildFadeAnim(QGraphicsItem *item, const ElementConfig &cfg, const QRectF &rect) {
    Q_UNUSED(rect)
    auto *group = new QParallelAnimationGroup(this);
    int dur = durationMs(cfg.speed);
    int loops = loopCount(cfg.iter);
    auto ec = parseEasing(cfg.easing.isEmpty() ? "ease-in-out" : cfg.easing);

    auto *effect = new QGraphicsOpacityEffect(item);
    item->setGraphicsEffect(effect);

    auto *a = new QPropertyAnimation(effect, "opacity");
    a->setDuration(dur);
    a->setEasingCurve(ec);
    a->setStartValue(1.0);
    a->setKeyValueAt(0.5, 0.15);
    a->setEndValue(1.0);
    a->setLoopCount(loops);
    group->addAnimation(a);
    return group;
}

QParallelAnimationGroup *AnimationEngine::buildDrawAnim(QGraphicsItem *item, const ElementConfig &cfg, const QRectF &rect) {
    Q_UNUSED(rect)
    auto *group = new QParallelAnimationGroup(this);
    int dur = durationMs(cfg.speed);
    int loops = loopCount(cfg.iter);
    auto ec = parseEasing(cfg.easing.isEmpty() ? "ease-in-out" : cfg.easing);

    // Simulate draw with opacity flash (true stroke-dashoffset needs SVG-specific handling)
    auto *effect = new QGraphicsOpacityEffect(item);
    item->setGraphicsEffect(effect);

    auto *a = new QPropertyAnimation(effect, "opacity");
    a->setDuration(dur);
    a->setEasingCurve(ec);
    a->setStartValue(0.0);
    a->setEndValue(1.0);
    a->setLoopCount(loops);
    group->addAnimation(a);
    return group;
}

QParallelAnimationGroup *AnimationEngine::buildShakeAnim(QGraphicsItem *item, const ElementConfig &cfg, const QRectF &rect) {
    Q_UNUSED(rect)
    auto *group = new QParallelAnimationGroup(this);
    int dur = durationMs(cfg.speed);
    int loops = loopCount(cfg.iter);
    double angle = cfg.directionAngle * M_PI / 180.0;
    double cosA = qCos(angle), sinA = qSin(angle);

    auto *ax = new QPropertyAnimation(item, "x");
    ax->setDuration(dur); ax->setEasingCurve(QEasingCurve::Linear); ax->setLoopCount(loops);
    double baseX = item->x();
    double amp = 8.0;
    ax->setStartValue(baseX);
    ax->setKeyValueAt(0.1, baseX - amp * cosA);
    ax->setKeyValueAt(0.2, baseX + amp * cosA);
    ax->setKeyValueAt(0.3, baseX - amp * cosA);
    ax->setKeyValueAt(0.4, baseX + amp * cosA);
    ax->setKeyValueAt(0.5, baseX - amp * cosA);
    ax->setKeyValueAt(0.6, baseX + amp * cosA);
    ax->setKeyValueAt(0.7, baseX - amp * cosA);
    ax->setKeyValueAt(0.8, baseX + amp * cosA);
    ax->setKeyValueAt(0.9, baseX - amp * cosA);
    ax->setEndValue(baseX);
    group->addAnimation(ax);

    auto *ay = new QPropertyAnimation(item, "y");
    ay->setDuration(dur); ay->setEasingCurve(QEasingCurve::Linear); ay->setLoopCount(loops);
    double baseY = item->y();
    ay->setStartValue(baseY);
    ay->setKeyValueAt(0.1, baseY - amp * sinA);
    ay->setKeyValueAt(0.2, baseY + amp * sinA);
    ay->setKeyValueAt(0.3, baseY - amp * sinA);
    ay->setKeyValueAt(0.4, baseY + amp * sinA);
    ay->setKeyValueAt(0.5, baseY - amp * sinA);
    ay->setKeyValueAt(0.6, baseY + amp * sinA);
    ay->setKeyValueAt(0.7, baseY - amp * sinA);
    ay->setKeyValueAt(0.8, baseY + amp * sinA);
    ay->setKeyValueAt(0.9, baseY - amp * sinA);
    ay->setEndValue(baseY);
    group->addAnimation(ay);

    return group;
}

QParallelAnimationGroup *AnimationEngine::buildFloatAnim(QGraphicsItem *item, const ElementConfig &cfg, const QRectF &rect) {
    Q_UNUSED(rect)
    auto *group = new QParallelAnimationGroup(this);
    int dur = durationMs(cfg.speed);
    int loops = loopCount(cfg.iter);
    auto ec = parseEasing(cfg.easing.isEmpty() ? "ease-in-out" : cfg.easing);
    double angle = cfg.directionAngle * M_PI / 180.0;

    double dx = 15.0 * qCos(angle);
    double dy = 15.0 * qSin(angle);

    auto *ax = new QPropertyAnimation(item, "x");
    ax->setDuration(dur); ax->setEasingCurve(ec); ax->setLoopCount(loops);
    ax->setStartValue(item->x());
    ax->setKeyValueAt(0.5, item->x() + dx);
    ax->setEndValue(item->x());
    group->addAnimation(ax);

    auto *ay = new QPropertyAnimation(item, "y");
    ay->setDuration(dur); ay->setEasingCurve(ec); ay->setLoopCount(loops);
    ay->setStartValue(item->y());
    ay->setKeyValueAt(0.5, item->y() + dy);
    ay->setEndValue(item->y());
    group->addAnimation(ay);

    return group;
}

QParallelAnimationGroup *AnimationEngine::buildLevitateAnim(QGraphicsItem *item, const ElementConfig &cfg, const QRectF &rect) {
    Q_UNUSED(rect)
    auto *group = new QParallelAnimationGroup(this);
    int dur = durationMs(cfg.speed);
    int loops = loopCount(cfg.iter);
    auto ec = parseEasing(cfg.easing.isEmpty() ? "ease-in-out" : cfg.easing);
    double angle = cfg.directionAngle * M_PI / 180.0;

    double dx = 25.0 * qCos(angle);
    double dy = 25.0 * qSin(angle);

    auto *ax = new QPropertyAnimation(item, "x");
    ax->setDuration(dur); ax->setEasingCurve(ec); ax->setLoopCount(loops);
    ax->setStartValue(item->x());
    ax->setKeyValueAt(0.25, item->x() + dx * 0.48);
    ax->setKeyValueAt(0.5, item->x() + dx);
    ax->setKeyValueAt(0.75, item->x() + dx * 0.48);
    ax->setEndValue(item->x());
    group->addAnimation(ax);

    auto *ay = new QPropertyAnimation(item, "y");
    ay->setDuration(dur); ay->setEasingCurve(ec); ay->setLoopCount(loops);
    ay->setStartValue(item->y());
    ay->setKeyValueAt(0.25, item->y() + dy * 0.48);
    ay->setKeyValueAt(0.5, item->y() + dy);
    ay->setKeyValueAt(0.75, item->y() + dy * 0.48);
    ay->setEndValue(item->y());
    group->addAnimation(ay);

    auto *as = new QPropertyAnimation(item, "scale");
    as->setDuration(dur); as->setEasingCurve(ec); as->setLoopCount(loops);
    as->setStartValue(1.0);
    as->setKeyValueAt(0.25, 1.02);
    as->setKeyValueAt(0.5, 0.98);
    as->setKeyValueAt(0.75, 1.01);
    as->setEndValue(1.0);
    group->addAnimation(as);

    return group;
}

QParallelAnimationGroup *AnimationEngine::buildArcAnim(QGraphicsItem *item, const ElementConfig &cfg, const QRectF &rect) {
    Q_UNUSED(rect)
    auto *group = new QParallelAnimationGroup(this);
    int dur = durationMs(cfg.speed);
    int loops = loopCount(cfg.iter);
    double angle = cfg.directionAngle * M_PI / 180.0;
    double cosA = qCos(angle), sinA = qSin(angle);
    double rx = cfg.arcRx, ry = cfg.arcRy;

    double baseX = item->x();
    double baseY = item->y();
    double pivotOffX = (cfg.pivotX >= 0) ? (cfg.pivotX - rect.center().x()) : 0;
    double pivotOffY = (cfg.pivotY >= 0) ? (cfg.pivotY - rect.center().y()) : 0;

    auto *ax = new QPropertyAnimation(item, "x");
    ax->setDuration(dur); ax->setEasingCurve(QEasingCurve::Linear); ax->setLoopCount(loops);
    ax->setStartValue(baseX + pivotOffX - rx * cosA);
    ax->setKeyValueAt(0.25, baseX + pivotOffX - rx * 0.7071 * cosA + ry * 0.7071 * sinA);
    ax->setKeyValueAt(0.5, baseX + pivotOffX + ry * sinA);
    ax->setKeyValueAt(0.75, baseX + pivotOffX + rx * 0.7071 * cosA + ry * 0.7071 * sinA);
    ax->setEndValue(baseX + pivotOffX + rx * cosA);
    group->addAnimation(ax);

    auto *ay = new QPropertyAnimation(item, "y");
    ay->setDuration(dur); ay->setEasingCurve(QEasingCurve::Linear); ay->setLoopCount(loops);
    ay->setStartValue(baseY + pivotOffY - rx * sinA);
    ay->setKeyValueAt(0.25, baseY + pivotOffY - rx * 0.7071 * sinA - ry * 0.7071 * cosA);
    ay->setKeyValueAt(0.5, baseY + pivotOffY - ry * cosA);
    ay->setKeyValueAt(0.75, baseY + pivotOffY + rx * 0.7071 * sinA - ry * 0.7071 * cosA);
    ay->setEndValue(baseY + pivotOffY + rx * sinA);
    group->addAnimation(ay);

    return group;
}

QParallelAnimationGroup *AnimationEngine::buildRadiateAnim(QGraphicsItem *item, const ElementConfig &cfg, const QRectF &rect) {
    auto *group = buildArcAnim(item, cfg, rect);

    // Add glow effect pulsing
    auto *effect = new QGraphicsDropShadowEffect(item);
    effect->setBlurRadius(4);
    effect->setColor(QColor(230, 126, 34, 76));
    effect->setOffset(0, 0);
    item->setGraphicsEffect(effect);

    int dur = durationMs(cfg.speed);
    int loops = loopCount(cfg.iter);

    auto *glow = new QPropertyAnimation(effect, "blurRadius");
    glow->setDuration(dur);
    glow->setEasingCurve(QEasingCurve::InOutQuad);
    glow->setStartValue(2);
    glow->setKeyValueAt(0.5, 24);
    glow->setEndValue(2);
    glow->setLoopCount(loops);
    group->addAnimation(glow);

    return group;
}

QParallelAnimationGroup *AnimationEngine::buildSpinAnim(QGraphicsItem *item, const ElementConfig &cfg, const QRectF &rect) {
    Q_UNUSED(rect)
    auto *group = new QParallelAnimationGroup(this);
    int dur = durationMs(cfg.speed);
    int loops = loopCount(cfg.iter);
    auto ec = parseEasing(cfg.easing.isEmpty() ? "ease-in-out" : cfg.easing);

    auto *ar = new QPropertyAnimation(item, "rotation");
    ar->setDuration(dur); ar->setEasingCurve(ec); ar->setLoopCount(loops);
    ar->setStartValue(0.0);
    ar->setKeyValueAt(0.5, 180.0);
    ar->setEndValue(360.0);
    group->addAnimation(ar);

    auto *as = new QPropertyAnimation(item, "scale");
    as->setDuration(dur); as->setEasingCurve(ec); as->setLoopCount(loops);
    as->setStartValue(1.0);
    as->setKeyValueAt(0.5, 0.85);
    as->setEndValue(1.0);
    group->addAnimation(as);

    return group;
}

QParallelAnimationGroup *AnimationEngine::buildGlowAnim(QGraphicsItem *item, const ElementConfig &cfg, const QRectF &rect) {
    Q_UNUSED(rect)
    auto *group = new QParallelAnimationGroup(this);
    int dur = durationMs(cfg.speed);
    int loops = loopCount(cfg.iter);

    auto *effect = new QGraphicsDropShadowEffect(item);
    effect->setBlurRadius(4);
    effect->setColor(QColor(108, 92, 231, 76));
    effect->setOffset(0, 0);
    item->setGraphicsEffect(effect);

    auto *a = new QPropertyAnimation(effect, "blurRadius");
    a->setDuration(dur);
    a->setEasingCurve(QEasingCurve::InOutQuad);
    a->setStartValue(4);
    a->setKeyValueAt(0.5, 24);
    a->setEndValue(4);
    a->setLoopCount(loops);
    group->addAnimation(a);
    return group;
}

QParallelAnimationGroup *AnimationEngine::buildWaveSineAnim(QGraphicsItem *item, const ElementConfig &cfg, const QRectF &rect) {
    Q_UNUSED(rect)
    auto *group = new QParallelAnimationGroup(this);
    int dur = durationMs(cfg.speed);
    int loops = loopCount(cfg.iter);
    double angle = cfg.directionAngle * M_PI / 180.0;
    double cosA = qCos(angle), sinA = qSin(angle);
    double baseX = item->x(), baseY = item->y();

    auto *ax = new QPropertyAnimation(item, "x");
    ax->setDuration(dur); ax->setEasingCurve(QEasingCurve::Linear); ax->setLoopCount(loops);
    ax->setStartValue(baseX);
    ax->setKeyValueAt(0.25, baseX + 80*cosA - 40*sinA);
    ax->setKeyValueAt(0.5, baseX + 160*cosA);
    ax->setKeyValueAt(0.75, baseX + 240*cosA + 40*sinA);
    ax->setEndValue(baseX + 320*cosA);
    group->addAnimation(ax);

    auto *ay = new QPropertyAnimation(item, "y");
    ay->setDuration(dur); ay->setEasingCurve(QEasingCurve::Linear); ay->setLoopCount(loops);
    ay->setStartValue(baseY);
    ay->setKeyValueAt(0.25, baseY + 80*sinA + 40*cosA);
    ay->setKeyValueAt(0.5, baseY + 160*sinA);
    ay->setKeyValueAt(0.75, baseY + 240*sinA - 40*cosA);
    ay->setEndValue(baseY + 320*sinA);
    group->addAnimation(ay);

    return group;
}

QParallelAnimationGroup *AnimationEngine::buildWaveSquareAnim(QGraphicsItem *item, const ElementConfig &cfg, const QRectF &rect) {
    Q_UNUSED(rect)
    auto *group = new QParallelAnimationGroup(this);
    int dur = durationMs(cfg.speed);
    int loops = loopCount(cfg.iter);
    double angle = cfg.directionAngle * M_PI / 180.0;
    double cosA = qCos(angle), sinA = qSin(angle);

    auto *ax = new QPropertyAnimation(item, "x");
    ax->setDuration(dur); ax->setEasingCurve(QEasingCurve::Steps(1)); ax->setLoopCount(loops);
    ax->setStartValue(item->x());
    ax->setKeyValueAt(0.49, item->x());
    ax->setKeyValueAt(0.5, item->x() + 80*cosA);
    ax->setEndValue(item->x() + 80*cosA);
    group->addAnimation(ax);

    auto *ay = new QPropertyAnimation(item, "y");
    ay->setDuration(dur); ay->setEasingCurve(QEasingCurve::Steps(1)); ay->setLoopCount(loops);
    ay->setStartValue(item->y());
    ay->setKeyValueAt(0.49, item->y());
    ay->setKeyValueAt(0.5, item->y() + 80*sinA);
    ay->setEndValue(item->y() + 80*sinA);
    group->addAnimation(ay);

    return group;
}

QParallelAnimationGroup *AnimationEngine::buildWaveTriangleAnim(QGraphicsItem *item, const ElementConfig &cfg, const QRectF &rect) {
    Q_UNUSED(rect)
    auto *group = new QParallelAnimationGroup(this);
    int dur = durationMs(cfg.speed);
    int loops = loopCount(cfg.iter);
    double angle = cfg.directionAngle * M_PI / 180.0;
    double cosA = qCos(angle), sinA = qSin(angle);

    auto *ax = new QPropertyAnimation(item, "x");
    ax->setDuration(dur); ax->setEasingCurve(QEasingCurve::Linear); ax->setLoopCount(loops);
    ax->setStartValue(item->x());
    ax->setKeyValueAt(0.5, item->x() + 80*cosA);
    ax->setEndValue(item->x());
    group->addAnimation(ax);

    auto *ay = new QPropertyAnimation(item, "y");
    ay->setDuration(dur); ay->setEasingCurve(QEasingCurve::Linear); ay->setLoopCount(loops);
    ay->setStartValue(item->y());
    ay->setKeyValueAt(0.5, item->y() + 80*sinA);
    ay->setEndValue(item->y());
    group->addAnimation(ay);

    return group;
}

void AnimationEngine::applyAnimation(QGraphicsItem *item, const ElementConfig &cfg, const QRectF &boundingRect) {
    // Stop existing animation
    stopAllAnimations(item);

    if (cfg.presetId.isEmpty()) return;

    // Check all active presets (primary + extras)
    QList<QString> activeIds;
    if (!cfg.presetId.isEmpty()) activeIds.append(cfg.presetId);
    for (const auto &ep : cfg.extraPresets) {
        if (!activeIds.contains(ep)) activeIds.append(ep);
    }

    auto *totalGroup = new QParallelAnimationGroup(this);

    for (const auto &id : activeIds) {
        QParallelAnimationGroup *anim = nullptr;
        if (id == "rotate") anim = buildRotationAnim(item, cfg, boundingRect);
        else if (id == "wheel") anim = buildWheelAnim(item, cfg, boundingRect);
        else if (id == "pulse") anim = buildPulseAnim(item, cfg, boundingRect);
        else if (id == "bounce") anim = buildBounceAnim(item, cfg, boundingRect);
        else if (id == "gravity") anim = buildGravityAnim(item, cfg, boundingRect);
        else if (id == "slide") anim = buildSlideAnim(item, cfg, boundingRect);
        else if (id == "oval") anim = buildOvalAnim(item, cfg, boundingRect);
        else if (id == "fade") anim = buildFadeAnim(item, cfg, boundingRect);
        else if (id == "draw") anim = buildDrawAnim(item, cfg, boundingRect);
        else if (id == "shake") anim = buildShakeAnim(item, cfg, boundingRect);
        else if (id == "float") anim = buildFloatAnim(item, cfg, boundingRect);
        else if (id == "levitate") anim = buildLevitateAnim(item, cfg, boundingRect);
        else if (id == "arc") anim = buildArcAnim(item, cfg, boundingRect);
        else if (id == "radiate") anim = buildRadiateAnim(item, cfg, boundingRect);
        else if (id == "spin") anim = buildSpinAnim(item, cfg, boundingRect);
        else if (id == "glow") anim = buildGlowAnim(item, cfg, boundingRect);
        else if (id == "wave-sine") anim = buildWaveSineAnim(item, cfg, boundingRect);
        else if (id == "wave-square") anim = buildWaveSquareAnim(item, cfg, boundingRect);
        else if (id == "wave-triangle") anim = buildWaveTriangleAnim(item, cfg, boundingRect);

        if (anim) {
            totalGroup->addAnimation(anim);
        }
    }

    if (totalGroup->animationCount() > 0) {
        m_animations[item] = totalGroup;
        totalGroup->start();
    } else {
        totalGroup->deleteLater();
    }
}

void AnimationEngine::applyAllAnimations(const QMap<int, QGraphicsItem *> &items,
                                          const QMap<int, ElementConfig> &configs,
                                          const QMap<int, QRectF> &boundingRects) {
    for (auto it = items.constBegin(); it != items.constEnd(); ++it) {
        int idx = it.key();
        if (configs.contains(idx) && !configs[idx].presetId.isEmpty()) {
            applyAnimation(it.value(), configs[idx], boundingRects.value(idx, QRectF()));
        }
    }
}

void AnimationEngine::stopAllAnimations(QGraphicsItem *item) {
    auto it = m_animations.find(item);
    if (it != m_animations.end()) {
        it.value()->stop();
        it.value()->deleteLater();
        m_animations.erase(it);
    }
    // Remove effects
    auto eit = m_effects.find(item);
    if (eit != m_effects.end()) {
        item->setGraphicsEffect(nullptr);
        eit.value()->deleteLater();
        m_effects.erase(eit);
    }
}

void AnimationEngine::pauseAllAnimations(QGraphicsItem *item) {
    auto it = m_animations.find(item);
    if (it != m_animations.end()) {
        it.value()->pause();
    }
}

void AnimationEngine::resumeAllAnimations(QGraphicsItem *item) {
    auto it = m_animations.find(item);
    if (it != m_animations.end()) {
        it.value()->resume();
    }
}

void AnimationEngine::stopAll(const QMap<int, QGraphicsItem *> &items) {
    for (auto it = items.constBegin(); it != items.constEnd(); ++it) {
        stopAllAnimations(it.value());
    }
}
