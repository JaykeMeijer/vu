package data_structures.implementation;

import data_structures.Sorted;
import java.util.concurrent.locks.ReentrantLock;

public class CoarseGrainedTree<T extends Comparable<T>> implements Sorted<T> {
    private Node root;
    private ReentrantLock lock = new ReentrantLock();

    public CoarseGrainedTree() {
        root = new TailNode();
        root.left = new HeadNode();
    }

	public void add(T t) {
        Node pred, curr;
        lock.lock();
        try {
            pred = root;
            curr = root.left;  // Sentinal nodes so we can do this.

            while(curr != null) {
                if(curr.compareTo(t) <= 0) {
                    pred = curr;
                    curr = curr.right;
                }
                else if(curr.compareTo(t) > 0) {  // maybe just else
                    pred = curr;
                    curr = curr.left;
                }
            }
            Node node = new TreeNode(t);
            if(pred.compareTo(t) <= 0) {
                pred.right = node;
            } else {
                pred.left = node;
            }
        } finally {
            lock.unlock();
        }
        return;
	}

	public void remove(T t) {
        Node pred, curr;
        lock.lock();
        try {
            pred = root;
            curr = root.left;

            /* find the node we want to remove */
            while(curr != null) {
                if(curr.compareTo(t) < 0) {
                    pred = curr;
                    curr = curr.right;
                }
                else if(curr.compareTo(t) > 0) {  // maybe just else
                    pred = curr;
                    curr = curr.left;
                }
                else
                    break;
            }
            /* Remove if it has the same item */
            if(curr.compareTo(t) == 0)
                remove(curr, pred);
            //System.out.println(toString());
        } finally {
            lock.unlock();
        }
        return;
	}

    private void remove(Node node, Node parent) {
        if(node.left != null && node.right != null) { /* node has 2 children */
            Node par_succ = findParSucc(node);
            Node succ;

            if(par_succ == node) {
                /* successor is right child of the node */
                succ = par_succ.right;
                node.item = succ.item;
                node.right = succ.right;
            } else {
                /* successor is left somewhere deeper in the tree */
                succ = par_succ.left;
                node.item = succ.item;
                par_succ.left = succ.right;
            }

        } else if(node.left != null || node.right != null) { /* Node has 1 child */
            /* Swap the node with its child */
            if(node.left != null)
                if(parent.left == node) {
                    parent.left = node.left;
                } else
                    parent.right = node.left;
            else 
                if(parent.left == node)
                    parent.left = node.right;
                else
                    parent.right = node.right;
        } else if(node.left == null && node.right == null) { /* node is a leave */
            /* just set the parent child reference to null*/
            if(parent.left == node)
                parent.left = null;
            else
                parent.right = null;
        }
        return;
    }

    /* returns parent of the inorder successor 
     * The successor is either the right or lef
     * child of the parent */
    private Node findParSucc(Node node) {
        Node pred = node;
        node = node.right;

        while(node.left != null) {
            pred = node;
            node = node.left;
        }
        return pred;
    }

	public String toString() {
        return root.print("", true, false);
	}

    abstract class Node {
        T item = null;
        Node left = null;
        Node right = null;

        protected abstract int compareTo(T t);

        /* Super awesome tree printing from stackoverflow:
         *   http://stackoverflow.com/questions/4965335/how-to-print-binary-tree-diagram 
         * which in term was from linux tree command */
        protected String print(String prefix, Boolean tail, Boolean l) {
            String s;
            if(l)
                s = prefix + (tail ? "└── l " : "├── l ");
            else
                s = prefix + (tail ? "└── r " : "├── r ");
            if(item != null)
                s += item.toString() + '\n';
            else
                s += "null\n";

            if(left != null) {  
                if(right == null) {  // only a left child
                    s += left.print(prefix + (tail ? "    " : "│   "), true, true);
                }
                else {    // left and right child
                    s += left.print(prefix + (tail ? "    " : "│   "), false, true);
                    s += right.print(prefix + (tail ? "    " : "│   "), true, false);
                }
            } else if(right != null) {  // only a right child
                s += right.print(prefix + (tail ? "    " : "│   "), false, false);
            }
            return s;
        }
    }

    /* sentinal head node */
    class HeadNode extends Node {
        protected int compareTo(T t) {
            return -1;
        }
    }

    /* sentinal tail node */
    class TailNode extends Node {
        protected int compareTo(T t) {
            return 1;
        }
    }

    /* normal list node */
    class TreeNode extends Node {
        TreeNode(T t) {
            item = t;
        }

        protected int compareTo(T t) {
            return item.compareTo(t);
        }
    }

}
