#include "MarkdownTool.h"
#include "MarkdownEditor.h"

void MarkdownTool::activate(QWidget *) {
    auto *editor = new MarkdownEditor;
    editor->setWindowFlags(Qt::Window);
    editor->show();
    editor->activateWindow();
}
