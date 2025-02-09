import tkinter as tk
from tkinter import filedialog, messagebox
import networkx as nx
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import sys
import numpy as np


class GraphGUI:
    def __init__(self, root, filename=None):
        self.root = root
        self.root.title("Graph Parser & Visualizer")
        self.graph = nx.DiGraph()
        self.canvas_frame = tk.Frame(root)
        self.canvas_frame.pack()

        if filename:
            try:
                self.parse_graph_from_file(filename)
                self.display_canvas()
            except Exception as e:
                messagebox.showerror("Error", str(e))
        else:
            self.load_button = tk.Button(root, text="Load Graph File", command=self.load_graph)
            self.load_button.pack()

    def load_graph(self):
        filename = filedialog.askopenfilename(filetypes=[("Text files", "*.txt")])
        if not filename:
            return
        
        try:
            self.parse_graph_from_file(filename)
            messagebox.showinfo("Success", "Graph loaded successfully!")
            self.display_canvas()
        except Exception as e:
            messagebox.showerror("Error", str(e))

    def parse_graph_from_file(self, filename):
        self.graph.clear()
        with open(filename, 'r') as file:
            for line in file:
                parts = line.strip().split()
                if not parts:
                    continue
                
                if parts[0] == '*':
                    self.graph.add_node(parts[1])
                elif parts[0] == '-':
                    self.graph.add_edge(parts[1], parts[2])

    def get_circular_layout(self):
        # Circular Layout
        return nx.circular_layout(self.graph)

    def display_canvas(self):
        for widget in self.canvas_frame.winfo_children():
            widget.destroy()
        
        fig, ax = plt.subplots(figsize=(8, 8))
        pos = self.get_circular_layout()

        # Draw edges
        nx.draw_networkx_edges(self.graph, pos,
                               edge_color='gray',
                               arrows=True,
                               arrowsize=20,
                               width=1,
                               min_source_margin=20,
                               min_target_margin=20)

        # Draw nodes
        nx.draw_networkx_nodes(self.graph, pos,
                               node_color='lightblue',
                               node_size=800,
                               edgecolors='white',
                               linewidths=2,
                               alpha=0.9)
        
        # Draw labels
        nx.draw_networkx_labels(self.graph, pos,
                                font_size=10,
                                font_weight='bold')
        
        # Remove axis
        plt.axis('off')
        ax.margins(0.2)
        
        canvas = FigureCanvasTkAgg(fig, master=self.canvas_frame)
        canvas.draw()
        canvas.get_tk_widget().pack()


if __name__ == "__main__":
    filename = sys.argv[1] if len(sys.argv) > 1 else None
    root = tk.Tk()
    app = GraphGUI(root, filename)
    root.mainloop()