/**
*  Assignment Concurrency & Multithreading.
*  
*  Rik van der Kooij, rkj800, 2526314
*  Richard Torenvliet, rtt210, 2526863
*
*  Program: LockFeeList.java
*       This program implements a concurrent data structure (list) with lock 
*       free synchronization. An element is added by setting a pointer in a 
*       single atomic step. Removing first marks a node to be logically
*       removed and then remove it physically.
*/


package data_structures.implementation;

import data_structures.Sorted;
import java.util.concurrent.atomic.AtomicMarkableReference;

public class LockFreeList<T extends Comparable<T>> implements Sorted<T> {
    private Node head;

    /**
    * Constructor. init head and tail node
    */
    public LockFreeList() {
        head = new HeadNode();
        head.next = new AtomicMarkableReference<Node>(new TailNode(), false);
    }

    /**
     * adds an item to the datastructure.
     *
     * @param    T   t   value of item of the new node.
     */
	public void add(T t) {
        while(true) {
            /* find the right place for the element */
            Window window = find(t);
            Node pred = window.pred;
            Node curr = window.curr;

            Node node = new ListNode(t);
            node.next = new AtomicMarkableReference<Node>(curr, false);

            if(pred.next.compareAndSet(curr, node, false, false))
                /* the compare and set succeded -> the node is added */
                return;
        }
	}

    /**
     * Find the node with value t and removes it
     * It traversers the list from the head to the element
     * to be removed. 
     *
     * @param    T   t   value of node to be removed
     */
	public void remove(T t) {
        boolean snip;
        while(true) {
            Window window = find(t);
            Node pred = window.pred;
            Node curr = window.curr;

            if(curr.compareTo(t) != 0)
                return;
            else {
                Node succ = curr.next.getReference();

                /* remove the node logically */
                snip = curr.next.compareAndSet(succ, succ, false, true);
                if(!snip)
                    continue;
                /* remove the node physically */
                pred.next.compareAndSet(curr, succ, false, false);
            }
            return;
        }
	}

    /**
    * prints the datastructure
    * This method should not be used when other threads are adding
    * and removing elements - the list might have changed in the mean
    * time.
    *
    * @return String 
    */
	public String toString() {
        String s = "";
        Node node = head;

        while(node != null) {
            s += node.item + " → ";
            node = node.next.getReference();
        }
        return s;
	}

    /**
     * Find the the place in the tree an node with item t
     * should be.
     *
     * @param   T   t   the item to be found
     * @return  Window  the two nodes where curr should have the item t
     *                  of where the new node should be placed in between
     */
    public Window find(T t) {
        Node pred = null, curr = null, succ = null;
        boolean [] marked = {false};
        boolean snip;
        
        retry: while(true) {
            pred = head;
            curr = pred.next.getReference();
            while(true) {
                succ = curr.next.get(marked);
                while(marked[0]) {
                    snip = pred.next.compareAndSet(curr, succ, false, false);
                    if(!snip)
                        continue retry;
                    curr = succ;
                    succ = curr.next.get(marked);
                }
                if(curr.compareTo(t) >= 0)
                   return new Window(pred, curr);
                pred = curr;
                curr = succ;
            }
        }
    }

    /**
     * class holding two nodes which is used for finding
     * nodes and places where it should be added. 
     */
    class Window {
        public Node pred, curr;

        Window(Node myPred, Node myCurr) {
            pred = myPred;
            curr = myCurr;
        }
    }

    abstract class Node {
        T item = null;
        AtomicMarkableReference<Node> next = new AtomicMarkableReference<Node>(null, false);

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
        ListNode(T t) {
            item = t;
        }

        protected int compareTo(T t) {
            return item.compareTo(t);
        }
    }
}
