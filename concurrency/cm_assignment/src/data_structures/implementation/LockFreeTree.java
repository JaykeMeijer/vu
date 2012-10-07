package data_structures.implementation;

import data_structures.Sorted;
import java.util.concurrent.atomic.AtomicMarkableReference;

public class LockFreeTree<T extends Comparable<T>> implements Sorted<T> {
    public enum flag {
        CLEAN, DFLAG, IFLAG, MARK
    }

    private Node root;

	public LockFreeTree() {
        root = new TailNode();
        root.left = new AtomicMarkableReference<Leaf> (new HeadLeaf(), false)
        root.right = new AtomicMarkableReference<Leaf> (new TailLeaf(), false);

    }


	public void add(T t) {
        Node p = new TreeNode(t, );
	}

	public void remove(T t) {
		
	}

	public String toString() {
		return "";
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

    class HeadLeaf extends Leaf {
        protected int compareTo(T t){
            return -1;
        }
    }

    class TailLeaf extends Leaf {
        update.state = CLEAN;

        protected int compareTo(T t){
            return -1;
        }
    }


    class HeadNode extends TreeNode {
        protected int compareTo(T t){
            return 1;
        }
    }

    class TailNode extends TreeNode {
        protected int compareTo(T t){
            return -1;
        }
    }

    class Update {
        flag state;
        Info info;
    }

    /* normal list node */
    class TreeNode extends Node {


        TreeNode(T t) {
            key = t;
        }


        TreeNode(T t, Node n) {
            key = t;
            left = null;
            right = null;
         }

        protected int compareTo(T t) {
            return key.compareTo(t);
        }
    }
}
