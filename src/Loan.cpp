#include "Loan.h"
#include <iostream>
#include <cstring>

using namespace std;

LoanNode::LoanNode(Loan l) {
    data = l;
    next = nullptr;
}

LoanList::LoanList() {
    head = nullptr;
}

LoanList::~LoanList() {
    LoanNode* current = head;
    while (current != nullptr) {
        LoanNode* temp = current;
        current = current->next;
        delete temp;
    }
}

// Thêm phiếu mượn mới vào đầu danh sách liên kết
void LoanList::addLoan(Loan l) {
    LoanNode* newNode = new LoanNode(l);
    newNode->next = head;
    head = newNode;
}

// Tìm kiếm phiếu mượn theo Mã phiếu (loanId)
LoanNode* LoanList::searchLoan(const char* loanId) {
    LoanNode* current = head;
    while (current != nullptr) {
        if (strcmp(current->data.id, loanId) == 0) {
            return current;
        }
        current = current->next;
    }
    return nullptr;
}

// In toàn bộ danh sách phiếu mượn
void LoanList::printLoans() {
    LoanNode* current = head;
    if (current == nullptr) {
        cout << "(Danh sach phieu mượn đang trong)\n";
        return;
    }
    while (current != nullptr) {
        cout << "Ma phieu: " << current->data.id
            << " | CCCD: " << current->data.cmnd
            << " | ISBN: " << current->data.isbn
            << " | Ngay muon: " << current->data.borrowDate
            << " | Trang thai: " << (current->data.returned ? "Da tra" : "Chua tra") << "\n";
        current = current->next;
    }
}



void LoanList::saveToFile(const std::string& filename) {
    std::ofstream outFile(filename, std::ios::binary);
    if (!outFile) return;
    LoanNode* current = head;
    while (current != nullptr) {
        outFile.write(reinterpret_cast<char*>(&(current->data)), sizeof(Loan));
        current = current->next;
    }
    outFile.close();
}

void LoanList::loadFromFile(const std::string& filename) {
    std::ifstream inFile(filename, std::ios::binary);
    if (!inFile) return;
    Loan l;
    while (inFile.read(reinterpret_cast<char*>(&l), sizeof(Loan))) {
        addLoan(l);
    }
    inFile.close();
}