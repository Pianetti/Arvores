#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

namespace Visualizer {

inline std::string escapeLabel(const std::string& str) {
    std::string escaped;
    for (char c : str) {
        if (c == '"') escaped += "\\\"";
        else if (c == '\\') escaped += "\\\\";
        else if (c == '\n') escaped += "\\n";
        else escaped += c;
    }
    return escaped;
}

inline void writeDotHeader(std::ofstream& out, const std::string& graphName = "Tree") {
    out << "digraph " << graphName << " {\n";
    out << "    rankdir=TB;\n";
    out << "    node [fontname=\"Helvetica\", fontsize=11, shape=circle, style=\"filled\", fillcolor=\"#F8FAFC\", color=\"#334155\", penwidth=1.5];\n";
    out << "    edge [fontname=\"Helvetica\", fontsize=10, color=\"#64748B\", penwidth=1.2];\n\n";
}

inline void writeDotFooter(std::ofstream& out) {
    out << "}\n";
}

} // namespace Visualizer
