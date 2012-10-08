package data_structures.implementation;

import data_structures.Sorted;
import java.util.concurrent.atomic.AtomicStampedReference;
import java.util.concurrent.atomic.AtomicReference;

public class LockFreeTree<T extends Comparable<T>> implements Sorted<T> {
    final static int CLEAN = 0, DFLAG = 1, IFLAG = 2, MARK = 3;

    private Internal root;

	public LockFreeTree() {
        root = new DummyNode();
        root.left = new DummyLeaf();
        root.right = new DummyLeaf();
    }

	public void add(T t) {
        int compare;
        Internal p, newInternal;
        Leaf l, newSibling;
        Update pupdate, newup;
        IInfo op;
        Info pInfo;
        int[] pState = new int[1];

        while(true){
            SearchObject res = search(t);
            compare = res.l.compareTo(t);
            if (compare == 0)
                return;
            else if (res.pupdate.state != CLEAN)
                help(res.pupdate);
            else {
                newSibling = new Leaf(res.l.key);
                if (compare > 0)
                    newInternal = new Internal(res.l.key, newSibling, newInternal);
                else
                    newInternal = new Internal(t, newInternal, newSibling);

                newInternal.update = new Update();
                pInfo = res.pupdate.info.get(pState);
                op = new IInfo(p, l, newInternal);
                
                /* set next insert */
                if (res.p.update.info.compareAndSet(pInfo, op, pState[0], IFLAG)){
                    helpInsert(op);
                    return;
                } else {
                    help(result);
                }
            }
        }
	}

	public void remove(T t) {
		
	}

    public void help(Update u) {
        
    }

	public String toString() {
		return root + " " + root.left + " " + root.right; 
	}

    private SearchObject search(T t){
        Internal gp = null, p = null;
        Node l = root;
        Update gpupdate = null, pupdate = null;

        while (l.getClass() == Internal.class){
            gp = p;
            p = (Internal) l;
            pupdate = p.update;
            if (l.compareTo(t) == -1)
                l = gp.left;
            else
                l = gp.right;
        }

        return new SearchObject(gp, p, l, pupdate, gpupdate);
    }

    class SearchObject {
        Internal gp, p;
        Node l;
        Update pupdate, gpupdate;

        SearchObject(Internal gp, Internal p, Node l, Update pupdate, Update gpupdate){
            this.gp = gp;
            this.p = p;
            this.l = l;
            this.pupdate = pupdate;
            this.gpupdate = gpupdate;
        }
    }

    abstract class Info {
        Internal p = null;
        Leaf l = null;
    }


    class DInfo extends Info {
        Internal gp = null;

        DInfo(Internal gp, Internal p, Leaf l) {
            this.gp = gp;
            this.p = p;
            this.l = l;
        }
    }

    class IInfo extends Info {
        Internal newInternal;

        IInfo(Internal p, Leaf l, Internal newInter) {
            this.p = p;
            this.l = l;
            this.newInternal = newInter;
        }
    }

    class Update {
        AtomicStampedReference<Info> info ;
    }

    abstract class Node {
        T key = null;

        protected abstract int compareTo(T t);
    }

    /* normal list node */
    class Internal extends Node {
        AtomicMarkableReference<Update>s update = null;
        Node left = null,
             right = null;

        Internal() {}
        Internal(T t) {
            key = t;
        }
        
        Internal(T t, Node l, Node r) {
            key = t;
            left = l;
            right = r;
        }

        protected int compareTo(T t) {
            return key.compareTo(t);
        }
    }

    class DummyNode extends Internal {
        protected int compareTo(T t){
            return 1;
        }
    }

    class Leaf extends Node {
        ReferenT key = null;

        Leaf() {}

        Leaf(T t){
            key = t;
        }

        protected int compareTo(T t) {
            return key.compareTo(t);
        }
    }

    class DummyLeaf extends Leaf {
        protected int compareTo(T t){
            return 1;
        }
    }
}
