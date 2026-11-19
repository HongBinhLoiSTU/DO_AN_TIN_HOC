#pragma once
#include "ThuVien.h"
#include <fstream>

const int TABLE_SIZE = 100; // Kích thước bảng băm

struct ReaderNode {
    Reader data;
    ReaderNode* next;

    ReaderNode(Reader r);
};

class ReaderHashTable {
private:
    ReaderNode* buckets[TABLE_SIZE];
    int hashFunction(const char* cmnd);

public:
    ReaderHashTable();
    ~ReaderHashTable();
    bool insertReader(Reader r);
    ReaderNode* searchReader(const char* cmnd);
    void printTable();

    ReaderNode* sequentialSearch(const char* cmnd, int& steps);
    ReaderNode* hashSearchWithSteps(const char* cmnd, int& steps);
    void compareSearch(const char* cmnd);

    
    void saveToFile(const std::string& filename);
    void loadFromFile(const std::string& filename);
};