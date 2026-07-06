#pragma once
#include <QObject>
#include <QList>
#include <QTimer>

struct Slide {
    QString name;
    QString svgStr;
};

class SlideshowManager : public QObject {
    Q_OBJECT
public:
    explicit SlideshowManager(QObject *parent = nullptr);

    void addSlide(const QString &name, const QString &svgStr);
    void removeSlide(int index);
    void moveSlide(int from, int to);

    int slideCount() const { return m_slides.size(); }
    const Slide &slide(int index) const { return m_slides[index]; }
    int currentIndex() const { return m_currentIndex; }

    void goTo(int index);
    void next();
    void previous();
    void play();
    void pause();
    void stop();
    bool isPlaying() const { return m_playing; }

    void setTransition(const QString &type) { m_transition = type; }
    QString transition() const { return m_transition; }
    void setSlideDuration(double seconds) { m_slideDuration = seconds; }
    double slideDuration() const { return m_slideDuration; }
    void setTransitionSpeed(double seconds) { m_transitionSpeed = seconds; }
    double transitionSpeed() const { return m_transitionSpeed; }

signals:
    void slideChanged(int index);
    void playStateChanged(bool playing);

private:
    QList<Slide> m_slides;
    int m_currentIndex = -1;
    bool m_playing = false;
    QString m_transition = "fade";
    double m_slideDuration = 3.0;
    double m_transitionSpeed = 0.6;
    QTimer *m_timer;
};
