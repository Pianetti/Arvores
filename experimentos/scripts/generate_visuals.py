#!/usr/bin/env python3
import re
import os
import glob
from collections import defaultdict, deque

def parse_dot(dot_text):
    title_match = re.search(r'label="([^"]+)";', dot_text)
    title = title_match.group(1) if title_match else "Árvore"

    nodes = {}
    edges = []

    # Parse nodes
    # e.g.: node_0 [label="root", shape=circle, fillcolor="#E2E8F0"];
    # or: n50 [shape=record, style=filled, fillcolor="#F0FDF4", color="#16A34A", label="{ chave: 50 | prioridade: 50 }"];
    node_pattern = re.compile(r'^\s*([a-zA-Z0-9_]+)\s*\[(.*?)\];', re.MULTILINE)
    for m in node_pattern.finditer(dot_text):
        nid, attrs_str = m.group(1), m.group(2)
        if nid.startswith("node") or nid.startswith("n"):
            attrs = {}
            for am in re.finditer(r'([a-zA-Z_]+)="?([^",\]]+)"?', attrs_str):
                attrs[am.group(1)] = am.group(2).strip('"')
            nodes[nid] = attrs

    # Parse edges
    # e.g.: node_0 -> node_1 [label=" c "];
    edge_pattern = re.compile(r'^\s*([a-zA-Z0-9_]+)\s*->\s*([a-zA-Z0-9_]+)\s*(?:\[(.*?)\])?;\s*$', re.MULTILINE)
    for m in edge_pattern.finditer(dot_text):
        u, v, attrs_str = m.group(1), m.group(2), m.group(3) or ""
        lbl_match = re.search(r'label="([^"]*)"', attrs_str)
        edge_lbl = lbl_match.group(1).strip() if lbl_match else ""
        edges.append((u, v, edge_lbl))

    return title, nodes, edges

