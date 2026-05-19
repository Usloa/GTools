#include "ToolRegistry.h"

ToolRegistry &ToolRegistry::instance() {
    static ToolRegistry reg;
    return reg;
}

void ToolRegistry::registerTool(std::unique_ptr<Tool> tool) {
    m_tools.push_back(std::move(tool));
}

Tool *ToolRegistry::findById(const QString &id) const {
    for (auto &t : m_tools) {
        if (t->id() == id) return t.get();
    }
    return nullptr;
}

std::vector<Tool *> ToolRegistry::search(const QString &query) const {
    std::vector<Tool *> results;
    if (query.isEmpty()) {
        for (auto &t : m_tools) results.push_back(t.get());
        return results;
    }
    for (auto &t : m_tools) {
        if (t->name().contains(query, Qt::CaseInsensitive) ||
            t->keyword().contains(query, Qt::CaseInsensitive)) {
            results.push_back(t.get());
        }
    }
    return results;
}
