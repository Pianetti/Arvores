#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>

#include "timer.hpp"
#include "dataset_generator.hpp"
#include "trie.hpp"
#include "patricia.hpp"
#include "splay_tree.hpp"
#include "bst.hpp"
#include "avl.hpp"
#include "treap.hpp"
#include "kdtree.hpp"

void runTrieVsPatricia() {
    std::cout << "\n=======================================================\n";
    std::cout << "[EXPERIMENTO 1] Trie vs. Patricia (Strings e Prefixos)\n";
    std::cout << "=======================================================\n";

    std::ofstream csv("experimentos/data/trie_vs_patricia.csv");
    csv << "tipo_dado,N,estrutura,tempo_insercao_ms,tempo_busca_ms,total_nos,memoria_estimada_kb,comparacoes\n";

    std::vector<size_t> sizes = {1000, 5000, 10000, 25000, 50000};
    std::vector<std::string> types = {"aleatorio", "prefixo_denso"};

    for (const auto& type : types) {
        for (size_t N : sizes) {
            std::vector<std::string> words;
            if (type == "aleatorio") {
                words = DatasetGenerator::generateRandomStrings(N, 5, 12, "abcdefghijklmnopqrstuvwxyz", 42);
            } else {
                words = DatasetGenerator::generatePrefixDenseStrings(N, 10, 5, 6, 42);
            }

            // Benchmark Trie
            {
                Trie trie;
                Timer t;
                for (const auto& w : words) {
                    trie.insert(w);
                }
                double insTime = t.elapsed_milliseconds();

                t.reset();
                for (const auto& w : words) {
                    trie.search(w);
                }
                double searchTime = t.elapsed_milliseconds();

                // Memória estimada: nós * (sizeof(TrieNode) + overhead de unordered_map)
                double memKB = (trie.nodeCount() * (sizeof(TrieNode) + 32)) / 1024.0;

                csv << type << "," << N << ",Trie,"
                    << insTime << "," << searchTime << ","
                    << trie.nodeCount() << "," << memKB << ","
                    << trie.getMetrics().comparisons << "\n";

                std::cout << "[" << std::setw(13) << type << "] N=" << std::setw(5) << N 
                          << " | Trie     -> Ins: " << std::setw(7) << insTime << "ms | Busca: " 
                          << std::setw(7) << searchTime << "ms | Nós: " << std::setw(8) << trie.nodeCount() << "\n";
            }

            // Benchmark Patricia
            {
                PatriciaTree pat;
                Timer t;
                for (const auto& w : words) {
                    pat.insert(w);
                }
                double insTime = t.elapsed_milliseconds();

                t.reset();
                for (const auto& w : words) {
                    pat.search(w);
                }
                double searchTime = t.elapsed_milliseconds();

                double memKB = (pat.nodeCount() * (sizeof(PatriciaNode) + 32)) / 1024.0;

                csv << type << "," << N << ",Patricia,"
                    << insTime << "," << searchTime << ","
                    << pat.nodeCount() << "," << memKB << ","
                    << pat.getMetrics().comparisons << "\n";

                std::cout << "[" << std::setw(13) << type << "] N=" << std::setw(5) << N 
                          << " | Patricia -> Ins: " << std::setw(7) << insTime << "ms | Busca: " 
                          << std::setw(7) << searchTime << "ms | Nós: " << std::setw(8) << pat.nodeCount() << "\n";
            }
        }
    }
    csv.close();
    std::cout << "[OK] Dados salvos em: experimentos/data/trie_vs_patricia.csv\n";
}

