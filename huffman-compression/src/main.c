#include "huffman.h"

void displayMenu() {
    printf("\n========== Huffman Coding Tool ==========\n");
    printf("1. Encode a text file\n");
    printf("2. Decode a compressed file\n");
    printf("3. Exit\n");
    printf("=========================================\n");
    printf("Enter your choice (1-3): ");
}

int main() {
    int choice;
    char inputFile[256];
    char outputFile[256];
    
    while (1) {
        displayMenu();
        scanf("%d", &choice);
        getchar(); // consume newline
        
        switch (choice) {
            case 1:
                printf("\nEnter input file path: ");
                fgets(inputFile, sizeof(inputFile), stdin);
                inputFile[strcspn(inputFile, "\n")] = 0; // Remove newline
                
                printf("Enter output file path: ");
                fgets(outputFile, sizeof(outputFile), stdin);
                outputFile[strcspn(outputFile, "\n")] = 0;
                
                encodeFile(inputFile, outputFile);
                break;
                
            case 2:
                printf("\nEnter compressed file path: ");
                fgets(inputFile, sizeof(inputFile), stdin);
                inputFile[strcspn(inputFile, "\n")] = 0;
                
                printf("Enter output file path: ");
                fgets(outputFile, sizeof(outputFile), stdin);
                outputFile[strcspn(outputFile, "\n")] = 0;
                
                decodeFile(inputFile, outputFile);
                break;
                
            case 3:
                printf("Exiting...\n");
                return 0;
                
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
    
    return 0;
}
