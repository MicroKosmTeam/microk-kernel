# 3 dispositions

## 1. Linear:
 Cspace                -O
  V                     V
  Cnode <-> Cnode <-> Cnode
              A         A
            Cspace     -O

- Easy to mismanage
- Overhead with userspace having to know where capabilites are
- Suboptimal usage of memory

## 2. Slab

Cspace -> Cslab
            V
            Cnode <-> Cnode <-> Cnode
       -> Cslab
            V
            Cnode <-> Cnode <-> Cnode

- Easy to manage
- Good memory usage
- Have to check through every CNode to see if capability is present in slab
 - Only mitigable with cache

## 3. Addess tree

Cspace
0xFF8810000 - 0xFF8860000

Slab A, Slab B, Slab C

0xFF8810000 0xFF8812000 0xFF8813000 0xFF8813000 0xFF8813000 0xFF8813000 ...
Node A      Node B      Node C      Node A      Node B      Node C      ...

- Easy capability indexing
- Obviously some kinds of capabilities will be more present than others (ie Frames vs CSpaces) and that may cause wasted space
 - Fixable with a different ratios (i.e. 1 cspace cnode per 1024 frames cnodes)
 - This may cause inflexibility however (i.e. running out of virtual address space)
- There can still be holes in between the nodes
 - This will lead to page faults if userspace tries to access a capability within the range of a cspace
 - Can be fixed by checking beforehand if the page is mapped (4 accesses to memory in the worst scenario)
 - If not, those must be handled accordingly by either
  - Invoking a sigsegv userspace handler (best option)
  - Automatically allocating an empty node if possible (eeh)
  - Just failing and stopping the thread pending resolution
- Can be difficult to mantain the overhead of virtual memory
- Finding a free slot isn't as easy as the slab method
 - We can do load balancing on the nodes (i.e. create alternatively on each one)
 - Also use a last created slot in addition to that
 - Also need to check that the node exists in the page structure
- Sharing can be complicated
 - Unknown who has to shoulder the responsibility for page mappings if its global
 - If its not global, how do you share Cspaces?
 - Can be done with on-demand paging, but that's SLOOOW as shit
 - Csaces are tied with address spaces

Addressing can follow the 0x00:0x0000000000:0x0000 method (within a specific Cspace)
                          kind           capability idx
                              cnode page idx


## 4. No capability storage at all

Cspace
- Kernel: Private Key
- User: Public Key

- Initial capabilities (signed) are given to userspace
- For stuff to be done, they must be correct
- Issue may be if resources are lost (i.e. lost capabilites)
 - Who cares, that is literally a memory leak, userspace issue

