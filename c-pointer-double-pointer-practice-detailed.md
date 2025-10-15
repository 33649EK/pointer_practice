# C Pointer & Double-Pointer Practice — Detailed Explanations

Below are deeper explanations for each problem: what it exercises, design/contract notes, common pitfalls, edge cases, and optional stretch goals. Pair this with the companion file of expected outputs if you want bite-size checks while you implement.

---

## Level 1 – Warmups (pointer basics & arrays)

### 1) Rotate three
**Goal:** Practice reading/writing through pointers, snapshotting values, and documenting **aliasing**.  
**Details:** Implement `void rotate3(int *a, int *b, int *c)` so `(a,b,c)->(b,c,a)` when all three are **distinct**. If any arguments alias (e.g., `a==b`), the permutation is ambiguous; define a deterministic rule by snapshotting (`old_a, old_b, old_c`) and then assigning in a specific order (e.g., `*c=old_a; *b=old_c; *a=old_b;`).  
**Pitfalls:** Forgetting to snapshot leads to clobbering (`*a=*b; *b=*c; *c=*a;` destroys `old_a`).  
**Edge cases:** All pointers equal; two equal; NULL pointers (either forbid or assert).  
**Stretch:** Provide a “strict” variant that **rejects** aliasing and returns an error code.

### 2) `my_strlen` / `reverse_inplace`
**Goal:** Force pointer arithmetic and careful `const` use.  
**Details:** Walk until `*p=='\0'` (don’t use `[]`). For reverse, use two pointers (`lo`, `hi`) and swap while `lo<hi`.  
**Pitfalls:** Off-by-one on empty string; writing through `const char *`; not handling `NULL` (decide your policy).  
**Stretch:** Make a `reverse_range(char *first, char *last)` that works on arbitrary subranges.

### 3) Slice view & sum
**Goal:** Understand **non-owning** pointer ranges `[begin,end)` (half-open).  
**Details:** A view is just two pointers; sum by walking `for (int *p=v.begin; p!=v.end; ++p)`.  
**Pitfalls:** Accessing when `begin > end`; mutating through the view when you intended const.  
**Stretch:** Add `size(view)`, `empty(view)`, and a bounds-checking debug mode.

---

## Level 2 – Double pointers (caller’s state mutation)

### 4) Push front (singly list)
**Goal:** Learn why a **double pointer** (`struct Node **head`) is needed to change the caller’s head.  
**Details:** Allocate a node, set `node->next=*head; *head=node;`.  
**Pitfalls:** Forgetting to initialize `next`; memory leak on allocation failure handling.  
**Stretch:** Add an out-parameter returning the new node pointer or an error code.

### 5) Pop first matching
**Goal:** Master the `T **cursor` pattern for deletion without a special-case for head.  
**Details:** Use `for (struct Node **pp=&head; *pp; pp=&(*pp)->next)`; when match, `dead=*pp; *pp=dead->next; free(dead); return 1;`.  
**Pitfalls:** Using separate `prev` and `cur` can miss the uniformity; forgetting to return after delete; double-free.  
**Stretch:** Return the removed value via out-parameter; remove **all** matches instead of first.

### 6) Grow buffer (owning, `realloc`)
**Goal:** Practice updating **two** pieces of caller state via pointers: the data pointer and its capacity.  
**Details:** If `*cap < min_needed`, compute new capacity (e.g., next power of two), call `realloc`, then update `*buf` and `*cap` **atomically** (use a temp). Return `0` on success, `-1` on failure (and leave inputs unmodified).  
**Pitfalls:** Losing the old pointer on `realloc` failure; integer overflow in size math `elem_size * new_cap`.  
**Stretch:** Add a growth policy cap, and a function to shrink to fit.

---

## Level 3 – Strings & tokenization

### 7) Split on delimiter
**Goal:** Returning a heap array of `char*` (**pointer-to-pointer**) and cleaning up on partial failures.  
**Details:** Two-pass design works well: count tokens, then allocate `char **tokens` and each `char *copy`. Always provide `split_free(tokens,count)`; also consider adding a null-terminator entry for ergonomic iteration.  
**Pitfalls:** Not handling consecutive delimiters (`"a,,b"` → empty token); forgetting to set `*out_count`; leaking already-allocated pieces when a later `malloc` fails.  
**Stretch:** Support multiple delimiters or quoted fields.

