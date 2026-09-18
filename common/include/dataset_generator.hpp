#pragma once
#include <string>
#include <vector>
#include <cstdint>

struct Point2D {
    double x, y;
};

struct Point3D {
    double x, y, z;
};

namespace DatasetGenerator {

std::vector<std::string> generateRandomStrings(
    size_t count, 
    size_t minLen = 4, 
    size_t maxLen = 12, 
    const std::string& alphabet = "abcdefghijklmnopqrstuvwxyz",
    uint32_t seed = 42
);

std::vector<std::string> generatePrefixDenseStrings(
    size_t count, 
    size_t numPrefixes = 5, 
    size_t prefixLen = 4, 
    size_t suffixLen = 6,
    uint32_t seed = 42
);

std::vector<int> generateRandomInts(size_t count, int minVal = 1, int maxVal = 1000000, uint32_t seed = 42);

std::vector<int> generateSortedInts(size_t count, int start = 1, int step = 1);

std::vector<int> generateReverseSortedInts(size_t count, int start = 1000000, int step = 1);

std::vector<size_t> generateZipfianIndices(size_t count, size_t domainSize, double alpha = 1.0, uint32_t seed = 42);

std::vector<Point2D> generateRandomPoints2D(size_t count, double minCoord = 0.0, double maxCoord = 1000.0, uint32_t seed = 42);

std::vector<Point3D> generateRandomPoints3D(size_t count, double minCoord = 0.0, double maxCoord = 1000.0, uint32_t seed = 42);

} // namespace DatasetGenerator
