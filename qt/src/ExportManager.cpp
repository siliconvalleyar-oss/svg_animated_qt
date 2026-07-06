#include "ExportManager.h"
#include <QFile>
#include <QTextStream>
#include <QDomDocument>
#include <QDomElement>
#include <QDomText>
#include <QtMath>

ExportManager::ExportManager(QObject *parent) : QObject(parent) {}

QString ExportManager::exportSvg(const QString &originalSvg,
                                  const QMap<int, ElementConfig> &configs) {
    QDomDocument doc;
    if (!doc.setContent(originalSvg)) return QString();

    QDomElement root = doc.documentElement();
    if (root.isNull()) return QString();

    // Build embedded style
    QString embeddedStyle;
    QString elementStyles;

    for (auto it = configs.constBegin(); it != configs.constEnd(); ++it) {
        int idx = it.key();
        const ElementConfig &cfg = it.value();
        if (cfg.presetId.isEmpty()) continue;

        QList<QString> activeIds;
        if (!cfg.presetId.isEmpty()) activeIds.append(cfg.presetId);
        for (const auto &ep : cfg.extraPresets) {
            if (!activeIds.contains(ep)) activeIds.append(ep);
        }

        QString varsStr;
        QString animParts;

        for (const auto &id : activeIds) {
            bool isTB = AnimationEngine().isTranslateBased(id);
            bool hasAngle = cfg.directionAngle != 0;

            QString kfName;
            if (isTB && hasAngle) {
                QString kf = generateKeyframes(id, idx, cfg.directionAngle);
                if (!kf.isEmpty() && !embeddedStyle.contains(QString("%1_export_%2_%3").arg(id).arg(idx).arg(static_cast<int>(cfg.directionAngle)))) {
                    embeddedStyle += kf + "\n";
                }
                kfName = QString("%1_export_%2_%3").arg(id).arg(idx).arg(static_cast<int>(cfg.directionAngle));
            } else {
                kfName = baseKeyframeName(id);
                if (!embeddedStyle.contains(kfName)) {
                    embeddedStyle += baseKeyframes(id) + "\n";
                }
            }

            const AnimationPreset *p = AnimationEngine::presetById(id);
            QString easing = p ? p->easing : "ease-in-out";
            animParts += QString("%1 %2s %3 %4 %5; ").arg(kfName).arg(cfg.speed).arg(easing).arg(cfg.iter).arg(cfg.dir);

            if (id == "oval") {
                varsStr += QString("--oval-rx: %1px; --oval-ry: %2px; ").arg(cfg.ovalRx).arg(cfg.ovalRy);
            }
            if (AnimationEngine().isArcPreset(id)) {
                varsStr += QString("--arc-rx: %1px; --arc-ry: %2px; ").arg(cfg.arcRx).arg(cfg.arcRy);
            }
            if (id == "draw") {
                varsStr += "stroke-dasharray: 1000; --path-length: 1000; ";
            }
        }

        elementStyles += QString("[data-anim-index=\"%1\"] { transform-origin: center; transform-box: view-box; %2 animation: %3 animation-delay: %4s; }\n")
            .arg(idx).arg(varsStr).arg(animParts).arg(cfg.delay);
    }

    if (!embeddedStyle.isEmpty() || !elementStyles.isEmpty()) {
        QDomElement styleEl = doc.createElement("style");
        QDomText styleText = doc.createTextNode(embeddedStyle + "\n" + elementStyles);
        styleEl.appendChild(styleText);
        root.insertBefore(styleEl, root.firstChild());
    }

    // Add data-anim-index attributes
    QDomNodeList elements = root.elementsByTagName("*");
    for (int i = 0; i < elements.count(); i++) {
        QDomElement el = elements.at(i).toElement();
        if (!el.isNull() && configs.contains(i)) {
            el.setAttribute("data-anim-index", i);
        }
    }

    return "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" + doc.toString(-1);
}

bool ExportManager::saveToFile(const QString &svgContent, const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return false;
    QTextStream stream(&file);
    stream << svgContent;
    file.close();
    return true;
}

QString ExportManager::baseKeyframeName(const QString &id) {
    QMap<QString, QString> map = {
        {"rotate", "svgRotate"}, {"wheel", "svgWheel"}, {"pulse", "svgPulse"},
        {"bounce", "svgBounce"}, {"gravity", "svgGravity"}, {"slide", "svgSlide"},
        {"oval", "svgOval"}, {"fade", "svgFade"}, {"draw", "svgDraw"},
        {"shake", "svgShake"}, {"float", "svgFloat"}, {"levitate", "svgLevitate"},
        {"arc", "svgArc"}, {"radiate", "svgRadiate"}, {"spin", "svgSpin"},
        {"glow", "svgGlow"}, {"wave-sine", "svgWaveSine"}, {"wave-square", "svgWaveSquare"},
        {"wave-triangle", "svgWaveTriangle"},
    };
    return map.value(id);
}

