import random
import time
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
from matplotlib.animation import FuncAnimation
from matplotlib.widgets import Button, RadioButtons, Slider
from matplotlib.gridspec import GridSpec

# ── Constants ──────────────────────────────────────────────────────────────
ANIM_BASE   = 300        # interval ms at speed = 0
ANIM_MIN    = 5          # interval ms at max speed
DEFAULT_N   = 20
DEFAULT_SPD = 100

C_DEFAULT = "#4a90d9"
C_COMPARE = "#e74c3c"
C_PIVOT   = "#9b59b6"
C_SORTED  = "#2ecc71"

SIZE_OPTIONS = ["5", "10", "15", "20", "30", "50"]

COMPLEXITY = {
    "Bubble":    ("O(n^2)",      "O(n^2)",      "O(1)"),
    "Insertion": ("O(n)",        "O(n^2)",      "O(1)"),
    "Selection": ("O(n^2)",      "O(n^2)",      "O(1)"),
    "Quick":     ("O(n log n)",  "O(n^2)",      "O(log n)"),
    "Merge":     ("O(n log n)",  "O(n log n)",  "O(n)"),
    "Shell":     ("O(n log n)",  "O(n^1.5)",    "O(1)"),
}

# ── Datasets ───────────────────────────────────────────────────────────────

def make_random(n):
    a = list(range(1, n + 1)); random.shuffle(a); return a

