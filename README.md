# 🌳 Estruturas de Dados Avançadas: Análise Empírica de Árvores Especializadas

[![Status](http://img.shields.io/static/v1?label=STATUS&message=FINALIZADO&color=GREEN&style=for-the-badge)]()
[![Linguagem](http://img.shields.io/static/v1?label=Linguagem&message=C%2B%2B&color=blue&style=for-the-badge)]()
[![Linguagem](http://img.shields.io/static/v1?label=Linguagem&message=Python&color=yellow&style=for-the-badge)]()
[![Linguagem](http://img.shields.io/static/v1?label=Documenta%C3%A7%C3%A3o&message=LaTeX&color=teal&style=for-the-badge)]()


## 📃 Introdução

Este projeto foi desenvolvido como trabalho prático e de pesquisa para a disciplina de **Algoritmos e Estruturas de Dados II (AEDS II)**, no Centro Federal de Educação Tecnológica de Minas Gerais (CEFET-MG), Campus V – Divinópolis.

O trabalho consiste na modelagem computacional, implementação em baixo nível (C++) e análise crítica profunda de cinco estruturas de dados hierárquicas avançadas: **Trie**, **Patricia**, **Treap**, **Splay Tree** e **KD-Tree**. O objetivo primordial é estabelecer um contraponto prático e teórico em relação às tradicionais Árvores Binárias de Busca (BST) e suas variantes estritamente balanceadas (AVL).

O estudo explora como o relaxamento de regras de balanceamento estrito e a alteração na mecânica fundamental de comparação de chaves propiciam ganhos exponenciais de desempenho em cenários do mundo real (como localidade temporal extrema, buscas multidimensionais espaciais e indexação lexicográfica densa).


## 📋 Problema Proposto

A engenharia de software moderna exige a manipulação de volumes massivos de dados cujas características fogem do comportamento aleatório e uniforme idealizado pela estatística pura. O projeto busca provar empiricamente que **não existe uma "árvore universal"**.

O problema propõe submeter essas arquiteturas a baterias de testes brutais de estresse (até 100.000 elementos) e rastrear seu comportamento em três perfis de dados desafiadores:
1. **Caos Uniforme Randômico:** Distribuição ideal forjada para testar a resiliência média.
2. **Ordem Constritiva (Degenerativa):** Injeção de sequências crescentes projetadas exclusivamente para causar degeneração linear e forçar o Pior Caso.
3. **Paradoxo Zipfiano (80/20):** Simulação de tráfego de servidores web, onde 80% das buscas recaem sobre uma mesma fatia de 20% das chaves (Alta localidade temporal).

### 📌 As Estruturas Analisadas

* **🌳 BST & AVL:** As bases de controle. A BST sofre com degeneração, e a AVL impõe um custo rotacional pesado para manter seu rigoroso fator de balanço.
* **🔤 Trie:** Autômato lexicográfico que busca strings caractere por caractere (O(L)), ideal para autocompletes, mas que sofre com altíssimo desperdício de memória primária.
* **📦 Patricia (Radix Tree):** Evolução direta da Trie que comprime caminhos longos sem ramificação (*Split*), economizando RAM e reduzindo *Cache Misses*.
* **🎲 Treap:** Estrutura híbrida (Tree + Heap) que utiliza prioridades pseudoaleatórias para manter a árvore balanceada estatisticamente, imune a dados ordenados.
* **🔄 Splay Tree:** Árvore autoajustável que não armazena fatores de balanço. Ela traciona dados recém-acessados para a raiz através de rotações duplas (Zig-Zig, Zig-Zag), dominando cenários de localidade temporal (Caches L1/L2).
* **🧭 KD-Tree:** Estrutura multidimensional que alterna eixos ortogonais a cada profundidade, essencial para mapeamentos espaciais, IA, e buscas de vizinhos mais próximos (K-NN).


## 🖥️ Ambiente de Criação

O código foi desenvolvido, testado e profilado utilizando as seguintes ferramentas e configurações:

[![Linguagem](https://img.shields.io/badge/Linguagem-C%2B%2B17-blue?style=for-the-badge)]()
[![Linguagem](https://img.shields.io/badge/Scripts-Python%203-yellow?style=for-the-badge)]()
[![IDE](https://img.shields.io/badge/IDE-Visual%20Studio%20Code-blueviolet?style=for-the-badge)]()
[![SO](https://img.shields.io/badge/OS-Ubuntu%2024.04%20LTS-orange?style=for-the-badge)]()
[![WSL](https://img.shields.io/badge/WSL-2-orange?style=for-the-badge)]()

### 💻 Especificações da Máquina

| Componente | Descrição |
|------------|-----------|
| **Processador** | AMD Ryzen 7 7735HS with Radeon Graphics |
| **Memória RAM** | 24 GB |
| **Sistema Operacional** | Ubuntu 24.04 LTS (via WSL 2 Windows) |
| **Compilador** | g++ (C++17) / CMake / Make |
| **Visualização** | Graphviz (DOT) / Matplotlib (Python) |


## 📁 Estrutura Geral do Projeto

```text
.
├── Makefile & CMakeLists.txt       # Automação da compilação de todo o projeto
├── bin/                            # Binários compilados (ex: benchmark executável)
├── common/                         # Interfaces, métricas compartilhadas e geradores
├── bst/                            # Módulo da Árvore Binária de Busca Clássica
├── avl/                            # Módulo da Árvore AVL
├── trie/                           # Módulo da Árvore Trie
├── patricia/                       # Módulo da Árvore Patricia
├── treap/                          # Módulo da Treap Probabilística
├── splay/                          # Módulo da Splay Tree
├── kdtree/                         # Módulo da KD-Tree Espacial
└── experimentos/
    ├── data/                       # Arquivos CSV contendo resultados brutos gerados pelos testes
    ├── plots/                      # Gráficos (PNG) e rastreamentos visuais em Graphviz (DOT)
    └── scripts/                    # Scripts Python para plotagem de gráficos com Matplotlib

```


## ⚙️ Metodologia e Execução

Esta seção descreve a orquestração do benchmark de testes de estresse:

### 📌 1. Bateria de Benchmarks Automáticos
Ao iniciar o binário de benchmark, o sistema injeta volumes de dados estritos de $N \in \{1.000, 5.000, 10.000, 25.000, 50.000, 100.000\}$. Para assegurar ausência de vícios de hardware, utilizou-se a máquina de geração pseudoaleatória **Mersenne Twister**.

O monitoramento (Observer Pattern) capturou:
* Tempo absoluto em Milissegundos (ms).
* Contagem exata de Comparações Lógicas de CPU.
* Contagem massiva de Rotações Estruturais.
* Número bruto de Nós fisicamente alocados na HEAP.

### 📌 2. Geração Visual (Rastreamento de Estados)
Para provar a fidelidade teórica, o projeto não se resumiu a relógios. Implementou-se um exportador semântico nativo para linguagem **DOT (Graphviz)**. As árvores exportam seu próprio formato em 3 fases mandatórias:
1. **Estado Inicial** (Inserções base).
2. **Estado Crítico / Transição** (O estilhaçamento na Patricia, o Splay de um nó à raiz, ou o *Bubble-Up* na Treap).
3. **Estado Pós-Remoção**.


## 📊 Análises e Conclusões

Os relatórios exportados confirmaram dogmas assintóticos e revelaram segredos de Hardware (CPU Caches):

* 🚨 **O Desastre da BST em Dados Ordenados:** Ao tentar inserir sequencialmente 50.000 elementos, a BST nativa formou uma lista degenerada, efetuando o volume irreal e estrondoso de **mais de 5 Bilhões (5.000.000.000) de comparações matemáticas** fúteis. Esse cenário causou colapso na latência.
* 🛡️ **O Escudo da Treap e Splay:** Sob a mesma injeção letal de ordem, a Splay achatou o grafo de baixo para cima organicamente, enquanto a Treap invocou o caos estocástico para manter-se balanceada sem esforço de cálculo de fatores, diluindo os tempos de volta a meros milissegundos.
* 🗜️ **Aglutinação Patricia vs Desperdício Trie:** Ao avaliar strings extensas com prefixos compartilhados, a Trie esgotou centenas de milhares de nós vazios na memória RAM. A Patricia comprimiu esses caminhos perfeitamente (*Split*), gerando agilidade extrema pela prevenção drástica de fragmentação e *Cache Misses*.
* 🔁 **Domínio da Splay no Paradoxo Zipfiano:** Quando o motor Zipf exigiu requisições repetidas das mesmas chaves (simulando um servidor web congestionado no mundo real), as outras árvores perderam força no processamento profundo de folhas. A Splay ejetou os *hubs* nevrálgicos para a raiz, assumindo total liderança temporal.
* 📐 **Exatidão K-Dimensional:** A KD-Tree provou de maneira cabal seu reinado isolando hiperplanos cartesianos dinamicamente. Suas buscas de Vizinhos Próximos (K-NN) mitigaram varreduras desnecessárias operando em $2.0$ milissegundos perante mapas globais.


## ⌨️ Instalação e Configuração

### 📌 Pré-requisitos
* Compilador **g++** compatível com C++17
* **Make** ou **CMake**
* **Python 3** e bibliotecas `matplotlib` e `pandas` (para geração dos gráficos)
* **Graphviz** (para converter os diagramas topológicos gerados `.dot` em `.png`)

### 📌 Passo a Passo

**1. Clone o repositório:**
```bash
git clone https://github.com/pianetti/AEDS2---Arvores.git
cd "AEDS2 - Arvores"
```

**2. Compile o projeto base (Binários C++):**
```bash
make
# ou utilizando CMake:
# mkdir build && cd build && cmake .. && make
```

**3. Execute a bateria de Benchmark:**
Isso irá estressar as árvores e gerar todos os arquivos CSV brutos dentro de `experimentos/data/`.
```bash
./bin/benchmark
```

**4. Gere os Gráficos Analíticos (Python):**
```bash
pip install matplotlib pandas
python3 experimentos/scripts/plot_graphs.py
```
*(Os gráficos `.png` de tempo, comparações e nós serão salvos em `experimentos/plots/`)*

**5. Gere o Rastreamento Visual das Árvores (Graphviz):**
```bash
sudo apt update && sudo apt install graphviz -y
mkdir -p experimentos/plots/rastreamento_png
for f in experimentos/plots/dot/*.dot; do dot -Tpng "$f" -o "experimentos/plots/rastreamento_png/$(basename "$f" .dot).png"; done
```


## 👨💻 Desenvolvido por

<div align="center">

<img src="https://media.licdn.com/dms/image/v2/D4E03AQFe0L1ZeCkDvg/profile-displayphoto-scale_200_200/B4EZgZ2lg9HEAs-/0/1752780398339?e=1776902400&v=beta&t=FT8v-wBpeCAIOPYT4VI-HSHUTbS_Jalbxrgtta31ZQY" width="150" style="border-radius: 50%;" />

### Samuel Pianetti Cordeiro

[![GitHub](https://img.shields.io/badge/GitHub-Perfil-000?style=for-the-badge&logo=github)](https://github.com/pianetti)
[![LinkedIn](https://img.shields.io/badge/LinkedIn-Perfil-0077B5?style=for-the-badge&logo=linkedin&logoColor=white)](https://www.linkedin.com/in/samuel-pianetti-cordeiro-979b742b0/)

</div>


### 🚀 Sobre

Estudante de Engenharia/Ciência da Computação com foco em tecnologia, programação de alto desempenho e construção de soluções modernas. Sempre buscando aprender, evoluir a fundo nos fundamentos teóricos e criar projetos eficientes e escaláveis.
