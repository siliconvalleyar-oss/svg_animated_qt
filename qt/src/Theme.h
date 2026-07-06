#pragma once
#include <QColor>
#include <QString>

namespace Theme {
    // Colors matching web app
    inline constexpr int BG_R = 15, BG_G = 17, BG_B = 23;
    inline constexpr int SURFACE_R = 26, SURFACE_G = 29, SURFACE_B = 39;
    inline constexpr int SURFACE2_R = 36, SURFACE2_G = 39, SURFACE2_B = 52;
    inline constexpr int BORDER_R = 46, BORDER_G = 50, BORDER_B = 69;
    inline constexpr int TEXT_R = 228, TEXT_G = 230, TEXT_B = 240;
    inline constexpr int TEXT_DIM_R = 139, TEXT_DIM_G = 143, TEXT_DIM_B = 167;
    inline constexpr int ACCENT_R = 108, ACCENT_G = 92, ACCENT_B = 231;
    inline constexpr int DANGER_R = 231, DANGER_G = 76, DANGER_B = 60;
    inline constexpr int SUCCESS_R = 46, SUCCESS_G = 204, SUCCESS_B = 113;

    inline QColor bg() { return QColor(BG_R, BG_G, BG_B); }
    inline QColor surface() { return QColor(SURFACE_R, SURFACE_G, SURFACE_B); }
    inline QColor surface2() { return QColor(SURFACE2_R, SURFACE2_G, SURFACE2_B); }
    inline QColor border() { return QColor(BORDER_R, BORDER_G, BORDER_B); }
    inline QColor text() { return QColor(TEXT_R, TEXT_G, TEXT_B); }
    inline QColor textDim() { return QColor(TEXT_DIM_R, TEXT_DIM_G, TEXT_DIM_B); }
    inline QColor accent() { return QColor(ACCENT_R, ACCENT_G, ACCENT_B); }
    inline QColor danger() { return QColor(DANGER_R, DANGER_G, DANGER_B); }
    inline QColor success() { return QColor(SUCCESS_R, SUCCESS_G, SUCCESS_B); }

    QString stylesheet();
}
