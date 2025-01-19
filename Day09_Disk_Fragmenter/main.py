import sys
import time

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

def printBlocks(blocks):
    for b in blocks:
        for i in range(b.block_size):
            if b.file_id == -1:
                print(".", end="")
            else:
                print(b.file_id, end="")
    print()    

def moveBlocks(dm, j):
    tmp = []
    if dm[j].file_id == -1:
        # print(f"Selected Block {dm[j]} is empty")
        return dm

    # print("Selected Block = ", dm[j])
    
    for i in range(len(dm)):
        if j < i:
            tmp.extend(dm[i:])
            break
        
        if dm[i].file_id != -1:
            tmp.append(dm[i])
            continue

        if dm[i].block_size < dm[j].block_size:
            tmp.append(dm[i])
            continue
        
        # if dm[j].file_id != -1 and dm[i].file_id == -1 and dm[i].block_size >= dm[j].block_size:
        # all these statements are already True so they are redundant
        if True:
            diff = dm[i].block_size - dm[j].block_size
            if diff > 0:
                tmp.append(Block(dm[j].block_size, dm[j].file_id))
                tmp.append(Block(diff,-1))
                dm[j].file_id = -1
            else:
                tmp.append(Block(dm[j].block_size, dm[j].file_id))
                dm[j].file_id = -1
                
            tmp.extend(dm[i+1:])
            break
     
    return tmp


def part2(data):
    t = Timer()
    dm = []
    for i in range(len(data)):
        if i % 2 == 0:
            dm.append(Block(data[i], int(i/2)))
        else:
            dm.append(Block(data[i], -1))
    
    # printBlocks(dm)

    for j in reversed(range(len(dm))):
        dm = moveBlocks(dm, j)
        # printBlocks(dm)

    result = 0
    pos = 0
    for b in dm:
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


