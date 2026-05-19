#pragma once
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QGroupBox>

class SettingsWindow : public QWidget {
    Q_OBJECT
public:
    explicit SettingsWindow(QWidget *parent = nullptr);

private slots:
    void onSaveClicked();

private:
    void setupUI();
    void loadSettings();

    QLineEdit *m_baiduApiKey;
    QLineEdit *m_baiduSecretKey;
    QLineEdit *m_tencentSecretId;
    QLineEdit *m_tencentSecretKey;
    QPushButton *m_saveBtn;
    QLabel *m_statusLabel;
};
