#include "huffman.h"
#include "priority_queue.h"

// Create a new Huffman tree node
Node* createNode(char data, int frequency) {
    Node *node = (Node*)malloc(sizeof(Node));
    node->data = data;
    node->frequency = frequency;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// Count frequency of each character in text
void countFrequencies(char *text, int *freq) {
    memset(freq, 0, sizeof(int) * 256);
    for (int i = 0; text[i] != '\0'; i++) {
        freq[(unsigned char)text[i]]++;
    }
}

// Build Huffman Tree from text
Node* buildHuffmanTree(char *text) {
    int freq[256];
    countFrequencies(text, freq);
    
    PriorityQueue *pq = createPriorityQueue();
    
    // Insert all characters with non-zero frequency into priority queue
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            insertNode(pq, createNode((char)i, freq[i]));
        }
    }
    
    // Build tree by repeatedly extracting two minimum nodes
    while (pq->size > 1) {
        Node *left = extractMin(pq);
        Node *right = extractMin(pq);
        
        Node *parent = createNode('\0', left->frequency + right->frequency);
        parent->left = left;
        parent->right = right;
        
        insertNode(pq, parent);
    }
    
    Node *root = extractMin(pq);
    freePriorityQueue(pq);
    
    return root;
}

// Generate Huffman codes from tree
void generateCodes(Node *root, CodeEntry *codes, char *code, int codeLen, int *codeCount) {
    if (!root) return;
    
    // Leaf node - store the code
    if (!root->left && !root->right) {
        code[codeLen] = '\0';
        codes[*codeCount].data = root->data;
        strcpy(codes[*codeCount].code, code);
        (*codeCount)++;
        return;
    }
    
    // Traverse left and right
    if (root->left) {
        code[codeLen] = '0';
        generateCodes(root->left, codes, code, codeLen + 1, codeCount);
    }
    
    if (root->right) {
        code[codeLen] = '1';
        generateCodes(root->right, codes, code, codeLen + 1, codeCount);
    }
}

// Print all generated codes
void printCodes(CodeEntry *codes, int codeCount) {
    printf("\n=== Huffman Codes Generated ===\n");
    for (int i = 0; i < codeCount; i++) {
        printf("'%c' (ASCII %d): %s\n", codes[i].data, (unsigned char)codes[i].data, codes[i].code);
    }
    printf("================================\n\n");
}

// Encode text file
void encodeFile(char *inputFile, char *outputFile) {
    FILE *in = fopen(inputFile, "r");
    if (!in) {
        printf("Error: Cannot open input file '%s'\n", inputFile);
        return;
    }
    
    // Read file into memory
    fseek(in, 0, SEEK_END);
    long size = ftell(in);
    fseek(in, 0, SEEK_SET);
    
    char *text = (char*)malloc(size + 1);
    fread(text, 1, size, in);
    text[size] = '\0';
    fclose(in);
    
    printf("Original text size: %ld bytes\n", size);
    printf("Building Huffman Tree...\n");
    
    // Build Huffman tree
    Node *root = buildHuffmanTree(text);
    
    // Generate codes
    CodeEntry codes[256];
    char code[MAX_CODE_LENGTH];
    int codeCount = 0;
    generateCodes(root, codes, code, 0, &codeCount);
    
    printCodes(codes, codeCount);
    
    // Encode text
    printf("Encoding file...\n");
    FILE *out = fopen(outputFile, "w");
    
    // Write code table to output file
    fprintf(out, "%d\n", codeCount);
    for (int i = 0; i < codeCount; i++) {
        fprintf(out, "%d %s\n", (unsigned char)codes[i].data, codes[i].code);
    }
    fprintf(out, "END_CODES\n");
    
    // Encode and write data
    fprintf(out, "BEGIN_DATA\n");
    for (int i = 0; text[i] != '\0'; i++) {
        for (int j = 0; j < codeCount; j++) {
            if (codes[j].data == text[i]) {
                fprintf(out, "%s", codes[j].code);
                break;
            }
        }
    }
    fprintf(out, "\nEND_DATA\n");
    
    fclose(out);
    
    printf("File encoded successfully to '%s'\n", outputFile);
    printf("Encoded data written.\n");
    
    // Calculate compression ratio
    FILE *compressed = fopen(outputFile, "r");
    fseek(compressed, 0, SEEK_END);
    long compressedSize = ftell(compressed);
    fclose(compressed);
    
    printf("Compressed size: %ld bytes\n", compressedSize);
    printf("Compression ratio: %.2f%%\n", (1.0 - (float)compressedSize / size) * 100);
    
    freeTree(root);
    free(text);
}

// Decode file
void decodeFile(char *inputFile, char *outputFile) {
    FILE *in = fopen(inputFile, "r");
    if (!in) {
        printf("Error: Cannot open input file '%s'\n", inputFile);
        return;
    }
    
    printf("Decoding file...\n");
    
    // Read code table
    CodeEntry codes[256];
    int codeCount;
    fscanf(in, "%d\n", &codeCount);
    
    for (int i = 0; i < codeCount; i++) {
        int ascii;
        char codeStr[MAX_CODE_LENGTH];
        fscanf(in, "%d %s\n", &ascii, codeStr);
        codes[i].data = (char)ascii;
        strcpy(codes[i].code, codeStr);
    }
    
    // Skip to data section
    char line[100];
    do {
        fgets(line, sizeof(line), in);
    } while (strcmp(line, "BEGIN_DATA\n") != 0);
    
    // Read and decode binary data
    FILE *out = fopen(outputFile, "w");
    char buffer[10000] = "";
    int bufferLen = 0;
    int c;
    
    while ((c = fgetc(in)) != EOF && c != '\n') {
        if (c == '0' || c == '1') {
            buffer[bufferLen++] = c;
        }
    }
    buffer[bufferLen] = '\0';
    
    // Decode using codes
    int pos = 0;
    while (pos < bufferLen) {
        int matched = 0;
        for (int len = 1; len <= bufferLen - pos; len++) {
            char substring[MAX_CODE_LENGTH];
            strncpy(substring, &buffer[pos], len);
            substring[len] = '\0';
            
            for (int j = 0; j < codeCount; j++) {
                if (strcmp(codes[j].code, substring) == 0) {
                    fputc(codes[j].data, out);
                    pos += len;
                    matched = 1;
                    break;
                }
            }
            if (matched) break;
        }
        if (!matched) pos++;
    }
    
    fclose(in);
    fclose(out);
    printf("File decoded successfully to '%s'\n", outputFile);
}

// Free Huffman tree
void freeTree(Node *root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}
