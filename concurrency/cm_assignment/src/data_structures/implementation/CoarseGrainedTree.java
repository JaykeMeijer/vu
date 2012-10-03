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
                    curr = curr.left;
                }
                else if(curr.compareTo(t) == -1) {  // maybe just else
                    pred = curr;
                    curr = curr.right;
                }
            }
            Node node = new TreeNode(t);
            if(pred.compareTo(t) <= 0) {
                pred.left = node;
            } else {
                pred.right = node;
            }
        } finally {
            lock.unlock();
        }
        return;
	}

	public void remove(T t) {
        lock.lock();
        try {

        } finally {
            lock.unlock();
        }
        return;
	}

	public String toString() {
        return "";
	}

    abstract class Node {
        T item = null;
        Node left = null;
        Node right = null;

        protected abstract int compareTo(T t);
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
