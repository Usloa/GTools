#pragma once
#include <QWidget>
#include <QLineEdit>
#include <QListWidget>
#include <QVBoxLayout>

class LauncherWindow : public QWidget {
    Q_OBJECT
public:
    explicit LauncherWindow(QWidget *parent = nullptr);

    void showLauncher();
    void hideLauncher();

protected:
    bool event(QEvent *e) override;
    void keyPressEvent(QKeyEvent *e) override;

private slots:
    void onInputChanged(const QString &text);
    void onItemActivated(QListWidgetItem *item);

private:
    void updateResults(const QString &query);

    QLineEdit *m_input;
    QListWidget *m_results;
    QVBoxLayout *m_layout;
};
