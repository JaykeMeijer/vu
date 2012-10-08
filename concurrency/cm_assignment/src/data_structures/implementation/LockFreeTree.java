package data_structures.implementation;

import data_structures.Sorted;
import java.util.concurrent.atomic.AtomicMarkableReference;

public class LockFreeTree<T extends Comparable<T>> implements Sorted<T> {
    public enum Flag {
        CLEAN, DFLAG, IFLAG, MARK
    }

    private Node root;

	public LockFreeTree() {
        root = new DummyNode();
        root.left = new AtomicMarkableReference<Leaf> (new DummyLeaf(), false)
        root.right = new AtomicMarkableReference<Leaf> (new DummyLeaf(), false);
    }


	public void add(T t) {
        Boolean compare;
        InternalNode p, newInternal;
        Leaf l, newSibling;
        Leaf newChild = new Leaf(t);
        Update pupdate, result, newup = new Update(CLEAN, null);
        IInfo op;

        while(true){
            SearchObject res = search(k);
            compare = res.l.key.compareTo(k);
            if (compare == 0)
                return;
            else if (res.pupdate.state != CLEAN)
                help(res.pupdate);
            else {
                newSibling = new Leaf(res.l.key);
                if (compare > 0) {
                    newInternal = new InternalNode(res.l.key)
                    newInteral.left = newSibling;
                    newInteral.right = newInternal;
                }
                else {
                    newInternal = new InternalNode(k);
                    newInteral.right = newSibling;
                    newInteral.left = newInternal;
                }

                newInternal.update = newup;
                op = new IInfo(p, l, newInternal);
                res.p.compareAndSet();


            }
        }
	}

	public void remove(T t) {
		
	}

    public help(){


    }

	public String toString() {
		return "";
	}

    private SearchObject search(T t){
        InternalNode gp, p;
        Update gpupdate, pupdate;
        InternalNode l = root;

        while (curr instanceof InternalNode){
            gp = p;
            p = l;
            pupdate = p.update;
            if (l.compareTo(t) == -1)
                l = p.left;
            else
                l = p.right;
        }

        return SearchObject(gp, p, l, pupdate, gpupdate); 
    }

    class SearchObject {
        InternalNode gp, p;
        Node l;
        Update pupdate, gpupdate;

        SearchObject(InternalNode gp, InternalNode p, Node l, Update pupdate, Update gpupdate){
            this.gp = gp;
            this.p = p;
            this.l = l;
            this.pupdate = pupdate;
            this.gpupdate = gpupdate;
        }
    }

    abstract class Node {
        T key = null;
        AtomicMarkableReference<Update> update = null;
        AtomicMarkableReference<Node> left = null;
        AtomicMarkableReference<Node> right = null;
        
        protected abstract int compareTo(T t);
    }

    abstract class Info {
        Node p = null;
        Node l = null;
    }


    class DInfo extends Info {
        Node gp = null;

        DInfo(Node gp, Node p, Node l) {
            this.gp = gp;
            this.p = p;
            this l = l;
        }
    }

    class IInfo {
        IInfo(Node p, Node l) {
            this.p = p;
            this l = l;
        }
    }

    class Leaf extends Node {
        T key = null;

        Leaf(T t){
            key = t;
        }
    }

    class DummyLeaf extends Leaf {
        protected int compareTo(T t){
            return 1;
        }
    }

    class DummyNode extends Internal {
        protected int compareTo(T t){
            return 1;
        }
    }

    class Update {
        Flag state;
        Info info;
    }

    /* normal list node */
    class InternalNode extends Node {
        InternalNode(T t) {
            key = t;
        }

        InternalNode(T t, Node n) {
            key = t;
            left = null;
            right = null;
         }

        protected int compareTo(T t) {
            return key.compareTo(t);
        }
    }
}
