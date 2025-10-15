# C Pointer & Double-Pointer Practice — Problems + Expected Outputs

This set is organized by level. Each item includes a tight **Spec** and example **Expected Output/Behavior** to sanity‑check your solution. Pointers may alias unless the spec says otherwise; if aliasing is relevant, the behavior shown is the intended contract.

---

## Level 1 – Warmups (pointer basics & arrays)

### 1) Rotate three
**Spec:** Implement `void rotate3(int *a, int *b, int *c);` so values rotate `(a,b,c) → (b,c,a)`. Pointers may alias; defined by assignment order `*c=old_a; *b=old_c; *a=old_b;` (document).  
**Expected:**
- `a=1,b=2,c=3` → `a=2,b=3,c=1`  
- `x=7; rotate3(&x,&x,&x)` → `x=7`  
- `a==b!=c` with `(X,X,Y)` → `*a/*b = X`, `*c = X`

### 2) `my_strlen` / `reverse_inplace`
**Spec:** `size_t my_strlen(const char *s);` and `void reverse_inplace(char *s);` using pointer arithmetic only.  
**Expected:**
- `my_strlen("hello")` → `5`  
- `"abcd"` reversed → `"dcba"`  
- `""` length `0`, reverse `""`

### 3) Slice view & sum
**Spec:** Define `struct view { int *begin, *end; }; int sum(struct view v);` sum half‑open range `[begin,end)`.  
**Expected:**
- `arr={1,2,3,4}; v={arr+1, arr+3}` → `sum(v)=5`  
- `begin==end` → `0`

---

## Level 2 – Double pointers (caller’s state mutation)

### 4) Push front (singly list)
**Spec:** `struct Node { int x; struct Node *next; }; void push_front(struct Node **head, int x);`  
**Expected:**  
- `[]` + `push_front(5)` → `[5]`  
- `[2,3]` + `push_front(1)` → `[1,2,3]`

### 5) Pop first matching
**Spec:** `int pop_first(struct Node **head, int value);` remove first node with `x==value`, return 1 if removed, else 0.  
**Expected:**  
- `[4,7,7,9]` + `pop_first(...,7)` → returns `1`, list `[4,7,9]`  
- `[1,2,3]` + `pop_first(...,8)` → returns `0`, list unchanged

### 6) Grow buffer (owning, `realloc`)
**Spec:** `int grow_buffer(void **buf, size_t *cap, size_t elem_size, size_t min_needed);` grow to next power of two if `*cap<min_needed`. Update both `*buf` and `*cap`.  
**Expected:**  
- `cap=4, min_needed=10` → return `0`, new `cap≈16`  
- `cap=32, min_needed=8` → return `0`, `cap` remains `32`

---

## Level 3 – Strings & tokenization

### 7) Split on delimiter
**Spec:** `char **split(const char *s, char delim, size_t *out_count);` Returns heap array of heap‑copied tokens; also provide `split_free`.  
**Expected:**  
- `"a,b,,c", ','` → tokens `["a","b","","c"]`, `out_count=4`  
- `""` → `[]`, `out_count=0`

### 8) Join with delimiter
**Spec:** `char *join(char *const *tokens, size_t count, const char *sep);`  
**Expected:**  
- `["a","b","c"], "-"` → `"a-b-c"`  
- `[], ","` → `""`  
- `["x"], "/"` → `"x"`

### 9) In‑place `strip`
**Spec:** `char *strip(char *s);` trim leading/trailing spaces in place; return pointer to first nonspace (may be interior).  
**Expected:**  
- `"  hello  "` → returns pointer to `"hello"`; buffer now `"hello\0o  "` (or equivalent trailing bytes)  
- `" \t\n "` → returns `""` (points at the `\0`)

---

## Level 4 – Matrices & 2D memory

### 10) Contiguous 2D matrix factory
**Spec:** `int **make_matrix(size_t rows, size_t cols);` returns row‑pointer table into a single contiguous `rows*cols` block. `void free_matrix(int **m);` should free with **one** call.  
**Expected:**  
- `m=make_matrix(2,3); m[0][0]=1; m[1][2]=5;`  
  - `&m[1][0] == &m[0][0] + 3` → `true`  
  - rows: `[1,0,0]`, `[0,0,5]`  
  - `free_matrix(m)` → ASan clean

### 11) Transpose square matrix in place
**Spec:** `void transpose_inplace(int *m, size_t n);` row‑major `n×n`.  
**Expected:**  
- `{1,2,3, 4,5,6, 7,8,9}` → `{1,4,7, 2,5,8, 3,6,9}`  
- `n=1` → unchanged

### 12) Insert row (triple pointer)
**Spec:** `int insert_row(int ***m, size_t *rows, size_t cols, size_t pos, const int *src);` realloc row table; new row owns `cols` ints.  
**Expected:**  
- Start `[[1,2,3],[4,5,6]]`, insert `pos=1`, `src=[9,9,9]` →  
  `[[1,2,3],[9,9,9],[4,5,6]]`, returns `0`, `*rows=3`  
