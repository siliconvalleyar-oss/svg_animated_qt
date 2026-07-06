#pragma once
#include <QObject>
#include <QTabWidget>
#include <QWidget>
#include <QMap>
#include "AnimationEngine.h"

struct WorkspaceData {
    QString name;
    QString svgString;
    QMap<int, ElementConfig> elementConfigs;
    QMap<int, QString> groupIds; // element index -> group id
    struct GroupData { QString name; QColor color; QList<int> elements; ElementConfig config; };
    QMap<QString, GroupData> groups;
    double zoomLevel = 1.0;
    bool boundaryActive = false;
    QString filePath;
};

class WorkspaceManager : public QObject {
    Q_OBJECT
public:
    explicit WorkspaceManager(QTabWidget *tabWidget, QObject *parent = nullptr);

    QString createWorkspace(const QString &name = QString());
    void closeWorkspace(const QString &id);
    void switchTo(const QString &id);
    void renameWorkspace(const QString &id, const QString &newName);

    QString activeWorkspaceId() const { return m_activeId; }
    WorkspaceData *activeWorkspace();
    WorkspaceData *workspace(const QString &id);

    void saveActiveState(const QString &svgStr, const QMap<int, ElementConfig> &configs,
                         double zoom, bool boundary, const QString &filePath);
    void restoreWorkspaceState(const QString &id);

    QStringList allWorkspaceIds() const { return m_workspaces.keys(); }

signals:
    void workspaceCreated(const QString &id);
    void workspaceClosed(const QString &id);
    void workspaceSwitched(const QString &id);
    void workspaceRenamed(const QString &id, const QString &name);

private:
    void rebuildTabs();

    QTabWidget *m_tabWidget;
    QMap<QString, WorkspaceData> m_workspaces;
    QString m_activeId;
    int m_nextNum = 1;
};
