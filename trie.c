#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "trie.h"


extern trie*
malloc_trie() {
    trie* ret = malloc(sizeof(trie));
    if (!ret) {
        return NULL;
    }

    ret->root = malloc_root_node();
    if (!ret->root) {
        free(ret);
        return NULL;
    }
    return ret;
}

static trie_node*
malloc_root_node() {
    trie_node *ret = malloc(sizeof(trie_node));
    u8 i;

    if (!ret) {
        return NULL;
    }

    ret->key = 0;
    ret->num_children = 0;
    
    for(i=0; i<MAX_CHILD_NODES; i++) {
        ret->children[i] = NULL;
    }
    
    return ret;
}

static bool
malloc_child_node(trie_node *root, char key) {
    trie_node *i = root;
    u8 j;
    
    i->children[TONUMBER(key)] = malloc(sizeof(trie_node));
    if (!i->children[TONUMBER(key)]) {
        return FALSE;
    }

    i->num_children++;
    i = i->children[TONUMBER(key)];
    i->key = key;
    i->num_children = 0;
    
    for(j=0; j<MAX_CHILD_NODES; j++) {
        i->children[j] = NULL;
    }

    return TRUE;
}

extern void
free_trie(trie* val) {
    free_nodes(val->root);
    free(val);
}

static void
free_nodes(trie_node *root) {
    u8 i;
    for(i=0; i<root->num_children; i++) {
        free_nodes(root->children[i]);
    }

    free(root);
}

extern bool
insert_word(trie *val, char *string) {
    u8 i;
    trie_node *cur = val->root;

    if (!strlen(string)) {
        return FALSE;
    }

    for(i=0; i<strlen(string); i++) {
       cur = insert_char(cur, string[i]);
       if (!cur) return FALSE;
    }
    
    return TRUE;
}

static trie_node*
insert_char(trie_node *val, char s) {
    s = TOUPPER(s);
    if (TONUMBER(s) >= (MAX_CHILD_NODES)) {
        return NULL;
    }

    if (!val->children[TONUMBER(s)]) {
        malloc_child_node(val, s);
    }

    return val->children[TONUMBER(s)];
}

extern bool
is_in_tree(trie *val, const char *string) {
    if (!val->root) {
        return FALSE;
    }

    return tree_find(val->root, string);
}

static bool
tree_find(trie_node *root, const char *string) {
    u8 i;
    printf("Searching tree for %s\n", string);
    trie_node *iter = root;
    for(i=0; i<strlen(string); i++) {
        char curChar = TOUPPER(string[i]);
        printf("Current char: %c  ", curChar);
        if (!iter->children[TONUMBER(curChar)]) {
            printf("Booooo.\n");
            return FALSE;
        }
        printf("Found key: %c\n", iter->key);
        iter = iter->children[TONUMBER(curChar)];
    }
    return TRUE;
}

static void
dump_trie(trie *tr) {
    char debug[100] = {0};
    if(tr->root)
        dump_nodes(tr->root, debug, 0);
}

static void
dump_node_info(trie_node *node, char *recur) {
    printf("Current node key: %c\n", node->key);
    printf("Children: %d\n\n", node->num_children);
}

static void
dump_nodes(trie_node *node, char recur[], int len) {
    u8 i;
    
    if (!node->num_children) { 
        //At the leaf, print out the total string.
        recur[len++] = node->key;
        recur[len] = '\0';
        printf("%s\n", recur);
    }
    else {
        if (node->key != '\0') {
            recur[len] = node->key;
            len++;
        }
        for(i=0; i<MAX_CHILD_NODES; i++) {
            if(node->children[i]) {
                dump_nodes(node->children[i], 
                           recur,
                           len);
            }
        }
    }
}

static void 
debug_assert(char* st, bool should_be_true)
{
    if(should_be_true)
        printf("Passed.\n");
    else
        printf("*** FAILED. %s ***", st);
}

int main(int argc, char** argv[]) {
    trie* a = malloc_trie();
    bool ret;
    ret = insert_word(a, "test");
    ret = insert_word(a, "tear");
    ret = insert_word(a, "boo");
    ret = insert_word(a, "asdf");
    ret = insert_word(a, "jjjjj");
    ret = insert_word(a, "aaaaaaaaaaaaaaaaaaaaaaaa");
    dump_trie(a);


    debug_assert("test", is_in_tree(a, "test"));
    debug_assert("tes", is_in_tree(a, "tes"));
    debug_assert("bod", !is_in_tree(a, "bod"));
    debug_assert("test", is_in_tree(a, "test"));

    return 0;
}
