#include <QApplication>
#include <QFont>
#include "MainWindow.h"
#include "Theme.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("SVG Animator");
    app.setOrganizationName("SVGAnimator");
    app.setApplicationVersion("1.0.0");

    QFont defaultFont("Segoe UI", 10);
    defaultFont.setStyleHint(QFont::SansSerif);
    app.setFont(defaultFont);

    app.setStyleSheet(Theme::stylesheet());

    MainWindow window;
    window.show();

    return app.exec();
}
