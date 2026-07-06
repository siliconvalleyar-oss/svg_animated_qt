#include "ControlsWidget.h"
#include "Theme.h"
#include <QScrollArea>
#include <QFrame>

ControlsWidget::ControlsWidget(QWidget *parent) : QWidget(parent) {
    m_config = AnimationEngine::defaultConfig();
    setupUi();
}

void ControlsWidget::setupUi() {
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    auto *scroll = new QScrollArea;
    scroll->setWidgetResizable(true);
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scroll->setFrameShape(QFrame::NoFrame);

    auto *content = new QWidget;
    auto *layout = new QVBoxLayout(content);
    layout->setContentsMargins(12, 8, 12, 8);
    layout->setSpacing(12);

    // === PRESET GRID ===
    auto *sectionTitle = new QLabel(tr("ANIMACIONES"));
    sectionTitle->setProperty("section", true);
    layout->addWidget(sectionTitle);

    m_presetGrid = new QGridLayout;
    m_presetGrid->setSpacing(6);
    int row = 0, col = 0;
    for (const auto &p : AnimationEngine::presets()) {
        auto *btn = new QPushButton(QString("<span style='display:inline-block;width:8px;height:8px;border-radius:50px;background:%1;margin-right:6px;'></span>%2").arg(p.color.name(), p.name));
        btn->setCheckable(true);
        btn->setMinimumHeight(32);
        btn->setProperty("presetId", p.id);
        m_presetButtons[p.id] = btn;
        m_presetGrid->addWidget(btn, row, col);
        connect(btn, &QPushButton::clicked, this, [this, id = p.id]() {
            emit presetToggled(id);
        });
        col++;
        if (col >= 2) { col = 0; row++; }
    }
    layout->addLayout(m_presetGrid);

    // Apply all button
    auto *applyAllBtn = new QPushButton(tr("Aplicar a todos"));
    applyAllBtn->setProperty("primary", true);
    layout->addWidget(applyAllBtn);

    // === PLAYBACK BAR ===
    auto *playbackLayout = new QHBoxLayout;
    m_playBtn = new QPushButton(QChar(9654)); // Play
    m_pauseBtn = new QPushButton(QChar(10074) % QChar(10074)); // Pause
    m_stopBtn = new QPushButton(QChar(9632)); // Stop
    for (auto *btn : {m_playBtn, m_pauseBtn, m_stopBtn}) {
        btn->setMinimumHeight(36);
        playbackLayout->addWidget(btn);
    }
    layout->addLayout(playbackLayout);
    connect(m_playBtn, &QPushButton::clicked, this, &ControlsWidget::playClicked);
    connect(m_pauseBtn, &QPushButton::clicked, this, &ControlsWidget::pauseClicked);
    connect(m_stopBtn, &QPushButton::clicked, this, &ControlsWidget::stopClicked);

    // === CONTROLS SECTION ===
    auto *ctrlTitle = new QLabel(tr("CONTROLES"));
    ctrlTitle->setProperty("section", true);
    layout->addWidget(ctrlTitle);

    auto *ctrlLayout = new QVBoxLayout;
    ctrlLayout->setSpacing(10);

    // Speed
    auto *speedRow = new QHBoxLayout;
    auto *speedLabel = new QLabel(tr("Velocidad"));
    speedLabel->setMinimumWidth(60);
    m_speedSlider = new QSlider(Qt::Horizontal);
    m_speedSlider->setRange(2, 160);
    m_speedSlider->setValue(10);
    m_speedValue = new QLabel("1.0s");
    m_speedValue->setMinimumWidth(36);
    m_speedValue->setAlignment(Qt::AlignRight);
    speedRow->addWidget(speedLabel);
    speedRow->addWidget(m_speedSlider);
    speedRow->addWidget(m_speedValue);
    ctrlLayout->addLayout(speedRow);

    connect(m_speedSlider, &QSlider::valueChanged, this, [this](int v) {
        m_config.speed = v / 10.0;
        updateDisplayValues();
        emit configChanged(m_config);
    });

    // Delay
    auto *delayRow = new QHBoxLayout;
    auto *delayLabel = new QLabel(tr("Retraso"));
    delayLabel->setMinimumWidth(60);
    m_delaySlider = new QSlider(Qt::Horizontal);
    m_delaySlider->setRange(0, 30);
    m_delaySlider->setValue(0);
    m_delayValue = new QLabel("0.0s");
    m_delayValue->setMinimumWidth(36);
    m_delayValue->setAlignment(Qt::AlignRight);
    delayRow->addWidget(delayLabel);
    delayRow->addWidget(m_delaySlider);
    delayRow->addWidget(m_delayValue);
    ctrlLayout->addLayout(delayRow);

    connect(m_delaySlider, &QSlider::valueChanged, this, [this](int v) {
        m_config.delay = v / 10.0;
        updateDisplayValues();
        emit configChanged(m_config);
    });

    // Iterations
    auto *iterRow = new QHBoxLayout;
    auto *iterLabel = new QLabel(tr("Repetir"));
    iterLabel->setMinimumWidth(60);
    m_iterGroup = new QButtonGroup(this);
    auto *iterInf = new QPushButton(QChar(8734)); // infinity
    auto *iter1 = new QPushButton("1");
    auto *iter3 = new QPushButton("3");
    auto *iterRand = new QPushButton("?");
    iterInf->setCheckable(true); iterInf->setChecked(true);
    iter1->setCheckable(true);
    iter3->setCheckable(true);
    iterRand->setCheckable(true);
    m_iterGroup->addButton(iterInf, 0); m_iterGroup->setId(iterInf, 0);
    m_iterGroup->addButton(iter1, 1); m_iterGroup->setId(iter1, 1);
    m_iterGroup->addButton(iter3, 2); m_iterGroup->setId(iter3, 2);
    m_iterGroup->addButton(iterRand, 3); m_iterGroup->setId(iterRand, 3);
    iterRow->addWidget(iterLabel);
    iterRow->addWidget(iterInf); iterRow->addWidget(iter1);
    iterRow->addWidget(iter3); iterRow->addWidget(iterRand);
    ctrlLayout->addLayout(iterRow);

    connect(m_iterGroup, QOverload<int>::of(&QButtonGroup::buttonClicked), this, [this](int id) {
        static const QStringList vals = {"infinite", "1", "3", "random"};
        if (id >= 0 && id < vals.size()) {
            if (vals[id] == "random") {
                static const QList<QString> opts = {"1","2","3","5","10","infinite"};
                m_config.iter = opts[qrand() % opts.size()];
            } else {
                m_config.iter = vals[id];
            }
        }
        emit configChanged(m_config);
    });

    // Direction
    auto *dirRow = new QHBoxLayout;
    auto *dirLabel = new QLabel(tr("Direccion"));
    dirLabel->setMinimumWidth(60);
    m_dirGroup = new QButtonGroup(this);
    auto *dirNorm = new QPushButton(tr("Normal"));
    auto *dirRev = new QPushButton(tr("Reversa"));
    auto *dirAlt = new QPushButton(tr("Alterno"));
    dirNorm->setCheckable(true); dirNorm->setChecked(true);
    dirRev->setCheckable(true);
    dirAlt->setCheckable(true);
    m_dirGroup->addButton(dirNorm); m_dirGroup->addButton(dirRev); m_dirGroup->addButton(dirAlt);
    dirRow->addWidget(dirLabel);
    dirRow->addWidget(dirNorm); dirRow->addWidget(dirRev); dirRow->addWidget(dirAlt);
    ctrlLayout->addLayout(dirRow);

    connect(m_dirGroup, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked), this, [this](QAbstractButton *btn) {
        if (btn->text() == tr("Normal")) m_config.dir = "normal";
        else if (btn->text() == tr("Reversa")) m_config.dir = "reverse";
        else m_config.dir = "alternate";
        emit configChanged(m_config);
    });

    layout->addLayout(ctrlLayout);

    // === DIRECTION CONTROLS ===
    m_directionControls = new QWidget;
    auto *dirCtrlLayout = new QVBoxLayout(m_directionControls);
    dirCtrlLayout->setContentsMargins(0, 0, 0, 0);
    dirCtrlLayout->setSpacing(6);

    auto *dirTitle = new QLabel(tr("SENTIDO / ANGULO"));
    dirTitle->setProperty("section", true);
    dirCtrlLayout->addWidget(dirTitle);

    auto *angleRow = new QHBoxLayout;
    auto *angleLabel = new QLabel(tr("Angulo"));
    angleLabel->setMinimumWidth(60);
    m_directionSlider = new QSlider(Qt::Horizontal);
    m_directionSlider->setRange(0, 360);
    m_directionSlider->setSingleStep(5);
    m_directionSlider->setValue(0);
    m_directionValue = new QLabel("0" + QChar(176));
    m_directionValue->setMinimumWidth(36);
    m_directionValue->setAlignment(Qt::AlignRight);
    angleRow->addWidget(angleLabel);
    angleRow->addWidget(m_directionSlider);
    angleRow->addWidget(m_directionValue);
    dirCtrlLayout->addLayout(angleRow);

    connect(m_directionSlider, &QSlider::valueChanged, this, [this](int v) {
        m_config.directionAngle = v;
        m_directionValue->setText(QString::number(v) + QChar(176));
        emit configChanged(m_config);
    });

    // Direction presets (8 arrows)
    m_directionPresetsWidget = new QWidget;
    auto *presetsLayout = new QGridLayout(m_directionPresetsWidget);
    presetsLayout->setSpacing(4);
    presetsLayout->setContentsMargins(0, 0, 0, 0);
    struct DirPreset { QString label; int angle; };
    QList<DirPreset> dirs = {
        {QStringLiteral("\u2192"), 0}, {QStringLiteral("\u2197"), 45},
        {QStringLiteral("\u2191"), 90}, {QStringLiteral("\u2196"), 135},
        {QStringLiteral("\u2190"), 180}, {QStringLiteral("\u2199"), 225},
        {QStringLiteral("\u2193"), 270}, {QStringLiteral("\u2198"), 315},
    };
    for (int i = 0; i < dirs.size(); i++) {
        auto *btn = new QPushButton(dirs[i].label);
        btn->setFixedSize(32, 32);
        presetsLayout->addWidget(btn, i / 4, i % 4);
        connect(btn, &QPushButton::clicked, this, [this, angle = dirs[i].angle]() {
            m_directionSlider->setValue(angle);
        });
    }
    dirCtrlLayout->addWidget(m_directionPresetsWidget);
    layout->addWidget(m_directionControls);
    m_directionControls->setVisible(false);

    // === OVAL CONTROLS ===
    m_ovalControls = new QWidget;
    auto *ovalLayout = new QVBoxLayout(m_ovalControls);
    ovalLayout->setContentsMargins(0, 0, 0, 0);
    ovalLayout->setSpacing(6);

    auto *ovalTitle = new QLabel(tr("OVALO"));
    ovalTitle->setProperty("section", true);
    ovalLayout->addWidget(ovalTitle);

    auto addOvalRow = [&](const QString &label, QSlider *&slider, QLabel *&val, int min, int max, int def) {
        auto *row = new QHBoxLayout;
        auto *lbl = new QLabel(label);
        lbl->setMinimumWidth(60);
        slider = new QSlider(Qt::Horizontal);
        slider->setRange(min, max);
        slider->setValue(def);
        val = new QLabel(QString::number(def) + "px");
        val->setMinimumWidth(36);
        val->setAlignment(Qt::AlignRight);
        row->addWidget(lbl); row->addWidget(slider); row->addWidget(val);
        ovalLayout->addLayout(row);
    };
    addOvalRow(tr("Ancho X"), m_ovalRxSlider, m_ovalRxVal, 10, 150, 80);
    addOvalRow(tr("Alto Y"), m_ovalRySlider, m_ovalRyVal, 10, 150, 40);
    addOvalRow(tr("Angulo"), m_ovalAngleSlider, m_ovalAngleVal, 0, 360, 0);

    connect(m_ovalRxSlider, &QSlider::valueChanged, this, [this](int v) { m_config.ovalRx = v; m_ovalRxVal->setText(QString::number(v) + "px"); emit configChanged(m_config); });
    connect(m_ovalRySlider, &QSlider::valueChanged, this, [this](int v) { m_config.ovalRy = v; m_ovalRyVal->setText(QString::number(v) + "px"); emit configChanged(m_config); });
    connect(m_ovalAngleSlider, &QSlider::valueChanged, this, [this](int v) { m_config.ovalAngle = v; m_ovalAngleVal->setText(QString::number(v) + "deg"); emit configChanged(m_config); });

    layout->addWidget(m_ovalControls);
    m_ovalControls->setVisible(false);

    // === ARC CONTROLS ===
    m_arcControls = new QWidget;
    auto *arcLayout = new QVBoxLayout(m_arcControls);
    arcLayout->setContentsMargins(0, 0, 0, 0);
    arcLayout->setSpacing(6);

    auto *arcTitle = new QLabel(tr("ARCO"));
    arcTitle->setProperty("section", true);
    arcLayout->addWidget(arcTitle);

    auto addArcRow = [&](const QString &label, QSlider *&slider, QLabel *&val, int min, int max, int def) {
        auto *row = new QHBoxLayout;
        auto *lbl = new QLabel(label);
        lbl->setMinimumWidth(60);
        slider = new QSlider(Qt::Horizontal);
        slider->setRange(min, max);
        slider->setValue(def);
        val = new QLabel(QString::number(def) + "px");
        val->setMinimumWidth(36);
        val->setAlignment(Qt::AlignRight);
        row->addWidget(lbl); row->addWidget(slider); row->addWidget(val);
        arcLayout->addLayout(row);
    };
    addArcRow(tr("Eje X"), m_arcRxSlider, m_arcRxVal, 10, 200, 80);
    addArcRow(tr("Eje Y"), m_arcRySlider, m_arcRyVal, 10, 200, 80);

    connect(m_arcRxSlider, &QSlider::valueChanged, this, [this](int v) { m_config.arcRx = v; m_arcRxVal->setText(QString::number(v) + "px"); emit configChanged(m_config); });
    connect(m_arcRySlider, &QSlider::valueChanged, this, [this](int v) { m_config.arcRy = v; m_arcRyVal->setText(QString::number(v) + "px"); emit configChanged(m_config); });

    layout->addWidget(m_arcControls);
    m_arcControls->setVisible(false);

    layout->addStretch();

    scroll->setWidget(content);
    mainLayout->addWidget(scroll);
}

