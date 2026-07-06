#include "ElementPanel.h"
#include "Theme.h"
#include <QScrollArea>

ElementPanel::ElementPanel(QWidget *parent) : QWidget(parent) {
    setupUi();
}

void ElementPanel::setupUi() {
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Header
    auto *header = new QWidget;
    auto *headerLayout = new QHBoxLayout(header);
    headerLayout->setContentsMargins(12, 8, 12, 4);

    m_titleLabel = new QLabel(tr("ELEMENTOS SVG"));
    m_titleLabel->setProperty("section", true);
    headerLayout->addWidget(m_titleLabel);
    headerLayout->addStretch();

    mainLayout->addWidget(header);

    // Group controls
    auto *groupBar = new QWidget;
    auto *groupBarLayout = new QHBoxLayout(groupBar);
    groupBarLayout->setContentsMargins(12, 4, 12, 4);
    groupBarLayout->setSpacing(4);

    m_groupBtn = new QPushButton(tr("Seleccionar"));
    m_groupBtn->setCheckable(true);
    groupBarLayout->addWidget(m_groupBtn);

    m_createGroupBtn = new QPushButton(tr("Grupo +"));
    m_createGroupBtn->setVisible(false);
    groupBarLayout->addWidget(m_createGroupBtn);

    mainLayout->addWidget(groupBar);

    connect(m_groupBtn, &QPushButton::clicked, this, [this]() {
        m_createGroupBtn->setVisible(m_groupBtn->isChecked());
    });
    connect(m_createGroupBtn, &QPushButton::clicked, this, &ElementPanel::createGroupRequested);

    // Element list
    m_listWidget = new QListWidget;
    m_listWidget->setSpacing(4);
    mainLayout->addWidget(m_listWidget);

    connect(m_listWidget, &QListWidget::itemClicked, this, [this](QListWidgetItem *item) {
        int idx = item->data(Qt::UserRole).toInt();
        if (idx == m_selectedIndex) {
            m_selectedIndex = -1;
            m_listWidget->clearSelection();
        } else {
            m_selectedIndex = idx;
            emit elementClicked(idx);
        }
    });
}

void ElementPanel::setElements(const QList<QString> &names, const QMap<int, QString> &presetColors) {
    m_elementNames = names;
    m_presetColors = presetColors;
    m_listWidget->clear();

    for (int i = 0; i < names.size(); i++) {
        auto *item = new QListWidgetItem;
        QString colorStr = presetColors.contains(i) ? presetColors[i] : "";
        QString colorDot = colorStr.isEmpty() ? "" :
            QString("<span style='display:inline-block;width:8px;height:8px;border-radius:50px;background:%1;margin-right:6px;'></span>").arg(colorStr);
        item->setText(colorStr.isEmpty() ? names[i] : names[i]);
        item->setData(Qt::UserRole, i);

        auto *widget = new QWidget;
        auto *wLayout = new QHBoxLayout(widget);
        wLayout->setContentsMargins(4, 4, 4, 4);
        wLayout->setSpacing(6);

        if (!colorStr.isEmpty()) {
            auto *dot = new QLabel;
            dot->setPixmap(QPixmap(8, 8));
            dot->setStyleSheet(QString("background:%1;border-radius:4px;").arg(colorStr));
            dot->setFixedSize(8, 8);
            wLayout->addWidget(dot);
        }

        auto *nameLbl = new QLabel(names[i]);
        nameLbl->setStyleSheet("font-size:11px;");
        wLayout->addWidget(nameLbl);
        wLayout->addStretch();

        auto *visBtn = new QPushButton(QChar(128065)); // eye
        visBtn->setFixedSize(20, 20);
        visBtn->setStyleSheet("border:none;font-size:12px;");
        visBtn->setProperty("index", i);
        connect(visBtn, &QPushButton::clicked, this, [this, i](bool checked) {
            Q_UNUSED(checked)
            emit elementVisibilityToggled(i, true);
        });
        wLayout->addWidget(visBtn);

        item->setSizeHint(widget->sizeHint());
        m_listWidget->addItem(item);
        m_listWidget->setItemWidget(item, widget);
    }
}

void ElementPanel::setSelectedIndex(int index) {
    m_selectedIndex = index;
    for (int i = 0; i < m_listWidget->count(); i++) {
        auto *item = m_listWidget->item(i);
        item->setSelected(item->data(Qt::UserRole).toInt() == index);
    }
}

void ElementPanel::setGroupInfo(const QMap<int, QString> &groupNames, const QMap<int, QColor> &groupColors) {
    // Add group info to existing items
    for (int i = 0; i < m_listWidget->count(); i++) {
        auto *item = m_listWidget->item(i);
        int idx = item->data(Qt::UserRole).toInt();
        auto *widget = m_listWidget->itemWidget(item);
        if (!widget) continue;

        // Find group for this element
        for (auto it = groupNames.constBegin(); it != groupNames.constEnd(); ++it) {
            if (it.key() == idx) {
                auto *wLayout = qobject_cast<QHBoxLayout *>(widget->layout());
                if (wLayout && wLayout->count() > 1) {
                    auto *badge = new QLabel(it.value());
                    badge->setStyleSheet(QString("background:%1;color:white;font-size:8px;padding:1px 4px;border-radius:3px;").arg(groupColors.value(it.key(), QColor("#666")).name()));
                    wLayout->insertWidget(wLayout->count() - 1, badge);
                }
                break;
            }
        }
    }
}
