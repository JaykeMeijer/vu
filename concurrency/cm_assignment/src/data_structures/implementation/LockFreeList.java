package data_structures.implementation;

import data_structures.Sorted;
import java.util.concurrent.atomic.AtomicMarkableReference;

public class LockFreeList<T extends Comparable<T>> implements Sorted<T> {
    private Node head;

    public LockFreeList() {
        head = new HeadNode();
        head.next = new AtomicMarkableReference<Node>(new TailNode(), false);
    }

	public void add(T t) {
        while(true) {
            Window window = find(t);
            Node pred = window.pred;
            Node curr = window.curr;

            Node node = new ListNode(t);
            node.next = new AtomicMarkableReference<Node>(curr, false);
            if(pred.next.compareAndSet(curr, node, false, false))
                return;
        }
	}

	public void remove(T t) {
        boolean snip;
        while(true) {
            Window window = find(t);
            Node pred = window.pred;
            Node curr = window.curr;

            if(curr.compareTo(t) != 0) {
                System.out.println("remove: element " + t + " not found");
            } else {
                Node succ = curr.next.getReference();
                snip = curr.next.compareAndSet(succ, succ, false, true);
                if(!snip)
                    continue;
                pred.next.compareAndSet(curr, succ, false, false);
            }
            return;
        }
	}

	public String toString() {
        String s = "";
        Node node = head;

        while(node != null) {
            s += node.item + " → ";
            node = node.next.getReference();
        }
        return s;
	}

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
