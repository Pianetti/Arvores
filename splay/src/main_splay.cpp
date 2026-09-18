#include "splay_tree.hpp"
#include <iostream>
#include <cassert>

int main() {
    std::cout << "=== [TESTE E DEMONSTRAÇÃO: ÁRVORE SPLAY] ===\n\n";

    SplayTree splay;

    // 1. Inserções e Estado 1
    std::cout << "-> Inserindo sequência de chaves: 10, 20, 30, 40, 50, 60, 70...\n";
    splay.insert(10);
    splay.insert(20);
    splay.insert(30);
    splay.insert(40);
    splay.insert(50);
    splay.insert(60);
    splay.insert(70);

    std::cout << "   Tamanho da árvore: " << splay.size() << " | Chave na raiz: " << splay.getRootKey() << "\n";
    assert(splay.getRootKey() == 70); // O último inserido sofreu splay para a raiz
    splay.exportDot("splay_estado1_inicial.dot", "Splay Tree - Estado 1: Pós Inserções (Raiz = 70)");
    std::cout << "   [OK] Gerado arquivo: splay_estado1_inicial.dot\n";

    // 2. Acesso a um nó profundo e Estado 2 (Reorganização / Rotações Zig-Zig e Zig-Zag)
    std::cout << "\n-> Buscando a chave profunda 20 para forçar reestruturação por Splay...\n";
    bool found20 = splay.search(20);
    assert(found20);
    std::cout << "   Busca por 20 concluída. Nova chave na raiz após Splay: " << splay.getRootKey() << "\n";
    assert(splay.getRootKey() == 20);
    std::cout << "   Total de rotações acumuladas: " << splay.getMetrics().rotations << "\n";
    splay.exportDot("splay_estado2_splay20.dot", "Splay Tree - Estado 2: Pós Busca de 20 (Raiz Reorganizada = 20)");
    std::cout << "   [OK] Gerado arquivo: splay_estado2_splay20.dot\n";

    // 3. Teste de Localidade Temporal (acessos repetidos à mesma chave)
    std::cout << "\n-> Testando localidade temporal (buscas sucessivas na raiz recém-acessada 20):\n";
    size_t rotsBefore = splay.getMetrics().rotations;
    splay.search(20);
    size_t rotsAfter = splay.getMetrics().rotations;
    std::cout << "   Rotações para re-acessar nó já na raiz: " << (rotsAfter - rotsBefore) << " (Custo O(1)!)\n";
    assert(rotsAfter == rotsBefore);

    // 4. Remoção e Estado 3
    std::cout << "\n-> Removendo chave 40...\n";
    bool rem40 = splay.remove(40);
    assert(rem40);
    assert(!splay.search(40)); // Busca de elemento inexistente splaya o vizinho mais próximo

    std::cout << "   Tamanho atual: " << splay.size() << " | Raiz atual: " << splay.getRootKey() << "\n";
    splay.exportDot("splay_estado3_pos_remocao.dot", "Splay Tree - Estado 3: Pós Remoção de 40");
    std::cout << "   [OK] Gerado arquivo: splay_estado3_pos_remocao.dot\n";

    std::cout << "\n=== [MÉTRICAS DA ÁRVORE SPLAY] ===\n";
    splay.getMetrics().print();

    std::cout << "\n[SUCESSO] Todos os testes da Árvore Splay foram aprovados!\n";
    return 0;
}
