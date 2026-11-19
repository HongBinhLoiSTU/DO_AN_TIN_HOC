#include <iostream>
#include <fstream>
#include <cstring>
#include "ThuVien.h"
#include "AVL.h"
#include "Reader.h"
#include "Loan.h"
#include "Transaction.h"

using namespace std;

int main() {
    system("chcp 65017 > nul");

    AVLNode* bookRoot = nullptr;
    ReaderHashTable readerTable;
    LoanList loanList;
    TransactionArray transHistory;

    // --- TỰ ĐỘNG LOAD DỮ LIỆU TỪ FILE KHI MỞ APP ---
    bookRoot = loadAVLFromFile("books.dat", bookRoot);
    readerTable.loadFromFile("readers.dat");
    loanList.loadFromFile("loans.dat");

    int choice;

    do {
        cout << "\n======================================\n";
        cout << "     QUAN LY THU VIEN   \n";
        cout << "======================================\n";
        cout << "--- QUAN LY SACH (AVL Tree) ---\n";
        cout << "1. Them sach moi\n";
        cout << "2. In danh muc sach\n";
        cout << "3. Xoa sach theo ISBN\n";
        cout << "4. Tim kiem sach theo ISBN\n";
        cout << "--- QUAN LY DOC GIA (Hash Table) ---\n";
        cout << "5. Them doc gia moi\n";
        cout << "6. In danh sach doc gia\n";
        cout << "7. Tim kiem doc gia theo CCCD\n";
        cout << "8. So sanh Hash Table vs Tim tuan tu\n";
        cout << "--- PHIEU MUON & GIAO DICH ---\n";
        cout << "9. Lap phieu muon sach\n";
        cout << "10. In danh sach phieu muon\n";
        cout << "11. Tim kiem phieu muon theo ma\n";
        cout << "12. Xem lich su giao dich (Dynamic Array)\n";
        cout << "0. Thoat va Luu du lieu\n";
        cout << "Nhap lua chon: ";
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            Book b;
            cout << "Nhap ma ISBN: "; cin.getline(b.isbn, 16);
            cout << "Nhap ten sach: "; cin.getline(b.title, 96);
            cout << "Nhap so luong: "; cin >> b.quantity;
            b.available = b.quantity;
            bookRoot = insertAVL(bookRoot, b);
            cout << "==> Da them sach thanh cong!\n";
        }
        else if (choice == 2) {
            cout << "\n--- DANH MUC SACH ---\n";
            if (bookRoot == nullptr) cout << "(Trong)\n";
            else inorderAVL(bookRoot);
        }
        else if (choice == 3) {
            char isbn[16];
            cout << "Nhap ISBN sach can xoa: "; cin.getline(isbn, 16);
            bookRoot = deleteAVL(bookRoot, isbn);
            cout << "==> Da xu ly xoa sach!\n";
        }
        else if (choice == 4) {
            char isbn[16];
            cout << "Nhap ISBN can tim: "; cin.getline(isbn, 16);
            AVLNode* found = searchAVL(bookRoot, isbn);
            if (found) cout << "--> Tim thay: " << found->data.title << "\n";
            else cout << "--> Khong tim thay!\n";
        }
        else if (choice == 5) {
            Reader r;
            cout << "Nhap CCCD/CMND: "; cin.getline(r.cmnd, 16);
            cout << "Nhap ho va ten: "; cin.getline(r.name, 64);
            cout << "Nhap email: "; cin.getline(r.email, 64);
            cout << "Nhap so dien thoai: "; cin.getline(r.phone, 16);
            cout << "Nhap loai the (0: SV, 1: GV...): "; cin >> r.cardType;
            cin.ignore();

            if (readerTable.insertReader(r)) {
                cout << "==> Them doc gia thanh cong!\n";
            }
            else {
                cout << "==> Loi: CCCD nay da ton tai trong he thong!\n";
            }
        }
        else if (choice == 6) {
            cout << "\n--- DANH SACH DOC GIA ---\n";
            readerTable.printTable();
        }
        else if (choice == 7) {
            char cmnd[16];
            cout << "Nhap CCCD can tim: "; cin.getline(cmnd, 16);
            ReaderNode* found = readerTable.searchReader(cmnd);
            if (found) {
                cout << "--> Tim thay! Ho ten: " << found->data.name << "\n";
            }
            else {
                cout << "--> Khong tim thay doc gia nay!\n";
            }
        }
        else if (choice == 8) {
            char cmnd[16];
            cout << "Nhap CCCD can so sanh hieu nang: "; cin.getline(cmnd, 16);
            readerTable.compareSearch(cmnd);
        }
        else if (choice == 9) {
            Loan l;
            cout << "Nhap ma phieu muon: "; cin.getline(l.id, 12);
            cout << "Nhap CCCD doc gia muon: "; cin.getline(l.cmnd, 16);
            cout << "Nhap ma ISBN sach muon: "; cin.getline(l.isbn, 16);
            cout << "Nhap ngay muon (YYYY-MM-DD): "; cin.getline(l.borrowDate, 11);
            cout << "Nhap ngay hen tra (YYYY-MM-DD): "; cin.getline(l.dueDate, 11);
            strcpy_s(l.returnDate, "");
            l.returned = false;
            l.fine = 0.0;

            loanList.addLoan(l);

            Transaction t;
            t.type = 1;
            strcpy_s(t.loanId, l.id);
            strcpy_s(t.cmnd, l.cmnd);
            strcpy_s(t.isbn, l.isbn);
            strcpy_s(t.date, l.borrowDate);
            t.amount = 0.0;
            transHistory.addTransaction(t);

            cout << "==> Lap phieu muon thanh cong!\n";
        }
        else if (choice == 10) {
            cout << "\n--- DANH SACH PHIEU MUON ---\n";
            loanList.printLoans();
        }
        else if (choice == 11) {
            char loanId[12];
            cout << "Nhap ma phieu muon can tim: "; cin.getline(loanId, 12);
            LoanNode* found = loanList.searchLoan(loanId);
            if (found) {
                cout << "--> Tim thay phieu! ISBN: " << found->data.isbn
                    << " | Trang thai: " << (found->data.returned ? "Da tra" : "Chua tra") << "\n";
            }
            else {
                cout << "--> Khong tim thay phieu muon nay!\n";
            }
        }
        else if (choice == 12) {
            cout << "\n--- LICH SU GIAO DICH ---\n";
            transHistory.printTransactions();
        }
        else if (choice == 0) {
            // --- TỰ ĐỘNG LƯU VÀO FILE KHI THOÁT ---
            ofstream bookFile("books.dat", ios::binary);
            saveAVLToFile(bookRoot, bookFile);
            bookFile.close();

            readerTable.saveToFile("readers.dat");
            loanList.saveToFile("loans.dat");

            cout << "==> Da luu tat ca du lieu xuong file thanh cong. Tam biet!\n";
        }
    } while (choice != 0);

    return 0;
}