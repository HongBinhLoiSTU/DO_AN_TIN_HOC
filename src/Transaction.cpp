#include "Transaction.h"
#include <iostream>

using namespace std;

TransactionArray::TransactionArray(int initCapacity) {
    capacity = initCapacity;
    count = 0;
    arr = new Transaction[capacity];
}

TransactionArray::~TransactionArray() {
    delete[] arr;
}

// Cơ chế cốt lõi của Dynamic Array: Cấp phát mảng mới gấp đôi sức chứa và chép dữ liệu sang
void TransactionArray::resize() {
    int newCapacity = capacity * 2;
    Transaction* newArr = new Transaction[newCapacity];

    for (int i = 0; i < count; i++) {
        newArr[i] = arr[i];
    }

    delete[] arr;
    arr = newArr;
    capacity = newCapacity;
}

void TransactionArray::addTransaction(Transaction t) {
    if (count == capacity) {
        resize();
    }
    arr[count] = t;
    count++;
}

void TransactionArray::printTransactions() {
    if (count == 0) {
        cout << "(Chua co lich su giao dich nao)\n";
        return;
    }
    for (int i = 0; i < count; i++) {
        cout << "[" << i + 1 << "] Loai GD: " << (arr[i].type == 1 ? "Muon sach" : (arr[i].type == 2 ? "Tra sach" : "Phat"))
            << " | Ma phieu: " << arr[i].loanId
            << " | CCCD: " << arr[i].cmnd
            << " | ISBN: " << arr[i].isbn
            << " | Ngay: " << arr[i].date
            << " | So tien: " << arr[i].amount << " VND\n";
    }
}