void runSplayVsTreap() {
    std::cout << "\n=======================================================\n";
    std::cout << "[EXPERIMENTO 2] Splay vs. Treap (Aleatório vs. Ordenado)\n";
    std::cout << "=======================================================\n";

    std::ofstream csv("experimentos/data/splay_vs_treap_insertion.csv");
    csv << "distribuicao,N,estrutura,tempo_insercao_ms,tempo_busca_ms,rotacoes,comparacoes\n";

    std::vector<size_t> sizes = {1000, 5000, 10000, 25000, 50000};
    std::vector<std::string> dists = {"aleatorio", "ordenado"};

    for (const auto& dist : dists) {
        for (size_t N : sizes) {
            std::vector<int> data;
            if (dist == "aleatorio") {
                data = DatasetGenerator::generateRandomInts(N, 1, 10000000, 42);
            } else {
                data = DatasetGenerator::generateSortedInts(N, 1, 1);
            }

            // Benchmark Splay
            {
                SplayTree splay;
                Timer t;
                for (int x : data) {
                    splay.insert(x);
                }
                double insTime = t.elapsed_milliseconds();

                t.reset();
                for (int x : data) {
                    splay.search(x);
                }
                double searchTime = t.elapsed_milliseconds();

                csv << dist << "," << N << ",Splay,"
                    << insTime << "," << searchTime << ","
                    << splay.getMetrics().rotations << ","
                    << splay.getMetrics().comparisons << "\n";

                std::cout << "[" << std::setw(9) << dist << "] N=" << std::setw(5) << N 
                          << " | Splay -> Ins: " << std::setw(7) << insTime << "ms | Busca: " 
                          << std::setw(7) << searchTime << "ms | Rot: " << std::setw(9) << splay.getMetrics().rotations << "\n";
            }

            // Benchmark Treap
            {
                Treap treap(42);
                Timer t;
                for (int x : data) {
                    treap.insert(x);
                }
                double insTime = t.elapsed_milliseconds();

                t.reset();
                for (int x : data) {
                    treap.search(x);
                }
                double searchTime = t.elapsed_milliseconds();

                csv << dist << "," << N << ",Treap,"
                    << insTime << "," << searchTime << ","
                    << treap.getMetrics().rotations << ","
                    << treap.getMetrics().comparisons << "\n";

                std::cout << "[" << std::setw(9) << dist << "] N=" << std::setw(5) << N 
                          << " | Treap -> Ins: " << std::setw(7) << insTime << "ms | Busca: " 
                          << std::setw(7) << searchTime << "ms | Rot: " << std::setw(9) << treap.getMetrics().rotations << "\n";
            }
            // Benchmark BST
            {
                BST bst;
                Timer t;
                for (int x : data) {
                    bst.insert(x);
                }
                double insTime = t.elapsed_milliseconds();

                t.reset();
                for (int x : data) {
                    bst.search(x);
                }
                double searchTime = t.elapsed_milliseconds();

                csv << dist << "," << N << ",BST,"
                    << insTime << "," << searchTime << ",0,"
                    << bst.getMetrics().comparisons << "\n";

                std::cout << "[" << std::setw(9) << dist << "] N=" << std::setw(5) << N 
                          << " | BST   -> Ins: " << std::setw(7) << insTime << "ms | Busca: " 
                          << std::setw(7) << searchTime << "ms | Comp: " << std::setw(9) << bst.getMetrics().comparisons << "\n";
            }

            // Benchmark AVL
            {
                AVL avl;
                Timer t;
                for (int x : data) {
                    avl.insert(x);
                }
                double insTime = t.elapsed_milliseconds();

                t.reset();
                for (int x : data) {
                    avl.search(x);
                }
                double searchTime = t.elapsed_milliseconds();

                csv << dist << "," << N << ",AVL,"
                    << insTime << "," << searchTime << ","
                    << avl.getMetrics().rotations << ","
                    << avl.getMetrics().comparisons << "\n";

                std::cout << "[" << std::setw(9) << dist << "] N=" << std::setw(5) << N 
                          << " | AVL   -> Ins: " << std::setw(7) << insTime << "ms | Busca: " 
                          << std::setw(7) << searchTime << "ms | Rot: " << std::setw(9) << avl.getMetrics().rotations << "\n";
            }

        }
    }
    csv.close();
    std::cout << "[OK] Dados salvos em: experimentos/data/splay_vs_treap_insertion.csv\n";
}

