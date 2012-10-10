/**
*  Assignment Concurrency & Multithreading.
*  
*  Rik van der Kooij, rkj800, 2526314
*  Richard Torenvliet, rtt210, 2526863
*
*  Program: FineGrainedTree.java
*       This program implements the a concurrent data structure with fine
*       grained synchronization. Locks are obtained in a hand-over-hand
*       fashion
*
*/

package data_structures.implementation;

import data_structures.Sorted;
import java.util.concurrent.locks.ReentrantLock;

public class FineGrainedList<T extends Comparable<T>> implements Sorted<T> {
    private Node head;

    /**
    * Constructor. init head and tail node
    */
    public FineGrainedList() {
        head = new HeadNode();
        head.next = new TailNode();
    }

    /**
    * Adds a node to the datastructure with Fine grained synchronization
    *
    * @param    T   t   item to add to the datastructure
    */
	public void add(T t) {
        /* start with acquiring lock on head */
        head.lock();
        /* copy of head */
        Node pred = head;
        Node curr = pred.next;

        try {
            /* acquire lock on current node */
            curr.lock(); 
            try {
                /* traverse list until, elem or index is found or  */
                while(curr.compareTo(t) == -1) {
                    pred.unlock();
                    pred = curr;
                    curr = curr.next;
                    curr.lock();
                }
                Node node = new ListNode(t, curr);
                pred.next = node;
            } finally {
                curr.unlock();
            }
        } finally {
            pred.unlock();
        }
        return;
    }

   /**
    * Find the node with value t and removes it.
    * It traversers the list from the head to the element
    * to be removed. 
    *
    * @param    T   t   value of node to be removed
    */
	public void remove(T t) {
        Node pred = null, curr = null;
        head.lock();
        try {
            pred = head;
            curr = pred.next;
            curr.lock();
            try {
                /* find the place in the list the Node with item t should be */
                while(curr.compareTo(t) == -1) {
                    pred.unlock();
                    pred = curr;
                    curr = curr.next;
                    curr.lock();
                }
                /* Remove the node if it has the item t */
                if(curr.compareTo(t) == 0) {
                    pred.next = curr.next;
                }
            } finally {
                curr.unlock();
            }
        } finally {
            pred.unlock();
        }
        return;
	}

    /**
    * prints the datastructure
    * @return String 
    */
	public String toString() {
        String s = "";
        Node node = head;

        while(node.next.item != null) {
            node = node.next;
            s += node.item + " ";
        }
        return s;
	}

    /**
     * Nodes now have an lock of each own which
     * have to be locked before using the next
     * field
     */
    abstract class Node {
        T item = null;
        Node next = null;
        ReentrantLock rtl = new ReentrantLock();

        /**
         * wrapper function to change the 
         * node.lock.lock() into node.lock()
         */
        protected void lock() {
            rtl.lock();
        }

        protected void unlock() {
            rtl.unlock();
        }

        protected abstract int compareTo(T t);
    }

    /* sentinal head node */
    class HeadNode extends Node {
        public int compareTo(T t) {
            return -1;
        }
    }

    /* sentinal tail node */
    class TailNode extends Node {
        public int compareTo(T t) {
            return 1;
        }
    }

    /* normal list node */
    class ListNode extends Node {
        ListNode(T t, Node n) {
            item = t; 
            next = n;
        }

        protected int compareTo(T t) {
            return item.compareTo(t);
        }
    }


}
