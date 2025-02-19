#ifndef SPELL_CHECKER_H
#define SPELL_CHECKER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the structure for a BST node
typedef struct TreeNode {
    char word[256];
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

// Function prototypes
void initializeTree(TreeNode** root);
void insertWord(TreeNode** root, const char* word);
bool searchWord(const TreeNode* root, const char* word);
void suggestWords(const TreeNode* root, const char* word);
void freeTree(TreeNode* root);

#endif
