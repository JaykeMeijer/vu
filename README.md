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
> 1.    lookup
> 2.    inserting/deleting.
> The lookup will be different for inserting and deletion.
>
> Let X and Y be a Thread that both have items to add to the list. For every
> item, the thread X and Y will have to require a lock. So
> if X has the lock, Thread Y has to wait until Thread X is 
> done with 1. and 2.
> This means that any number of Threads will have to wait before they can start
> doing anything with the data structure. Which will exactly be the same
> as a sequential implementation of the linked list data structure.
> 
> E: number of elements
> T: number of threads
> A: amount of work per thread
> if E grows and T stays equal, only the lookup could influence the performance.
>   i.e. more lookups, more work
> if T grows and E stays equal, there will be no influence on the performance.

## Coarse Grained Tree
> ### Estimate performance
> The Course Grained synchronization in a tree also locks the entire
> data structure. By inserting an item the lookup and inserting will have to
> require a lock but now in a tree fashion. Thread Y also needs to wait until
> Thread X is done with these operation for inserting and deleting.
>
> E: number of elements
> T: number of threads
> A: amount of work per thread
> if E grows and T stays equal, only the lookup could influence the performance.
>   i.e. more lookups, more work
> if T grows and E stays equal, there will be no influence on the performance.






