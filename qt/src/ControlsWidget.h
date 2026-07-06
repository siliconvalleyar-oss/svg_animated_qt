#pragma once
#include <QWidget>
#include <QSlider>
#include <QLabel>
#include <QPushButton>
#include <QButtonGroup>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include "AnimationEngine.h"

class ControlsWidget : public QWidget {
    Q_OBJECT
public:
    explicit ControlsWidget(QWidget *parent = nullptr);

    ElementConfig currentConfig() const;
    void loadConfig(const ElementConfig &cfg);
    void setPresetActive(const QString &presetId, bool active);

signals:
    void configChanged(const ElementConfig &cfg);
    void presetToggled(const QString &presetId);
    void playClicked();
    void pauseClicked();
    void stopClicked();

private:
    void setupUi();
    void updateDisplayValues();

    // Preset buttons
    QGridLayout *m_presetGrid;
    QMap<QString, QPushButton *> m_presetButtons;
    QButtonGroup *m_presetGroup;

    // Controls
    QSlider *m_speedSlider;
    QLabel *m_speedValue;
    QSlider *m_delaySlider;
    QLabel *m_delayValue;

    QButtonGroup *m_iterGroup;
    QButtonGroup *m_dirGroup;

    QSlider *m_directionSlider;
    QLabel *m_directionValue;
    QWidget *m_directionPresetsWidget;

    QSlider *m_ovalRxSlider, *m_ovalRySlider, *m_ovalAngleSlider;
    QLabel *m_ovalRxVal, *m_ovalRyVal, *m_ovalAngleVal;
    QWidget *m_ovalControls;

    QSlider *m_arcRxSlider, *m_arcRySlider;
    QLabel *m_arcRxVal, *m_arcRyVal;
    QWidget *m_arcControls;

    QWidget *m_directionControls;

    QPushButton *m_playBtn, *m_pauseBtn, *m_stopBtn;

    ElementConfig m_config;
};