def render_tree_to_svg(title, nodes, edges, output_file):
    # Find root (node with in-degree 0)
    in_degree = defaultdict(int)
    adj = defaultdict(list)
    for u, v, lbl in edges:
        in_degree[v] += 1
        adj[u].append((v, lbl))

    all_node_ids = list(nodes.keys())
    if not all_node_ids:
        return

    roots = [nid for nid in all_node_ids if in_degree[nid] == 0]
    root = roots[0] if roots else all_node_ids[0]

    # Calculate tree levels
    level = {}
    q = deque([(root, 0)])
    level[root] = 0
    visited = {root}
    while q:
        curr, d = q.popleft()
        for nxt, _ in adj[curr]:
            if nxt not in visited:
                visited.add(nxt)
                level[nxt] = d + 1
                q.append((nxt, d + 1))

    # Group by level
    by_level = defaultdict(list)
    for nid, d in level.items():
        by_level[d].append(nid)

    max_d = max(by_level.keys()) if by_level else 0

    # Assign positions
    node_x = {}
    node_y = {}
    level_height = 85
    start_y = 70

    # Layout nodes horizontally per level
    max_nodes_in_level = max(len(v) for v in by_level.values()) if by_level else 1
    svg_w = max(700, max_nodes_in_level * 140)
    svg_h = max(350, (max_d + 1) * level_height + 110)

    for d in range(max_d + 1):
        row = by_level[d]
        n_row = len(row)
        step = svg_w / (n_row + 1)
        for i, nid in enumerate(row):
            node_x[nid] = (i + 1) * step
            node_y[nid] = start_y + d * level_height

    # Build SVG
    svg = []
    svg.append(f'<svg width="{svg_w}" height="{svg_h}" xmlns="http://www.w3.org/2000/svg">')
    svg.append(f'  <rect width="100%" height="100%" fill="#F8FAFC" rx="10" />')
    svg.append(f'  <text x="{svg_w/2}" y="36" font-family="Helvetica, sans-serif" font-size="16" font-weight="bold" fill="#0F172A" text-anchor="middle">{title}</text>')

    # Edges
    for u, v, lbl in edges:
        if u not in node_x or v not in node_x: continue
        x1, y1 = node_x[u], node_y[u]
        x2, y2 = node_x[v], node_y[v]

        svg.append(f'  <line x1="{x1}" y1="{y1}" x2="{x2}" y2="{y2}" stroke="#64748B" stroke-width="2" />')
        if lbl:
            # Clean label
            clean_lbl = lbl.replace('\\"', '').replace('"', '').strip()
            mx, my = (x1 + x2) / 2, (y1 + y2) / 2 - 4
            svg.append(f'  <rect x="{mx - 18}" y="{my - 12}" width="36" height="16" fill="#FFFFFF" rx="4" stroke="#CBD5E1" stroke-width="1" />')
            svg.append(f'  <text x="{mx}" y="{my}" font-family="Helvetica, sans-serif" font-size="10" font-weight="bold" fill="#2563EB" text-anchor="middle">{clean_lbl}</text>')

    # Nodes
    for nid, attrs in nodes.items():
        if nid not in node_x: continue
        x, y = node_x[nid], node_y[nid]
        raw_label = attrs.get('label', nid)
        clean_lbl = raw_label.replace('{', '').replace('}', '').replace('\\"', '').replace('"', '').strip()
        
        # Color styling
        fill = attrs.get('fillcolor', '#EFF6FF')
        stroke = attrs.get('color', '#3B82F6')

        # Check if record or simple
        if '|' in clean_lbl:
            parts = [p.strip() for p in clean_lbl.split('|')]
            rw = 130
            rh = 40
            svg.append(f'  <rect x="{x - rw/2}" y="{y - rh/2}" width="{rw}" height="{rh}" rx="6" fill="{fill}" stroke="{stroke}" stroke-width="2" />')
            svg.append(f'  <text x="{x}" y="{y - 4}" font-family="Helvetica, sans-serif" font-size="11" font-weight="bold" fill="#0F172A" text-anchor="middle">{parts[0]}</text>')
            svg.append(f'  <text x="{x}" y="{y + 12}" font-family="Helvetica, sans-serif" font-size="10" fill="#475569" text-anchor="middle">{parts[1]}</text>')
        else:
            is_root = 'root' in clean_lbl.lower()
            is_terminal = attrs.get('shape') == 'doublecircle'
            r = 24 if not is_terminal else 26
            
            svg.append(f'  <circle cx="{x}" cy="{y}" r="{r}" fill="{fill}" stroke="{stroke}" stroke-width="2.5" />')
            if is_terminal:
                svg.append(f'  <circle cx="{x}" cy="{y}" r="{r - 4}" fill="none" stroke="{stroke}" stroke-width="1.5" />')
            
            disp_txt = clean_lbl if clean_lbl else nid
            if len(disp_txt) > 10:
                disp_txt = disp_txt[:9] + "…"
            svg.append(f'  <text x="{x}" y="{y + 4}" font-family="Helvetica, sans-serif" font-size="11" font-weight="bold" fill="#0F172A" text-anchor="middle">{disp_txt}</text>')

    svg.append('</svg>')
    with open(output_file, 'w', encoding='utf-8') as f:
        f.write("\n".join(svg))

def main():
    dest_dir = "experimentos/plots/diagramas"
    os.makedirs(dest_dir, exist_ok=True)

    dot_files = glob.glob("*.dot")
    print(f"Encontrados {len(dot_files)} arquivos .dot para renderizar...")

    for df in dot_files:
        with open(df, 'r', encoding='utf-8') as f:
            content = f.read()
        title, nodes, edges = parse_dot(content)
        base = os.path.splitext(os.path.basename(df))[0]
        out_svg = os.path.join(dest_dir, f"{base}.svg")
        render_tree_to_svg(title, nodes, edges, out_svg)
        print(f"  [RENDER] {df} -> {out_svg}")

    # Copia os svgs espaciais da kdtree para a pasta de diagramas
    for kf in glob.glob("kdtree_particao2d_*.svg"):
        os.system(f"cp '{kf}' '{dest_dir}/'")
        print(f"  [COPIADO] {kf} -> {dest_dir}/")

    print("[SUCESSO] Todos os diagramas visuais foram renderizados em SVG!")

if __name__ == "__main__":
    main()
