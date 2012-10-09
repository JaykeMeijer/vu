package data_structures.implementation;

import data_structures.Sorted;
import java.util.concurrent.atomic.AtomicMarkableReference;

public class LockFreeTree<T extends Comparable<T>> implements Sorted<T> {
    final static int CLEAN = 0, DFLAG = 1, IFLAG = 2, MARK = 3;

    private Internal root;

	public LockFreeTree() {
        root = new DummyNode();
        root.left = new DummyLeaf();
        root.right = new DummyLeaf();
    }

	public void add(T t) {
        /*Boolean compare;
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
                    newInternal = new InternalNode(res.l.key);
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
        }*/
	}

	public void remove(T t) {
        Internal gp, p;
        Leaf l;
        Update pupdate, gpupdate, result;
        DInfo op;
        
        /*while True {
            ⟨gp, p, l, pupdate, gpupdate⟩ := Search(k)
            if l → key ̸= k then
                return False
            if gpupdate.state ̸= Clean then
                Help(gpupdate)
            else if pupdate.state ̸= Clean then
                Help(pupdate)
            else {
                op := pointer to a new DInfo record containing ⟨gp, p, l, pupdate⟩
                result := CAS(gp → update, gpupdate, ⟨DFlag, op⟩)
                if result = gpupdate then {
                    if HelpDelete(op) then
                        return True
                }
                else 
                    Help(result)   ◃ The dflag CAS failed; help the operation that caused the failure
            }
        }*/

        while(true) {
            SearchObject res = search(t);
            if(res.l.key.compareTo(t) != 0)
                return;
            if(res.gpupdate.state != CLEAN)
                help(res.gpupdate);
            else if(res.pupdate.state != CLEAN)
                help(res.pupdate);
            else {
                op = new DInfo(res.gp, res.p, (Leaf) res.l, res.pupdate);
                
                /* FIXME compare and swap to compare and set + get */
                //result := CAS(gp → update, gpupdate, ⟨DFlag, op⟩)
                if(res.gp.update.compareAndSet(res.gpupdate.info, op, res.gpupdate.state, DFLAG))
                //if(result == res.gpupdate) {
                    if(helpDelete(op))
                        return;
                //}
                else
                    help(result);
            }
            break;
        }
        return;
	}

    public void help(Update u) {
        
    }

    public boolean helpDelete(DInfo op) {
        /*◃ Precondition: op points to a DInfo record (i.e., it is not ⊥)
        Update result
        result := CAS(op → p → update, op → pupdate, ⟨Mark, op⟩)
        if result = op → pupdate or result = ⟨Mark, op⟩ then {
            HelpMarked(op)
            return true;
        } else {
            Help(result)
            CAS(op → gp → update, ⟨DFlag, op⟩, ⟨Clean, op⟩)
            return false;
        }*/

        if(op == null)
            return false;

        Update result;
        
        //if(result 

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

    //op = new DInfo(res.gp, res.p, res.l, res.pupdate);
    class DInfo extends Info {
        Internal gp = null;
        Update pupdate;

        DInfo(Internal gp, Internal p, Leaf l, Update u) {
            this.gp = gp;
            this.p = p;
            this.l = l;
            this.pupdate = u;
        }
    }

    class IInfo extends Info {
        Internal newInternal;

        IInfo(Internal p, Leaf l) {
            this.p = p;
            this.l = l;
        }
    }

    class Update {
        int state = CLEAN;
        Info info = null;
    }

    abstract class Node {
        T key = null;

        protected abstract int compareTo(T t);
    }

    /* normal list node */
    class Internal extends Node {
        Update update = null;
        Node left = null,
             right = null;

        Internal() {}
        Internal(T t) {
            key = t;
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
        T key = null;

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
