#pragma once
#include <vector>
#include <memory>
#include "Tool.h"

class ToolRegistry {
public:
    static ToolRegistry &instance();

    void registerTool(std::unique_ptr<Tool> tool);
    const std::vector<std::unique_ptr<Tool>> &tools() const { return m_tools; }
    Tool *findById(const QString &id) const;
    std::vector<Tool *> search(const QString &query) const;

private:
    ToolRegistry() = default;
    std::vector<std::unique_ptr<Tool>> m_tools;
};
