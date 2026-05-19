#include "MarkdownEditor.h"
#include "MarkdownHighlighter.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QToolBar>
#include <QAction>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

MarkdownEditor::MarkdownEditor(QWidget *parent) : QWidget(parent) {
    setWindowTitle(QString::fromUtf8("Markdown 编辑器"));
    setMinimumSize(900, 600);
    setAttribute(Qt::WA_DeleteOnClose);

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    setupToolbar();

    auto *splitter = new QSplitter(Qt::Horizontal, this);

    m_editor = new QPlainTextEdit;
    m_editor->setFont(QFont("Consolas", 11));
    m_editor->setTabStopDistance(28);
    m_editor->setLineWrapMode(QPlainTextEdit::WidgetWidth);
    m_highlighter = new MarkdownHighlighter(m_editor->document());

    m_preview = new QTextBrowser;
    m_preview->setOpenExternalLinks(true);

    splitter->addWidget(m_editor);
    splitter->addWidget(m_preview);
    splitter->setSizes({450, 450});
    mainLayout->addWidget(splitter);

    m_updateTimer = new QTimer(this);
    m_updateTimer->setSingleShot(true);
    m_updateTimer->setInterval(300);
    connect(m_updateTimer, &QTimer::timeout, this, &MarkdownEditor::updatePreview);
    connect(m_editor, &QPlainTextEdit::textChanged, [this]() {
        m_updateTimer->start();
    });
}

void MarkdownEditor::setupToolbar() {
    auto *toolbar = new QToolBar(this);
    toolbar->setMovable(false);

    auto *newAct = toolbar->addAction(QString::fromUtf8("新建"));
    connect(newAct, &QAction::triggered, [this]() {
        m_editor->clear();
        m_currentFile.clear();
        setWindowTitle(QString::fromUtf8("Markdown 编辑器"));
    });

    auto *openAct = toolbar->addAction(QString::fromUtf8("打开"));
    connect(openAct, &QAction::triggered, [this]() {
        QString path = QFileDialog::getOpenFileName(this,
            QString::fromUtf8("打开 Markdown 文件"), QString(),
            "Markdown (*.md *.markdown);;All Files (*)");
        if (!path.isEmpty()) openFile(path);
    });

    auto *saveAct = toolbar->addAction(QString::fromUtf8("保存"));
    connect(saveAct, &QAction::triggered, this, &MarkdownEditor::saveFile);

    static_cast<QVBoxLayout *>(layout())->addWidget(toolbar);
}

void MarkdownEditor::updatePreview() {
    m_preview->setMarkdown(m_editor->toPlainText());
}

void MarkdownEditor::openFile(const QString &path) {
    QFile file(path);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        m_editor->setPlainText(in.readAll());
        m_currentFile = path;
        setWindowTitle(path);
    }
}

void MarkdownEditor::saveFile() {
    if (m_currentFile.isEmpty()) {
        m_currentFile = QFileDialog::getSaveFileName(this,
            QString::fromUtf8("保存 Markdown 文件"), QString(),
            "Markdown (*.md);;All Files (*)");
        if (m_currentFile.isEmpty()) return;
    }
    QFile file(m_currentFile);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << m_editor->toPlainText();
        setWindowTitle(m_currentFile);
    } else {
        QMessageBox::warning(this, "Error",
            QString::fromUtf8("无法保存文件: ") + file.errorString());
    }
}
