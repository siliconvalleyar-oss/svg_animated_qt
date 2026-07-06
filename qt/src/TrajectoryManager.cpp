#include "TrajectoryManager.h"

static const QList<QColor> TRAJ_COLORS = {
    QColor("#f39c12"), QColor("#e74c3c"), QColor("#2ecc71"), QColor("#3498db"),
    QColor("#9b59b6"), QColor("#1abc9c"), QColor("#e67e22"), QColor("#6c5ce7"),
};

TrajectoryManager::TrajectoryManager(QObject *parent) : QObject(parent) {}

const QList<QColor> &TrajectoryManager::trajectoryColors() { return TRAJ_COLORS; }

QString TrajectoryManager::addTrajectory(const QString &name) {
    QString trajName = name.isEmpty() ? QString("Trayectoria %1").arg(m_nextNum) : name;
    QString id = QString("traj_%1").arg(m_nextNum++);

    Trajectory traj;
    traj.id = id;
    traj.name = trajName;
    traj.color = TRAJ_COLORS[(m_nextNum - 2) % TRAJ_COLORS.size()];
    traj.points = {
        QPointF(30, 100), QPointF(55, 60), QPointF(100, 40),
        QPointF(145, 60), QPointF(170, 100)
    };

    m_trajectories[id] = traj;
    m_selectedId = id;
    m_editing = true;

    emit trajectoryAdded(id);
    return id;
}

void TrajectoryManager::removeTrajectory(const QString &id) {
    if (!m_trajectories.contains(id)) return;
    m_trajectories.remove(id);
    if (m_selectedId == id) { m_selectedId.clear(); m_editing = false; }
    emit trajectoryRemoved(id);
}

Trajectory *TrajectoryManager::trajectory(const QString &id) {
    return m_trajectories.contains(id) ? &m_trajectories[id] : nullptr;
}

QList<Trajectory *> TrajectoryManager::allTrajectories() {
    QList<Trajectory *> result;
    for (auto it = m_trajectories.begin(); it != m_trajectories.end(); ++it) {
        result.append(&it.value());
    }
    return result;
}

QStringList TrajectoryManager::trajectoryIds() const {
    return m_trajectories.keys();
}