### 8) Join with delimiter
**Goal:** Use pointer arithmetic and careful length accounting across many small pieces.  
**Details:** First pass sums lengths plus separators; second pass copies with `memcpy`. Signature `char *join(char *const *tokens, size_t count, const char *sep)` conveys you don’t modify the token pointers.  
**Pitfalls:** Separator placement (no leading/trailing sep); overflow on total length.  
**Stretch:** Add a `join_into(char *dst, size_t cap, ...)` variant that returns needed size.

### 9) In-place `strip`
**Goal:** Return an **interior pointer** into the same buffer while trimming ends in place.  
**Details:** Advance `left` over space; retreat `right` over space; put a `\0` after the last nonspace; return `left`.  
**Pitfalls:** All-whitespace string should yield an empty string (`*s='\0'`) and return pointer to that terminator; don’t free the original buffer since you didn’t reallocate.  
**Stretch:** Support Unicode whitespace (hard in plain C) or a caller-provided predicate.

---

## Level 4 – Matrices & 2D memory

### 10) Contiguous 2D matrix factory
**Goal:** Return an `int **` that indexes like `m[r][c]`, but all elements live in one contiguous block for cache-friendliness and single `free`.  
**Details:** Allocate one big block for `rows*cols` and a small block for `int *rowptrs[rows]` that each point into the big block at offsets `i*cols`. Free them with **one** call by storing a header or pointing `rowptrs` right before the big block.  
**Pitfalls:** Doing `rows` separate `malloc`s (harder to free and not contiguous); forgetting to store metadata to free correctly.  
**Stretch:** Provide both “contiguous” and “jagged” allocators and measure performance differences.

### 11) Transpose square matrix in place
**Goal:** Convert index algebra into pointer arithmetic and avoid double-swapping on the diagonal.  
**Details:** For each `0 ≤ r < n`, swap `(r,c)` with `(c,r)` only for `c>r`. Address is `m + r*n + c`.  
**Pitfalls:** Swapping diagonal elements; using `int**` for a flat array (keep it `int*`).  
**Stretch:** Transpose non-square matrices into another buffer.

### 12) Insert row (triple pointer)
**Goal:** Manage a **table of pointers** (`int **`) that itself is heap-allocated, while also owning each row’s heap data: hence `int ***m` to replace the caller’s table pointer.  
**Details:** Realloc the row-pointer table to size `*rows+1`, allocate a new row of `cols`, copy from `src`, shift pointers at/after `pos`, and update `*rows`. On any failure, roll back.  
**Pitfalls:** Leaking the new row when `realloc` fails; forgetting to shift from the end to avoid overwrite.  
**Stretch:** Support deleting rows and resizing columns (with contiguous layout variant).

---

## Level 5 – Data structures (pointer-to-pointer idioms)

### 13) Insert into sorted singly list
**Goal:** Idiomatic insertion using `struct Node **pp` to avoid a special head case.  
**Details:** Walk with `for (pp=&head; *pp && (*pp)->x < x; pp=&(*pp)->next) {}` then insert at `*pp`.  
**Pitfalls:** Stability when duplicates are allowed/forbidden; allocating the node before or after the loop (allocate once you know location).  
**Stretch:** Return a pointer to the inserted node and/or a boolean “was duplicate”.

### 14) Hash table (separate chaining)
**Goal:** Pass an “array of list heads” and operate on the relevant bucket via `struct Node **`.  
**Details:** Compute bucket `i = hash(key) % N`; in that chain, either update existing node or insert at head. Keep a size counter.  
**Pitfalls:** Poor hash leading to long chains; forgetting to handle updates vs inserts; memory management on delete.  
**Stretch:** Implement rehashing when load factor exceeds a threshold.

### 15) Remove all satisfying predicate
**Goal:** Robust deletion with `T **cursor` across multiple removals.  
**Details:** Loop: if `pred((*pp)->x)` then splice and free; else advance `pp` to `&(*pp)->next`. Return count removed.  
**Pitfalls:** Advancing the pointer incorrectly after deletion; freeing then dereferencing.  
**Stretch:** Provide a stable partition function that preserves relative order of kept nodes.

