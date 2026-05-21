#pragma once
#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QKeyEvent>

class ToolButton : public QPushButton {
    Q_OBJECT
public:
    explicit ToolButton(QWidget *parent = nullptr) : QPushButton(parent) {}

protected:
    void keyPressEvent(QKeyEvent *e) override {
        // 忽略空格键，防止自动触发
        if (e->key() == Qt::Key_Space) {
            e->ignore();
            return;
        }
        QPushButton::keyPressEvent(e);
    }
};

class ToolPanel : public QWidget {
    Q_OBJECT
public:
    explicit ToolPanel(QWidget *parent = nullptr);

signals:
    void toolActivated(const QString &toolId);
    void settingsRequested();

protected:
    bool event(QEvent *e) override;

private:
    void setupUI();
    void addToolButton(const QString &id, const QString &name, const QString &iconPath);

    QGridLayout *m_gridLayout;
    int m_currentRow = 0;
    int m_currentCol = 0;
};
