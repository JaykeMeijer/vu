package data_structures.implementation;

import data_structures.Sorted;
import java.util.concurrent.locks.ReentrantLock;

public class FineGrainedList<T extends Comparable<T>> implements Sorted<T> {
    private Node head;

    public FineGrainedList() {
        head = new HeadNode();
        head.next = new TailNode();
    }

	public void add(T t) {
        head.lock();
        Node pred = head;
        try {
            Node curr = pred.next;
            curr.lock();
            try {
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

	public void remove(T t) {
        Node pred = null, curr = null;
        head.lock();
        try {
            pred = head;
            curr = pred.next;
            curr.lock();
            try {
                while(curr.compareTo(t) == -1) {
                    pred.unlock();
                    pred = curr;
                    curr = curr.next;
                    curr.lock();
                }
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
        ReentrantLock rtl = new ReentrantLock();

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
