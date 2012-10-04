package data_structures.implementation;

import data_structures.Sorted;

public class FineGrainedTree<T extends Comparable<T>> implements Sorted<T> {

	public void add(T t) {
		Node pred, curr;

		
	}

	public void remove(T t) {
		
	}

	public String toString() {
		
	}

   	abstract class Node {
   		private ReentrantLock lock = new ReentrantLock();
        T item = null;
        Node left = null;
        Node right = null;

        protected abstract int compareTo(T t);
        protected abstract lock();
        protected abstract unlock();
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

        public lock(){
        	lock.lock();
        }

        public lock(){
        	lock.unlock();
        }

        protected int compareTo(T t) {
            return item.compareTo(t);
        }
    }
}
