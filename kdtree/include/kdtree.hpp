#pragma once
#include <vector>
#include <fstream>
#include <string>
#include <cmath>
#include <limits>
#include "metrics.hpp"

struct KDPoint {
    std::vector<double> coords;

    KDPoint() = default;
    KDPoint(std::initializer_list<double> list) : coords(list) {}
    KDPoint(const std::vector<double>& c) : coords(c) {}

    size_t dim() const { return coords.size(); }
    double operator[](size_t i) const { return coords[i]; }
    double& operator[](size_t i) { return coords[i]; }

    bool operator==(const KDPoint& other) const {
        if (coords.size() != other.coords.size()) return false;
        for (size_t i = 0; i < coords.size(); ++i) {
            if (std::abs(coords[i] - other.coords[i]) > 1e-9) return false;
        }
        return true;
    }

    double distanceSquared(const KDPoint& other) const {
        double distSq = 0.0;
        for (size_t i = 0; i < coords.size(); ++i) {
            double d = coords[i] - other.coords[i];
            distSq += d * d;
        }
        return distSq;
    }

    double distance(const KDPoint& other) const {
        return std::sqrt(distanceSquared(other));
    }
};

struct KDNode {
    KDPoint point;
    size_t axis; // Dimensão de partição deste nó (0 = X, 1 = Y, etc.)
    KDNode* left;
    KDNode* right;

    KDNode(const KDPoint& pt, size_t ax) 
        : point(pt), axis(ax), left(nullptr), right(nullptr) {}
    ~KDNode() = default;
};

class KDTree {
private:
    KDNode* root;
    size_t dimension;
    size_t node_count;
    Metrics metrics;

    KDNode* insertHelper(KDNode* node, const KDPoint& pt, size_t depth, bool& inserted);
    KDNode* findMinNode(KDNode* node, size_t targetDim, size_t depth);
    KDNode* minNode(KDNode* a, KDNode* b, KDNode* c, size_t targetDim);
    KDNode* removeHelper(KDNode* node, const KDPoint& pt, size_t depth, bool& removed);

    void clearHelper(KDNode* node);
    void rangeSearchHelper(KDNode* node, const KDPoint& lower, const KDPoint& upper, 
                           std::vector<KDPoint>& results) const;
    void nearestNeighborHelper(KDNode* node, const KDPoint& target, 
                               KDPoint& bestPoint, double& bestDistSq) const;
    void exportDotHelper(std::ofstream& out, KDNode* node, size_t& nextId) const;

public:
    KDTree(size_t k = 2);
    ~KDTree();

    // Fundamental operations
    bool insert(const KDPoint& pt);
    bool search(const KDPoint& pt);
    bool remove(const KDPoint& pt);

    // Specific spatial operations
    std::vector<KDPoint> rangeSearch(const KDPoint& lower, const KDPoint& upper) const;
    KDPoint nearestNeighbor(const KDPoint& target) const;

    // Utility & Metrics
    size_t size() const { return node_count; }
    size_t dim() const { return dimension; }
    bool empty() const { return root == nullptr; }
    void clear();

    const Metrics& getMetrics() const { return metrics; }
    void resetMetrics() { metrics.reset(); }

    // Visualization
    bool exportDot(const std::string& filename, const std::string& graphTitle = "KD-Tree") const;
    bool exportPartitionSvg2D(const std::string& filename, double minX, double maxX, double minY, double maxY) const;
};
