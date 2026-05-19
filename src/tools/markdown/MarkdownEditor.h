#pragma once
#include <QWidget>
#include <QPlainTextEdit>
#include <QTextBrowser>
#include <QTimer>

class MarkdownHighlighter;

class MarkdownEditor : public QWidget {
    Q_OBJECT
public:
    explicit MarkdownEditor(QWidget *parent = nullptr);

    void openFile(const QString &path);
    void saveFile();

private:
    void updatePreview();
    void setupToolbar();

    QPlainTextEdit *m_editor;
    QTextBrowser *m_preview;
    MarkdownHighlighter *m_highlighter;
    QTimer *m_updateTimer;
    QString m_currentFile;
};
