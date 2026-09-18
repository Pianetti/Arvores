#include "dataset_generator.hpp"
#include <random>
#include <algorithm>
#include <cmath>

namespace DatasetGenerator {

std::vector<std::string> generateRandomStrings(
    size_t count, 
    size_t minLen, 
    size_t maxLen, 
    const std::string& alphabet,
    uint32_t seed
) {
    std::mt19937 rng(seed);
    std::uniform_int_distribution<size_t> lenDist(minLen, maxLen);
    std::uniform_int_distribution<size_t> charDist(0, alphabet.size() - 1);

    std::vector<std::string> result;
    result.reserve(count);

    for (size_t i = 0; i < count; ++i) {
        size_t len = lenDist(rng);
        std::string s;
        s.reserve(len);
        for (size_t j = 0; j < len; ++j) {
            s += alphabet[charDist(rng)];
        }
        result.push_back(std::move(s));
    }
    return result;
}

std::vector<std::string> generatePrefixDenseStrings(
    size_t count, 
    size_t numPrefixes, 
    size_t prefixLen, 
    size_t suffixLen,
    uint32_t seed
) {
    std::mt19937 rng(seed);
    const std::string alphabet = "abcdefghijklmnopqrstuvwxyz";
    std::uniform_int_distribution<size_t> charDist(0, alphabet.size() - 1);

    std::vector<std::string> prefixes;
    prefixes.reserve(numPrefixes);
    for (size_t i = 0; i < numPrefixes; ++i) {
        std::string p;
        p.reserve(prefixLen);
        for (size_t j = 0; j < prefixLen; ++j) {
            p += alphabet[charDist(rng)];
        }
        prefixes.push_back(p);
    }

    std::uniform_int_distribution<size_t> prefixDist(0, numPrefixes - 1);
    std::vector<std::string> result;
    result.reserve(count);

    for (size_t i = 0; i < count; ++i) {
        std::string s = prefixes[prefixDist(rng)];
        for (size_t j = 0; j < suffixLen; ++j) {
            s += alphabet[charDist(rng)];
        }
        result.push_back(std::move(s));
    }
    return result;
}

std::vector<int> generateRandomInts(size_t count, int minVal, int maxVal, uint32_t seed) {
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> dist(minVal, maxVal);

    std::vector<int> result;
    result.reserve(count);
    for (size_t i = 0; i < count; ++i) {
        result.push_back(dist(rng));
    }
    return result;
}

std::vector<int> generateSortedInts(size_t count, int start, int step) {
    std::vector<int> result;
    result.reserve(count);
    int curr = start;
    for (size_t i = 0; i < count; ++i) {
        result.push_back(curr);
        curr += step;
    }
    return result;
}

std::vector<int> generateReverseSortedInts(size_t count, int start, int step) {
    std::vector<int> result;
    result.reserve(count);
    int curr = start;
    for (size_t i = 0; i < count; ++i) {
        result.push_back(curr);
        curr -= step;
    }
    return result;
}

std::vector<size_t> generateZipfianIndices(size_t count, size_t domainSize, double alpha, uint32_t seed) {
    std::mt19937 rng(seed);
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    std::vector<double> cdf(domainSize);
    double sum = 0.0;
    for (size_t i = 1; i <= domainSize; ++i) {
        sum += 1.0 / std::pow(static_cast<double>(i), alpha);
        cdf[i - 1] = sum;
    }
    for (size_t i = 0; i < domainSize; ++i) {
        cdf[i] /= sum;
    }

    std::vector<size_t> result;
    result.reserve(count);
    for (size_t i = 0; i < count; ++i) {
        double r = dist(rng);
        auto it = std::lower_bound(cdf.begin(), cdf.end(), r);
        size_t idx = std::distance(cdf.begin(), it);
        if (idx >= domainSize) idx = domainSize - 1;
        result.push_back(idx);
    }
    return result;
}

std::vector<Point2D> generateRandomPoints2D(size_t count, double minCoord, double maxCoord, uint32_t seed) {
    std::mt19937 rng(seed);
    std::uniform_real_distribution<double> dist(minCoord, maxCoord);

    std::vector<Point2D> result;
    result.reserve(count);
    for (size_t i = 0; i < count; ++i) {
        result.push_back({dist(rng), dist(rng)});
    }
    return result;
}

std::vector<Point3D> generateRandomPoints3D(size_t count, double minCoord, double maxCoord, uint32_t seed) {
    std::mt19937 rng(seed);
    std::uniform_real_distribution<double> dist(minCoord, maxCoord);

    std::vector<Point3D> result;
    result.reserve(count);
    for (size_t i = 0; i < count; ++i) {
        result.push_back({dist(rng), dist(rng), dist(rng)});
    }
    return result;
}

} // namespace DatasetGenerator
