#include "SlideshowManager.h"
#include <QtMath>

SlideshowManager::SlideshowManager(QObject *parent)
    : QObject(parent)
    , m_timer(new QTimer(this)) {
    connect(m_timer, &QTimer::timeout, this, &SlideshowManager::next);
}

void SlideshowManager::addSlide(const QString &name, const QString &svgStr) {
    Slide s;
    s.name = name.isEmpty() ? QString("Slide %1").arg(m_slides.size() + 1) : name;
    s.svgStr = svgStr;
    m_slides.append(s);
}

void SlideshowManager::removeSlide(int index) {
    if (index < 0 || index >= m_slides.size()) return;
    m_slides.removeAt(index);
    if (m_currentIndex >= m_slides.size()) m_currentIndex = m_slides.size() - 1;
}

void SlideshowManager::moveSlide(int from, int to) {
    if (from < 0 || from >= m_slides.size() || to < 0 || to >= m_slides.size()) return;
    m_slides.move(from, to);
    if (m_currentIndex == from) m_currentIndex = to;
}

void SlideshowManager::goTo(int index) {
    if (index < 0 || index >= m_slides.size()) return;
    m_currentIndex = index;
    emit slideChanged(index);
}

void SlideshowManager::next() {
    if (m_slides.isEmpty()) return;
    goTo((m_currentIndex + 1) % m_slides.size());
}

void SlideshowManager::previous() {
    if (m_slides.isEmpty()) return;
    goTo((m_currentIndex - 1 + m_slides.size()) % m_slides.size());
}

void SlideshowManager::play() {
    if (m_slides.size() < 2) return;
    m_playing = true;
    m_timer->start(static_cast<int>(m_slideDuration * 1000));
    emit playStateChanged(true);
}

void SlideshowManager::pause() {
    m_playing = false;
    m_timer->stop();
    emit playStateChanged(false);
}

void SlideshowManager::stop() {
    m_playing = false;
    m_timer->stop();
    m_currentIndex = -1;
    emit playStateChanged(false);
}