- `pos=*rows` → append (OK)

---

## Level 5 – Data structures (pointer‑to‑pointer idioms)

### 13) Insert into sorted singly list
**Spec:** `void insert_sorted(struct Node **head, int x);` (allow duplicates).  
**Expected:**  
- `[]` + `5` → `[5]`  
- `[1,3,7]` + `4` → `[1,3,4,7]`  
- `[1,3,4,7]` + `3` → `[1,3,3,4,7]`

### 14) Hash table (separate chaining)
**Spec:** Buckets as `struct Node *buckets[N];` `int ht_put(struct Node **buckets, size_t N, int key, int val);` insert or update.  
**Expected:**  
- Put `(10,1)` then `(10,5)` → value becomes `5`, size unchanged  
- Put `(21,9)` with `N=11` → lands in bucket `21%11=10` (example hash), chain contains `(21,9)`

### 15) Remove all satisfying predicate
**Spec:** `size_t list_remove_if(struct Node **head, int (*pred)(int));`  
**Expected:**  
- `[1,2,3,4,5]` with “even” → returns `2`, list `[1,3,5]`  
- predicate “>=0” → returns `5`, list `[]`

### 16) Flatten multilevel list
**Spec:** `struct N { int v; struct N *next, *child; }; void flatten(struct N **head);` depth‑first splice children into `next`.  
**Expected:**  
- `1->2->3`, with `2->child: 7->8` and `3->child: 9` → `[1,2,7,8,3,9]`  
- No children → unchanged

---

## Level 6 – Memory & systems‑y

### 17) `my_memmove`
**Spec:** `void *my_memmove(void *dst, const void *src, size_t n);` handle overlaps correctly using byte pointers.  
**Expected:**  
- `"abcdef"; memmove(buf+2, buf, 4)` → `"ababcd"`  
- `"abcdef"; memmove(buf, buf+2, 4)` → `"cdefef"`

### 18) Tiny arena allocator
**Spec:**  
```c
struct arena { unsigned char *base; size_t used, cap; };
int   arena_init(struct arena *a, size_t cap);
void *arena_alloc(struct arena *a, size_t n, size_t align);
void  arena_reset(struct arena *a);
void  arena_free(struct arena *a);
```
**Expected:**  
- After `arena_init(64)`:  
  - `p1=arena_alloc(13,8)` → `p1!=NULL`, `used` rounds up to `16`  
  - `p2=arena_alloc(32,16)` → `used = 16 + 32 = 48`  
  - `arena_alloc(40,8)` → `NULL` (insufficient space)  
  - `arena_reset()` → `used=0`; next alloc succeeds  
  - `arena_free()` → releases storage

### 19) Endianness swapper
**Spec:** `uint32_t bswap32(uint32_t x);` and `void bswap_buf32(uint32_t *p, size_t n);`  
**Expected:**  
- `bswap32(0x12345678)` → `0x78563412`  
- `bswap_buf32([0xAABBCCDD, 0x00000001],2)` → `[0xDDCCBBAA, 0x01000000]`

### 20) Pointer‑based ring buffer
**Spec:** Fixed‑size byte ring with head/tail pointers; avoid modulo by pointer comparisons/wrap. API: `rb_push`, `rb_pop`, `rb_peek` return counts actually moved. “Keep one slot empty” full/empty rule.  
**Expected:** (capacity 8)  
- Push `"ABCDE"` → wrote `5`, size `5`  
- Pop `3` → `"ABC"`, size `2`  
- Push `"123456"` → remaining capacity `8-1-2=5`, writes `5`, size `7`  
- Peek `4` → `"DE12"`

---

## “Trick” micro‑exercises (quick reps)

- **`cmp_ptr` for `qsort` of `int*`:** With `*x=2,*y=1,*z=3`, sorting pointers by pointed value yields order `(&y,&x,&z)` → values `[1,2,3]`.  
- **`my_strdup("hi")`:** returns new pointer `p != "hi"` with content `"hi"`.  
- **`set_all([0,0,0,0],4,7)`:** becomes `[7,7,7,7]`.  
- **`take_ownership(dst=&p, src=q)`:** frees old `*dst`, sets `*dst=q`, returns `0`.  
- **`find_char("hello",'l')`:** returns pointer to first `'l'` (index 2); `"hello",'x'` → `NULL`.

---

### Build & Test Tips
- Compile with: `-Wall -Wextra -Werror -fsanitize=address,undefined`  
- Add adversarial aliasing tests (e.g., pass the **same** pointer twice) where the spec allows it.  
- For matrix allocator, verify contiguity: `&row[i][j] == base + (i*cols + j)`.
