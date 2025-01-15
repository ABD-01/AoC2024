import sys

class Node:
    def __init__(self, value):
        self.value = value
        self.left = None  # Left child (Multiplication)
        self.right = None  # Right child (Addition)

def build_tree(lst, index=0, current_value=1, target=None):
    # Base case: if all elements are used or current value exceeds the target
    if index == len(lst):
        return Node(current_value)
    
    if target is not None and current_value > target:
        return None
    
    # Create the current node with the current value
    root = Node(current_value)
    
    # Left child: multiply by the next element
    root.left = build_tree(lst, index + 1, current_value * lst[index], target)
    
    # Right child: add the next element
    root.right = build_tree(lst, index + 1, current_value + lst[index], target)
    
    return root

def check_target(root, target):
    # Base case: if the node is None
    if not root:
        return False
    
    # If we've reached a leaf node, check if the value equals the target
    if not root.left and not root.right:
        return root.value == target
    
    # Recursively check left and right children
    return check_target(root.left, target) or check_target(root.right, target)


if __name__ == "__main__":

    if len(sys.argv) != 2:
        print("Please provide the target number as an argument.")
        sys.exit(1)

    with open(sys.argv[1], "r") as f:
        s = f.readlines()
        inp = [(int(t), list(map(int, nums.split()))) for t, nums in
            (map(lambda x: x.split(":"), s))]

    result = 0
    for target, lst in inp:
        # Build the tree with pruning
        root = build_tree(lst, target=target)

        # Check if any leaf node results in the target value
        if check_target(root, target):
            result += target
    print(f"Part 1: {result}")


# Refs:
# https://chatgpt.com/share/677e6c88-a504-8001-8b77-3ab4d3f7ad4e
