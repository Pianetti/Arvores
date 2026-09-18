#include "kdtree.hpp"
#include <iostream>
#include <cassert>

int main() {
    std::cout << "=== [TESTE E DEMONSTRAÇÃO: ÁRVORE KD-TREE (K-DIMENSIONAL TREE)] ===\n\n";

    KDTree kd(2); // 2D Tree

    // 1. Inserções iniciais
    std::cout << "-> Inserindo pontos 2D: (30,40), (5,25), (70,70), (10,12), (50,30), (35,45)...\n";
    kd.insert({30, 40});
    kd.insert({5, 25});
    kd.insert({70, 70});
    kd.insert({10, 12});
    kd.insert({50, 30});
    kd.insert({35, 45});

    std::cout << "   Tamanho da KD-Tree: " << kd.size() << "\n";
    kd.exportDot("kdtree_estado1_inicial.dot", "KD-Tree - Estado 1: Partição Ortogonal Inicial");
    kd.exportPartitionSvg2D("kdtree_particao2d_estado1.svg", 0, 100, 0, 100);
    std::cout << "   [OK] Gerados: kdtree_estado1_inicial.dot e kdtree_particao2d_estado1.svg\n";

    // 2. Inserção intermediária com particionamento estendido
    std::cout << "\n-> Inserindo pontos adicionais: (60,10), (80,85)...\n";
    kd.insert({60, 10});
    kd.insert({80, 85});
    std::cout << "   Tamanho atual: " << kd.size() << "\n";
    kd.exportDot("kdtree_estado2_particionamento.dot", "KD-Tree - Estado 2: Particionamento Espacial Estendido");
    kd.exportPartitionSvg2D("kdtree_particao2d_estado2.svg", 0, 100, 0, 100);
    std::cout << "   [OK] Gerados: kdtree_estado2_particionamento.dot e kdtree_particao2d_estado2.svg\n";

    // 3. Consultas Espaciais
    std::cout << "\n-> Testando busca exata:\n";
    assert(kd.search({30, 40}) == true);
    assert(kd.search({10, 12}) == true);
    assert(kd.search({99, 99}) == false);
    std::cout << "   Busca por (30,40): ENCONTRADO\n";
    std::cout << "   Busca por (99,99): NÃO ENCONTRADO\n";

    std::cout << "\n-> Testando Busca por Faixa (Range Search) na região [0, 40] x [0, 50]:\n";
    auto inRange = kd.rangeSearch({0, 0}, {40, 50});
    std::cout << "   Pontos encontrados no retângulo (" << inRange.size() << "):\n";
    for (const auto& p : inRange) {
        std::cout << "     (" << p[0] << ", " << p[1] << ")\n";
    }
    assert(inRange.size() >= 3); // (30,40), (5,25), (10,12), (35,45)

    std::cout << "\n-> Testando Busca do Vizinho Mais Próximo (1-NN) para alvo (32, 42):\n";
    KDPoint target = {32, 42};
    KDPoint nn = kd.nearestNeighbor(target);
    std::cout << "   Vizinho mais próximo encontrado: (" << nn[0] << ", " << nn[1] << ") com distância: " 
              << nn.distance(target) << "\n";
    assert(nn == KDPoint({30, 40}) || nn == KDPoint({35, 45}));

    // 4. Remoção de ponto interno
    std::cout << "\n-> Removendo ponto interno (70, 70) com substituição pelo mínimo da dimensão de corte...\n";
    bool rem = kd.remove({70, 70});
    assert(rem);
    assert(!kd.search({70, 70}));
    std::cout << "   Tamanho pós-remoção: " << kd.size() << "\n";
    kd.exportDot("kdtree_estado3_pos_remocao.dot", "KD-Tree - Estado 3: Pós Remoção de (70,70)");
    kd.exportPartitionSvg2D("kdtree_particao2d_estado3.svg", 0, 100, 0, 100);
    std::cout << "   [OK] Gerados: kdtree_estado3_pos_remocao.dot e kdtree_particao2d_estado3.svg\n";

    std::cout << "\n=== [MÉTRICAS DA KD-TREE] ===\n";
    kd.getMetrics().print();

    std::cout << "\n[SUCESSO] Todos os testes da KD-Tree foram aprovados!\n";
    return 0;
}
