#include "patricia.hpp"
#include <iostream>
#include <cassert>

int main() {
    std::cout << "=== [TESTE E DEMONSTRAÇÃO: ÁRVORE PATRICIA (RADIX TREE COMPACTA)] ===\n\n";

    PatriciaTree patricia;

    // 1. Inserções iniciais
    std::cout << "-> Inserindo chaves iniciais: 'casa', 'carro', 'caminhao', 'pato', 'pasta'...\n";
    patricia.insert("casa");
    patricia.insert("carro");
    patricia.insert("caminhao");
    patricia.insert("pato");
    patricia.insert("pasta");

    std::cout << "   Total de palavras: " << patricia.size() << " | Nós compactados: " << patricia.nodeCount() << "\n";
    patricia.exportDot("patricia_estado1_inicial.dot", "Patricia - Estado 1: Inserção com Compactação");
    std::cout << "   [OK] Gerado arquivo: patricia_estado1_inicial.dot\n";

    // 2. Inserção intermediária com divisões de arestas (Edge Splitting / Bifurcação)
    std::cout << "\n-> Inserindo palavras para forçar divisão de arestas: 'casamento', 'carreta', 'carroceria'...\n";
    patricia.insert("casamento");
    patricia.insert("carreta");
    patricia.insert("carroceria");

    std::cout << "   Total de palavras: " << patricia.size() << " | Nós totais: " << patricia.nodeCount() << "\n";
    std::cout << "   Divisões de aresta realizadas: " << patricia.getMetrics().edge_splits << "\n";
    patricia.exportDot("patricia_estado2_split.dot", "Patricia - Estado 2: Bifurcação e Divisão de Arestas");
    std::cout << "   [OK] Gerado arquivo: patricia_estado2_split.dot\n";

    // 3. Consultas e buscas
    std::cout << "\n-> Testando buscas exatas, prefixos e longest prefix:\n";
    assert(patricia.search("casa") == true);
    assert(patricia.search("casamento") == true);
    assert(patricia.search("carro") == true);
    assert(patricia.search("carroceria") == true);
    assert(patricia.search("computador") == false);
    assert(patricia.startsWith("car") == true);
    assert(patricia.startsWith("cas") == true);
    assert(patricia.startsWith("xyz") == false);

    std::cout << "   Busca por 'casa': " << (patricia.search("casa") ? "ENCONTRADA" : "NÃO ENCONTRADA") << "\n";
    std::cout << "   Busca por 'computador': " << (patricia.search("computador") ? "ENCONTRADA" : "NÃO ENCONTRADA") << "\n";

    auto prefixWords = patricia.wordsWithPrefix("car");
    std::cout << "   Palavras com prefixo 'car' (" << prefixWords.size() << "): ";
    for (const auto& w : prefixWords) std::cout << w << " ";
    std::cout << "\n";

    std::cout << "   Maior prefixo de 'casas_populares': '" << patricia.longestPrefixOf("casas_populares") << "'\n";
    assert(patricia.longestPrefixOf("casas_populares") == "casa");

    // Inserindo chaves dedicadas para testar e demonstrar Edge Merging
    patricia.insert("rede");
    patricia.insert("redirecionar"); // aresta 're' -> 'de' e 'direcionar'
    std::cout << "\n-> Inserido 'rede' e 'redirecionar'. Removendo 'redirecionar' para comprovar coalescência de aresta...\n";
    assert(patricia.remove("redirecionar") == true);

    // 4. Remoção e Fusão de Arestas (Edge Merging / Coalescência)
    std::cout << "\n-> Testando remoção com fusão de arestas:\n";
    std::cout << "   Removendo 'carroceria' e 'casamento'...\n";
    bool rem1 = patricia.remove("carroceria");
    bool rem2 = patricia.remove("casamento");
    assert(rem1 && rem2);
    assert(patricia.search("carroceria") == false);
    assert(patricia.search("carro") == true);
    assert(patricia.search("casa") == true);

    std::cout << "   Fusões de aresta realizadas: " << patricia.getMetrics().edge_merges << "\n";
    std::cout << "   Palavras restantes: " << patricia.size() << " | Nós atuais: " << patricia.nodeCount() << "\n";
    patricia.exportDot("patricia_estado3_pos_remocao.dot", "Patricia - Estado 3: Pós-Remoção com Fusão de Arestas");
    std::cout << "   [OK] Gerado arquivo: patricia_estado3_pos_remocao.dot\n";

    std::cout << "\n=== [MÉTRICAS DA PATRICIA] ===\n";
    patricia.getMetrics().print();

    std::cout << "\n[SUCESSO] Todos os testes da Árvore Patricia foram aprovados!\n";
    return 0;
}