void runLocalityBenchmark() {
    std::cout << "\n=======================================================\n";
    std::cout << "[EXPERIMENTO 3] Localidade Temporal de Acesso (Zipf)\n";
    std::cout << "=======================================================\n";

    std::ofstream csv("experimentos/data/locality_benchmark.csv");
    csv << "padrao_acesso,num_consultas,estrutura,tempo_busca_ms,rotacoes,comparacoes\n";

    size_t N = 10000;
    size_t Q = 100000;

    std::vector<int> keys = DatasetGenerator::generateRandomInts(N, 1, 1000000, 42);

    // Índices uniformes
    std::vector<int> uniformQueries;
    uniformQueries.reserve(Q);
    auto randIndices = DatasetGenerator::generateRandomInts(Q, 0, N - 1, 123);
    for (int idx : randIndices) {
        uniformQueries.push_back(keys[idx]);
    }

    // Índices Zipfianos (forte viés temporal)
    std::vector<int> zipfQueries;
    zipfQueries.reserve(Q);
    auto zipfIndices = DatasetGenerator::generateZipfianIndices(Q, N, 1.25, 123);
    for (size_t idx : zipfIndices) {
        zipfQueries.push_back(keys[idx]);
    }

    std::vector<std::pair<std::string, std::vector<int>>> patterns = {
        {"Uniforme", uniformQueries},
        {"Zipfiano (80-20)", zipfQueries}
    };

    for (const auto& pat : patterns) {
        const std::string& name = pat.first;
        const auto& queries = pat.second;

        // Splay
        {
            SplayTree splay;
            for (int k : keys) splay.insert(k);
            splay.resetMetrics();

            Timer t;
            for (int q : queries) {
                splay.search(q);
            }
            double timeMs = t.elapsed_milliseconds();

            csv << name << "," << Q << ",Splay,"
                << timeMs << "," << splay.getMetrics().rotations << ","
                << splay.getMetrics().comparisons << "\n";

            std::cout << "[" << std::setw(16) << name << "] Splay -> Tempo: " 
                      << std::setw(8) << timeMs << "ms | Rot: " << std::setw(8) << splay.getMetrics().rotations 
                      << " | Comp: " << splay.getMetrics().comparisons << "\n";
        }

        // Treap
        {
            Treap treap(42);
            for (int k : keys) treap.insert(k);
            treap.resetMetrics();

            Timer t;
            for (int q : queries) {
                treap.search(q);
            }
            double timeMs = t.elapsed_milliseconds();

            csv << name << "," << Q << ",Treap,"
                << timeMs << "," << treap.getMetrics().rotations << ","
                << treap.getMetrics().comparisons << "\n";

            std::cout << "[" << std::setw(16) << name << "] Treap -> Tempo: " 
                      << std::setw(8) << timeMs << "ms | Rot: " << std::setw(8) << treap.getMetrics().rotations 
                      << " | Comp: " << treap.getMetrics().comparisons << "\n";
        }
        // BST
        {
            BST bst;
            for (int k : keys) bst.insert(k);
            bst.resetMetrics();

            Timer t;
            for (int q : queries) {
                bst.search(q);
            }
            double timeMs = t.elapsed_milliseconds();

            csv << name << "," << Q << ",BST,"
                << timeMs << ",0,"
                << bst.getMetrics().comparisons << "\n";

            std::cout << "[" << std::setw(16) << name << "] BST   -> Tempo: " 
                      << std::setw(8) << timeMs << "ms | Rot: " << std::setw(8) << 0 
                      << " | Comp: " << bst.getMetrics().comparisons << "\n";
        }

        // AVL
        {
            AVL avl;
            for (int k : keys) avl.insert(k);
            avl.resetMetrics();

            Timer t;
            for (int q : queries) {
                avl.search(q);
            }
            double timeMs = t.elapsed_milliseconds();

            csv << name << "," << Q << ",AVL,"
                << timeMs << "," << avl.getMetrics().rotations << ","
                << avl.getMetrics().comparisons << "\n";

            std::cout << "[" << std::setw(16) << name << "] AVL   -> Tempo: " 
                      << std::setw(8) << timeMs << "ms | Rot: " << std::setw(8) << avl.getMetrics().rotations 
                      << " | Comp: " << avl.getMetrics().comparisons << "\n";
        }

    }

    csv.close();
    std::cout << "[OK] Dados salvos em: experimentos/data/locality_benchmark.csv\n";
}

