#pragma once
#include <QWidget>
#include <QLineEdit>
#include <QListWidget>
#include <QVBoxLayout>
#include <QToolButton>

class FloatingBall;

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
    void onCollapseToFloat();
    void onExpandFromFloat();

private:
    void updateResults(const QString &query);

    QLineEdit *m_input;
    QToolButton *m_menuBtn;
    QListWidget *m_results;
    QVBoxLayout *m_layout;
    FloatingBall *m_floatingBall;
};
