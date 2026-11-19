#pragma once
#include <iostream>
#include <string>
#include <cstring>

using namespace std;

// 1. Struct Thực thể Sách (Book)
struct Book {
    char isbn[16];       // Mã ISBN (Khoá chính cho AVLTree)
    char title[96];      // Tên sách
    char author[64];     // Tác giả
    char genre[32];      // Thể loại
    int year;            // Năm xuất bản
    int quantity;        // Tổng số lượng sách
    int available;       // Số lượng hiện có sẵn trong thư viện
};

// 2. Struct Thực thể Độc giả (Reader)
struct Reader {
    char cmnd[16];       // CMND/CCCD (Khoá chính cho HashTable)
    char name[64];       // Họ và tên
    char email[64];      // Email liên hệ
    char phone[16];      // Số điện thoại
    int cardType;        // Loại thẻ độc giả (0: SV, 1: Giảng viên, ...)
};

// 3. Struct Thực thể Phiếu mượn (Loan)
struct Loan {
    char id[12];         // Mã phiếu mượn
    char cmnd[16];       // Mã độc giả mượn
    char isbn[16];       // Mã sách mượn
    char borrowDate[11]; // Ngày mượn (YYYY-MM-DD)
    char dueDate[11];    // Ngày hẹn trả (YYYY-MM-DD)
    char returnDate[11]; // Ngày thực tế trả
    bool returned;       // Đã trả sách chưa (true/false)
    double fine;         // Tiền phạt (nếu quá hạn)
};

// 4. Struct Thực thể Lịch sử Giao dịch (Transaction)
struct Transaction {
    int type;            // Loại giao dịch (1: Mượn, 2: Trả, 3: Phạt...)
    char loanId[12];     // Mã phiếu mượn liên quan
    char cmnd[16];       // CMND độc giả
    char isbn[16];       // Mã sách
    char date[11];       // Ngày thực hiện giao dịch
    double amount;       // Số tiền (nếu có thanh toán phạt)
};