void runKDTreeBenchmark() {
    std::cout << "\n=======================================================\n";
    std::cout << "[EXPERIMENTO 4] KD-Tree (Consultas Espaciais em 2D e 3D)\n";
    std::cout << "=======================================================\n";

    std::ofstream csv("experimentos/data/kdtree_benchmark.csv");
    csv << "dimensao,N,tempo_insercao_ms,tempo_busca_exata_ms,tempo_range_search_ms,tempo_1nn_ms,comparacoes\n";

    std::vector<size_t> sizes = {1000, 5000, 10000, 25000, 50000, 100000};

    for (size_t N : sizes) {
        auto pts2D = DatasetGenerator::generateRandomPoints2D(N, 0.0, 1000.0, 42);

        KDTree kd(2);
        Timer t;
        for (const auto& p : pts2D) {
            kd.insert({p.x, p.y});
        }
        double insTime = t.elapsed_milliseconds();

        // 1. Busca exata (1000 amostras)
        size_t samples = 1000;
        t.reset();
        for (size_t i = 0; i < samples; ++i) {
            kd.search({pts2D[i].x, pts2D[i].y});
        }
        double searchTime = t.elapsed_milliseconds();

        // 2. Range Search (100 caixas de consulta)
        size_t rangeQueries = 100;
        t.reset();
        for (size_t i = 0; i < rangeQueries; ++i) {
            double cx = pts2D[i].x;
            double cy = pts2D[i].y;
            kd.rangeSearch({cx - 25.0, cy - 25.0}, {cx + 25.0, cy + 25.0});
        }
        double rangeTime = t.elapsed_milliseconds();

        // 3. 1-NN (1000 pontos de consulta aleatórios)
        auto queryPoints = DatasetGenerator::generateRandomPoints2D(1000, 0.0, 1000.0, 999);
        t.reset();
        for (const auto& qp : queryPoints) {
            kd.nearestNeighbor({qp.x, qp.y});
        }
        double nnTime = t.elapsed_milliseconds();

        csv << "2D," << N << ","
            << insTime << "," << searchTime << ","
            << rangeTime << "," << nnTime << ","
            << kd.getMetrics().comparisons << "\n";

        std::cout << "[KD-Tree 2D] N=" << std::setw(6) << N 
                  << " | Ins: " << std::setw(7) << insTime << "ms | Busca: " 
                  << std::setw(6) << searchTime << "ms | Range: " << std::setw(6) << rangeTime 
                  << "ms | 1-NN: " << std::setw(6) << nnTime << "ms\n";
    }

    csv.close();
    std::cout << "[OK] Dados salvos em: experimentos/data/kdtree_benchmark.csv\n";
}

int main() {
    std::cout << "===============================================================\n";
    std::cout << " BATERIA COMPLETA DE EXPERIMENTOS DE DESEMPENHO (AEDS 2)\n";
    std::cout << "===============================================================\n";

    runTrieVsPatricia();
    runSplayVsTreap();
    runLocalityBenchmark();
    runKDTreeBenchmark();

    std::cout << "\n===============================================================\n";
    std::cout << " [SUCESSO] Todos os experimentos foram executados e salvos!\n";
    std::cout << "===============================================================\n";
    return 0;
}
