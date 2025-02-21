#!/usr/bin/env python3
"""
Day 9: Disk Fragmenter — Animated GIF Generator
Requires: pip install Pillow imageio

Usage:
    python make_gif.py sample.txt
    python make_gif.py sample.txt --part 2 --fps 6
    python make_gif.py input.txt --part 2 --step 5
    python make_gif.py input.txt --part 1 --step 200
    python make_gif.py input.txt --width 1200 --height 600
"""

import sys
import math
import argparse
import numpy as np
from PIL import Image
import imageio.v2 as iio

# ---------------------------------------------------------------------------
# Colours
# ---------------------------------------------------------------------------

_PAL = [
    (231,  76,  60), ( 52, 152, 219), ( 46, 204, 113), (243, 156,  18),
    (155,  89, 182), ( 26, 188, 156), (230, 126,  34), (233,  30,  99),
    (  0, 188, 212), (139, 195,  74), (255,  87,  34), (  3, 169, 244),
    (255, 193,   7), ( 76, 175,  80), (255, 152,   0), (156,  39, 176),
    (121,  85,  72), ( 96, 125, 139), (192,  57,  43), ( 41, 128, 185),
]
_PAL_NP   = np.array(_PAL,              dtype=np.uint8)
_EMPTY    = np.array([30,  30,  46],    dtype=np.uint8)
_BG       = np.array([18,  18,  30],    dtype=np.uint8)
_SRC      = np.array([255, 255, 255],   dtype=np.uint8)   # white  — moving from
_DST      = np.array([255, 215,   0],   dtype=np.uint8)   # gold   — moving to


# ---------------------------------------------------------------------------
# Rendering  (fixed display window — never more than max_cells cells)
# ---------------------------------------------------------------------------

def compute_layout(n_cells: int, W: int, H: int, min_cell: int = 2):
    """
    Find the largest cell size (>= min_cell) that fits n_cells inside W×H.
    If even min_cell is too large, truncate cells to what fits.
    Returns (cell, cpw, display_cells).
    """
    for cell in range(min(W, H), min_cell - 1, -1):
        cpw  = W // cell
        rows = math.ceil(n_cells / cpw)
        if rows * cell <= H:
            return cell, cpw, n_cells
    # Can't fit all cells — use min_cell and truncate
    cell     = min_cell
    cpw      = W // cell
    max_rows = H // cell
    return cell, cpw, cpw * max_rows


def render(disk: np.ndarray, src=(), dst=(), cell: int = 4,
           cpw: int = 100, n_show: int = None) -> np.ndarray:
    """Render the disk to a fixed-size RGB uint8 image."""
    n    = n_show if n_show is not None else len(disk)
    view = disk[:n]
    rows = math.ceil(n / cpw)
    gap  = 1 if cell >= 3 else 0   # no gap for tiny cells

    grid = np.empty((rows * cpw, 3), dtype=np.uint8)
    grid[:]  = _EMPTY

    fids  = view
    valid = fids >= 0
    grid[:n][~valid] = _EMPTY
    if valid.any():
        grid[:n][valid] = _PAL_NP[fids[valid] % len(_PAL_NP)]

    # Highlights
    for i in src:
        if i < n: grid[i] = _SRC
    for i in dst:
        if i < n: grid[i] = _DST

    color_grid = grid.reshape(rows, cpw, 3)
    img        = np.repeat(np.repeat(color_grid, cell, axis=0), cell, axis=1)

    if gap:
        for i in range(rows):
            img[i * cell + cell - 1, :] = _BG
        for j in range(cpw):
            img[:, j * cell + cell - 1] = _BG

    return img


# ---------------------------------------------------------------------------
# Disk helpers
# ---------------------------------------------------------------------------

