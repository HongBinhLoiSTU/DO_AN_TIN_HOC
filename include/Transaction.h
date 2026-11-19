#pragma once
#include "ThuVien.h"

class TransactionArray {
private:
    Transaction* arr;
    int capacity;
    int count;

    void resize(); // Hàm tự động nhân đôi kích thước mảng động khi đầy

public:
    TransactionArray(int initCapacity = 10);
    ~TransactionArray();
    void addTransaction(Transaction t);
    void printTransactions();
};