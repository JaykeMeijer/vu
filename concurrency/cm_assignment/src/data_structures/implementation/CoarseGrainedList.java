/*
*  Assignment Concurrency & Multithreading.
*  
*  Rik van der Kooij, rij---,  
*  Richard Torenvliet, rtt210, 2526863
*
*  Program: CoarseGrainedList.java
*       This program implements the a concurrent data structure (list) with coarse
*       grained synchronization. Locks are obtained on the whole datastructure,
*       so no other thread can manipulate the datastructure concurrenty.
*/


package data_structures.implementation;

import data_structures.Sorted;
import java.util.concurrent.locks.ReentrantLock;

public class CoarseGrainedList<T extends Comparable<T>> implements Sorted<T> {
    private Node head;
    private ReentrantLock lock = new ReentrantLock();

    public CoarseGrainedList() {
        head = new HeadNode();
        head.next = new TailNode();
    }

   /**
    *
    * adds an item to the datastructure.
    *
    * @param    T   t   value of item of the new node.
    */
	public void add(T t) {
        Node pred, curr;
        lock.lock();
        try {
            pred = head;
            curr = head.next;
            while(curr.compareTo(t) == -1) {
                pred = curr;
                curr = curr.next;
            }
            Node node = new ListNode(t, curr);
            pred.next = node;
        } finally {
            lock.unlock();
        }
        return;
	}
   
   /**
    * Removes the node with value t and removes it.
    * It traversers the list from the head to the element
    * to be removed. 
    *
    * @param    T   t   value of node to be removed
    */
	public void remove(T t) {
        Node pred, curr;
        lock.lock();
        try {
            pred = head;
            curr = head.next;

            while(curr.compareTo(t) == -1) {
                pred = curr;
                curr = curr.next;
            }
            if(curr.compareTo(t) == 0)
                pred.next = curr.next;
        } finally {
            lock.unlock();
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

    abstract class Node {
        T item = null;
        Node next = null;

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
