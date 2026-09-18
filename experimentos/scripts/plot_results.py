#!/usr/bin/env python3
import csv
import os

def create_svg_chart(title, x_label, y_label, series, width=640, height=400):
    """
    series is a list of dicts:
    {
        'label': 'Series Name',
        'color': '#hex',
        'points': [(x1, y1), (x2, y2), ...]
    }
    """
    pad_left = 75
    pad_right = 30
    pad_top = 50
    pad_bottom = 55
    plot_w = width - pad_left - pad_right
    plot_h = height - pad_top - pad_bottom

    all_x = [p[0] for s in series for p in s['points']]
    all_y = [p[1] for s in series for p in s['points']]

    min_x = min(all_x) if all_x else 0
    max_x = max(all_x) if all_x else 1
    min_y = 0
    max_y = max(all_y) * 1.1 if all_y else 1

    if max_x == min_x: max_x += 1
    if max_y == min_y: max_y += 1

    def scale_x(val):
        return pad_left + (val - min_x) / (max_x - min_x) * plot_w

    def scale_y(val):
        return pad_top + plot_h - (val - min_y) / (max_y - min_y) * plot_h

    svg = []
    svg.append(f'<svg width="{width}" height="{height}" xmlns="http://www.w3.org/2000/svg">')
    svg.append(f'  <rect width="100%" height="100%" fill="#FFFFFF" rx="8" />')
    svg.append(f'  <text x="{width/2}" y="28" font-family="Helvetica, sans-serif" font-size="15" font-weight="bold" fill="#1E293B" text-anchor="middle">{title}</text>')

    # Gridlines and Y ticks
    num_y_ticks = 5
    for i in range(num_y_ticks + 1):
        y_val = min_y + (max_y - min_y) * (i / num_y_ticks)
        y_pos = scale_y(y_val)
        svg.append(f'  <line x1="{pad_left}" y1="{y_pos}" x2="{width - pad_right}" y2="{y_pos}" stroke="#E2E8F0" stroke-width="1" />')
        val_str = f"{y_val:.1f}" if max_y < 10 else f"{int(y_val):,}".replace(",", ".")
        svg.append(f'  <text x="{pad_left - 10}" y="{y_pos + 4}" font-family="Helvetica, sans-serif" font-size="11" fill="#64748B" text-anchor="end">{val_str}</text>')

    # X axis ticks
    unique_x = sorted(list(set(all_x)))
    for x_val in unique_x:
        x_pos = scale_x(x_val)
        svg.append(f'  <line x1="{x_pos}" y1="{pad_top}" x2="{x_pos}" y2="{pad_top + plot_h}" stroke="#F1F5F9" stroke-width="1" />')
        svg.append(f'  <line x1="{x_pos}" y1="{pad_top + plot_h}" x2="{x_pos}" y2="{pad_top + plot_h + 5}" stroke="#94A3B8" stroke-width="1.5" />')
        x_str = f"{int(x_val):,}".replace(",", ".")
        svg.append(f'  <text x="{x_pos}" y="{pad_top + plot_h + 20}" font-family="Helvetica, sans-serif" font-size="11" fill="#64748B" text-anchor="middle">{x_str}</text>')

    # Axes lines
    svg.append(f'  <line x1="{pad_left}" y1="{pad_top + plot_h}" x2="{width - pad_right}" y2="{pad_top + plot_h}" stroke="#475569" stroke-width="1.5" />')
    svg.append(f'  <line x1="{pad_left}" y1="{pad_top}" x2="{pad_left}" y2="{pad_top + plot_h}" stroke="#475569" stroke-width="1.5" />')

    # Axis Labels
    svg.append(f'  <text x="{pad_left + plot_w/2}" y="{height - 12}" font-family="Helvetica, sans-serif" font-size="12" fill="#334155" text-anchor="middle">{x_label}</text>')
    svg.append(f'  <text x="18" y="{pad_top + plot_h/2}" font-family="Helvetica, sans-serif" font-size="12" fill="#334155" text-anchor="middle" transform="rotate(-90 18 {pad_top + plot_h/2})">{y_label}</text>')

    # Draw series lines and points
    for s in series:
        pts = s['points']
        if not pts: continue
        d_str = " ".join([f"{'M' if i==0 else 'L'} {scale_x(p[0]):.1f} {scale_y(p[1]):.1f}" for i, p in enumerate(pts)])
        svg.append(f'  <path d="{d_str}" fill="none" stroke="{s["color"]}" stroke-width="2.5" />')
        for p in pts:
            svg.append(f'  <circle cx="{scale_x(p[0]):.1f}" cy="{scale_y(p[1]):.1f}" r="4" fill="{s["color"]}" stroke="#FFFFFF" stroke-width="1.5" />')

    # Legend
    leg_x = pad_left + 15
    leg_y = pad_top + 15
    for i, s in enumerate(series):
        lx = leg_x + i * 140
        svg.append(f'  <line x1="{lx}" y1="{leg_y}" x2="{lx + 20}" y2="{leg_y}" stroke="{s["color"]}" stroke-width="3" />')
        svg.append(f'  <circle cx="{lx + 10}" cy="{leg_y}" r="3.5" fill="{s["color"]}" />')
        svg.append(f'  <text x="{lx + 26}" y="{leg_y + 4}" font-family="Helvetica, sans-serif" font-size="11" font-weight="bold" fill="#334155">{s["label"]}</text>')

    svg.append('</svg>')
    return "\n".join(svg)