QString ExportManager::baseKeyframes(const QString &id) {
    QMap<QString, QString> kfs = {
        {"rotate", "@keyframes svgRotate { from { transform: rotate(0deg); } to { transform: rotate(360deg); } }"},
        {"wheel", "@keyframes svgWheel { 0% { transform: rotate(0deg); } 25% { transform: rotate(90deg); } 50% { transform: rotate(180deg); } 75% { transform: rotate(270deg); } 100% { transform: rotate(360deg); } }"},
        {"pulse", "@keyframes svgPulse { 0%,100% { transform: scale(1); } 50% { transform: scale(1.15); } }"},
        {"bounce", "@keyframes svgBounce { 0%,100% { transform: translateY(0); } 50% { transform: translateY(-20px); } }"},
        {"gravity", "@keyframes svgGravity { 0% { transform: translateY(-100px); } 30% { transform: translateY(80px); } 50% { transform: translateY(-40px); } 70% { transform: translateY(30px); } 85% { transform: translateY(-10px); } 100% { transform: translateY(0); } }"},
        {"slide", "@keyframes svgSlide { 0%,100% { transform: translateX(-80px); } 50% { transform: translateX(80px); } }"},
        {"oval", "@keyframes svgOval { 0% { transform: translate(0,0); } 25% { transform: translate(var(--oval-rx,80px),0); } 50% { transform: translate(0,var(--oval-ry,40px)); } 75% { transform: translate(calc(-1*var(--oval-rx,80px)),0); } 100% { transform: translate(0,0); } }"},
        {"fade", "@keyframes svgFade { 0%,100% { opacity: 1; } 50% { opacity: 0.15; } }"},
        {"draw", "@keyframes svgDraw { from { stroke-dashoffset: var(--path-length,1000); } to { stroke-dashoffset: 0; } }"},
        {"shake", "@keyframes svgShake { 0%,100% { transform: translateX(0); } 10%,30%,50%,70%,90% { transform: translateX(-8px); } 20%,40%,60%,80% { transform: translateX(8px); } }"},
        {"float", "@keyframes svgFloat { 0%,100% { transform: translateY(0); } 50% { transform: translateY(-15px); } }"},
        {"levitate", "@keyframes svgLevitate { 0%,100% { transform: translateY(0) scale(1); } 25% { transform: translateY(-12px) scale(1.02); } 50% { transform: translateY(-25px) scale(0.98); } 75% { transform: translateY(-12px) scale(1.01); } }"},
        {"arc", "@keyframes svgArc { 0% { transform: translate(calc(var(--arc-offset-x,0px) + var(--arc-rx,80px) * -1), calc(var(--arc-offset-y,0px))); } 25% { transform: translate(calc(var(--arc-offset-x,0px) + var(--arc-rx,80px) * -0.7071), calc(var(--arc-offset-y,0px) + var(--arc-ry,80px) * -0.7071)); } 50% { transform: translate(calc(var(--arc-offset-x,0px)), calc(var(--arc-offset-y,0px) + var(--arc-ry,80px) * -1)); } 75% { transform: translate(calc(var(--arc-offset-x,0px) + var(--arc-rx,80px) * 0.7071), calc(var(--arc-offset-y,0px) + var(--arc-ry,80px) * -0.7071)); } 100% { transform: translate(calc(var(--arc-offset-x,0px) + var(--arc-rx,80px) * 1), calc(var(--arc-offset-y,0px))); } }"},
        {"radiate", "@keyframes svgRadiate { 0% { transform: translate(calc(var(--arc-offset-x,0px) + var(--arc-rx,80px) * -1), calc(var(--arc-offset-y,0px))); filter: drop-shadow(0 0 2px rgba(230,126,34,0.2)); } 50% { transform: translate(calc(var(--arc-offset-x,0px)), calc(var(--arc-offset-y,0px) + var(--arc-ry,80px) * -1)); filter: drop-shadow(0 0 24px rgba(230,126,34,0.9)); } 100% { transform: translate(calc(var(--arc-offset-x,0px) + var(--arc-rx,80px) * 1), calc(var(--arc-offset-y,0px))); filter: drop-shadow(0 0 2px rgba(230,126,34,0.2)); } }"},
        {"spin", "@keyframes svgSpin { 0% { transform: rotate(0deg) scale(1); } 50% { transform: rotate(180deg) scale(0.85); } 100% { transform: rotate(360deg) scale(1); } }"},
        {"glow", "@keyframes svgGlow { 0%,100% { filter: drop-shadow(0 0 4px rgba(108,92,231,0.3)); } 50% { filter: drop-shadow(0 0 24px rgba(108,92,231,0.9)); } }"},
        {"wave-sine", "@keyframes svgWaveSine { 0% { transform: translate(0,0); } 25% { transform: translate(80px,-40px); } 50% { transform: translate(160px,0); } 75% { transform: translate(240px,40px); } 100% { transform: translate(320px,0); } }"},
        {"wave-square", "@keyframes svgWaveSquare { 0%,49% { transform: translateX(0); } 50%,100% { transform: translateX(80px); } }"},
        {"wave-triangle", "@keyframes svgWaveTriangle { 0% { transform: translateX(0); } 50% { transform: translateX(80px); } 100% { transform: translateX(0); } }"},
    };
    return kfs.value(id);
}

QString ExportManager::generateKeyframes(const QString &presetId, int elemIndex, double angle) {
    double rad = angle * M_PI / 180.0;
    double cosA = qCos(rad);
    double sinA = qSin(rad);
    QString name = QString("%1_export_%2_%3").arg(presetId).arg(elemIndex).arg(static_cast<int>(angle));

    if (presetId == "slide") {
        return QString("@keyframes %1 { 0%,100% { transform: translate(%2px,%3px); } 50% { transform: translate(%4px,%5px); } }")
            .arg(name).arg(-80*cosA).arg(-80*sinA).arg(80*cosA).arg(80*sinA);
    }
    if (presetId == "bounce") {
        return QString("@keyframes %1 { 0%,100% { transform: translate(0,0); } 50% { transform: translate(%2px,%3px); } }")
            .arg(name).arg(20*cosA).arg(20*sinA);
    }
    if (presetId == "float") {
        return QString("@keyframes %1 { 0%,100% { transform: translate(0,0); } 50% { transform: translate(%2px,%3px); } }")
            .arg(name).arg(15*cosA).arg(15*sinA);
    }
    return QString();
}
