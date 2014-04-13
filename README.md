Pandaboard-Instruction-Count
============================

MCR instruction to enable performance counters on Pandaboard or any ARM A9 board

I would give credit to the blog where I found it, but I really don't remember where I found it.

```
        /* enable user-mode access to the performance counter*/
        asm ("MCR p15, 0, %0, C9, C14, 0\n\t" :: "r"(1));

        /* disable counter overflow interrupts (just in case)*/
        asm ("MCR p15, 0, %0, C9, C14, 2\n\t" :: "r"(0x8000000f));
```
