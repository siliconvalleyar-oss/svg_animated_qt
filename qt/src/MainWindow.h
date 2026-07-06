#pragma once
#include <QMainWindow>
#include <QSplitter>
#include <QTabWidget>
#include <QPushButton>
#include <QScrollArea>
#include <QLabel>
#include <QMap>
#include <QUndoStack>
#include "SvgView.h"
#include "ElementPanel.h"
#include "ControlsWidget.h"
#include "AnimationEngine.h"
#include "WorkspaceManager.h"
#include "ExportManager.h"
#include "TrajectoryManager.h"
#include "SlideshowManager.h"
#include "BackgroundImageManager.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    void setupUi();
    void setupMenuBar();
    void setupLeftPanel();
    void setupRightPanel();
    void setupShortcuts();

    void loadSvgFile(const QString &path);
    void loadSvgString(const QString &str);
    void loadShape(const QString &name);

    void onElementSelected(int index);
    void onPresetToggled(const QString &presetId);
    void onConfigChanged(const ElementConfig &cfg);
    void applyCurrentAnimation();
    void applyAllAnimations();
    void refreshElementList();

    // Undo/Redo
    void pushUndoState();
    void undo();
    void redo();

    // Workspace
    void onWorkspaceSwitched(const QString &id);

    // Slideshow
    void addCurrentAsSlide();
    void refreshSlideList();

    // Export
    void exportAnimatedSvg();

    // Widgets
    QSplitter *m_splitter;
    QTabWidget *m_workspaceTabs;
    QWidget *m_leftPanel;
    SvgView *m_svgView;
    ElementPanel *m_elementPanel;
    ControlsWidget *m_controls;

    // Managers
    AnimationEngine *m_animEngine;
    WorkspaceManager *m_wsManager;
    ExportManager *m_exportManager;
    TrajectoryManager *m_trajManager;
    SlideshowManager *m_slideManager;
    BackgroundImageManager *m_bgManager;

    // State
    QMap<int, ElementConfig> m_elementConfigs;
    int m_selectedElementIndex = -1;
    bool m_animationPlaying = true;

    // Undo
    QUndoStack *m_undoStack;
    QList<QMap<int, ElementConfig>> m_undoStates;
    int m_undoIndex = -1;
    static const int MAX_UNDO = 50;

    // Left panel widgets (need access for show/hide)
    QScrollArea *m_leftScroll;
    QWidget *m_presetsSection;
    QWidget *m_controlsSection;
    QWidget *m_exportSection;
    QWidget *m_historySection;
    QWidget *m_slidesSection;
    QWidget *m_shapesSection;

    // Slideshow widgets
    QListWidget *m_slideListWidget;
    QPushButton *m_slidePlayBtn;
};
