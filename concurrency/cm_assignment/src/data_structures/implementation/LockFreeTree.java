package data_structures.implementation;

import data_structures.Sorted;
import java.util.concurrent.atomic.*;

public class LockFreeTree<T extends Comparable<T>> implements Sorted<T> {
    final static int CLEAN = 0, DFLAG = 1, IFLAG = 2, MARK = 3;

    private Internal root;

    public LockFreeTree() {
        root = new Internal();
        root.left = new AtomicReference<Node>(new Leaf());
        root.right = new AtomicReference<Node>(new Leaf());
    }

    public void add(T t) {
        int compare;
        Internal p = null, newInternal = null;
        Leaf newSibling, newLeaf = new Leaf(t);
        Update pupdate, newup;
        IInfo op;
        Info pInfo;
        int[] pState = new int[1];

        while(true) {
            SearchObject res = search(t);
            
            compare = res.l.compareTo(t);

            if (compare == 0)
                return;
            
            if (res.pupdate.info.getStamp() != CLEAN)
                help(res.pupdate);
            else {
                newSibling = new Leaf(res.l.key);
                if (compare < 0) // res.l < t
                    newInternal = new Internal(t, newSibling, newLeaf);
                else             // res.l >= t
                    if(res.l.key == null)
                        newInternal = new Internal(newLeaf, newSibling);
                    else
                        newInternal = new Internal(res.l.key, newLeaf, newSibling);
                newInternal.update = new Update();
                pInfo = res.pupdate.info.get(pState);
                op = new IInfo(res.p, res.l, newInternal);
                
                /* set next insert */
                if(res.p.update.info.compareAndSet(pInfo, op, pState[0], IFLAG)) {
                    helpInsert(op);
                    return;
                } else {
                    //help(result);  //FIXME dit stond er maar compile error
                    //System.out.println("Holy shit deze hebben we nog niet!");
                    pInfo = res.p.update.info.get(pState);

                    help(res.p.update);
                }
            }
        }
    }

    public void remove(T t) {
        Internal gp;
        Update gpupdate, result;
        DInfo op;

        //System.out.println(toString() + "\n");

        while(true) {
            SearchObject res = search(t);

            if(res.l.compareTo(t) != 0)
                return;
            if(res.gpupdate.info.getStamp() != CLEAN) {
                help(res.gpupdate);
            } else if(res.pupdate.info.getStamp() != CLEAN) {
                help(res.pupdate);
            } else {
                op = new DInfo(res.gp, res.p, res.l, res.pupdate);
                
                // FIXME compare and swap to compare and set + get

                //result := CAS(gp → update, gpupdate, ⟨DFlag, op⟩)
                int[] stamp = new int[1];
                Info info = res.gp.update.info.get(stamp);
                if(res.gp.update.info.compareAndSet(res.gpupdate.info.getReference(), (Info) op, res.gpupdate.info.getStamp(), DFLAG))
                    if(helpDelete(op))
                        return;
                else {
                    //System.out.println("failed compareAndSet 1");
                    //help(result); // FIXME result to null voor compile error
                    help(res.gp.update);
                }
            }
        }
    }

    public void help(Update u) {
        int uState = u.info.getStamp();
        if (uState == IFLAG)
            helpInsert((IInfo)u.info.getReference());
        else if(uState == MARK)
            helpMarked((DInfo)u.info.getReference());
        else if(uState == DFLAG)
            helpDelete((DInfo)u.info.getReference());
    }

    public void helpInsert(IInfo op) {
        if(op == null) {
            return;
        }

        casChild(op.p, op.l, op.newInternal);

        op.p.update.info.compareAndSet(op, op, IFLAG, CLEAN);
            //System.out.println("failed compareAndSet 2");

         
    }

    public void casChild(Internal parent, Node old, Node nw) {
        T key = nw.key;

        if(nw.getClass() == Leaf.class)
            key = ((Leaf)nw).key;
            
        //if(parent.compareTo(key) > 0) // -1   
        if(nw.compareTo(parent.key) < 0) // -1   
            // parent > nw.key
            parent.left.compareAndSet(old, nw);
                //System.out.println("failed compareAndSet 3");
        else
            // parent <= nw.key
            parent.right.compareAndSet(old, nw);
                //System.out.println("failed compareAndSet 4");
    }


