package data_structures.implementation;

import data_structures.Sorted;

public class LockFreeList<T extends Comparable<T>> implements Sorted<T> {
    private Node head;

    public LockFreeList() {
        head = new HeadNode();
        head.next = new TailNode();
    }

	public void add(T t) {
        return;
	}

	public void remove(T t) {
        String s = "";
        Node node = head;

        while(node.next.item != null) {
            node = node.next;
            s += node.item + " → ";
        }
        return;
	}

	public String toString() {
        return "→";
	}

    /*public Window find(T t) {
        Node pred = null, curr = null, succ = null;
        boolean [] marked = {false};
        boolean snip;
        
        while(true) {
            succ = curr.next.get(marked);
            while(true) {
                pred = head;
                curr = pred.next.getReference();
                while(marked[0]) {
                    snip = pred.next.compareAndSet(curr, succ, false, false);
                    if(!snip)
                        continue;
                    curr = succ;
                    succ = curr.next.get(marked);
                }
                if(curr.compareTo(t) > 0)
                   return new Window(pred, curr);
                pred = curr;
                curr = succ;
            }
        }
    }

    class Window {
        public Node pred, curr;

        Window(Node myPred, Node myCurr) {
            pred = myPred;
            curr = myCurr;
        }
    }*/

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
