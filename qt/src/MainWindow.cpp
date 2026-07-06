#include "MainWindow.h"
#include "Theme.h"
#include "Shapes.h"
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QShortcut>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QScrollBar>
#include <QGridLayout>
#include <QFrame>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    m_animEngine = new AnimationEngine(this);
    m_exportManager = new ExportManager(this);
    m_trajManager = new TrajectoryManager(this);
    m_slideManager = new SlideshowManager(this);
    m_bgManager = new BackgroundImageManager(this);
    m_undoStack = new QUndoStack(this);

    setWindowTitle("SVG Animator");
    setMinimumSize(1200, 800);
    setAcceptDrops(true);

    setupUi();
    setupMenuBar();
    setupShortcuts();

    statusBar()->showMessage(tr("Listo"));
}

void MainWindow::setupUi() {
    m_splitter = new QSplitter(Qt::Horizontal);
    setCentralWidget(m_splitter);

    // Left panel
    m_leftPanel = new QWidget;
    auto *leftLayout = new QVBoxLayout(m_leftPanel);
    leftLayout->setContentsMargins(0, 0, 0, 0);
    leftLayout->setSpacing(0);

    // Workspace tabs
    m_workspaceTabs = new QTabWidget;
    m_workspaceTabs->setTabsClosable(true);
    m_workspaceTabs->setMaximumHeight(100);
    m_wsManager = new WorkspaceManager(m_workspaceTabs, this);

    leftLayout->addWidget(m_workspaceTabs);

    // Workspace title
    auto *wsTitle = new QLabel(tr("SVG Animator"));
    wsTitle->setAlignment(Qt::AlignCenter);
    wsTitle->setStyleSheet("font-size:10px;color:#8b8fa7;padding:2px 0;border-bottom:1px solid #2e3245;");
    leftLayout->addWidget(wsTitle);

    // Scrollable left content
    m_leftScroll = new QScrollArea;
    m_leftScroll->setWidgetResizable(true);
    m_leftScroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_leftScroll->setFrameShape(QFrame::NoFrame);

    auto *leftContent = new QWidget;
    auto *leftVBox = new QVBoxLayout(leftContent);
    leftVBox->setContentsMargins(0, 0, 0, 0);
    leftVBox->setSpacing(0);

    // === SHAPES SECTION ===
    m_shapesSection = new QWidget;
    auto *shapesLayout = new QVBoxLayout(m_shapesSection);
    shapesLayout->setContentsMargins(12, 8, 12, 8);
    auto *shapesTitle = new QLabel(tr("GENERAR SVG"));
    shapesTitle->setProperty("section", true);
    shapesLayout->addWidget(shapesTitle);

    auto *shapeGrid = new QGridLayout;
    shapeGrid->setSpacing(5);
    const auto &shapes = Shapes::all();
    for (int i = 0; i < shapes.size(); i++) {
        auto *btn = new QPushButton(shapes[i].name);
        btn->setMinimumHeight(40);
        btn->setProperty("shapeName", shapes[i].name);
        shapeGrid->addWidget(btn, i / 3, i % 3);
        connect(btn, &QPushButton::clicked, this, [this, name = shapes[i].name]() {
            loadShape(name);
        });
    }
    shapesLayout->addLayout(shapeGrid);
    leftVBox->addWidget(m_shapesSection);

    // === PRESETS SECTION ===
    m_presetsSection = new QWidget;
    auto *presetsLayout = new QVBoxLayout(m_presetsSection);
    presetsLayout->setContentsMargins(12, 8, 12, 8);

    auto *presetsTitle = new QLabel(tr("ANIMACIONES"));
    presetsTitle->setProperty("section", true);
    presetsLayout->addWidget(presetsTitle);

    auto *presetGrid = new QGridLayout;
    presetGrid->setSpacing(6);
    int row = 0, col = 0;
    for (const auto &p : AnimationEngine::presets()) {
        auto *btn = new QPushButton(QString("<span style='display:inline-block;width:8px;height:8px;border-radius:50px;background:%1;margin-right:6px;'></span>%2").arg(p.color.name(), p.name));
        btn->setCheckable(true);
        btn->setMinimumHeight(32);
        presetGrid->addWidget(btn, row, col);
        connect(btn, &QPushButton::clicked, this, [this, id = p.id]() {
            onPresetToggled(id);
        });
        col++;
        if (col >= 2) { col = 0; row++; }
    }
    presetsLayout->addLayout(presetGrid);

    auto *applyAllBtn = new QPushButton(tr("Aplicar a todos"));
    applyAllBtn->setProperty("primary", true);
    presetsLayout->addWidget(applyAllBtn);
    connect(applyAllBtn, &QPushButton::clicked, this, &MainWindow::applyAllAnimations);

    // Playback bar
    auto *playbackLayout = new QHBoxLayout;
    auto *playBtn = new QPushButton(QChar(9654));
    auto *pauseBtn = new QPushButton(QChar(10074) % QChar(10074));
    auto *stopBtn = new QPushButton(QChar(9632));
    playBtn->setMinimumHeight(36); pauseBtn->setMinimumHeight(36); stopBtn->setMinimumHeight(36);
    playbackLayout->addWidget(playBtn); playbackLayout->addWidget(pauseBtn); playbackLayout->addWidget(stopBtn);
    presetsLayout->addLayout(playbackLayout);

    connect(playBtn, &QPushButton::clicked, this, [this]() { m_animationPlaying = true; applyCurrentAnimation(); });
    connect(pauseBtn, &QPushButton::clicked, this, [this]() { m_animationPlaying = false; m_svgView->pauseAnimations(); });
    connect(stopBtn, &QPushButton::clicked, this, [this]() { m_svgView->stopAllAnimations(); m_animationPlaying = false; });

    leftVBox->addWidget(m_presetsSection);

    // === CONTROLS SECTION ===
    m_controlsSection = new QWidget;
    auto *ctrlLayout = new QVBoxLayout(m_controlsSection);
    ctrlLayout->setContentsMargins(12, 8, 12, 8);
    m_controls = new ControlsWidget;
    ctrlLayout->addWidget(m_controls);
    leftVBox->addWidget(m_controlsSection);

    connect(m_controls, &ControlsWidget::configChanged, this, &MainWindow::onConfigChanged);

    // === EXPORT SECTION ===
    m_exportSection = new QWidget;
    auto *exportLayout = new QVBoxLayout(m_exportSection);
    exportLayout->setContentsMargins(12, 8, 12, 8);

    auto *exportTitle = new QLabel(tr("EXPORTAR"));
    exportTitle->setProperty("section", true);
    exportLayout->addWidget(exportTitle);

    auto *exportBtn = new QPushButton(tr("Descargar SVG Animado"));
    exportBtn->setProperty("primary", true);
    exportLayout->addWidget(exportBtn);
    connect(exportBtn, &QPushButton::clicked, this, &MainWindow::exportAnimatedSvg);

    auto *boundaryBtn = new QPushButton(tr("Mostrar limite"));
    boundaryBtn->setCheckable(true);
    exportLayout->addWidget(boundaryBtn);
    connect(boundaryBtn, &QPushButton::clicked, this, [this](bool checked) {
        m_svgView->setBoundaryVisible(checked);
    });

    leftVBox->addWidget(m_exportSection);

    // === HISTORY SECTION ===
    m_historySection = new QWidget;
    auto *historyLayout = new QVBoxLayout(m_historySection);
    historyLayout->setContentsMargins(12, 8, 12, 8);

    auto *historyTitle = new QLabel(tr("HISTORIAL"));
    historyTitle->setProperty("section", true);
    historyLayout->addWidget(historyTitle);

    auto *histRow = new QHBoxLayout;
    auto *undoBtn = new QPushButton(tr("Deshacer"));
    auto *redoBtn = new QPushButton(tr("Rehacer"));
    undoBtn->setEnabled(false); redoBtn->setEnabled(false);
    histRow->addWidget(undoBtn); histRow->addWidget(redoBtn);
    historyLayout->addLayout(histRow);

    connect(undoBtn, &QPushButton::clicked, this, &MainWindow::undo);
    connect(redoBtn, &QPushButton::clicked, this, &MainWindow::redo);

    auto *resetBtn = new QPushButton(tr("Resetear"));
    historyLayout->addWidget(resetBtn);
    connect(resetBtn, &QPushButton::clicked, this, [this]() {
        if (QMessageBox::question(this, tr("Resetear"), tr("Resetear todo?")) == QMessageBox::Yes) {
            m_elementConfigs.clear();
            m_selectedElementIndex = -1;
            QString svg = m_svgView->currentSvgString();
            if (!svg.isEmpty()) loadSvgString(svg);
        }
    });

    leftVBox->addWidget(m_historySection);

    // === SLIDES SECTION ===
    m_slidesSection = new QWidget;
    auto *slidesLayout = new QVBoxLayout(m_slidesSection);
    slidesLayout->setContentsMargins(12, 8, 12, 8);

    auto *slidesTitle = new QLabel(tr("SLIDES"));
    slidesTitle->setProperty("section", true);
    slidesLayout->addWidget(slidesTitle);

    m_slideListWidget = new QListWidget;
    m_slideListWidget->setMaximumHeight(120);
    slidesLayout->addWidget(m_slideListWidget);

    auto *addSlideBtn = new QPushButton(tr("+ Agregar SVG actual"));
    addSlideBtn->setStyleSheet("border:1px dashed #2e3245;background:transparent;");
    slidesLayout->addWidget(addSlideBtn);
    connect(addSlideBtn, &QPushButton::clicked, this, &MainWindow::addCurrentAsSlide);

    // Slide controls
    auto *slideCtrlLayout = new QVBoxLayout;
    auto *transRow = new QHBoxLayout;
    auto *transLabel = new QLabel(tr("Transicion"));
    transLabel->setMinimumWidth(60);
    auto *transCombo = new QComboBox;
    transCombo->addItems({"fade", "slide-h", "slide-v", "zoom", "flip", "blur"});
    transRow->addWidget(transLabel); transRow->addWidget(transCombo);
    slideCtrlLayout->addLayout(transRow);

    connect(transCombo, &QComboBox::currentTextChanged, this, [this](const QString &t) {
        m_slideManager->setTransition(t);
    });

    auto *durRow = new QHBoxLayout;
    auto *durLabel = new QLabel(tr("Dur. slide"));
    durLabel->setMinimumWidth(60);
    auto *durSlider = new QSlider(Qt::Horizontal);
    durSlider->setRange(10, 100);
    durSlider->setValue(30);
    auto *durVal = new QLabel("3.0s");
    durRow->addWidget(durLabel); durRow->addWidget(durSlider); durRow->addWidget(durVal);
    slideCtrlLayout->addLayout(durRow);

    connect(durSlider, &QSlider::valueChanged, this, [this, durVal](int v) {
        double s = v / 10.0;
        m_slideManager->setSlideDuration(s);
        durVal->setText(QString::number(s, 'f', 1) + "s");
    });

    auto *navRow = new QHBoxLayout;
    auto *prevBtn = new QPushButton(QChar(9664));
    m_slidePlayBtn = new QPushButton(tr("Play"));
    auto *nextBtn = new QPushButton(QChar(9654));
    prevBtn->setMinimumHeight(32); m_slidePlayBtn->setMinimumHeight(32); nextBtn->setMinimumHeight(32);
    navRow->addWidget(prevBtn); navRow->addWidget(m_slidePlayBtn); navRow->addWidget(nextBtn);
    slideCtrlLayout->addLayout(navRow);

    connect(prevBtn, &QPushButton::clicked, this, [this]() { m_slideManager->previous(); });
    connect(nextBtn, &QPushButton::clicked, this, [this]() { m_slideManager->next(); });
    connect(m_slidePlayBtn, &QPushButton::clicked, this, [this]() {
        if (m_slideManager->isPlaying()) { m_slideManager->pause(); m_slidePlayBtn->setText(tr("Play")); }
        else { m_slideManager->play(); m_slidePlayBtn->setText(tr("Pausa")); }
    });

    slidesLayout->addLayout(slideCtrlLayout);
    leftVBox->addWidget(m_slidesSection);

    leftVBox->addStretch();

    m_leftScroll->setWidget(leftContent);
    leftLayout->addWidget(m_leftScroll);

    m_splitter->addWidget(m_leftPanel);

    // === CENTER - SVG VIEW ===
    m_svgView = new SvgView;
    m_splitter->addWidget(m_svgView);

    // === RIGHT PANEL ===
    m_elementPanel = new ElementPanel;
    m_splitter->addWidget(m_elementPanel);

    // Splitter sizes
    m_splitter->setSizes({320, 800, 220});

    connect(m_svgView, &SvgView::elementClicked, this, &MainWindow::onElementSelected);
    connect(m_elementPanel, &ElementPanel::elementClicked, this, &MainWindow::onElementSelected);

    connect(m_wsManager, &WorkspaceManager::workspaceSwitched, this, &MainWindow::onWorkspaceSwitched);
}

