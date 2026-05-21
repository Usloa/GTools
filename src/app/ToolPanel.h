#pragma once
#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>

class ToolPanel : public QWidget {
    Q_OBJECT
public:
    explicit ToolPanel(QWidget *parent = nullptr);

signals:
    void toolActivated(const QString &toolId);

protected:
    bool event(QEvent *e) override;

private:
    void setupUI();
    void addToolButton(const QString &id, const QString &name, const QString &iconPath);

    QGridLayout *m_gridLayout;
    int m_currentRow = 0;
    int m_currentCol = 0;
};
