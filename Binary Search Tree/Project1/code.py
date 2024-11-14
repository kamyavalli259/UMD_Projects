def traverse(node: Node, root):
        value_list = []
        if node is None:
            return []
        result = []
        if node.key == search_key:
            result.append(node.value)
        # Continue searching in the left and right subtrees.
        result.extend(traverse(node.left))
        result.extend(traverse(node.right))
        return result

    # Construct value_list by traversing the tree.
    value_list = traverse(root)
    return json.dumps(value_list, indent=2)