void MainWindow::setupMenuBar() {
    auto *fileMenu = menuBar()->addMenu(tr("Archivo"));

    auto *openAct = fileMenu->addAction(tr("Abrir SVG..."));
    openAct->setShortcut(QKeySequence::Open);
    connect(openAct, &QAction::triggered, this, [this]() {
        QString path = QFileDialog::getOpenFileName(this, tr("Abrir SVG"), QString(), tr("SVG (*.svg)"));
        if (!path.isEmpty()) loadSvgFile(path);
    });

    fileMenu->addSeparator();

    auto *exportAct = fileMenu->addAction(tr("Exportar SVG Animado"));
    connect(exportAct, &QAction::triggered, this, &MainWindow::exportAnimatedSvg);

    fileMenu->addSeparator();

    auto *exitAct = fileMenu->addAction(tr("Salir"));
    exitAct->setShortcut(QKeySequence::Quit);
    connect(exitAct, &QAction::triggered, this, &QMainWindow::close);

    auto *editMenu = menuBar()->addMenu(tr("Editar"));
    auto *undoAct = editMenu->addAction(tr("Deshacer"));
    undoAct->setShortcut(QKeySequence::Undo);
    connect(undoAct, &QAction::triggered, this, &MainWindow::undo);

    auto *redoAct = editMenu->addAction(tr("Rehacer"));
    redoAct->setShortcut(QKeySequence::Redo);
    connect(redoAct, &QAction::triggered, this, &MainWindow::redo);

    auto *viewMenu = menuBar()->addMenu(tr("Vista"));
    auto *zoomInAct = viewMenu->addAction(tr("Zoom +"));
    zoomInAct->setShortcut(QKeySequence::ZoomIn);
    connect(zoomInAct, &QAction::triggered, this, [this]() { m_svgView->setZoom(m_svgView->zoomLevel() * 1.3); });

    auto *zoomOutAct = viewMenu->addAction(tr("Zoom -"));
    zoomOutAct->setShortcut(QKeySequence::ZoomOut);
    connect(zoomOutAct, &QAction::triggered, this, [this]() { m_svgView->setZoom(m_svgView->zoomLevel() / 1.3); });

    auto *zoomFitAct = viewMenu->addAction(tr("Ajustar"));
    zoomFitAct->setShortcut(Qt::Key_0);
    connect(zoomFitAct, &QAction::triggered, this, [this]() { m_svgView->setZoom(1.0); });
}