def create_svg_bar_chart(title, categories, series, y_label, width=600, height=380):
    pad_left = 75
    pad_right = 30
    pad_top = 50
    pad_bottom = 50
    plot_w = width - pad_left - pad_right
    plot_h = height - pad_top - pad_bottom

    all_vals = [v for s in series for v in s['values']]
    max_val = max(all_vals) * 1.15 if all_vals else 1

    svg = []
    svg.append(f'<svg width="{width}" height="{height}" xmlns="http://www.w3.org/2000/svg">')
    svg.append(f'  <rect width="100%" height="100%" fill="#FFFFFF" rx="8" />')
    svg.append(f'  <text x="{width/2}" y="28" font-family="Helvetica, sans-serif" font-size="15" font-weight="bold" fill="#1E293B" text-anchor="middle">{title}</text>')

    num_y_ticks = 5
    for i in range(num_y_ticks + 1):
        y_val = max_val * (i / num_y_ticks)
        y_pos = pad_top + plot_h - (y_val / max_val) * plot_h
        svg.append(f'  <line x1="{pad_left}" y1="{y_pos}" x2="{width - pad_right}" y2="{y_pos}" stroke="#E2E8F0" stroke-width="1" />')
        svg.append(f'  <text x="{pad_left - 10}" y="{y_pos + 4}" font-family="Helvetica, sans-serif" font-size="11" fill="#64748B" text-anchor="end">{y_val:.1f}</text>')

    svg.append(f'  <line x1="{pad_left}" y1="{pad_top + plot_h}" x2="{width - pad_right}" y2="{pad_top + plot_h}" stroke="#475569" stroke-width="1.5" />')
    svg.append(f'  <line x1="{pad_left}" y1="{pad_top}" x2="{pad_left}" y2="{pad_top + plot_h}" stroke="#475569" stroke-width="1.5" />')
    svg.append(f'  <text x="18" y="{pad_top + plot_h/2}" font-family="Helvetica, sans-serif" font-size="12" fill="#334155" text-anchor="middle" transform="rotate(-90 18 {pad_top + plot_h/2})">{y_label}</text>')

    n_cats = len(categories)
    n_series = len(series)
    group_width = plot_w / n_cats
    bar_width = (group_width * 0.6) / n_series

    for ci, cat in enumerate(categories):
        group_center = pad_left + (ci + 0.5) * group_width
        svg.append(f'  <text x="{group_center}" y="{pad_top + plot_h + 20}" font-family="Helvetica, sans-serif" font-size="12" font-weight="bold" fill="#334155" text-anchor="middle">{cat}</text>')
        
        start_x = group_center - (n_series * bar_width) / 2
        for si, s in enumerate(series):
            bx = start_x + si * bar_width
            b_val = s['values'][ci]
            bh = (b_val / max_val) * plot_h
            by = pad_top + plot_h - bh
            svg.append(f'  <rect x="{bx}" y="{by}" width="{bar_width - 4}" height="{bh}" fill="{s["color"]}" rx="3" />')
            svg.append(f'  <text x="{bx + (bar_width-4)/2}" y="{by - 6}" font-family="Helvetica, sans-serif" font-size="10" fill="#1E293B" text-anchor="middle">{b_val:.1f}</text>')

    # Legend
    leg_x = pad_left + 15
    leg_y = pad_top + 15
    for i, s in enumerate(series):
        lx = leg_x + i * 140
        svg.append(f'  <rect x="{lx}" y="{leg_y - 8}" width="14" height="14" fill="{s["color"]}" rx="2" />')
        svg.append(f'  <text x="{lx + 20}" y="{leg_y + 3}" font-family="Helvetica, sans-serif" font-size="11" font-weight="bold" fill="#334155">{s["label"]}</text>')

    svg.append('</svg>')
    return "\n".join(svg)