ElementConfig ControlsWidget::currentConfig() const {
    return m_config;
}

void ControlsWidget::loadConfig(const ElementConfig &cfg) {
    m_config = cfg;
    m_speedSlider->setValue(static_cast<int>(cfg.speed * 10));
    m_delaySlider->setValue(static_cast<int>(cfg.delay * 10));
    m_directionSlider->setValue(static_cast<int>(cfg.directionAngle));
    m_ovalRxSlider->setValue(static_cast<int>(cfg.ovalRx));
    m_ovalRySlider->setValue(static_cast<int>(cfg.ovalRy));
    m_ovalAngleSlider->setValue(static_cast<int>(cfg.ovalAngle));
    m_arcRxSlider->setValue(static_cast<int>(cfg.arcRx));
    m_arcRySlider->setValue(static_cast<int>(cfg.arcRy));

    // Update preset buttons
    for (auto it = m_presetButtons.constBegin(); it != m_presetButtons.constEnd(); ++it) {
        it.value()->setChecked(it.key() == cfg.presetId);
    }

    // Show/hide specialized controls
    bool isOval = (cfg.presetId == "oval");
    bool isArc = (cfg.presetId == "arc" || cfg.presetId == "radiate");
    bool isTranslateBased = AnimationEngine().isTranslateBased(cfg.presetId);
    m_ovalControls->setVisible(isOval);
    m_arcControls->setVisible(isArc);
    m_directionControls->setVisible(isTranslateBased);

    updateDisplayValues();
}

void ControlsWidget::setPresetActive(const QString &presetId, bool active) {
    if (m_presetButtons.contains(presetId)) {
        m_presetButtons[presetId]->setChecked(active);
    }
}

void ControlsWidget::updateDisplayValues() {
    m_speedValue->setText(QString::number(m_config.speed, 'f', 1) + "s");
    m_delayValue->setText(QString::number(m_config.delay, 'f', 1) + "s");
    m_directionValue->setText(QString::number(static_cast<int>(m_config.directionAngle)) + QChar(176));
    m_ovalRxVal->setText(QString::number(static_cast<int>(m_config.ovalRx)) + "px");
    m_ovalRyVal->setText(QString::number(static_cast<int>(m_config.ovalRy)) + "px");
    m_ovalAngleVal->setText(QString::number(static_cast<int>(m_config.ovalAngle)) + "deg");
    m_arcRxVal->setText(QString::number(static_cast<int>(m_config.arcRx)) + "px");
    m_arcRyVal->setText(QString::number(static_cast<int>(m_config.arcRy)) + "px");
}
