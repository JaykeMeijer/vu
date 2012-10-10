/**
*  Assignment Concurrency & Multithreading.
*  
*  Rik van der Kooij, rkj800, 2526314
*  Richard Torenvliet, rtt210, 2526863
*
*  Program: FineGrainedTree.java
*       This program implements the a concurrent data structure with fine
*       grained synchronization. Locks are obtained by in a hand-over-hand
*       fashion
*
*/
package data_structures.implementation;

import data_structures.Sorted;
import java.util.concurrent.locks.ReentrantLock;

public class FineGrainedTree<T extends Comparable<T>> implements Sorted<T> {
	private Node root;

    /**
    * Constructor. init head and tail node
    */
    public FineGrainedTree() {
        root = new TailNode();
        root.left = new HeadNode();
    }

    /**
    *
    * adds an item to the datastructure.
    *
    * @param    T   t   value of item of the new node.
    */
	public void add(T t) {
		Node pred = root,
             curr = null;

		curr = pred.left;
		pred.lock();
		try {
            /* traverse tree until we get the spot to insert our element */
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
            
            /* find the right place to store the node */
			if (pred.compareTo(t) <= 0){
				pred.right = node;
			} else {	
				pred.left = node;
	   		}
		} finally {
			pred.unlock();
		}
	}

   /**
    * Removes the node with value t and removes it.
    * It traversers the tree from the root to the element
    * to be removed. 
    *
    * @param    T   t   value of node to be removed
    */
    public void remove(T t) {
        Node pred = null, curr = null;
        root.lock();
        pred = root;

        try {
            curr = root.left;
            try {
                curr.lock();
                /* find the node we want to remove */
                while(curr != null) {
                    if(curr.compareTo(t) < 0) {
                        pred.unlock();
                        pred = curr;
                        curr = curr.right;
                        curr.lock();
                    } else if(curr.compareTo(t) > 0) {  // maybe just else
                        pred.unlock();
                        pred = curr;
                        curr = curr.left;
                        curr.lock();
                    }
                    else 
                        break;
                }

                /* remove the item if it has the same item as t */
                if (curr.compareTo(t) == 0)
                    remove(curr, pred);
                
            } finally {
                if (curr != null) // in case element is not found, curr is null
                    curr.unlock();
            }
        } finally {
            pred.unlock();
        }
        return;
    }

    /**
    * Removes the node from the datastructure. 3 basic cases are evaluated.
    * The node to be removed has:
    * 1. 2 child nodes, set node do in order successor
    * 2. on child node, set parent left or right to left or right child.
    * 3. no children, just set to null
    *
    * @param    Node    node    node remove
    * @param    Node    parent  parent of node to remove
    */
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
        /* create a copy of curr */
        Node pred = node;
        /* one step right */
        node = node.right;
        /* acquire the lock */
        node.lock();
        try {
            /* step left, until there node == null */
            while(node.left != null) {
                /* copy current node */
                pred = node;
                /* step left */
                node = node.left;
                /* lock this node */
                node.lock();
                pred.unlock();
            }
        } finally {
            node.unlock();
        }
        return pred;
    }

    /**
    * prints the datastructure
    * @return String 
    */
	public String toString() {
        return root.print("", true, false) + "\n\n";
	}

    /**
     * Nodes now have an lock of each own which
     * have to be locked before using the next
     * field
     */
   	abstract class Node {
   		public ReentrantLock lock = new ReentrantLock();
        T item = null;
        Node left = null;
        Node right = null;

        protected abstract int compareTo(T t);
        protected abstract void lock();
        protected abstract void unlock();

        /**
         * Recusivly get a string representing the 
         * tree. 
         *
         * @param   String  prefix  Prefix to print left from the current
         *                          Node.
         * @param   Boolean tail    Last child of parent 
         * @param   Boolean l       Left child of tree
         */
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
