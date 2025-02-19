#ifndef SPELL_CHECKER_H
#define SPELL_CHECKER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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
void suggestWords(const TreeNode* root, const char* word, int maxDistance);
void freeTree(TreeNode* root);
int levenshteinDistance(const char* str1, const char* str2);

#endif
