"""Regenerates the figures and the web-Google numbers shown in the README.

    pip install numpy scipy pandas matplotlib
    python docs/figures/make_figures.py

Figure 1 is drawn from the C sources (class notes and tp1-tp3) and tp4/test.txt;
Figure 2 and the printed numbers are computed from tp4/web-Google.txt.
"""
from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import scipy.sparse as sp
from matplotlib import font_manager
from matplotlib.patches import Circle, Rectangle
from scipy.sparse.csgraph import connected_components

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[1]
for f in Path("/usr/share/fonts/lm").glob("lm*10-*.otf"):  # Latin Modern, if installed
    font_manager.fontManager.addfont(str(f))
plt.style.use(HERE / "paper.mplstyle")
C = plt.rcParams["axes.prop_cycle"].by_key()["color"]
INK, GRAY, TINT = "#1a1a1a", "#8c8c8c", "#e3eaf3"


def save(fig, name):
    fig.savefig(HERE / name, metadata={"Date": None})
    plt.close(fig)


# ---- drawing helpers (panel coordinates: x in [0, 10], y in [0, 6.4])
def cell(ax, x, y, w, h, s="", fc="white", fs=7.5):
    ax.add_patch(Rectangle((x, y), w, h, fc=fc, ec=INK, lw=0.6))
    if s:
        ax.text(x + w / 2, y + h / 2, s, ha="center", va="center", fontsize=fs, family="monospace")


def ptr(ax, a, b, rad=0.0, color=INK):
    ax.annotate("", xy=b, xytext=a, arrowprops=dict(arrowstyle="->,head_length=0.35,head_width=0.18", lw=0.7,
                                                    ec=color, shrinkA=0, shrinkB=0,
                                                    connectionstyle=f"arc3,rad={rad}"))


def dot(ax, x, y):
    ax.plot(x, y, "o", ms=2.2, color=INK)


def label(ax, x, y, s, **kw):
    ax.text(x, y, s, **{"fontsize": 7, "color": "#4d4d4d", "ha": "center", "va": "center", **kw})


def node(ax, x, y, v, w=1.3, h=0.7, last=False):
    """Singly linked node [value | next] with its lower-left corner at (x, y)."""
    cell(ax, x, y, w * 0.55, h, str(v), TINT)
    cell(ax, x + w * 0.55, y, w * 0.45, h, "/" if last else "")
    if not last:
        dot(ax, x + w * 0.775, y + h / 2)


def tnode(ax, x, y, v, r=0.38):
    ax.add_patch(Circle((x, y), r, fc=TINT, ec=INK, lw=0.6, zorder=2))
    ax.text(x, y, str(v), ha="center", va="center", fontsize=7.5, family="monospace", zorder=3)


def edge(ax, a, b, r=0.38, arrow=False):
    (x1, y1), (x2, y2) = a, b
    d = np.hypot(x2 - x1, y2 - y1)
    p = (x1 + (x2 - x1) * r / d, y1 + (y2 - y1) * r / d)
    q = (x2 - (x2 - x1) * r / d, y2 - (y2 - y1) * r / d)
    if arrow:
        ptr(ax, p, q)
    else:
        ax.plot(*zip(p, q), color=INK, lw=0.7, zorder=1)


fig, axes = plt.subplots(2, 4, figsize=(7.4, 3.5))
for ax in axes.flat:
    ax.set_xlim(0, 10)
    ax.set_ylim(0, 6.4)
    ax.set_aspect("equal")
    ax.axis("off")

# (a) singly linked list with first/last pointers (classes 3-4)
ax = axes[0, 0]
ax.set_title("(a) Linked list", fontsize=9)
cell(ax, 0.6, 4.6, 2.2, 0.7, "primero", fs=6.5)
cell(ax, 2.8, 4.6, 1.6, 0.7, "n = 3", fs=6.5)
cell(ax, 4.4, 4.6, 2.0, 0.7, "ultimo", fs=6.5)
for i, v in enumerate((7, 3, 9)):
    node(ax, 0.6 + 3.2 * i, 2.4, v, last=i == 2)
    if i < 2:
        ptr(ax, (0.6 + 3.2 * i + 1.0, 2.75), (0.6 + 3.2 * (i + 1), 2.75))