### 16) Flatten multilevel list
**Goal:** Splice `child` lists into `next` pointers **in place**, depth-first, without extra containers.  
**Details:** A recursive approach can return the tail after flattening a sublist; an iterative approach uses `struct N **tail_ref` to stitch children where found. Ensure cycles aren’t introduced.  
**Pitfalls:** Losing the rest of the `next` chain when inserting a child; infinite loops if `child`/`next` cross-link.  
**Stretch:** Implement an unflatten that restores `child` pointers.

---

## Level 6 – Memory & systems-y

### 17) `my_memmove`
**Goal:** Implement overlap-safe copy logic with byte pointers.  
**Details:** If `dst < src` or regions don’t overlap, copy forward; else copy backward from the end. Use `unsigned char *` for byte-wise copy.  
**Pitfalls:** Using `memcpy` semantics (undefined for overlap); pointer subtraction with `void *` (cast to `unsigned char *`).  
**Stretch:** Optimize with word-sized chunks while handling tails.

### 18) Tiny arena allocator
**Goal:** Manage a bump-pointer allocator: fast `alloc`, cheap `reset`, one `free`.  
**Details:** Keep `base`, `used`, `cap`. For each `alloc(n,align)`, round `used` up to `align`, check capacity, return `base+used`, then bump `used`. `reset` sets `used=0`; `free` releases the whole block.  
**Pitfalls:** Incorrect alignment math (use `(x + (a-1)) & ~(a-1)`); overflow when rounding.  
**Stretch:** Add sub-arenas, temporary checkpoints, or a linear “free to mark”.

### 19) Endianness swapper
**Goal:** Practice bitwise shifts, masks, and in-place traversal via pointers.  
**Details:** `bswap32(x)` swaps byte order; for buffers, iterate through `p[i]=bswap32(p[i])`.  
**Pitfalls:** Shifting by wrong amounts; aliasing issues if you reinterpret arbitrary byte arrays as `uint32_t*` without alignment guarantees.  
**Stretch:** Provide `bswap16`, `bswap64`, and host endianness detection.

### 20) Pointer-based ring buffer
**Goal:** Use pointers and wrap-around without modulo; carefully manage **full vs empty** distinction.  
**Details:** Maintain `head`, `tail`, `end` (one past last slot). Write until you’d collide with `tail-1` (keep one slot empty). When a pointer hits `end`, wrap to `base`. For `peek`, copy without advancing.  
**Pitfalls:** Off-by-one errors on wrap; ambiguous full/empty if you don’t reserve a slot; copying across the physical end (needs two `memcpy`s).  
**Stretch:** Add a blocking variant, or a multi-producer/consumer lock-free design (much harder).

---

## “Trick” micro-exercises

- **`cmp_ptr` for `qsort` of `int*`:** The comparator receives `const void *a`/`b` which actually point to elements of the array—i.e., **pointers to `int*`**. You must cast to `const int *const *`, then dereference once to compare pointed ints. Exercises pointer-to-pointer types and `void*` casting discipline.  
- **`my_strdup`:** Reinforces length calculation, allocation, copying, and returning a newly owned pointer.  
- **`set_all`:** Classic simple pointer walk that avoids indexing—great for practicing `p < base+n`.  
- **`take_ownership`:** Shows why `char **dst` is needed to update the caller’s pointer atomically; be careful to `free(*dst)` before overwriting it.  
- **`find_char`:** Trains returning interior pointers; the parameter `int c` matches standard library signatures allowing any `unsigned char` value plus EOF in C’s I/O APIs.

---

## Contracts, Aliasing & Safety Recap

- **Document** whether aliasing is allowed. If it is, define behavior via assignment order or snapshotting.  
- Use `restrict` **only** when you can guarantee non-aliasing; violating it produces undefined behavior.  
- Always consider: ownership (who `free`s?), lifetime (is it an interior pointer?), and failure paths (free partially-allocated pieces).  
- Build with `-Wall -Wextra -Werror -fsanitize=address,undefined` during development.

---

If you’d like, I can generate scaffold `.h/.c` files for any subset with TODOs, asserts, and minimal tests tailored to these notes.