def main():
    os.makedirs("experimentos/plots", exist_ok=True)

    # 1. Plot Trie vs Patricia (Nós e Memória)
    trie_nodes = []
    pat_nodes = []
    trie_ins_time = []
    pat_ins_time = []

    with open("experimentos/data/trie_vs_patricia.csv", "r") as f:
        reader = csv.DictReader(f)
        for row in reader:
            if row['tipo_dado'] == 'aleatorio':
                n = float(row['N'])
                nodes = float(row['total_nos'])
                ins_time = float(row['tempo_insercao_ms'])
                if row['estrutura'] == 'Trie':
                    trie_nodes.append((n, nodes))
                    trie_ins_time.append((n, ins_time))
                else:
                    pat_nodes.append((n, nodes))
                    pat_ins_time.append((n, ins_time))

    svg1 = create_svg_chart(
        "Consumo de Nós: Trie vs. Patricia (Radix Compacta)",
        "Tamanho da Entrada (N)", "Quantidade Total de Nós Alocados",
        [
            {'label': 'Trie', 'color': '#EF4444', 'points': trie_nodes},
            {'label': 'Patricia', 'color': '#2563EB', 'points': pat_nodes}
        ]
    )
    with open("experimentos/plots/trie_vs_patricia_nos.svg", "w") as f:
        f.write(svg1)

    svg2 = create_svg_chart(
        "Tempo de Inserção: Trie vs. Patricia (ms)",
        "Tamanho da Entrada (N)", "Tempo de Execução (ms)",
        [
            {'label': 'Trie', 'color': '#EF4444', 'points': trie_ins_time},
            {'label': 'Patricia', 'color': '#2563EB', 'points': pat_ins_time}
        ]
    )
    with open("experimentos/plots/trie_vs_patricia_tempo.svg", "w") as f:
        f.write(svg2)

    # 2. Plot Splay vs Treap
    splay_rand = []
    treap_rand = []
    splay_sort = []
    treap_sort = []

    with open("experimentos/data/splay_vs_treap_insertion.csv", "r") as f:
        reader = csv.DictReader(f)
        for row in reader:
            n = float(row['N'])
            ins_time = float(row['tempo_insercao_ms'])
            if row['distribuicao'] == 'aleatorio':
                if row['estrutura'] == 'Splay': splay_rand.append((n, ins_time))
                else: treap_rand.append((n, ins_time))
            else:
                if row['estrutura'] == 'Splay': splay_sort.append((n, ins_time))
                else: treap_sort.append((n, ins_time))

    svg3 = create_svg_chart(
        "Tempo de Inserção: Splay vs. Treap (Aleatório)",
        "Tamanho da Entrada (N)", "Tempo de Execução (ms)",
        [
            {'label': 'Splay Tree', 'color': '#8B5CF6', 'points': splay_rand},
            {'label': 'Treap', 'color': '#10B981', 'points': treap_rand}
        ]
    )
    with open("experimentos/plots/splay_vs_treap_aleatorio.svg", "w") as f:
        f.write(svg3)

    svg4 = create_svg_chart(
        "Tempo de Inserção: Splay vs. Treap (Sequencial Ordenado)",
        "Tamanho da Entrada (N)", "Tempo de Execução (ms)",
        [
            {'label': 'Splay Tree', 'color': '#8B5CF6', 'points': splay_sort},
            {'label': 'Treap', 'color': '#10B981', 'points': treap_sort}
        ]
    )
    with open("experimentos/plots/splay_vs_treap_ordenado.svg", "w") as f:
        f.write(svg4)

    # 3. Plot Localidade Temporal
    cats = []
    splay_times = []
    treap_times = []
    with open("experimentos/data/locality_benchmark.csv", "r") as f:
        reader = csv.DictReader(f)
        for row in reader:
            if row['estrutura'] == 'Splay':
                cats.append(row['padrao_acesso'])
                splay_times.append(float(row['tempo_busca_ms']))
            else:
                treap_times.append(float(row['tempo_busca_ms']))

    svg5 = create_svg_bar_chart(
        "Impacto da Localidade Temporal: Splay vs. Treap (100k Consultas)",
        cats,
        [
            {'label': 'Splay Tree', 'color': '#8B5CF6', 'values': splay_times},
            {'label': 'Treap', 'color': '#10B981', 'values': treap_times}
        ],
        "Tempo Total de Busca (ms)"
    )
    with open("experimentos/plots/localidade_temporal.svg", "w") as f:
        f.write(svg5)

    # 4. Plot KD-Tree
    kd_range = []
    kd_nn = []
    kd_search = []
    with open("experimentos/data/kdtree_benchmark.csv", "r") as f:
        reader = csv.DictReader(f)
        for row in reader:
            n = float(row['N'])
            kd_range.append((n, float(row['tempo_range_search_ms'])))
            kd_nn.append((n, float(row['tempo_1nn_ms'])))
            kd_search.append((n, float(row['tempo_busca_exata_ms'])))

    svg6 = create_svg_chart(
        "KD-Tree 2D: Desempenho de Consultas Espaciais",
        "Total de Pontos (N)", "Tempo de Execução (ms)",
        [
            {'label': '1-NN (1.000 queries)', 'color': '#F59E0B', 'points': kd_nn},
            {'label': 'Range Search (100 queries)', 'color': '#06B6D4', 'points': kd_range},
            {'label': 'Busca Exata (1.000 queries)', 'color': '#6366F1', 'points': kd_search}
        ]
    )
    with open("experimentos/plots/kdtree_escalabilidade.svg", "w") as f:
        f.write(svg6)

    print("[SUCESSO] Todos os gráficos SVG foram gerados em experimentos/plots/!")

if __name__ == "__main__":
    main()