    public boolean helpDelete(DInfo op) {
        int[] opFlag = new int[1];
        Info opInfo;

        if(op == null)
            return false;

        opInfo = op.pupdate.info.get(opFlag);
        if(op.p.update.info.compareAndSet(opInfo, op, opFlag[0], MARK)){
            helpMarked(op);
            return true;
        } else {
            //System.out.println("failed compareAndSet 5");
            help(op.p.update);
            
            //CAS(op ->  gp -> update, <DFlag, op>, <Clean, op>)
            op.gp.update.info.compareAndSet(op, op, DFLAG, CLEAN);
                //System.out.println("failed compareAndSet 6");
            return false;
        }
    }

    public void helpMarked(DInfo op) {
        //System.out.println("ENTER HELP MARKED");
        int[] opFlag = new int[1];
        Node other;

        // p(43).l == 43 
        if(op.p.right.get() == op.l)
            other = op.p.left.get();
        else
            other = op.p.right.get();

        casChild(op.gp, op.p, other);
        op.gp.update.info.compareAndSet(op, op, DFLAG, CLEAN);
            //System.out.println("failed compareAndSet 7");
        return;
    }

    /**
    * Returns the tree in a string.
    * @return String
    */
    public String toString() {
        return root.print("", true, false);
    }

    private SearchObject search(T t){
        Internal gp = null,
                 p = root;

        Node l = root.left.get();

        Update gpupdate = null,
               pupdate = root.update;
        
        while (l.getClass() == Internal.class) {
            gp = p;
            p = (Internal) l;
            gpupdate = pupdate;
            pupdate = p.update;
            
            if (l.compareTo(t) > 0) { 
                l = p.left.get();
            } else {
                l = p.right.get();
            }
        }
        return new SearchObject(gp, p, (Leaf) l, pupdate, gpupdate);
    }

    class SearchObject {
        Internal gp, p;
        Leaf l;
        Update pupdate, gpupdate;

        SearchObject(Internal gp, Internal p, Leaf l, Update pupdate, Update gpupdate){
            int[] pState = new int[1];
            Info inf;
            this.gp = gp;
            this.p = p;
            this.l = l;
            
            inf = pupdate.info.get(pState);
            this.pupdate = new Update(inf, pState[0]);

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

        IInfo(Internal p, Leaf l, Internal newInter) {
            this.p = p;
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
        protected abstract String print(String prefix, Boolean tail, Boolean l);
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
        
        Internal(Node l, Node r) {
            left = new AtomicReference<Node>(l);
            right = new AtomicReference<Node>(r);
        }
        Internal(T t, Node l, Node r) {
            key = t;
            left = new AtomicReference<Node>(l);
            right = new AtomicReference<Node>(r);
        }

        protected int compareTo(T t) {
            if(key == null)
                return 1;
            if(t == null)
                return -1;
            return key.compareTo(t);
        }

        protected String print(String prefix, Boolean tail, Boolean l) {
            String s = prefix;
            if(l)
                s += (tail ? " l " : "├── l ");
            else
                s += (tail ? "└── r " : "├── r ");
            if(key != null)
                s += key.toString() + '\n';
            else
                s += "null\n";
            if(left != null && left.get() != null) {
                if(right == null || right.get() == null) {  // only a left child
                    s += left.get().print(prefix + (tail ? "    " : "│   "), true, true);
                }
                else {  // left and right child
                    s += left.get().print(prefix + (tail ? "    " : "│   "), false, true);
                    s += right.get().print(prefix + (tail ? "    " : "│   "), true, false);
                }
            } else if(right != null && right.get() != null) // only a right child
                s += right.get().print(prefix + (tail ? "    " : "│   "), false, false);
            return s;
        }
    }
     
    class DummyNode extends Internal {
        DummyNode() {}
        DummyNode(Node l, Node r) {
            left = new AtomicReference<Node>(l);
            right = new AtomicReference<Node>(r);
        }

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
            if(key == null)
                return 1;
            if(t == null)
                return -1;
            return key.compareTo(t);
        }

        protected String print(String prefix, Boolean tail, Boolean l) {
             String s = prefix;
             if(l)
                 s += (tail ? "└── l " : "├── l ");
             else
                 s += (tail ? "└── r " : "├── r ");
             if(key != null)
                 s += key.toString() + '\n';
             else
                 s += "null\n";
             return s;
         }
    }

    class DummyLeaf extends Leaf {
        protected int compareTo(T t){
            return 1;
        }
    }
}
