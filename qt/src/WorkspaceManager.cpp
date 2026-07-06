#include "WorkspaceManager.h"
#include <QInputDialog>
#include <QMessageBox>

WorkspaceManager::WorkspaceManager(QTabWidget *tabWidget, QObject *parent)
    : QObject(parent), m_tabWidget(tabWidget) {
    // Create default workspace
    createWorkspace(tr("Espacio 1"));

    connect(m_tabWidget, &QTabWidget::tabCloseRequested, this, [this](int index) {
        QString id = m_tabWidget->tabData(index).toString();
        closeWorkspace(id);
    });
    connect(m_tabWidget, &QTabWidget::currentChanged, this, [this](int index) {
        if (index >= 0) {
            QString id = m_tabWidget->tabData(index).toString();
            if (id != m_activeId) switchTo(id);
        }
    });
}

QString WorkspaceManager::createWorkspace(const QString &name) {
    QString wsName = name.isEmpty() ? tr("Espacio %1").arg(m_nextNum++) : name;
    QString id = QString("ws_%1_%2").arg(QDateTime::currentMSecsSinceEpoch()).arg(qrand() % 10000);

    WorkspaceData ws;
    ws.name = wsName;
    m_workspaces[id] = ws;

    m_tabWidget->addTab(new QWidget, wsName);
    m_tabWidget->setTabData(m_tabWidget->count() - 1, id);
    m_tabWidget->setTabToolTip(m_tabWidget->count() - 1, wsName);

    if (m_activeId.isEmpty()) m_activeId = id;

    emit workspaceCreated(id);
    return id;
}

void WorkspaceManager::closeWorkspace(const QString &id) {
    if (m_workspaces.size() <= 1) return;

    if (QMessageBox::question(m_tabWidget, tr("Cerrar espacio"),
        tr("Cerrar '%1'?").arg(m_workspaces[id].name)) != QMessageBox::Yes)
        return;

    for (int i = 0; i < m_tabWidget->count(); i++) {
        if (m_tabWidget->tabData(i).toString() == id) {
            m_tabWidget->removeTab(i);
            break;
        }
    }
    m_workspaces.remove(id);

    if (m_activeId == id) {
        m_activeId = m_workspaces.isEmpty() ? QString() : m_workspaces.constBegin().key();
        if (!m_activeId.isEmpty()) {
            for (int i = 0; i < m_tabWidget->count(); i++) {
                if (m_tabWidget->tabData(i).toString() == m_activeId) {
                    m_tabWidget->setCurrentIndex(i);
                    break;
                }
            }
        }
    }

    emit workspaceClosed(id);
}

void WorkspaceManager::switchTo(const QString &id) {
    if (id == m_activeId || !m_workspaces.contains(id)) return;
    m_activeId = id;

    for (int i = 0; i < m_tabWidget->count(); i++) {
        if (m_tabWidget->tabData(i).toString() == id) {
            m_tabWidget->setCurrentIndex(i);
            break;
        }
    }

    emit workspaceSwitched(id);
}

void WorkspaceManager::renameWorkspace(const QString &id, const QString &newName) {
    if (!m_workspaces.contains(id)) return;
    m_workspaces[id].name = newName;

    for (int i = 0; i < m_tabWidget->count(); i++) {
        if (m_tabWidget->tabData(i).toString() == id) {
            m_tabWidget->setTabText(i, newName);
            break;
        }
    }

    emit workspaceRenamed(id, newName);
}

WorkspaceData *WorkspaceManager::activeWorkspace() {
    return m_workspaces.contains(m_activeId) ? &m_workspaces[m_activeId] : nullptr;
}

WorkspaceData *WorkspaceManager::workspace(const QString &id) {
    return m_workspaces.contains(id) ? &m_workspaces[id] : nullptr;
}

void WorkspaceManager::saveActiveState(const QString &svgStr, const QMap<int, ElementConfig> &configs,
                                        double zoom, bool boundary, const QString &filePath) {
    auto *ws = activeWorkspace();
    if (!ws) return;
    ws->svgString = svgStr;
    ws->elementConfigs = configs;
    ws->zoomLevel = zoom;
    ws->boundaryActive = boundary;
    ws->filePath = filePath;
}

void WorkspaceManager::restoreWorkspaceState(const QString &id) {
    Q_UNUSED(id)
    // State restoration handled by the caller
}

void WorkspaceManager::rebuildTabs() {
    // Not needed since we manage tabs directly
}
