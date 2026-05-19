#pragma once
#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

class LogWindow : public QWidget {
    Q_OBJECT
public:
    explicit LogWindow(QWidget *parent = nullptr);

private slots:
    void onLogAdded();
    void onClearClicked();
    void onRefreshClicked();

private:
    void setupUI();
    void refreshLogs();
    QString levelColor(const QString& level) const;

    QTableWidget *m_table;
    QPushButton *m_clearBtn;
    QPushButton *m_refreshBtn;
};
