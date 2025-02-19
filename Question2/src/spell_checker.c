#include "spell_checker.h"
#include <ctype.h>

// Initialize an empty BST
void initializeTree(TreeNode** root) {
    *root = NULL;
}

// Insert a word into the BST
void insertWord(TreeNode** root, const char* word) {
    if (*root == NULL) {
        *root = malloc(sizeof(TreeNode));
        strcpy((*root)->word, word);
        (*root)->left = (*root)->right = NULL;
    } else if (strcmp(word, (*root)->word) < 0) {
        insertWord(&(*root)->left, word);
    } else if (strcmp(word, (*root)->word) > 0) {
        insertWord(&(*root)->right, word);
    }
    // Duplicate words are ignored
}

// Search for a word in the BST
bool searchWord(const TreeNode* root, const char* word) {
    if (root == NULL) {
        return false;
    }
    int cmp = strcmp(word, root->word);
    if (cmp == 0) {
        return true;
    } else if (cmp < 0) {
        return searchWord(root->left, word);
    } else {
        return searchWord(root->right, word);
    }
}

// Suggest similar words based on Levenshtein distance (simple implementation)
void suggestWords(const TreeNode* root, const char* word) {
    if (root == NULL) {
        return;
    }

    // Simple similarity check: Check words with one character difference
    if (strlen(root->word) == strlen(word)) {
        int diff = 0;
        for (size_t i = 0; i < strlen(word); i++) {
            if (tolower(root->word[i]) != tolower(word[i])) {
                diff++;
                if (diff > 1) break;
            }
        }
        if (diff == 1) {
            printf("Did you mean: %s\n", root->word);
        }
    }

    suggestWords(root->left, word);
    suggestWords(root->right, word);
}

// Free memory allocated for the BST
void freeTree(TreeNode* root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}
