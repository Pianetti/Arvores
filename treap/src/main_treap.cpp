#include "treap.hpp"
#include <iostream>
#include <cassert>

int main() {
    std::cout << "=== [TESTE E DEMONSTRAÇÃO: ÁRVORE TREAP (TREE + HEAP)] ===\n\n";

    Treap treap(42);

    // 1. Inserção determinística para rastreamento visual didático
    std::cout << "-> Inserindo nós com prioridades controladas para rastreamento:\n";
    std::cout << "   (chave: 50, prio: 50), (chave: 30, prio: 40), (chave: 70, prio: 30), (chave: 20, prio: 20), (chave: 40, prio: 10)...\n";
    treap.insert(50, 50);
    treap.insert(30, 40);
    treap.insert(70, 30);
    treap.insert(20, 20);
    treap.insert(40, 10);

    assert(treap.verifyInvariants());
    std::cout << "   Tamanho: " << treap.size() << " | Raiz (chave=" << treap.getRootKey() << ", prio=" << treap.getRootPriority() << ")\n";
    treap.exportDot("treap_estado1_inicial.dot", "Treap - Estado 1: Inserções Iniciais com Invariantes BST+Heap");
    std::cout << "   [OK] Gerado arquivo: treap_estado1_inicial.dot\n";

    // 2. Inserção intermediária com alta prioridade que força subida por rotações até a raiz
    std::cout << "\n-> Inserindo (chave: 25, prioridade MÁXIMA: 99) para forçar cascata de rotações até a raiz...\n";
    treap.insert(25, 99);
    assert(treap.verifyInvariants());
    std::cout << "   Nova raiz após rotações: chave=" << treap.getRootKey() << ", prio=" << treap.getRootPriority() << "\n";
    assert(treap.getRootKey() == 25);
    assert(treap.getRootPriority() == 99);
    std::cout << "   Total de rotações acumuladas: " << treap.getMetrics().rotations << "\n";
    treap.exportDot("treap_estado2_alta_prioridade.dot", "Treap - Estado 2: Pós-Inserção com Prioridade 99 (Elevada à Raiz)");
    std::cout << "   [OK] Gerado arquivo: treap_estado2_alta_prioridade.dot\n";

    // 3. Consultas e buscas
    std::cout << "\n-> Testando buscas exatas:\n";
    assert(treap.search(25) == true);
    assert(treap.search(50) == true);
    assert(treap.search(999) == false);
    std::cout << "   Busca por 25: ENCONTRADA\n";
    std::cout << "   Busca por 999: NÃO ENCONTRADA\n";

    // 4. Remoção da raiz (chave 25) com rotações descendentes
    std::cout << "\n-> Removendo a raiz (chave 25) forçando rotações descendentes preservando Max-Heap...\n";
    bool remRoot = treap.remove(25);
    assert(remRoot);
    assert(!treap.search(25));
    assert(treap.verifyInvariants());
    std::cout << "   Raiz restaurada: chave=" << treap.getRootKey() << ", prio=" << treap.getRootPriority() << "\n";
    treap.exportDot("treap_estado3_pos_remocao.dot", "Treap - Estado 3: Pós Remoção da Raiz com Rotações Descendentes");
    std::cout << "   [OK] Gerado arquivo: treap_estado3_pos_remocao.dot\n";

    // 5. Teste massivo de invariantes com 5.000 chaves aleatórias
    std::cout << "\n-> Teste de estresse: inserindo 5.000 chaves aleatórias...\n";
    Treap bigTreap(12345);
    for (int i = 0; i < 5000; ++i) {
        bigTreap.insert(i * 3 + 7);
    }
    assert(bigTreap.verifyInvariants());
    std::cout << "   [OK] Invariantes BST e Max-Heap 100% verificados em 5.000 nós!\n";

    std::cout << "\n=== [MÉTRICAS DA TREAP (DEMO)] ===\n";
    treap.getMetrics().print();

    std::cout << "\n[SUCESSO] Todos os testes da Árvore Treap foram aprovados!\n";
    return 0;
}
