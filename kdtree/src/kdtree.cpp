#include "kdtree.hpp"
#include "visualizer.hpp"
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <algorithm>

KDTree::KDTree(size_t k) : root(nullptr), dimension(k), node_count(0) {}

KDTree::~KDTree() {
    clear();
}

void KDTree::clearHelper(KDNode* node) {
    if (!node) return;
    clearHelper(node->left);
    clearHelper(node->right);
    delete node;
    metrics.node_deallocations++;
}

void KDTree::clear() {
    clearHelper(root);
    root = nullptr;
    node_count = 0;
}

KDNode* KDTree::insertHelper(KDNode* node, const KDPoint& pt, size_t depth, bool& inserted) {
    if (!node) {
        inserted = true;
        metrics.node_allocations++;
        return new KDNode(pt, depth % dimension);
    }

    if (node->point == pt) {
        inserted = false;
        return node;
    }

    size_t axis = depth % dimension;
    metrics.comparisons++;

    if (pt[axis] < node->point[axis]) {
        node->left = insertHelper(node->left, pt, depth + 1, inserted);
    } else {
        node->right = insertHelper(node->right, pt, depth + 1, inserted);
    }

    return node;
}

bool KDTree::insert(const KDPoint& pt) {
    if (pt.dim() != dimension) {
        throw std::invalid_argument("Dimensão do ponto incompatível com a KD-Tree");
    }
    bool inserted = false;
    root = insertHelper(root, pt, 0, inserted);
    if (inserted) node_count++;
    return inserted;
}

bool KDTree::search(const KDPoint& pt) {
    if (pt.dim() != dimension) return false;

    KDNode* curr = root;
    size_t depth = 0;

    while (curr) {
        metrics.comparisons++;
        if (curr->point == pt) return true;

        size_t axis = depth % dimension;
        if (pt[axis] < curr->point[axis]) {
            curr = curr->left;
        } else {
            curr = curr->right;
        }
        depth++;
    }

    return false;
}

KDNode* KDTree::minNode(KDNode* a, KDNode* b, KDNode* c, size_t targetDim) {
    KDNode* res = a;
    if (b && (!res || b->point[targetDim] < res->point[targetDim])) {
        res = b;
    }
    if (c && (!res || c->point[targetDim] < res->point[targetDim])) {
        res = c;
    }
    return res;
}

KDNode* KDTree::findMinNode(KDNode* node, size_t targetDim, size_t depth) {
    if (!node) return nullptr;

    size_t axis = depth % dimension;

    if (axis == targetDim) {
        if (!node->left) return node;
        return findMinNode(node->left, targetDim, depth + 1);
    }

    return minNode(
        node,
        findMinNode(node->left, targetDim, depth + 1),
        findMinNode(node->right, targetDim, depth + 1),
        targetDim
    );
}

KDNode* KDTree::removeHelper(KDNode* node, const KDPoint& pt, size_t depth, bool& removed) {
    if (!node) {
        removed = false;
        return nullptr;
    }

    size_t axis = depth % dimension;

    if (node->point == pt) {
        removed = true;
        if (node->right) {
            KDNode* minR = findMinNode(node->right, axis, depth + 1);
            node->point = minR->point;
            node->right = removeHelper(node->right, minR->point, depth + 1, removed);
        } else if (node->left) {
            KDNode* minL = findMinNode(node->left, axis, depth + 1);
            node->point = minL->point;
            node->right = removeHelper(node->left, minL->point, depth + 1, removed);
            node->left = nullptr; // Subárvore esquerda transferida para a direita
        } else {
            delete node;
            metrics.node_deallocations++;
            return nullptr;
        }
        return node;
    }

    metrics.comparisons++;
    if (pt[axis] < node->point[axis]) {
        node->left = removeHelper(node->left, pt, depth + 1, removed);
    } else {
        node->right = removeHelper(node->right, pt, depth + 1, removed);
    }

    return node;
}

