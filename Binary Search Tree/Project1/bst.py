# BST Variation 2

from __future__ import annotations
import json

# The class for a particular node in the tree.
# DO NOT MODIFY!
class Node():
    def  __init__(self,
                  key        : int  = None,
                  value      : int  = None,
                  leftchild  : Node = None,
                  rightchild : Node = None):
        self.key        = key
        self.value      = value
        self.leftchild  = leftchild
        self.rightchild = rightchild

# For the tree rooted at root:
# Return the json.dumps of the list with indent=2.
# DO NOT MODIFY!
def dump(root: Node) -> str:
    def _to_dict(node) -> dict:
        return {
            "key"        : node.key,
            "value"      : node.value,
            "leftchild"  : (_to_dict(node.leftchild) if node.leftchild is not None else None),
            "rightchild" : (_to_dict(node.rightchild) if node.rightchild is not None else None)
        }
    if root == None:
        dict_repr = {}
    else:
        dict_repr = _to_dict(root)
    return json.dumps(dict_repr,indent = 2)

# For the tree rooted at root and the key and value given:
# Insert the key/value pair.
# The key is guaranteed to not be in the tree.
# Follow the variation rules as per the PDF.
def insert(root: Node, key: int, value: int) -> Node:
    if root == None:
        node = Node(key, value)
        return node
    elif root.leftchild == None and root.rightchild==None:
        node = Node(key, value)
        if root.key < key:
            node.leftchild = root
        else:
            node.rightchild = root
        return node
    
    elif(root.leftchild == None or predecessor(root).key<key) and (root.rightchild==None or successor(root).key > key):
        node = Node(key, value)
        if root.key<key:
            node.rightchild = root.rightchild
            node.leftchild = insert(root.leftchild, root.key, root.value)
        else:
            node.leftchild = root.leftchild
            node.rightchild = insert(root.rightchild, root.key, root.value)
        return node
    else:
        if key<root.key:
            root.leftchild = insert(root.leftchild, key, value)
        else:
            root.rightchild = insert(root.rightchild, key, value)
        return root

def predecessor(root):
    node = root.leftchild
    while node.rightchild != None:
        node = node.rightchild
    return node

def successor(root):
    node = root.rightchild
    while node.leftchild!= None:
        node = node.leftchild
    return node

def remove_predecessor(root):
    parent = root
    node= root.leftchild 
    if(node == None):
        return None
    while node.rightchild != None:
        parent = node
        node = node.rightchild
    if(parent != root):
        parent.rightchild = node.leftchild
    else:
        parent.leftchild = node.leftchild
    return node

def remove_successor(root):
    parent = root  
    node = root.rightchild
    if(node ==None):
        return None
    while node.leftchild!= None:
        parent = node
        node = node.leftchild
    if(parent!= root):
        parent.leftchild = node.rightchild
    else:
        parent.rightchild = node.rightchild
    return node

def height(root):
    if root == None:
        return 0
    else:
        #return 1 + max(height(root.leftchild),height(root.rightchild)) 
        return 1 + height(root.rightchild)+ height(root.leftchild)
# For the tree rooted at root and the key given, delete the key.
# Follow the variation rules as per the PDF.
def delete(root: Node, key: int) -> Node:
    if(root.key == key):
        if(root.rightchild == None):
            return root.leftchild
        elif(root.leftchild == None):
            return root.rightchild
        
        elif height(root.leftchild) > height(root.rightchild):
            pred = remove_predecessor(root)
            if(pred == None):
                return root.rightchild
            else:
                pred.leftchild = root.leftchild
                pred.rightchild = root.rightchild
                return pred    
        
        else:
            successor= remove_successor(root)
            if(successor == None):
                return root.leftchild
            else:
                successor.leftchild = root.leftchild
                successor.rightchild = root.rightchild
                return successor
    elif(root.key < key):
        root.rightchild= delete(root.rightchild, key)
    else:
        root.leftchild= delete(root.leftchild, key)
    return root

# For the tree rooted at root and the key given:
# Calculate the list of values on the path from the root down to and including the search key node.
# The key is guaranteed to be in the tree.
# Return the json.dumps of the list with indent=2.

def find_key(root: Node, key: int):
    if root is None:
        return []
    elif root.key == key:
        return [root.value]
    elif key < root.key:
        path = find_key(root.leftchild, key)
    elif key > root.key:
         path = find_key(root.rightchild, key)
    # Prepend the current node's value to the path found in the subtree
    return [root.value] + path



def search(root: Node, search_key: int) -> str:
    # Remove the next line and fill in code to construct value_list.
   
    value_list = [] 
    value_list = find_key(root, search_key)
    # Return the JSON representation of value_list.
    return json.dumps(value_list, indent=2)