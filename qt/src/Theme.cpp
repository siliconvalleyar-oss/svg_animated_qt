#include "Theme.h"

QString Theme::stylesheet() {
    return QStringLiteral(R"(
        * { box-sizing: border-box; }
        QWidget {
            background-color: %1;
            color: %4;
            font-family: 'Segoe UI', system-ui, sans-serif;
            font-size: 12px;
        }
        QMainWindow { background-color: %1; }
        QSplitter::handle { background-color: %3; width: 1px; }

        /* Scrollbars */
        QScrollBar:vertical { background: %1; width: 6px; }
        QScrollBar::handle:vertical { background: %3; border-radius: 3px; min-height: 20px; }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0; }
        QScrollBar:horizontal { background: %1; height: 6px; }
        QScrollBar::handle:horizontal { background: %3; border-radius: 3px; min-width: 20px; }
        QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal { width: 0; }

        /* Buttons */
        QPushButton {
            background-color: %2;
            color: %4;
            border: 1px solid %3;
            border-radius: 6px;
            padding: 6px 10px;
            font-size: 11px;
        }
        QPushButton:hover { border-color: %6; background-color: rgba(108,92,231,40); }
        QPushButton:pressed { background-color: rgba(108,92,231,60); }
        QPushButton:disabled { opacity: 0.4; color: %5; }

        /* Primary buttons */
        QPushButton[primary="true"] {
            background-color: %6;
            color: white;
            border: none;
            font-weight: 600;
        }
        QPushButton[primary="true"]:hover { background-color: %7; }

        /* Sliders */
        QSlider::groove:horizontal {
            height: 4px;
            background: %3;
            border-radius: 2px;
        }
        QSlider::handle:horizontal {
            width: 14px; height: 14px;
            margin: -5px 0;
            background: %6;
            border-radius: 7px;
        }
        QSlider::sub-page:horizontal { background: %6; border-radius: 2px; }

        /* Labels */
        QLabel { background: transparent; }
        QLabel[dim="true"] { color: %5; font-size: 11px; }

        /* Section titles */
        QLabel[section="true"] {
            color: %5;
            font-size: 9px;
            text-transform: uppercase;
            letter-spacing: 1px;
            font-weight: 600;
            padding: 4px 0;
        }

        /* Tab widget */
        QTabWidget::pane { border: 1px solid %3; background: %2; border-radius: 4px; }
        QTabBar::tab {
            background: %2;
            color: %5;
            border: 1px solid %3;
            border-radius: 4px;
            padding: 4px 10px;
            margin-right: 2px;
            font-size: 10px;
        }
        QTabBar::tab:selected {
            background: %6;
            border-color: %6;
            color: white;
        }
        QTabBar::tab:hover:!selected { background: %3; color: %4; }

        /* List views */
        QListWidget, QTreeWidget {
            background: %2;
            border: 1px solid %3;
            border-radius: 6px;
            padding: 4px;
            outline: none;
        }
        QListWidget::item, QTreeWidget::item {
            padding: 6px 8px;
            border-radius: 4px;
            border: 1px solid transparent;
        }
        QListWidget::item:hover, QTreeWidget::item:hover {
            border-color: %6;
            background: rgba(108,92,231,25);
        }
        QListWidget::item:selected, QTreeWidget::item:selected {
            border-color: %6;
            background: rgba(108,92,231,40);
            color: %6;
        }

        /* Combo boxes */
        QComboBox {
            background: %2;
            color: %4;
            border: 1px solid %3;
            border-radius: 4px;
            padding: 4px 8px;
            font-size: 10px;
        }
        QComboBox:hover { border-color: %6; }
        QComboBox::drop-down { border: none; width: 16px; }
        QComboBox QAbstractItemView {
            background: %2;
            color: %4;
            border: 1px solid %3;
            selection-background-color: %6;
        }

        /* Spin box */
        QSpinBox, QDoubleSpinBox {
            background: %2;
            color: %4;
            border: 1px solid %3;
            border-radius: 4px;
            padding: 2px 4px;
            font-size: 10px;
        }
        QSpinBox:focus, QDoubleSpinBox:focus { border-color: %6; }

        /* Input fields */
        QLineEdit {
            background: %2;
            color: %4;
            border: 1px solid %3;
            border-radius: 4px;
            padding: 4px 8px;
        }
        QLineEdit:focus { border-color: %6; }

        /* Menu */
        QMenu {
            background: %2;
            color: %4;
            border: 1px solid %3;
            border-radius: 8px;
            padding: 4px;
        }
        QMenu::item { padding: 8px 18px; border-radius: 4px; }
        QMenu::item:selected { background: %6; color: white; }

        /* Tooltips */
        QToolTip {
            background: %2;
            color: %4;
            border: 1px solid %3;
            padding: 4px 8px;
            border-radius: 4px;
            font-size: 11px;
        }

        /* Splitter */
        QSplitter::handle:hover { background: %6; }

        /* Status bar */
        QStatusBar { background: %2; border-top: 1px solid %3; color: %5; font-size: 10px; }
    )")
    .arg(bg().name())           // %1
    .arg(surface().name())      // %2
    .arg(border().name())       // %3
    .arg(text().name())         // %4
    .arg(textDim().name())      // %5
    .arg(accent().name())       // %6
    .arg(accent().lighter(1.2).name()); // %7
}
