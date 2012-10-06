package data_structures.implementation;

import data_structures.Sorted;
import java.util.concurrent.locks.ReentrantLock;

public class FineGrainedTree<T extends Comparable<T>> implements Sorted<T> {
	private Node root;

    public FineGrainedTree() {
        root = new TailNode();
        root.left = new HeadNode();
    }

	public void add(T t) {
		Node pred = null, curr = null;
		root.lock();

		pred = root;
		curr = root.left; // left of sentinal nodes
		try {
			while(curr != null){
				curr.lock();
				if (curr.compareTo(t) <= 0){
					pred.unlock();
					pred = curr;
					curr = curr.right;
				} else if (curr.compareTo(t) > 0){
					pred.unlock();
					pred = curr;
					curr = curr.left;
				}
			}
			Node node = new TreeNode(t);
			if (pred.compareTo(t) <= 0){
				pred.right = node;
			} else {	
				pred.left = node;
	   		}
		} finally {
			pred.unlock();
		}
	}

    public void remove(T t) {
        Node pred = null, curr = null;
        root.lock();

        pred = root;
        curr = root.left;
        try {
            try {
                /* find the node we want to remove() */
                while(curr != null) {
                    curr.lock();
                    if(curr.compareTo(t) < 0) {
                        // go right
                        pred.unlock();
                        pred = curr;
                        curr = curr.right;
                    } else if(curr.compareTo(t) > 0) {  // maybe just else
                        // go left
                        pred.unlock();
                        pred = curr;
                        curr = curr.left;
                    }
                    // either curr == null
                    // or element to be removed
                    else 
                        break;
                }

                if (curr.compareTo(t) == 0){
                        remove(curr, pred);
                }
                
            } finally {
                if (curr != null) // in case element is not found, curr is null
                    curr.unlock();
            }
        } finally {
            pred.unlock();
        }
        return;
    }

     private void remove(Node node, Node parent) {
        Node par_succ = null;
        if(node.left != null && node.right != null) { /* node has 2 children */
            par_succ = findParSucc(node);
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
            if(parent.left == node){
                parent.left = null;
            }
            else {
                parent.right = null;
            }
        }
        return;
    }

    /* returns parent of the inorder successor 
     * The successor is either the right or lef
     * child of the parent */
    private Node findParSucc(Node node) {
        /* node is still locked by this thread */
        Node pred = node;
        node = node.right;
        node.lock();

        while(node.left != null) {
            pred = node;
            node = node.left;
            node.lock();
            pred.unlock();
        }

        /*leaving with lock on node and lock on pred*/
        return pred;
    }


	public String toString() {
        return root.print("", true, false) + "\n\n";
	}

   	abstract class Node {
   		public ReentrantLock lock = new ReentrantLock();
        T item = null;
        Node left = null;
        Node right = null;

        protected abstract int compareTo(T t);
        protected abstract void lock();
        protected abstract void unlock();

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

        public void lock(){
        	lock.lock();
        }

        public void unlock(){
        	lock.unlock();
        }

        public String lockstatus(){
            return lock.toString();
        }
    }

    /* sentinal tail node */
    class TailNode extends Node {
        protected int compareTo(T t) {
            return 1;
        }

        public void lock(){
        	lock.lock();
        }

        public void unlock(){
        	lock.unlock();
        }

    }

    /* normal list node */
    class TreeNode extends Node {
        TreeNode(T t) {
            item = t;
        }

        public void lock(){
        	lock.lock();
        }

        public void unlock(){
        	lock.unlock();
        }

        protected int compareTo(T t) {
            return item.compareTo(t);
        }
    }
}