void MainWindow::setupShortcuts() {
    auto *spaceShortcut = new QShortcut(Qt::Key_Space, this);
    connect(spaceShortcut, &QShortcut::activated, this, [this]() {
        if (m_animationPlaying) { m_animationPlaying = false; m_svgView->pauseAnimations(); }
        else { m_animationPlaying = true; applyCurrentAnimation(); }
    });
}

void MainWindow::loadSvgFile(const QString &path) {
    if (m_svgView->loadSvgFile(path)) {
        m_elementConfigs.clear();
        m_selectedElementIndex = -1;
        refreshElementList();
        m_presetsSection->setVisible(true);
        m_controlsSection->setVisible(true);
        m_exportSection->setVisible(true);
        m_historySection->setVisible(true);
        m_slidesSection->setVisible(true);
        statusBar()->showMessage(tr("SVG cargado: %1").arg(path));
    }
}

void MainWindow::loadSvgString(const QString &str) {
    if (m_svgView->loadSvgString(str)) {
        m_elementConfigs.clear();
        m_selectedElementIndex = -1;
        refreshElementList();
        m_presetsSection->setVisible(true);
        m_controlsSection->setVisible(true);
        m_exportSection->setVisible(true);
        m_historySection->setVisible(true);
        m_slidesSection->setVisible(true);
    }
}

