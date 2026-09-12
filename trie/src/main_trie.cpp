#include "trie.hpp"
#include <iostream>
#include <cassert>

int main() {
    std::cout << "=== [TESTE E DEMONSTRAÇÃO: ÁRVORE TRIE] ===\n\n";

    Trie trie;

    // 1. Inserções e demonstração de estados
    std::cout << "-> Inserindo chaves iniciais: 'casa', 'carro', 'caminhao', 'pato', 'pasta'...\n";
    trie.insert("casa");
    trie.insert("carro");
    trie.insert("caminhao");
    trie.insert("pato");
    trie.insert("pasta");

    std::cout << "   Total de palavras: " << trie.size() << " | Nós alocados: " << trie.nodeCount() << "\n";
    trie.exportDot("trie_estado1_inicial.dot", "Trie - Estado 1: Após Inserções Iniciais");
    std::cout << "   [OK] Gerado arquivo: trie_estado1_inicial.dot\n";

    // 2. Inserção intermediária com prefixos compartilhados adicionais
    std::cout << "\n-> Inserindo palavras com sobreposição de prefixos: 'casamento', 'carreta', 'carroceria'...\n";
    trie.insert("casamento");
    trie.insert("carreta");
    trie.insert("carroceria");
    std::cout << "   Total de palavras: " << trie.size() << " | Nós alocados: " << trie.nodeCount() << "\n";
    trie.exportDot("trie_estado2_prefixos.dot", "Trie - Estado 2: Compartilhamento de Prefixos Estendido");
    std::cout << "   [OK] Gerado arquivo: trie_estado2_prefixos.dot\n";

    // 3. Consultas e buscas
    std::cout << "\n-> Testando buscas exatas e prefixos:\n";
    assert(trie.search("casa") == true);
    assert(trie.search("casamento") == true);
    assert(trie.search("carro") == true);
    assert(trie.search("computador") == false);
    assert(trie.startsWith("ca") == true);
    assert(trie.startsWith("pas") == true);
    assert(trie.startsWith("xyz") == false);

    std::cout << "   Busca por 'casa': " << (trie.search("casa") ? "ENCONTRADA" : "NÃO ENCONTRADA") << "\n";
    std::cout << "   Busca por 'computador': " << (trie.search("computador") ? "ENCONTRADA" : "NÃO ENCONTRADA") << "\n";

    auto prefixWords = trie.wordsWithPrefix("car");
    std::cout << "   Palavras com prefixo 'car' (" << prefixWords.size() << "): ";
    for (const auto& w : prefixWords) std::cout << w << " ";
    std::cout << "\n";

    // 4. Remoção e poda de nós
    std::cout << "\n-> Testando remoção e poda de nós:\n";
    std::cout << "   Removendo 'carroceria' e 'casamento'...\n";
    bool rem1 = trie.remove("carroceria");
    bool rem2 = trie.remove("casamento");
    assert(rem1 && rem2);
    assert(trie.search("carroceria") == false);
    assert(trie.search("carro") == true); // O prefixo 'carro' deve continuar existindo!
    assert(trie.search("casa") == true);

    std::cout << "   Palavras restantes: " << trie.size() << " | Nós atuais: " << trie.nodeCount() << "\n";
    trie.exportDot("trie_estado3_pos_remocao.dot", "Trie - Estado 3: Pós Remoção com Poda Estrutural");
    std::cout << "   [OK] Gerado arquivo: trie_estado3_pos_remocao.dot\n";

    std::cout << "\n=== [MÉTRICAS DA TRIE] ===\n";
    trie.getMetrics().print();

    std::cout << "\n[SUCESSO] Todos os testes da Trie foram aprovados!\n";
    return 0;
}
