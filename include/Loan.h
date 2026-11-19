#pragma once
#include "ThuVien.h"
#include <fstream>

struct LoanNode {
    Loan data;
    LoanNode* next;

    LoanNode(Loan l);
};

class LoanList {
private:
    LoanNode* head;

public:
    LoanList();
    ~LoanList();
    void addLoan(Loan l);
    LoanNode* searchLoan(const char* loanId);
    void printLoans();


    void saveToFile(const std::string& filename);
    void loadFromFile(const std::string& filename);
};