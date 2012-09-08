VU
==

VU Computer Science Master

assignment 1
===========

## Coarse Grained List
> ### Estimate performance    
> Every thread that uses this list will lock the
> whole data structure before entering the critical section.
> The critical section in this case is: 
> 1.lookup
> 2.inserting/deleting.
> The lookup will be different for inserting and deletion.
>
> Let X and Y be a Thread that both have items to add to the list. For any
> item i, the thread X and Y will have to require a lock. This means that
> if X is in the inserting function, Thread Y has to wait until Thread X is 
> done with 1. and 2.
> This means that any number of Threads have to wait before they can start
> to do anything with the data structure. Which will be exactly the same
> as a sequential implementation of the linked list data structure.

