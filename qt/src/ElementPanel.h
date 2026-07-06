#pragma once
#include <QWidget>
#include <QListWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QMap>
#include "AnimationEngine.h"

class ElementPanel : public QWidget {
    Q_OBJECT
public:
    explicit ElementPanel(QWidget *parent = nullptr);

    void setElements(const QList<QString> &names, const QMap<int, QString> &presetColors);
    void setSelectedIndex(int index);
    void setGroupInfo(const QMap<int, QString> &groupNames, const QMap<int, QColor> &groupColors);

signals:
    void elementClicked(int index);
    void elementVisibilityToggled(int index, bool visible);
    void groupClicked(const QString &groupId);
    void createGroupRequested();
    void deleteGroupRequested(const QString &groupId);

private:
    void setupUi();

    QListWidget *m_listWidget;
    QLabel *m_titleLabel;
    QPushButton *m_groupBtn;
    QPushButton *m_createGroupBtn;

    QList<QString> m_elementNames;
    QMap<int, QString> m_presetColors;
    int m_selectedIndex = -1;
};
