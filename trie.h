typedef unsigned char u8;
typedef unsigned int u32;

#ifndef bool
#define bool u8
#endif
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#define MAX_CHILD_NODES 26

#define TOUPPER(s) (s)>90 ? ((s) - 0x20) : (s)
#define TONUMBER(s) ((s) - 0x41)

typedef struct trie_node {
    u8 key;
    int num_children;
    struct trie_node *children[MAX_CHILD_NODES];
} trie_node;

typedef struct {
    trie_node *root;
} trie;

/*************************************/
/*          Initialization           */
/*************************************/
extern trie*
malloc_trie();

static trie_node*
malloc_root_node();

static bool
malloc_child_node(trie_node *, char);

extern void
free_trie(trie *);

static void
free_nodes(trie_node *);

/*************************************/
/*            Insertion              */
/*************************************/

extern bool
insert_word(trie *, char *);

static trie_node*
insert_char(trie_node *, char);


/*************************************/
/*            Searching              */
/*************************************/

extern bool
is_in_tree(char *);

/*************************************/
/*              Debug                */
/*************************************/

static void
dump_trie(trie *);

static void
dump_nodes(trie_node *, char *, int);