void MainWindow::loadShape(const QString &name) {
    QString svg = Shapes::svgByName(name);
    if (!svg.isEmpty()) loadSvgString(svg);
}

void MainWindow::onElementSelected(int index) {
    m_selectedElementIndex = index;
    m_elementPanel->setSelectedIndex(index);

    if (index >= 0 && m_elementConfigs.contains(index)) {
        m_controls->loadConfig(m_elementConfigs[index]);
    }
}

void MainWindow::onPresetToggled(const QString &presetId) {
    if (m_selectedElementIndex < 0) {
        // Select first element
        auto elements = m_svgView->svgElements();
        if (!elements.isEmpty()) {
            m_selectedElementIndex = 0;
            m_elementPanel->setSelectedIndex(0);
        } else {
            return;
        }
    }

    pushUndoState();

    if (!m_elementConfigs.contains(m_selectedElementIndex)) {
        m_elementConfigs[m_selectedElementIndex] = AnimationEngine::defaultConfig();
    }

    auto &cfg = m_elementConfigs[m_selectedElementIndex];

    // Toggle logic
    if (cfg.presetId == presetId) {
        if (!cfg.extraPresets.isEmpty()) {
            cfg.presetId = cfg.extraPresets.takeFirst();
        } else {
            cfg.presetId.clear();
        }
    } else if (cfg.extraPresets.contains(presetId)) {
        cfg.extraPresets.removeAll(presetId);
    } else {
        if (!cfg.presetId.isEmpty()) {
            cfg.extraPresets.append(cfg.presetId);
        }
        cfg.presetId = presetId;
    }

    m_controls->loadConfig(cfg);
    applyCurrentAnimation();
    refreshElementList();
}

