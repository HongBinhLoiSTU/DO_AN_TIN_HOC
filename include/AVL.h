#pragma once
#include"ThuVien.h"
#include <fstream>
struct AVLNode {
    Book data;
    AVLNode* left;
    AVLNode* right;
    int height;

    AVLNode(Book b); // Khai báo constructor
};

// Khai báo nguyên mẫu các hàm
int height(AVLNode* N);
AVLNode* rightRotate(AVLNode* y);
AVLNode* leftRotate(AVLNode* x);
int getBalance(AVLNode* N);
AVLNode* insertAVL(AVLNode* node, Book book);
AVLNode* searchAVL(AVLNode* root, const char* isbn);
void inorderAVL(AVLNode* root);
AVLNode* minValueNode(AVLNode* node);
AVLNode* deleteAVL(AVLNode* root, const char* isbn);

// Nguyên mẫu hàm file I/O
void saveAVLToFile(AVLNode* root, std::ofstream& outFile);
AVLNode* loadAVLFromFile(const std::string& filename, AVLNode* root);