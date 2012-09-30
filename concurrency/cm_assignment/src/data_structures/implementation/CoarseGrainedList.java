package data_structures.implementation;

import data_structures.Sorted;
import java.util.concurrent.locks.ReentrantLock;

public class CoarseGrainedList<T extends Comparable<T>> implements Sorted<T> {

    private Node<T> head;
    private ReentrantLock lock = new ReentrantLock();

    public CoarseGrainedList() {
        head = new Node<T>(Integer.MIN_VALUE);
        head.next = new Node<T>(Integer.MAX_VALUE);
    }

	public void add(T t) {
        Node pred, curr;
        int key = t.hashCode();
        lock.lock();
        try {
            pred = head;
            curr = head.next;
            while(curr.key < key) {
                pred = curr;
                curr = curr.next;
            }
            Node<T> node = new Node<T>(t, key);
            pred.next = node;
            node.next = curr;
        } finally {
            lock.unlock();
        }
        return;
	}

	public void remove(T t) {
        Node pred, curr;
        int key = t.hashCode();
        lock.lock();
        try {
            pred = head;
            curr = head.next;
            while(curr.key < key) {
                pred = curr;
                curr = curr.next;
            }
            if(curr.key == key)
                pred.next = curr.next;
        } finally {
            lock.unlock();
        }
        return;
	}

	public String toString() {
        return "";
	}

    private static class Node<T> {
        public T item = null;
        public final int key;
        public CoarseGrainedList.Node next;

        Node(int k) {
            key = k;
        }

        Node(T i, int k) {
            item = i; 
            key = k; 
        }
    }
}
