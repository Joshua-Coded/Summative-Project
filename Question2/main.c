#include "spell_checker.h"

int main() {
    TreeNode* root = NULL;
    initializeTree(&root);

    // Load dictionary file
    FILE* dictFile = fopen("dictionary.txt", "r");
    if (dictFile == NULL) {
        printf("Error: Dictionary file not found.\n");
        return 1;
    }

    char word[256];
    while (fscanf(dictFile, "%s", word) != EOF) {
        insertWord(&root, word);
    }
    fclose(dictFile);

    printf("Spell Checker Tool\n");
    printf("==================\n");

    while (1) {
        printf("\nEnter a word (or 'exit' to quit): ");
        scanf("%s", word);

        if (strcmp(word, "exit") == 0) {
            break;
        }

        if (searchWord(root, word)) {
            printf("Word found.\n");
        } else {
            printf("Word not found.\n");
            printf("Suggestions:\n");
            suggestWords(root, word, 2); // Suggest words with edit distance ≤ 2
        }
    }

    freeTree(root);
    return 0;
}
