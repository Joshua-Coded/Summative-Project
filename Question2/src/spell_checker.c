#include "spell_checker.h"
#include <ctype.h>

// Define a macro for finding the minimum of three values
#define MIN(a, b, c) ((a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b) : (c)))

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

// Suggest similar words based on Levenshtein Distance
void suggestWords(const TreeNode* root, const char* word, int maxDistance) {
    if (root == NULL) return;

    // Calculate edit distance
    int distance = levenshteinDistance(word, root->word); // Use the declared function
    if (distance <= maxDistance) {
        printf("Did you mean: %s\n", root->word);
    }

    // Traverse left and right subtrees
    suggestWords(root->left, word, maxDistance);
    suggestWords(root->right, word, maxDistance);
}

// Function to calculate Levenshtein Distance
int levenshteinDistance(const char* str1, const char* str2) {
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);

    if (len1 == 0) return len2;
    if (len2 == 0) return len1;

    int matrix[len1 + 1][len2 + 1];

    // Initialize matrix
    for (size_t i = 0; i <= len1; i++) matrix[i][0] = i;
    for (size_t j = 0; j <= len2; j++) matrix[0][j] = j;

    // Compute distances
    for (size_t i = 1; i <= len1; i++) {
        for (size_t j = 1; j <= len2; j++) {
            int cost = (str1[i - 1] == str2[j - 1]) ? 0 : 1;
            matrix[i][j] = MIN(
                matrix[i - 1][j] + 1,      // Deletion
                matrix[i][j - 1] + 1,      // Insertion
                matrix[i - 1][j - 1] + cost // Substitution
            );
        }
    }

    return matrix[len1][len2];
}

// Free memory allocated for the BST
void freeTree(TreeNode* root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}
