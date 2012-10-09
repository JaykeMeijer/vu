package data_structures.implementation;

import data_structures.Sorted;
import java.util.concurrent.atomic.*;

public class LockFreeTree<T extends Comparable<T>> implements Sorted<T> {
    final static int CLEAN = 0, DFLAG = 1, IFLAG = 2, MARK = 3;

    private Internal root;

	public LockFreeTree() {
        root = new DummyNode();
        root.left = new AtomicReference<Node>(new DummyLeaf());
        root.right = new AtomicReference<Node>(new DummyLeaf());
    }

    public void add(T t) {
        int compare;
        Internal p = null, newInternal = null;
        Leaf l = null, newSibling;
        Update pupdate, newup;
        IInfo op;
        Info pInfo;
        int[] pState = new int[1];

        while(true) {
            SearchObject res = search(t);

            compare = res.l.compareTo(t);

            if (compare == 0)
                return;
            else if (res.pupdate.info.getStamp() != CLEAN)
                help(res.pupdate);
            else {
                newSibling = new Leaf(res.l.key);
                if (compare > 0)
                    newInternal = new Internal(res.l.key, newSibling, newInternal);
                else
                    newInternal = new Internal(t, newInternal, newSibling);
                newInternal.update = new Update();
                pInfo = res.pupdate.info.get(pState);
                op = new IInfo(res.p, res.l, newInternal);
                
                /* set next insert */
                if(res.p.update.info.compareAndSet(pInfo, op, pState[0], IFLAG)) {
                    helpInsert(op);
                    return;
                } else {
                    //help(result);  //FIXME dit stond er maar compile error
                    help(null);
                }
            }
        }
    }

    public void remove(T t) {
        Internal gp, p;
        Leaf l;
        Update pupdate, gpupdate, result;
        DInfo op;

        /*while(true) {
            SearchObject res = search(t);
            if(res.l.key.compareTo(t) != 0)
                return;
            if(res.gpupdate.info.getStamp() != CLEAN)
                help(res.gpupdate);
            else if(res.pupdate.info.getStamp() != CLEAN)
                help(res.pupdate);
            else {
                op = new DInfo(res.gp, res.p, res.l, res.pupdate);
                
                // FIXME compare and swap to compare and set + get

                //result := CAS(gp → update, gpupdate, ⟨DFlag, op⟩)
                if(res.gp.update.info.compareAndSet(res.gpupdate.info.getReference(), (Info) op, res.gpupdate.info.getStamp(), DFLAG))
                    if(helpDelete(op))
                        return;
                else
                    //help(result); // FIXME result to null voor compile error
                    help(null);
            }
        }*/
    }

    public void help(Update u) {
    }

    public void helpInsert(IInfo op) {
        if(op == null) {
            System.out.println("helpInsert op == null");
            return;
        }

        //CAS-Child(op → p,op → l,op → newInternal)
        //find correct child
        op.p.get().left.compareAndSet(op.l, op.newInternal);
        op.p.get().right.compareAndSet(op.l, op.newInternal);

        //CAS(op → p → update, ⟨IFlag, op⟩, ⟨Clean, op⟩)
        op.p.get().update.info.compareAndSet(op, op, IFLAG, CLEAN);
         
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

        return true; // FIXME just return true for now
    }

	/**
    * Returns the tree in a string.
    * @return String
    */
	public String toString() {
        return "";
        //return root.print("", true, false);
	}

    private SearchObject search(T t){
        Internal gp = null, p = root;
        Node l = root.left.get();;
        Update gpupdate = null, pupdate = root.update;
        int i = 0;

        while (l.getClass() == Internal.class || l.getClass() == DummyNode.class){
            gp = p;
            p = (Internal) l;
            pupdate = p.update;
            if (l.compareTo(t) > 0)
                l = gp.left.get();
            else
                l = gp.right.get();
        }
        System.out.println("BYE search");
        return new SearchObject(gp, p, (Leaf) l, pupdate, gpupdate);
    }

    class SearchObject {
        Internal gp, p;
        Leaf l;
        Update pupdate, gpupdate;

        SearchObject(Internal gp, Internal p, Leaf l, Update pupdate, Update gpupdate){
            this.gp = gp;
            this.p = p;
            this.l = l;
            this.pupdate = pupdate;
            this.gpupdate = gpupdate;
        }
    }

    abstract class Info {
        AtomicReference<Internal> p = null;
        Leaf l = null;
    }

    //op = new DInfo(res.gp, res.p, res.l, res.pupdate);
    class DInfo extends Info {
        Internal gp = null;
        Update pupdate;

        DInfo(Internal gp, Internal p, Leaf l, Update u) {
            this.gp = gp;
            this.p = new AtomicReference<Internal>(p);
            this.l = l;
            this.pupdate = u;
        }
    }

    class IInfo extends Info {
        Internal newInternal;

        IInfo(Internal p, Leaf l, Internal newInter) {
            this.p = new AtomicReference<Internal>(p);
            this.l = l;
            this.newInternal = newInter;
        }
    }

    class Update {
        AtomicStampedReference<Info> info ;

        Update() {
            info = new AtomicStampedReference<Info>(null, 0);
        }

        Update(Info i, int s) {
            info = new AtomicStampedReference<Info>(i, s);
        }
    }

    abstract class Node {
        T key = null;

        protected abstract int compareTo(T t);
    }

    /* normal list node */
    class Internal extends Node {
        Update update = new Update();
        AtomicReference<Node> left = null,
                              right = null;

        Internal() {}
        Internal(T t) {
            key = t;
        }
        
        Internal(T t, Node l, Node r) {
            key = t;
            left = new AtomicReference<Node>(l);
            right = new AtomicReference<Node>(r);
        }

        protected int compareTo(T t) {
            return key.compareTo(t);
        }

  //   protected String print(String prefix, Boolean tail, Boolean l) {
  //        String s;
  //        Internal le, r;
  //        if(l)
  //            s = prefix + (tail ? "└── l " : "├── l ");
  //        else
  //            s = prefix + (tail ? "└── r " : "├── r ");
  //        if(key != null)
  //            s += key.toString() + '\n';
  //        else
  //            s += "null\n";
  //        if(left != null && left.get().getClass() == Internal.class) {
  //            left = (Internal) left;
  //            right = (Internal) right;
  //            if(right == null) {  // only a left child
  //                s += left.get().print(prefix + (tail ? "    " : "│   "), true, true);
  //            }
  //            else {  // left and right child
  //                s += left.get().print(prefix + (tail ? "    " : "│   "), false, true);
  //                s += right.get().print(prefix + (tail ? "    " : "│   "), true, false);
  //            }
  //        } else if(right!= null && right.getClass() ==  Internal.class) {  // only a right child
  //            s += right.get().print(prefix + (tail ? "    " : "│   "), false, false);
  //        }
  //        System.out.println(s);
  //        return s;
  //    }
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
            //key = new AtomicReference<T>(t);
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
