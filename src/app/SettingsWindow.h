#pragma once
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QCheckBox>

class SettingsWindow : public QWidget {
    Q_OBJECT
public:
    explicit SettingsWindow(QWidget *parent = nullptr);

private slots:
    void onSaveClicked();

private:
    void setupUI();
    void setupApiTab();
    void setupGeneralTab();
    void setupPreferenceTab();
    void setupAdvancedTab();
    void loadSettings();

    QTabWidget *m_tabWidget;
    QPushButton *m_saveBtn;
    QLabel *m_statusLabel;

    QLineEdit *m_baiduApiKey;
    QLineEdit *m_baiduSecretKey;
    QLineEdit *m_tencentSecretId;
    QLineEdit *m_tencentSecretKey;

    QLineEdit *m_hotkeyShowLauncher;
    QLineEdit *m_hotkeyDetachWindow;

    QCheckBox *m_startOnBoot;
};