def expand(data) -> np.ndarray:
    out = []
    for i, n in enumerate(data):
        out.extend([i // 2 if i % 2 == 0 else -1] * n)
    return np.array(out, dtype=np.int32)


class Block:
    __slots__ = ("size", "file_id")
    def __init__(self, s, f=-1):
        self.size    = s
        self.file_id = f


def _blocks_flat(blocks) -> np.ndarray:
    out = []
    for b in blocks:
        out.extend([b.file_id] * b.size)
    return np.array(out, dtype=np.int32)


def _block_start(blocks, idx: int) -> int:
    return sum(blocks[i].size for i in range(idx))


# ---------------------------------------------------------------------------
# GIF streaming writer
# ---------------------------------------------------------------------------

class GifWriter:
    def __init__(self, path: str, fps: int):
        self._w     = iio.get_writer(path, format="GIF", mode="I", fps=fps, loop=0)
        self._path  = path
        self._count = 0

    def write(self, img: np.ndarray):
        self._w.append_data(img)
        self._count += 1

    def close(self):
        self._w.close()
        print(f"Saved  {self._path}  ({self._count} frames)")

    def __enter__(self):  return self
    def __exit__(self, *_): self.close()


# ---------------------------------------------------------------------------
# Part 1
# ---------------------------------------------------------------------------

def animate_part1(data, writer, cell, cpw, n_show, step, hold):
    disk  = expand(data)
    left  = 0
    right = len(disk) - 1
    moves = 0

    writer.write(render(disk, cell=cell, cpw=cpw, n_show=n_show))

    while left < right:
        while left < right and disk[left]  != -1: left  += 1
        while left < right and disk[right] == -1: right -= 1
        if left >= right: break

        if moves % step == 0:
            writer.write(render(disk, src=[right], dst=[left],
                                cell=cell, cpw=cpw, n_show=n_show))

        disk[left], disk[right] = disk[right], disk[left]
        moves += 1

        if moves % step == 0:
            writer.write(render(disk, cell=cell, cpw=cpw, n_show=n_show))

        left  += 1
        right -= 1

    final = render(disk, cell=cell, cpw=cpw, n_show=n_show)
    for _ in range(hold): writer.write(final)

    cs = int(np.sum(np.arange(len(disk), dtype=np.int64) *
                    np.where(disk >= 0, disk.astype(np.int64), 0)))
    print(f"Part 1  checksum={cs}  moves={moves}")


# ---------------------------------------------------------------------------
# Part 2
# ---------------------------------------------------------------------------

def animate_part2(data, writer, cell, cpw, n_show, step, hold):
    blocks = [Block(n, i // 2 if i % 2 == 0 else -1) for i, n in enumerate(data)]
    id_max = max(b.file_id for b in blocks if b.file_id != -1)

    writer.write(render(_blocks_flat(blocks), cell=cell, cpw=cpw, n_show=n_show))

    for fid in range(id_max, -1, -1):
        j = next((i for i, b in enumerate(blocks) if b.file_id == fid), None)
        if j is None: continue

        fsz = blocks[j].size
        if fsz == 0: continue

        target = next((i for i in range(j)
                       if blocks[i].file_id == -1 and blocks[i].size >= fsz), None)
        if target is None: continue

        emit = (id_max - fid) % step == 0

        if emit:
            disk    = _blocks_flat(blocks)
            src_pos = _block_start(blocks, j)
            dst_pos = _block_start(blocks, target)
            writer.write(render(disk,
                                src=range(src_pos, src_pos + fsz),
                                dst=range(dst_pos, dst_pos + fsz),
                                cell=cell, cpw=cpw, n_show=n_show))

        diff = blocks[target].size - fsz
        if diff > 0:
            blocks[target] = Block(fsz, fid)
            blocks.insert(target + 1, Block(diff, -1))
            blocks[j + 1].file_id = -1
        else:
            blocks[target] = Block(fsz, fid)
            blocks[j].file_id = -1

        if emit:
            disk = _blocks_flat(blocks)
            writer.write(render(disk, cell=cell, cpw=cpw, n_show=n_show))

    final = render(_blocks_flat(blocks), cell=cell, cpw=cpw, n_show=n_show)
    for _ in range(hold): writer.write(final)

    flat = _blocks_flat(blocks)
    cs   = int(np.sum(np.arange(len(flat), dtype=np.int64) *
                      np.where(flat >= 0, flat.astype(np.int64), 0)))
    print(f"Part 2  checksum={cs}")


# ---------------------------------------------------------------------------
# Entry point
# ---------------------------------------------------------------------------

if __name__ == "__main__":
    ap = argparse.ArgumentParser()
    ap.add_argument("file")
    ap.add_argument("--part",   choices=["1","2","both"], default="both")
    ap.add_argument("--fps",    type=int, default=8)
    ap.add_argument("--width",  type=int, default=600, help="output image width  (default 600)")
    ap.add_argument("--height", type=int, default=400, help="output image height (default 400)")
    ap.add_argument("--step",   type=int, default=1,
                    help="1 frame per N moves (default 1; use ~200 for real Part 1)")
    ap.add_argument("--hold",   type=int, default=None,
                    help="frames to hold on final state (default fps*2)")
    args = ap.parse_args()

    with open(args.file) as f:
        s = f.readline().strip()
    data = [int(c) for c in s]

    total = sum(data)
    hold  = args.hold if args.hold is not None else args.fps * 2

    cell, cpw, n_show = compute_layout(total, args.width, args.height)
    img_w = cpw  * cell
    img_h = math.ceil(n_show / cpw) * cell

    print(f"Disk: {total} cells  |  showing: {n_show}  |  "
          f"cell: {cell}px  |  {cpw} per row  |  image: {img_w}x{img_h}px  |  step: {args.step}")

    base = args.file.rsplit(".", 1)[0]

    if args.part in ("1", "both"):
        with GifWriter(f"{base}_part1.gif", args.fps) as w:
            animate_part1(data, w, cell, cpw, n_show, args.step, hold)

    if args.part in ("2", "both"):
        with GifWriter(f"{base}_part2.gif", args.fps) as w:
            animate_part2(data, w, cell, cpw, n_show, args.step, hold)