bool KDTree::remove(const KDPoint& pt) {
    if (pt.dim() != dimension) return false;
    bool removed = false;
    root = removeHelper(root, pt, 0, removed);
    if (removed) node_count--;
    return removed;
}

void KDTree::rangeSearchHelper(KDNode* node, const KDPoint& lower, const KDPoint& upper, 
                               std::vector<KDPoint>& results) const {
    if (!node) return;

    // Verifica se o ponto do nó está dentro do hiper-retângulo
    bool inside = true;
    for (size_t i = 0; i < dimension; ++i) {
        if (node->point[i] < lower[i] || node->point[i] > upper[i]) {
            inside = false;
            break;
        }
    }
    if (inside) {
        results.push_back(node->point);
    }

    size_t axis = node->axis;
    if (lower[axis] <= node->point[axis]) {
        rangeSearchHelper(node->left, lower, upper, results);
    }
    if (upper[axis] >= node->point[axis]) {
        rangeSearchHelper(node->right, lower, upper, results);
    }
}

std::vector<KDPoint> KDTree::rangeSearch(const KDPoint& lower, const KDPoint& upper) const {
    std::vector<KDPoint> results;
    rangeSearchHelper(root, lower, upper, results);
    return results;
}

void KDTree::nearestNeighborHelper(KDNode* node, const KDPoint& target, 
                                   KDPoint& bestPoint, double& bestDistSq) const {
    if (!node) return;

    double dSq = node->point.distanceSquared(target);
    if (dSq < bestDistSq) {
        bestDistSq = dSq;
        bestPoint = node->point;
    }

    size_t axis = node->axis;
    double diff = target[axis] - node->point[axis];

    KDNode* first = (diff < 0) ? node->left : node->right;
    KDNode* second = (diff < 0) ? node->right : node->left;

    // Explora primeiro a subárvore mais próxima
    nearestNeighborHelper(first, target, bestPoint, bestDistSq);

    // Poda branch-and-bound: só visita o outro lado se a hiperesfera intersectar o plano de corte
    if ((diff * diff) < bestDistSq) {
        nearestNeighborHelper(second, target, bestPoint, bestDistSq);
    }
}

KDPoint KDTree::nearestNeighbor(const KDPoint& target) const {
    if (!root) throw std::runtime_error("KD-Tree vazia");
    KDPoint bestPoint = root->point;
    double bestDistSq = root->point.distanceSquared(target);
    nearestNeighborHelper(root, target, bestPoint, bestDistSq);
    return bestPoint;
}

void KDTree::exportDotHelper(std::ofstream& out, KDNode* node, size_t& nextId) const {
    if (!node) return;

    size_t currId = nextId++;
    const char* axisNames[] = {"X", "Y", "Z", "W"};
    std::string axisStr = (node->axis < 4) ? axisNames[node->axis] : ("D" + std::to_string(node->axis));

    std::ostringstream ptLabel;
    ptLabel << "(";
    for (size_t i = 0; i < dimension; ++i) {
        ptLabel << std::fixed << std::setprecision(1) << node->point[i] << (i + 1 < dimension ? ", " : "");
    }
    ptLabel << ")";

    std::string fillColor = (node->axis % 2 == 0) ? "#E0E7FF" : "#FEF3C7";
    std::string strokeColor = (node->axis % 2 == 0) ? "#4338CA" : "#D97706";

    out << "    n" << currId << " [shape=record, style=filled, fillcolor=\"" << fillColor 
        << "\", color=\"" << strokeColor << "\", label=\"{ " << ptLabel.str() 
        << " | corte: " << axisStr << " }\"];\n";

    if (node->left) {
        size_t leftId = nextId;
        out << "    n" << currId << " -> n" << leftId << " [label=\" < \", color=\"#2563EB\"];\n";
        exportDotHelper(out, node->left, nextId);
    }
    if (node->right) {
        size_t rightId = nextId;
        out << "    n" << currId << " -> n" << rightId << " [label=\" >= \", color=\"#DC2626\"];\n";
        exportDotHelper(out, node->right, nextId);
    }
}

