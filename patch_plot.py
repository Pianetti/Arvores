with open("experimentos/scripts/plot_graphs.py", "r") as f:
    c = f.read()

# Update splay vs treap plot to include BST and AVL
c = c.replace("for est in ['Splay', 'Treap']:", "for est in ['Splay', 'Treap', 'BST', 'AVL']:")
c = c.replace("Splay vs Treap: Tempo de Inserção", "Comparação de Tempo de Inserção")
c = c.replace("Splay vs Treap: Número de Rotações", "Número de Rotações")

with open("experimentos/scripts/plot_graphs.py", "w") as f:
    f.write(c)
