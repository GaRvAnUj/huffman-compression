#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TREE_CODES 256
#define MAX_CODE_LENGTH 256

// Huffman Tree Node
typedef struct Node {
    char data;
    int frequency;
    struct Node *left;
    struct Node *right;
} Node;

// Huffman Code Table
typedef struct {
    char data;
    char code[MAX_CODE_LENGTH];
} CodeEntry;

// Function declarations
Node* createNode(char data, int frequency);
Node* buildHuffmanTree(char *text);
void generateCodes(Node *root, CodeEntry *codes, char *code, int codeLen, int *codeCount);
void printCodes(CodeEntry *codes, int codeCount);
void encodeFile(char *inputFile, char *outputFile);
void decodeFile(char *inputFile, char *outputFile);
void freeTree(Node *root);
int countUniqueChars(char *text);

#endif