ptr(ax, (1.7, 4.6), (1.0, 3.1))
ptr(ax, (5.4, 4.6), (7.2, 3.1))
label(ax, 5, 1.2, "insert first / last O(1)\ndelete last O(n)")

# (b) circular list (class 3)
ax = axes[0, 1]
ax.set_title("(b) Circular list", fontsize=9)
cx, cy, R = 5.0, 3.0, 1.9
ang = np.deg2rad([90, -30, 210])
P = [(cx + R * np.cos(a), cy + R * np.sin(a)) for a in ang]
for (x, y), v in zip(P, (4, 8, 1)):
    tnode(ax, x, y, v)
for i in range(3):
    a, b = P[i], P[(i + 1) % 3]
    (x1, y1), (x2, y2) = a, b
    d = np.hypot(x2 - x1, y2 - y1)
    ptr(ax, (x1 + (x2 - x1) * 0.4 / d, y1 + (y2 - y1) * 0.4 / d),
        (x2 - (x2 - x1) * 0.4 / d, y2 - (y2 - y1) * 0.4 / d), rad=-0.35)
ax.text(1.0, 5.6, "primero", fontsize=6.5, family="monospace")
ptr(ax, (2.6, 5.5), (P[0][0] - 0.45, P[0][1] + 0.1))
label(ax, 5, 0.35, "insert at current O(1)")

# (c) stack and queue (class 4)
ax = axes[0, 2]
ax.set_title("(c) Stack and queue", fontsize=9)
for i, v in enumerate((5, 2, 6)):
    cell(ax, 0.9, 1.9 + 0.75 * i, 1.6, 0.75, str(v), TINT)
ax.text(0.2, 4.95, "tope", fontsize=6.5, family="monospace")
ptr(ax, (0.8, 4.8), (1.3, 4.15))
label(ax, 1.7, 1.2, "LIFO, O(1)")
for i, v in enumerate((3, 1, 4)):
    cell(ax, 4.4 + 1.4 * i, 2.4, 1.4, 0.75, str(v), TINT)
ax.text(4.1, 4.15, "primero", fontsize=6.5, family="monospace")
ax.text(7.2, 4.15, "ultimo", fontsize=6.5, family="monospace")
ptr(ax, (5.0, 4.0), (5.1, 3.2))
ptr(ax, (7.8, 4.0), (7.9, 3.2))
label(ax, 6.5, 1.2, "FIFO, O(1)")

# (d) doubly linked list with iterator (TP2)
ax = axes[0, 3]
ax.set_title("(d) Doubly linked list (TP2)", fontsize=9)
xs = (0.4, 3.7, 7.0)
for i, x in enumerate(xs):
    cell(ax, x, 2.6, 0.6, 0.7)
    cell(ax, x + 0.6, 2.6, 1.0, 0.7, "v%d" % (i + 1), TINT)
    cell(ax, x + 1.6, 2.6, 0.6, 0.7)
    if i < 2:
        ptr(ax, (x + 1.9, 3.1), (xs[i + 1], 3.1))
        ptr(ax, (xs[i + 1] + 0.3, 2.8), (x + 2.2, 2.8))
ax.text(0.3, 4.9, "head", fontsize=6.5, family="monospace")
ax.text(7.9, 4.9, "tail", fontsize=6.5, family="monospace")
ptr(ax, (0.8, 4.75), (1.0, 3.35))
ptr(ax, (8.3, 4.75), (8.1, 3.35))
cell(ax, 3.6, 0.6, 2.4, 0.7, "iter.curr", fs=6.5)
ptr(ax, (4.8, 1.3), (4.8, 2.55))
label(ax, 5, 5.85, "list_t: head, tail, size", fontsize=6.5)

