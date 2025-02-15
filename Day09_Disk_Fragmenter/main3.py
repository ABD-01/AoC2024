# same as https://github.com/ABD-01/AoC2024/blob/master/Day09_Disk_Fragmenter/main.py
# Uses List of Lists for diskmap

import sys
import time
from typing import List

class Timer:
    def __init__(self):
        self.start = time.time()
    def __del__(self):
        end = time.time()
        print(f"Elapsed time: {(end - self.start)*1e6:.0f} us")

class Block:
    def __init__(self,x,y=-1):
        self.block_size = x
        self.file_id = y
    def __repr__(self):
        return f"({self.block_size},{self.file_id})"

def printBlocks(blocks: List[List[Block]]):
    for _ in blocks:
        for b in _:
            for i in range(b.block_size):
                if b.file_id == -1:
                    print(".", end="")
                else:
                    print(b.file_id, end="")
    print()    

def moveBlocks(dm: List[List[Block]], j):
    if dm[j][0].file_id == -1:
        # print(f"Selected Block {dm[j]} is empty")
        return

    # print("Selected Block = ", dm[j])
    
    for i in range(len(dm)):
        if j < i:
            break

        for k in range(len(dm[i])):
        
            if dm[i][k].file_id != -1:
                continue

            if dm[i][k].block_size < dm[j][0].block_size:
                continue
        
            # if dm[j].file_id != -1 and dm[i].file_id == -1 and dm[i].block_size >= dm[j].block_size:
            # all these statements are already True so they are redundant
            if True:
                diff = dm[i][k].block_size - dm[j][0].block_size
                if diff > 0:
                    dm[i][k].block_size = dm[j][0].block_size 
                    dm[i][k].file_id = dm[j][0].file_id 
                    dm[i].append(Block(diff, -1))
                    dm[j][0].file_id = -1
                else:
                    dm[i][k], dm[j][0] = dm[j][0], dm[i][k]
                    
                return
     
    return


def part2(data):
    t = Timer()
    dm: List[List[Block]] = []
    for i in range(len(data)):
        if i % 2 == 0:
            dm.append([Block(data[i], int(i/2))])
        else:
            dm.append([Block(data[i], -1)])
    
    # printBlocks(dm)

    for j in reversed(range(len(dm))):
        moveBlocks(dm, j)
        # printBlocks(dm)

    result = 0
    pos = 0
    for _ in dm:
        for b in _:
            for i in range(b.block_size):
                if b.file_id != -1:
                    result += (b.file_id*pos)
                pos += 1   
    print(f"Part 2: {result}")
    return
    


if __name__ == "__main__":

    if len(sys.argv) != 2:
        print("Please provide the target number as an argument.")
        sys.exit(1)

    with open(sys.argv[1], "r") as f:
        s = f.readlines()[0].strip()

    data = [int(i) for i in s]
    part2(data)


