#include "MarkdownHighlighter.h"

MarkdownHighlighter::MarkdownHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent) {

    QTextCharFormat headingFmt;
    headingFmt.setFontWeight(QFont::Bold);
    headingFmt.setForeground(QColor("#1a73e8"));
    m_rules.append({QRegularExpression("^#{1,6}\\s.*$"), headingFmt});

    QTextCharFormat boldFmt;
    boldFmt.setFontWeight(QFont::Bold);
    m_rules.append({QRegularExpression("\\*\\*(.+?)\\*\\*"), boldFmt});
    m_rules.append({QRegularExpression("__(.+?)__"), boldFmt});

    QTextCharFormat italicFmt;
    italicFmt.setFontItalic(true);
    m_rules.append({QRegularExpression("\\*(.+?)\\*"), italicFmt});
    m_rules.append({QRegularExpression("_(.+?)_"), italicFmt});

    QTextCharFormat codeFmt;
    codeFmt.setFontFamilies({"Consolas"});
    codeFmt.setForeground(QColor("#d63384"));
    codeFmt.setBackground(QColor("#f8f9fa"));
    m_rules.append({QRegularExpression("`[^`]+`"), codeFmt});

    QTextCharFormat codeBlockFmt;
    codeBlockFmt.setFontFamilies({"Consolas"});
    codeBlockFmt.setForeground(QColor("#6c757d"));
    m_rules.append({QRegularExpression("^```.*$"), codeBlockFmt});

    QTextCharFormat linkFmt;
    linkFmt.setForeground(QColor("#0d6efd"));
    linkFmt.setFontUnderline(true);
    m_rules.append({QRegularExpression("\\[.+?\\]\\(.+?\\)"), linkFmt});

    QTextCharFormat listFmt;
    listFmt.setForeground(QColor("#e65100"));
    m_rules.append({QRegularExpression("^\\s*[-*+]\\s"), listFmt});
    m_rules.append({QRegularExpression("^\\s*\\d+\\.\\s"), listFmt});

    QTextCharFormat quoteFmt;
    quoteFmt.setForeground(QColor("#6c757d"));
    quoteFmt.setFontItalic(true);
    m_rules.append({QRegularExpression("^>\\s.*$"), quoteFmt});
}

void MarkdownHighlighter::highlightBlock(const QString &text) {
    for (const auto &rule : m_rules) {
        auto it = rule.pattern.globalMatch(text);
        while (it.hasNext()) {
            auto match = it.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
}