bool KDTree::exportDot(const std::string& filename, const std::string& graphTitle) const {
    std::ofstream out(filename);
    if (!out.is_open()) return false;

    Visualizer::writeDotHeader(out, "KDTree");
    out << "    labelloc=\"t\";\n";
    out << "    label=\"" << Visualizer::escapeLabel(graphTitle) << "\";\n";
    out << "    fontsize=14;\n\n";

    if (root) {
        size_t nextId = 0;
        exportDotHelper(out, root, nextId);
    }

    Visualizer::writeDotFooter(out);
    return true;
}

static void drawSvgPartition(std::ofstream& out, KDNode* node, 
                             double minX, double maxX, double minY, double maxY,
                             double scaleX, double scaleY, double pad) {
    if (!node) return;

    double px = pad + (node->point[0] - minX) * scaleX;
    double py = pad + (maxY - node->point[1]) * scaleY; // Inverte Y no SVG

    if (node->axis == 0) { // Corte vertical em X
        double lineX = px;
        double lineY1 = pad + (maxY - maxY) * scaleY;
        double lineY2 = pad + (maxY - minY) * scaleY;
        out << "  <line x1=\"" << lineX << "\" y1=\"" << lineY1 
            << "\" x2=\"" << lineX << "\" y2=\"" << lineY2 
            << "\" stroke=\"#2563EB\" stroke-width=\"2\" stroke-dasharray=\"4\" />\n";

        drawSvgPartition(out, node->left, minX, node->point[0], minY, maxY, scaleX, scaleY, pad);
        drawSvgPartition(out, node->right, node->point[0], maxX, minY, maxY, scaleX, scaleY, pad);
    } else { // Corte horizontal em Y
        double lineY = py;
        double lineX1 = pad + (minX - minX) * scaleX;
        double lineX2 = pad + (maxX - minX) * scaleX;
        out << "  <line x1=\"" << lineX1 << "\" y1=\"" << lineY 
            << "\" x2=\"" << lineX2 << "\" y2=\"" << lineY 
            << "\" stroke=\"#DC2626\" stroke-width=\"2\" stroke-dasharray=\"4\" />\n";

        drawSvgPartition(out, node->left, minX, maxX, minY, node->point[1], scaleX, scaleY, pad);
        drawSvgPartition(out, node->right, minX, maxX, node->point[1], maxY, scaleX, scaleY, pad);
    }

    // Desenha o ponto
    out << "  <circle cx=\"" << px << "\" cy=\"" << py << "\" r=\"5\" fill=\"#1E293B\" stroke=\"#FFFFFF\" stroke-width=\"2\" />\n";
    out << "  <text x=\"" << (px + 7) << "\" y=\"" << (py - 7) 
        << "\" font-family=\"Helvetica, sans-serif\" font-size=\"11\" fill=\"#0F172A\">(" 
        << (int)node->point[0] << "," << (int)node->point[1] << ")</text>\n";
}

bool KDTree::exportPartitionSvg2D(const std::string& filename, double minX, double maxX, double minY, double maxY) const {
    std::ofstream out(filename);
    if (!out.is_open()) return false;

    double pad = 40.0;
    double width = 500.0;
    double height = 500.0;
    double scaleX = (width - 2 * pad) / (maxX - minX);
    double scaleY = (height - 2 * pad) / (maxY - minY);

    out << "<svg width=\"" << width << "\" height=\"" << height << "\" xmlns=\"http://www.w3.org/2000/svg\">\n";
    out << "  <rect width=\"100%\" height=\"100%\" fill=\"#F8FAFC\" />\n";
    out << "  <rect x=\"" << pad << "\" y=\"" << pad << "\" width=\"" << (width - 2 * pad) 
        << "\" height=\"" << (height - 2 * pad) << "\" fill=\"#FFFFFF\" stroke=\"#94A3B8\" stroke-width=\"2\" />\n";

    drawSvgPartition(out, root, minX, maxX, minY, maxY, scaleX, scaleY, pad);

    out << "</svg>\n";
    return true;
}
