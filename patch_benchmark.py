import re

with open('experimentos/src/benchmark_runner.cpp', 'r') as f:
    content = f.read()

# Fix the include if needed
if '#include "bst.hpp"' not in content:
    content = content.replace('#include "splay_tree.hpp"', '#include "splay_tree.hpp"\n#include "bst.hpp"\n#include "avl.hpp"')

# --- PATCH 1: Splay vs Treap (Now Splay vs Treap vs BST vs AVL) ---
bst_avl_block = """
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
                    << bst.getMetrics().comparisons << "\\n";

                std::cout << "[" << std::setw(9) << dist << "] N=" << std::setw(5) << N 
                          << " | BST   -> Ins: " << std::setw(7) << insTime << "ms | Busca: " 
                          << std::setw(7) << searchTime << "ms | Comp: " << std::setw(9) << bst.getMetrics().comparisons << "\\n";
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
                    << avl.getMetrics().comparisons << "\\n";

                std::cout << "[" << std::setw(9) << dist << "] N=" << std::setw(5) << N 
                          << " | AVL   -> Ins: " << std::setw(7) << insTime << "ms | Busca: " 
                          << std::setw(7) << searchTime << "ms | Rot: " << std::setw(9) << avl.getMetrics().rotations << "\\n";
            }
"""

if 'BST   -> Ins' not in content:
    # Insert right after Treap benchmark in runSplayVsTreap
    # Find the end of Treap block
    marker = '<< " | Comp: " << treap.getMetrics().comparisons << "\\n";\n        }'
    treap_block_end_idx = content.find('std::cout << "[" << std::setw(9) << dist << "] N=" << std::setw(5) << N \n                          << " | Treap -> Ins: "')
    
    if treap_block_end_idx != -1:
        end_brace_idx = content.find('}', treap_block_end_idx)
        if end_brace_idx != -1:
            content = content[:end_brace_idx+1] + bst_avl_block + content[end_brace_idx+1:]


# --- PATCH 2: Locality Benchmark ---
bst_avl_locality = """
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
                << bst.getMetrics().comparisons << "\\n";

            std::cout << "[" << std::setw(16) << name << "] BST   -> Tempo: " 
                      << std::setw(8) << timeMs << "ms | Rot: " << std::setw(8) << 0 
                      << " | Comp: " << bst.getMetrics().comparisons << "\\n";
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
                << avl.getMetrics().comparisons << "\\n";

            std::cout << "[" << std::setw(16) << name << "] AVL   -> Tempo: " 
                      << std::setw(8) << timeMs << "ms | Rot: " << std::setw(8) << avl.getMetrics().rotations 
                      << " | Comp: " << avl.getMetrics().comparisons << "\\n";
        }
"""

if 'BST   -> Tempo' not in content:
    # Insert right after Treap in runLocalityBenchmark
    loc = content.find('std::cout << "[" << std::setw(16) << name << "] Treap -> Tempo: "')
    if loc != -1:
        end_brace_idx = content.find('}', loc)
        if end_brace_idx != -1:
            content = content[:end_brace_idx+1] + bst_avl_locality + content[end_brace_idx+1:]

with open('experimentos/src/benchmark_runner.cpp', 'w') as f:
    f.write(content)