# (e) binary min-heap, tree and array views (classes 6-7)
ax = axes[1, 0]
ax.set_title("(e) Binary min-heap", fontsize=9)
heap = [1, 3, 2, 7, 4, 5, 8]
pos = {0: (5, 5.4), 1: (2.8, 4.1), 2: (7.2, 4.1), 3: (1.6, 2.8), 4: (4.0, 2.8), 5: (6.0, 2.8), 6: (8.4, 2.8)}
for i in range(1, 7):
    edge(ax, pos[(i - 1) // 2], pos[i])
for i, v in enumerate(heap):
    tnode(ax, *pos[i], v)
for i, v in enumerate(heap):
    cell(ax, 1.5 + 1.0 * i, 0.9, 1.0, 0.7, str(v), TINT)
    ax.text(2.0 + 1.0 * i, 0.55, str(i), ha="center", va="center", fontsize=6, color=GRAY)
label(ax, 5, 0.05, "children of i at 2i+1, 2i+2", fontsize=6.5)

# (f) binary search tree (class 9)
ax = axes[1, 1]
ax.set_title("(f) Binary search tree", fontsize=9)
bst = {29: (5, 5.4), 16: (2.8, 4.1), 40: (7.2, 4.1), 10: (1.6, 2.8), 20: (4.0, 2.8), 35: (6.0, 2.8)}
for a, b in ((29, 16), (29, 40), (16, 10), (16, 20), (40, 35)):
    edge(ax, bst[a], bst[b])
for k, p in bst.items():
    tnode(ax, *p, k)
label(ax, 5, 1.55, "left < node < right", fontsize=6.5)
label(ax, 5, 0.75, "in-order: 10 16 20 29 35 40", fontsize=6.5)
label(ax, 5, 0.05, "O(log n) balanced, O(n) degenerate", fontsize=6.5)

# (g) hash table with separate chaining (TP3); buckets from the course's hash function, table size 5
ax = axes[1, 2]
ax.set_title("(g) Hash table (TP3)", fontsize=9)


def oaat(key, n):  # Jenkins one-at-a-time, as in tp3/tp3.c
    h = 0
    for c in key.encode():
        h = (h + c) & 0xFFFFFFFF
        h = (h + (h << 10)) & 0xFFFFFFFF
        h ^= h >> 6
    h = (h + (h << 3)) & 0xFFFFFFFF
    h ^= h >> 11
    h = (h + (h << 15)) & 0xFFFFFFFF
    return h % n


chains = {i: [] for i in range(5)}
for k in ("key1", "key2", "dos"):  # inserted in this order; new nodes go to the head of the chain
    chains[oaat(k, 5)].insert(0, k)
for i in range(5):
    y = 4.9 - 0.85 * i
    cell(ax, 0.6, y, 0.9, 0.85)
    ax.text(0.3, y + 0.42, str(i), ha="center", va="center", fontsize=6, color=GRAY)
    x = 2.2
    if chains[i]:
        dot(ax, 1.05, y + 0.42)
        ptr(ax, (1.05, y + 0.42), (x, y + 0.42))
    for j, k in enumerate(chains[i]):
        cell(ax, x, y + 0.08, 1.8, 0.7, k, TINT, fs=6.5)
        cell(ax, x + 1.8, y + 0.08, 0.6, 0.7)
        if j < len(chains[i]) - 1:
            dot(ax, x + 2.1, y + 0.43)
            ptr(ax, (x + 2.1, y + 0.43), (x + 3.0, y + 0.43))
        x += 3.0
label(ax, 5, 0.35, "size doubles when $n/m \\geq 0.75$", fontsize=6.5)

# (h) directed graph as a dict of dicts (TP4), excerpt of tp4/test.txt
ax = axes[1, 3]
ax.set_title("(h) Directed graph (TP4)", fontsize=9)
G = {1: (1.0, 4.6), 2: (2.9, 4.6), 3: (1.95, 3.0), 13: (5.0, 5.0), 14: (6.6, 5.0), 15: (6.6, 3.4), 16: (5.0, 3.4),
     17: (7.6, 1.8), 18: (6.0, 1.8), 19: (4.4, 1.8), 20: (2.8, 1.8)}
for a, b in ((1, 2), (2, 3), (3, 1), (13, 14), (14, 15), (15, 16), (16, 13), (17, 18), (18, 19), (19, 20)):
    edge(ax, G[a], G[b], r=0.36, arrow=True)
for k, p in G.items():
    ax.add_patch(Circle(p, 0.36, fc=TINT, ec=INK, lw=0.6, zorder=2))
    ax.text(*p, str(k), ha="center", va="center", fontsize=6.5, family="monospace", zorder=3)
label(ax, 5, 0.6, "{'1': {'neighbors': {'2': None}}, ...}", fontsize=6, family="monospace")
fig.subplots_adjust(left=0.01, right=0.99, top=0.94, bottom=0.01, wspace=0.08, hspace=0.18)
save(fig, "fig1-data-structures.svg")

# ---- Figure 2: the web-Google graph used in TP4
edges = pd.read_csv(ROOT / "tp4/web-Google.txt", sep="\t", comment="#", header=None, dtype=np.int64).to_numpy()
ids, idx = np.unique(edges, return_inverse=True)
idx = idx.reshape(edges.shape)
n = len(ids)
A = sp.csr_matrix((np.ones(len(edges)), (idx[:, 0], idx[:, 1])), shape=(n, n))
outdeg = np.diff(A.indptr)
indeg = np.bincount(idx[:, 1], minlength=n)
ncomp, lab = connected_components(A, directed=True, connection="weak")
sizes = np.sort(np.bincount(lab))[::-1]
A2 = A @ A
closed3 = int(A2.multiply(A.T).sum())  # closed directed walks u->v->w->u, what tp4.triangles counts
links = np.asarray(A2.multiply(A).sum(axis=1)).ravel()
k = outdeg.astype(float)
clustering = np.where(k >= 2, links / np.maximum(k * (k - 1), 1), 0).mean()


def ccdf(d):
    v, c = np.unique(d[d > 0], return_counts=True)
    return v, c[::-1].cumsum()[::-1] / len(d)  # P(K >= v) over all nodes


fig, axes = plt.subplots(1, 2, figsize=(7.2, 2.7))
for d, lab_, c in ((indeg, "in-degree", C[0]), (outdeg, "out-degree", C[1])):
    v, p = ccdf(d)
    axes[0].loglog(v, p, "-", lw=1.1, color=c, label=lab_)
axes[0].set_xlabel("degree $k$")
axes[0].set_ylabel("$P(K \\geq k)$")
axes[0].set_title("(a) Degree distribution")
axes[0].legend(loc="lower left")
r = np.arange(1, len(sizes) + 1)
axes[1].loglog(r, sizes, "-", lw=1.1, color=C[2], drawstyle="steps-post")
axes[1].annotate(f"giant component: {sizes[0]:,} nodes", xy=(1, sizes[0]), xytext=(3, sizes[0] / 3),
                 fontsize=7.5, arrowprops=dict(arrowstyle="->", lw=0.6))
axes[1].set_xlabel("component rank")
axes[1].set_ylabel("nodes in component")
axes[1].set_title(f"(b) {ncomp:,} weakly connected components")
fig.tight_layout()
save(fig, "fig2-web-google.svg")

print(f"nodes {n:,}, edges {len(edges):,}, mean out-degree {outdeg.mean():.2f}, "
      f"max in-degree {indeg.max():,}, max out-degree {outdeg.max():,}")
print(f"weak components {ncomp:,}, largest {sizes[0]:,}")
print(f"closed directed 3-walks {closed3:,} (= 3 x {closed3 // 3:,} directed 3-cycles)")
print(f"mean local clustering (out-neighbours) {clustering:.6f}")
