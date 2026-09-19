import pandas as pd
import matplotlib.pyplot as plt
import os

# Create plots directory if it doesn't exist
os.makedirs("experimentos/plots", exist_ok=True)
data_dir = "experimentos/data"

def plot_trie_vs_patricia():
    df = pd.read_csv(f"{data_dir}/trie_vs_patricia.csv")
    df_rand = df[df['tipo_dado'] == 'aleatorio']
    
    # 1. Insertion Time
    plt.figure(figsize=(10, 6))
    for est in ['Trie', 'Patricia']:
        subset = df_rand[df_rand['estrutura'] == est]
        plt.plot(subset['N'], subset['tempo_insercao_ms'], marker='o', label=est)
    
    plt.title("Trie vs Patricia: Tempo de Inserção (Dados Aleatórios)")
    plt.xlabel("Número de Elementos (N)")
    plt.ylabel("Tempo (ms)")
    plt.legend()
    plt.grid(True)
    plt.savefig("experimentos/plots/1_trie_vs_patricia_tempo.png")
    plt.close()

    # 2. Nodes Memory
    plt.figure(figsize=(10, 6))
    for est in ['Trie', 'Patricia']:
        subset = df_rand[df_rand['estrutura'] == est]
        plt.plot(subset['N'], subset['total_nos'], marker='s', label=est)
    
    plt.title("Trie vs Patricia: Consumo de Memória (Total de Nós)")
    plt.xlabel("Número de Elementos (N)")
    plt.ylabel("Quantidade de Nós Alocados")
    plt.legend()
    plt.grid(True)
    plt.savefig("experimentos/plots/2_trie_vs_patricia_nos.png")
    plt.close()

def plot_splay_vs_treap():
    df = pd.read_csv(f"{data_dir}/splay_vs_treap_insertion.csv")
    
    # 1. Insertion Random vs Sorted (Time)
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(15, 6))
    
    df_rand = df[df['distribuicao'] == 'aleatorio']
    for est in ['Splay', 'Treap']:
        sub = df_rand[df_rand['estrutura'] == est]
        ax1.plot(sub['N'], sub['tempo_insercao_ms'], marker='o', label=est)
    ax1.set_title("Tempo de Inserção - Dados Aleatórios")
    ax1.set_xlabel("N")
    ax1.set_ylabel("Tempo (ms)")
    ax1.legend()
    ax1.grid(True)

    df_sort = df[df['distribuicao'] == 'ordenado']
    for est in ['Splay', 'Treap']:
        sub = df_sort[df_sort['estrutura'] == est]
        ax2.plot(sub['N'], sub['tempo_insercao_ms'], marker='^', label=est)
    ax2.set_title("Tempo de Inserção - Dados Ordenados")
    ax2.set_xlabel("N")
    ax2.legend()
    ax2.grid(True)

    plt.tight_layout()
    plt.savefig("experimentos/plots/3_splay_vs_treap_tempo.png")
    plt.close()

    # 2. Rotations (Random)
    plt.figure(figsize=(10, 6))
    for est in ['Splay', 'Treap']:
        sub = df_rand[df_rand['estrutura'] == est]
        plt.plot(sub['N'], sub['rotacoes'], marker='x', label=est)
    plt.title("Splay vs Treap: Número de Rotações (Dados Aleatórios)")
    plt.xlabel("Número de Elementos (N)")
    plt.ylabel("Total de Rotações")
    plt.legend()
    plt.grid(True)
    plt.savefig("experimentos/plots/4_splay_vs_treap_rotacoes.png")
    plt.close()

def plot_locality():
    df = pd.read_csv(f"{data_dir}/locality_benchmark.csv")
    
    # Pivot for grouped bar chart
    pivot_df = df.pivot(index='padrao_acesso', columns='estrutura', values='tempo_busca_ms')
    
    ax = pivot_df.plot(kind='bar', figsize=(10, 6), colormap='Set2')
    plt.title("Impacto da Localidade Temporal (Splay vs Treap)")
    plt.xlabel("Padrão de Acesso")
    plt.ylabel("Tempo de Busca (ms)")
    plt.xticks(rotation=0)
    plt.grid(axis='y')
    plt.savefig("experimentos/plots/5_localidade_zipf.png")
    plt.close()

def plot_kdtree():
    df = pd.read_csv(f"{data_dir}/kdtree_benchmark.csv")
    df_2d = df[df['dimensao'] == '2D']
    
    plt.figure(figsize=(10, 6))
    plt.plot(df_2d['N'], df_2d['tempo_busca_exata_ms'], marker='o', label="Busca Exata")
    plt.plot(df_2d['N'], df_2d['tempo_range_search_ms'], marker='s', label="Busca em Região (Range)")
    plt.plot(df_2d['N'], df_2d['tempo_1nn_ms'], marker='^', label="Busca Vizinho + Próximo (1-NN)")
    
    plt.title("KD-Tree: Desempenho de Consultas Espaciais (2D)")
    plt.xlabel("Número de Elementos (N)")
    plt.ylabel("Tempo (ms)")
    plt.legend()
    plt.grid(True)
    plt.savefig("experimentos/plots/6_kdtree_consultas.png")
    plt.close()

if __name__ == "__main__":
    print("Gerando gráficos...")
    plot_trie_vs_patricia()
    plot_splay_vs_treap()
    plot_locality()
    plot_kdtree()
    print("Gráficos gerados com sucesso na pasta experimentos/plots/!")