def make_nearly(n):
    a = list(range(1, n + 1))
    for _ in range(max(1, n // 8)):
        i, j = random.randint(0, n-1), random.randint(0, n-1)
        a[i], a[j] = a[j], a[i]
    return a

def make_reverse(n):
    return list(range(n, 0, -1))

def make_dupes(n):
    return [random.randint(1, max(2, n // 4)) for _ in range(n)]

DATASETS = {
    "Random":          make_random,
    "Nearly Sorted":   make_nearly,
    "Reverse":         make_reverse,
    "Duplicate Heavy": make_dupes,
}

# ── Sorting generators ─────────────────────────────────────────────────────
# Each frame: (arr_snapshot, [active_indices], pivot_idx, [sorted_indices], comps, swaps, label)

def bubble_sort(arr):
    n = len(arr); c = s = 0
    for i in range(n):
        swapped = False
        for j in range(n - i - 1):
            c += 1
            yield arr[:], [j, j+1], -1, list(range(n-i, n)), c, s, "Comparing"
            if arr[j] > arr[j+1]:
                arr[j], arr[j+1] = arr[j+1], arr[j]; s += 1; swapped = True
                yield arr[:], [j, j+1], -1, list(range(n-i, n)), c, s, "Swapping"
        if not swapped:
            break
    yield arr[:], [], -1, list(range(n)), c, s, "Done"


def insertion_sort(arr):
    n = len(arr); c = s = 0
    for i in range(1, n):
        key = arr[i]; j = i - 1
        while j >= 0:
            c += 1
            yield arr[:], [j, j+1], -1, list(range(i)), c, s, "Comparing"
            if arr[j] > key:
                arr[j+1] = arr[j]; s += 1; j -= 1
                yield arr[:], [j+1, j+2], -1, list(range(i)), c, s, "Shifting"
            else:
                break
        arr[j+1] = key
        yield arr[:], [j+1], -1, list(range(i+1)), c, s, "Inserting"
    yield arr[:], [], -1, list(range(n)), c, s, "Done"


def selection_sort(arr):
    n = len(arr); c = s = 0
    for i in range(n):
        m = i
        for j in range(i+1, n):
            c += 1
            yield arr[:], [m, j], -1, list(range(i)), c, s, "Comparing"
            if arr[j] < arr[m]: m = j
        if m != i:
            arr[i], arr[m] = arr[m], arr[i]; s += 1
            yield arr[:], [i, m], -1, list(range(i)), c, s, "Swapping"
    yield arr[:], [], -1, list(range(n)), c, s, "Done"


def quick_sort(arr):
    n = len(arr); c = s = 0
    stack = [(0, n-1)]
    while stack:
        lo, hi = stack.pop()
        if lo >= hi: continue
        pv = arr[hi]; i = lo
        for j in range(lo, hi):
            c += 1
            yield arr[:], [j, hi], hi, [], c, s, "Comparing"
            if arr[j] <= pv:
                arr[i], arr[j] = arr[j], arr[i]; s += 1; i += 1
                yield arr[:], [i-1, j], hi, [], c, s, "Swapping"
        arr[i], arr[hi] = arr[hi], arr[i]; s += 1
        yield arr[:], [i], i, [], c, s, "Pivot placed"
        stack.append((lo, i-1)); stack.append((i+1, hi))
    yield arr[:], [], -1, list(range(n)), c, s, "Done"


def merge_sort(arr):
    n = len(arr); c = s = 0; w = 1
    while w < n:
        for i in range(0, n, 2*w):
            lo, mid, hi = i, min(i+w, n), min(i+2*w, n)
            L, R = arr[lo:mid], arr[mid:hi]
            li = ri = 0; k = lo
            while li < len(L) and ri < len(R):
                c += 1
                yield arr[:], [lo+li, mid+ri], -1, [], c, s, "Comparing"
                if L[li] <= R[ri]: arr[k] = L[li]; li += 1
                else:              arr[k] = R[ri]; ri += 1
                s += 1; k += 1
                yield arr[:], [k-1], -1, [], c, s, "Merging"
            while li < len(L):
                arr[k] = L[li]; li += 1; k += 1; s += 1
                yield arr[:], [k-1], -1, [], c, s, "Merging"
            while ri < len(R):
                arr[k] = R[ri]; ri += 1; k += 1; s += 1
                yield arr[:], [k-1], -1, [], c, s, "Merging"
        w *= 2
    yield arr[:], [], -1, list(range(n)), c, s, "Done"


def shell_sort(arr):
    n = len(arr); c = s = 0; gap = n // 2
    while gap > 0:
        for i in range(gap, n):
            tmp = arr[i]; j = i
            while j >= gap:
                c += 1
                yield arr[:], [j-gap, j], -1, [], c, s, "Comparing"
                if arr[j-gap] > tmp:
                    arr[j] = arr[j-gap]; s += 1; j -= gap
                    yield arr[:], [j, j+gap], -1, [], c, s, "Shifting"
                else:
                    break
            arr[j] = tmp
        gap //= 2
    yield arr[:], [], -1, list(range(n)), c, s, "Done"


ALGORITHMS = {
    "Bubble":    bubble_sort,
    "Insertion": insertion_sort,
    "Selection": selection_sort,
    "Quick":     quick_sort,
    "Merge":     merge_sort,
    "Shell":     shell_sort,
}

# ── Visualizer ─────────────────────────────────────────────────────────────

class Visualizer:
    def __init__(self):
        # ALL state initialised here — no AttributeErrors possible
        self.anim          = None          # FuncAnimation object (or None)
        self.is_running    = False
        self.is_paused     = False
        self.start_time    = None
        self.elapsed_acc   = 0.0
        self.algo_name     = "Bubble"
        self.dataset_name  = "Random"
        self.array_size    = DEFAULT_N
        self.speed         = DEFAULT_SPD
        self.arr           = []
        self.bars          = None

        self._build_figure()
        self._build_controls()
        self._fresh_array()

    # ── Figure ─────────────────────────────────────────────────────────────
    def _build_figure(self):
        self.fig = plt.figure(figsize=(14, 8), facecolor="#1a1a2e")
        self.fig.canvas.manager.set_window_title("Sorting Visualizer")

        gs = GridSpec(3, 4, figure=self.fig,
                      left=0.25, right=0.97, top=0.93, bottom=0.08,
                      hspace=0.5, wspace=0.4)

        self.ax       = self.fig.add_subplot(gs[:2, :])
        self.ax_comps = self.fig.add_subplot(gs[2, 0])
        self.ax_swaps = self.fig.add_subplot(gs[2, 1])
        self.ax_time  = self.fig.add_subplot(gs[2, 2])
        self.ax_stat  = self.fig.add_subplot(gs[2, 3])

        for a in [self.ax, self.ax_comps, self.ax_swaps, self.ax_time, self.ax_stat]:
            a.set_facecolor("#16213e")
            for sp in a.spines.values():
                sp.set_edgecolor("#0f3460")

        self.fig.text(0.6, 0.965, "Sorting Algorithm Visualizer",
                      color="white", fontsize=13, fontweight="bold", ha="center")

    # ── Controls ───────────────────────────────────────────────────────────
    def _build_controls(self):
        bg  = "#16213e"
        lkw = dict(color="#a0aec0", fontsize=8)

        self.fig.text(0.025, 0.885, "Algorithm", **lkw)
        ax_algo = plt.axes([0.01, 0.66, 0.21, 0.22], facecolor=bg)
        self.radio_algo = RadioButtons(ax_algo, list(ALGORITHMS.keys()), activecolor="#e94560")
        _style_radio(self.radio_algo)
        self.radio_algo.on_clicked(self._cb_algo)

        self.fig.text(0.025, 0.635, "Dataset", **lkw)
        ax_data = plt.axes([0.01, 0.46, 0.21, 0.17], facecolor=bg)
        self.radio_data = RadioButtons(ax_data, list(DATASETS.keys()), activecolor="#e94560")
        _style_radio(self.radio_data)
        self.radio_data.on_clicked(self._cb_data)

        self.fig.text(0.025, 0.44, "Array Size", **lkw)
        ax_size = plt.axes([0.01, 0.29, 0.21, 0.14], facecolor=bg)
        self.radio_size = RadioButtons(ax_size, SIZE_OPTIONS, activecolor="#e94560")
        _style_radio(self.radio_size)
        self.radio_size.set_active(SIZE_OPTIONS.index(str(DEFAULT_N)))
        self.radio_size.on_clicked(self._cb_size)

        self.fig.text(0.025, 0.27, "Speed", **lkw)
        ax_spd = plt.axes([0.01, 0.22, 0.21, 0.04], facecolor=bg)
        self.spd_slider = Slider(ax_spd, "", 1, 200,
                                 valinit=DEFAULT_SPD, color="#e94560", initcolor="none")
        self.spd_slider.valtext.set_color("white")
        self.spd_slider.on_changed(self._cb_speed)

        bkw = dict(color="#0f3460", hovercolor="#533483")
        ax_start = plt.axes([0.01,  0.155, 0.095, 0.055])
        ax_pause = plt.axes([0.115, 0.155, 0.095, 0.055])
        ax_stop  = plt.axes([0.01,  0.09,  0.095, 0.055])
        ax_reset = plt.axes([0.115, 0.09,  0.095, 0.055])

        self.btn_start = Button(ax_start, "Start",  **bkw)
        self.btn_pause = Button(ax_pause, "Pause",  **bkw)
        self.btn_stop  = Button(ax_stop,  "Stop",   **bkw)
        self.btn_reset = Button(ax_reset, "Reset",  **bkw)

        for b in [self.btn_start, self.btn_pause, self.btn_stop, self.btn_reset]:
            b.label.set_color("white"); b.label.set_fontsize(9)

        self.btn_start.on_clicked(self._cb_start)
        self.btn_pause.on_clicked(self._cb_pause)
        self.btn_stop.on_clicked(self._cb_stop)
        self.btn_reset.on_clicked(self._cb_reset)

        self.fig.legend(
            handles=[
                mpatches.Patch(facecolor=C_DEFAULT, label="Default"),
                mpatches.Patch(facecolor=C_COMPARE, label="Comparing"),
                mpatches.Patch(facecolor=C_PIVOT,   label="Pivot"),
                mpatches.Patch(facecolor=C_SORTED,  label="Sorted"),
            ],
            loc="lower left", bbox_to_anchor=(0.005, 0.002),
            ncol=2, fontsize=7, facecolor="#16213e",
            edgecolor="#0f3460", labelcolor="white"
        )

    # ── Drawing helpers ────────────────────────────────────────────────────
    def _fresh_array(self):
        self.arr = DATASETS[self.dataset_name](self.array_size)
        self._redraw(self.arr, [], -1, [])
        self._draw_stats(0, 0, 0.0, "Ready")

    def _redraw(self, arr, active, pivot, sorted_idx):
        """Clear ax and redraw all bars with correct colours."""
        self.ax.clear()
        self.ax.set_facecolor("#16213e")
        for sp in self.ax.spines.values():
            sp.set_edgecolor("#0f3460")

        colors = []
        for k in range(len(arr)):
            if k in sorted_idx:  colors.append(C_SORTED)
            elif k == pivot:     colors.append(C_PIVOT)
            elif k in active:    colors.append(C_COMPARE)
            else:                colors.append(C_DEFAULT)

        self.bars = self.ax.bar(range(len(arr)), arr, color=colors, edgecolor="none")
        self.ax.tick_params(colors="#a0aec0", labelsize=7)
        self.ax.set_xlim(-0.5, len(arr) - 0.5)
        self.ax.set_ylim(0, max(arr) * 1.12 if arr else 1)

        best, worst, space = COMPLEXITY[self.algo_name]
        self.ax.set_title(
            f"{self.algo_name} Sort    Best: {best}  |  Worst: {worst}  |  Space: {space}",
            color="white", fontsize=9, pad=5)

    def _draw_stats(self, comps, swaps, elapsed, label):
        for ax, val, title in [
            (self.ax_comps, f"{comps:,}",      "Comparisons"),
            (self.ax_swaps, f"{swaps:,}",      "Swaps"),
            (self.ax_time,  f"{elapsed:.2f}s", "Time"),
            (self.ax_stat,  label,             "Status"),
        ]:
            ax.clear()
            ax.set_facecolor("#16213e")
            for sp in ax.spines.values(): sp.set_edgecolor("#0f3460")
            ax.set_xticks([]); ax.set_yticks([])
            ax.text(0.5, 0.62, val,   transform=ax.transAxes,
                    color="white",   fontsize=11, fontweight="bold", ha="center", va="center")
            ax.text(0.5, 0.18, title, transform=ax.transAxes,
                    color="#a0aec0", fontsize=7,  ha="center", va="center")

    def _interval(self):
        return max(ANIM_MIN, int(ANIM_BASE - self.speed))

    def _kill_anim(self):
        """Safely stop and discard the current animation."""
        if self.anim is not None:
            try: self.anim.event_source.stop()
            except Exception: pass
            self.anim = None
        self.is_running = False
        self.is_paused  = False
        self.elapsed_acc = 0.0
        self.btn_pause.label.set_text("Pause")

    # ── Callbacks ──────────────────────────────────────────────────────────
    def _cb_algo(self, label):
        if self.is_running: return
        self.algo_name = label
        # refresh title without restarting
        self._redraw(self.arr, [], -1, [])
        self.fig.canvas.draw_idle()

    def _cb_data(self, label):
        if self.is_running: return
        self.dataset_name = label

    def _cb_size(self, label):
        if self.is_running: return
        self.array_size = int(label)

    def _cb_speed(self, val):
        self.speed = val
        if self.anim is not None:
            self.anim.event_source.interval = self._interval()

    def _cb_start(self, event):
        self._kill_anim()
        self.elapsed_acc = 0.0

        # build a fresh array and generator
        self.arr = DATASETS[self.dataset_name](self.array_size)
        gen = ALGORITHMS[self.algo_name](self.arr)   # generator over a copy

        # draw initial state
        self._redraw(self.arr, [], -1, [])
        self._draw_stats(0, 0, 0.0, "Running")
        self.fig.canvas.draw_idle()

        self.is_running = True
        self.start_time = time.time()

        def update(frame):
            if self.is_paused:
                return
            snap, active, pivot, sorted_idx, comps, swaps, label = frame
            elapsed = time.time() - self.start_time + self.elapsed_acc

            # update bar heights and colours in-place (fast, no ax.clear())
            for k, bar in enumerate(self.bars):
                bar.set_height(snap[k])
                if k in sorted_idx:  bar.set_color(C_SORTED)
                elif k == pivot:     bar.set_color(C_PIVOT)
                elif k in active:    bar.set_color(C_COMPARE)
                else:                bar.set_color(C_DEFAULT)

            self._draw_stats(comps, swaps, elapsed, label)

            if label == "Done":
                for bar in self.bars: bar.set_color(C_SORTED)
                self.is_running = False

        self.anim = FuncAnimation(
            self.fig, update, frames=gen,
            interval=self._interval(), repeat=False,
            cache_frame_data=False
        )
        self.fig.canvas.draw_idle()

    def _cb_pause(self, event):
        if not self.is_running: return
        if self.is_paused:
            self.is_paused   = False
            self.start_time  = time.time()
            self.btn_pause.label.set_text("Pause")
            if self.anim: self.anim.event_source.start()
        else:
            self.is_paused    = True
            self.elapsed_acc += time.time() - self.start_time
            self.btn_pause.label.set_text("Resume")
            if self.anim: self.anim.event_source.stop()
        self.fig.canvas.draw_idle()

    def _cb_stop(self, event):
        self._kill_anim()
        self._fresh_array()
        self.fig.canvas.draw_idle()

    def _cb_reset(self, event):
        self._kill_anim()
        self._fresh_array()
        self.fig.canvas.draw_idle()


# ── Utility ────────────────────────────────────────────────────────────────

def _style_radio(radio):
    for lbl in radio.labels:
        lbl.set_color("white"); lbl.set_fontsize(8)
    radio.ax.set_facecolor("#16213e")
    for sp in radio.ax.spines.values():
        sp.set_edgecolor("#0f3460")


# ── Entry point ────────────────────────────────────────────────────────────

if __name__ == "__main__":
    vis = Visualizer()
    plt.show()