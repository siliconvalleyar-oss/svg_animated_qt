#pragma once
#include <QObject>
#include <QMap>
#include <QPointF>
#include <QColor>

struct Trajectory {
    QString id;
    QString name;
    QColor color;
    QList<QPointF> points;
};

class TrajectoryManager : public QObject {
    Q_OBJECT
public:
    explicit TrajectoryManager(QObject *parent = nullptr);

    QString addTrajectory(const QString &name = QString());
    void removeTrajectory(const QString &id);
    Trajectory *trajectory(const QString &id);
    QList<Trajectory *> allTrajectories();
    QStringList trajectoryIds() const;

    void setSelected(const QString &id) { m_selectedId = id; }
    QString selectedId() const { return m_selectedId; }

    void setEditing(bool editing) { m_editing = editing; }
    bool isEditing() const { return m_editing; }

    static const QList<QColor> &trajectoryColors();

signals:
    void trajectoryAdded(const QString &id);
    void trajectoryRemoved(const QString &id);
    void trajectoryModified(const QString &id);

private:
    QMap<QString, Trajectory> m_trajectories;
    QString m_selectedId;
    bool m_editing = false;
    int m_nextNum = 1;
};