void MainWindow::onConfigChanged(const ElementConfig &cfg) {
    if (m_selectedElementIndex < 0) return;
    m_elementConfigs[m_selectedElementIndex] = cfg;
    applyCurrentAnimation();
}

void MainWindow::applyCurrentAnimation() {
    if (m_selectedElementIndex < 0 || !m_elementConfigs.contains(m_selectedElementIndex)) return;

    auto elements = m_svgView->svgElements();
    if (m_selectedElementIndex >= elements.size()) return;

    auto *item = elements[m_selectedElementIndex];
    auto rect = item->boundingRect();
    m_animEngine->applyAnimation(item, m_elementConfigs[m_selectedElementIndex], rect);
}

void MainWindow::applyAllAnimations() {
    auto elements = m_svgView->svgElements();
    QMap<int, QRectF> bounds;
    for (int i = 0; i < elements.size(); i++) {
        bounds[i] = elements[i]->boundingRect();
    }
    QMap<int, QGraphicsItem *> items;
    for (int i = 0; i < elements.size(); i++) {
        items[i] = elements[i];
    }
    m_animEngine->applyAllAnimations(items, m_elementConfigs, bounds);
}

void MainWindow::refreshElementList() {
    auto elements = m_svgView->svgElements();
    QList<QString> names;
    QMap<int, QString> colors;

    for (int i = 0; i < elements.size(); i++) {
        names.append(QString("Elemento %1").arg(i + 1));
        if (m_elementConfigs.contains(i) && !m_elementConfigs[i].presetId.isEmpty()) {
            auto *p = AnimationEngine::presetById(m_elementConfigs[i].presetId);
            if (p) colors[i] = p->color.name();
        }
    }

    m_elementPanel->setElements(names, colors);
    m_elementPanel->setSelectedIndex(m_selectedElementIndex);
}

void MainWindow::pushUndoState() {
    if (m_undoIndex < m_undoStates.size() - 1) {
        m_undoStates = m_undoStates.mid(0, m_undoIndex + 1);
    }
    m_undoStates.append(m_elementConfigs);
    if (m_undoStates.size() > MAX_UNDO) m_undoStates.removeFirst();
    m_undoIndex = m_undoStates.size() - 1;
}

void MainWindow::undo() {
    if (m_undoIndex <= 0) return;
    m_undoIndex--;
    m_elementConfigs = m_undoStates[m_undoIndex];
    refreshElementList();
    applyAllAnimations();
}

void MainWindow::redo() {
    if (m_undoIndex >= m_undoStates.size() - 1) return;
    m_undoIndex++;
    m_elementConfigs = m_undoStates[m_undoIndex];
    refreshElementList();
    applyAllAnimations();
}

void MainWindow::onWorkspaceSwitched(const QString &id) {
    auto *ws = m_wsManager->workspace(id);
    if (!ws) return;
    if (!ws->svgString.isEmpty()) {
        loadSvgString(ws->svgString);
        m_elementConfigs = ws->elementConfigs;
        m_svgView->setZoom(ws->zoomLevel);
        m_svgView->setBoundaryVisible(ws->boundaryActive);
        refreshElementList();
    }
}

void MainWindow::addCurrentAsSlide() {
    QString svg = m_svgView->currentSvgString();
    if (svg.isEmpty()) return;
    m_slideManager->addSlide(QString(), svg);
    refreshSlideList();
}

void MainWindow::refreshSlideList() {
    m_slideListWidget->clear();
    for (int i = 0; i < m_slideManager->slideCount(); i++) {
        m_slideListWidget->addItem(m_slideManager->slide(i).name);
    }
}

void MainWindow::exportAnimatedSvg() {
    QString svg = m_svgView->currentSvgString();
    if (svg.isEmpty()) return;

    QString result = m_exportManager->exportSvg(svg, m_elementConfigs);
    if (result.isEmpty()) return;

    QString path = QFileDialog::getSaveFileName(this, tr("Exportar SVG"), "animated.svg", tr("SVG (*.svg)"));
    if (path.isEmpty()) return;

    if (m_exportManager->saveToFile(result, path)) {
        statusBar()->showMessage(tr("Exportado: %1").arg(path));
    } else {
        QMessageBox::warning(this, tr("Error"), tr("No se pudo guardar el archivo"));
    }
}
