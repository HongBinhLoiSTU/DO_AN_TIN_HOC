#include "Reader.h"
#include <iostream>
#include <cstring>

using namespace std;

ReaderNode::ReaderNode(Reader r) {
    data = r;
    next = nullptr;
}

ReaderHashTable::ReaderHashTable() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        buckets[i] = nullptr;
    }
}

ReaderHashTable::~ReaderHashTable() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        ReaderNode* current = buckets[i];
        while (current != nullptr) {
            ReaderNode* temp = current;
            current = current->next;
            delete temp;
        }
    }
}

// Hàm băm tính tổng mã ASCII của chuỗi CMND/CCCD
int ReaderHashTable::hashFunction(const char* cmnd) {
    int sum = 0;
    for (int i = 0; cmnd[i] != '\0'; i++) {
        sum += cmnd[i];
    }
    return sum % TABLE_SIZE;
}

// Thêm độc giả vào bảng băm (Xử lý va chạm bằng Separate Chaining)
bool ReaderHashTable::insertReader(Reader r) {
    int index = hashFunction(r.cmnd);

    // Kiểm tra xem CMND đã tồn tại chưa để tránh trùng lặp
    ReaderNode* current = buckets[index];
    while (current != nullptr) {
        if (strcmp(current->data.cmnd, r.cmnd) == 0) {
            return false; // Đã tồn tại
        }
        current = current->next;
    }

    // Thêm node mới vào đầu danh sách liên kết tại bucket[index]
    ReaderNode* newNode = new ReaderNode(r);
    newNode->next = buckets[index];
    buckets[index] = newNode;
    return true;
}

// Tìm kiếm độc giả theo số CCCD/CMND
ReaderNode* ReaderHashTable::searchReader(const char* cmnd) {
    int index = hashFunction(cmnd);
    ReaderNode* current = buckets[index];
    while (current != nullptr) {
        if (strcmp(current->data.cmnd, cmnd) == 0) {
            return current;
        }
        current = current->next;
    }
    return nullptr; // Không tìm thấy
}

// In toàn bộ danh sách độc giả trong bảng băm
void ReaderHashTable::printTable() {
    bool isEmpty = true;
    for (int i = 0; i < TABLE_SIZE; i++) {
        ReaderNode* current = buckets[i];
        while (current != nullptr) {
            isEmpty = false;
            cout << "CCCD: " << current->data.cmnd
                << " | Ho ten: " << current->data.name
                << " | Email: " << current->data.email << "\n";
            current = current->next;
        }
    }
    if (isEmpty) {
        cout << "(Bang bam doc gia dang trong)\n";
    }
}



// 1. Tìm kiếm tuần tự trên toàn bộ bảng băm (duyệt qua tất cả bucket)
ReaderNode* ReaderHashTable::sequentialSearch(const char* cmnd, int& steps) {
    steps = 0;
    for (int i = 0; i < TABLE_SIZE; i++) {
        ReaderNode* current = buckets[i];
        while (current != nullptr) {
            steps++;
            if (strcmp(current->data.cmnd, cmnd) == 0) {
                return current;
            }
            current = current->next;
        }
    }
    return nullptr;
}

// 2. Tìm kiếm bằng Hash Table (vào thẳng bucket) kèm đếm số bước
ReaderNode* ReaderHashTable::hashSearchWithSteps(const char* cmnd, int& steps) {
    steps = 0;
    int index = hashFunction(cmnd);
    ReaderNode* current = buckets[index];
    while (current != nullptr) {
        steps++;
        if (strcmp(current->data.cmnd, cmnd) == 0) {
            return current;
        }
        current = current->next;
    }
    return nullptr;
}

// 3. Hàm so sánh hiệu năng trực quan in ra màn hình
void ReaderHashTable::compareSearch(const char* cmnd) {
    int seqSteps = 0, hashSteps = 0;

    // Đo thời gian tìm kiếm tuần tự
    clock_t startSeq = clock();
    ReaderNode* foundSeq = sequentialSearch(cmnd, seqSteps);
    clock_t endSeq = clock();
    double timeSeq = (double)(endSeq - startSeq) / CLOCKS_PER_SEC * 1000; // Đổi sang mili-giây

    // Đo thời gian tìm kiếm bằng Hash Table
    clock_t startHash = clock();
    ReaderNode* foundHash = hashSearchWithSteps(cmnd, hashSteps);
    clock_t endHash = clock();
    double timeHash = (double)(endHash - startHash) / CLOCKS_PER_SEC * 1000; // Đổi sang mili-giây

    cout << "\n========================================\n";
    cout << "    SO SANH HIEU NANG TIM KIEM DOC GIA    \n";
    cout << "========================================\n";
    if (foundSeq != nullptr) {
        cout << "-> Tim thay doc gia: " << foundSeq->data.name << "\n";
    }
    else {
        cout << "-> Khong tim thay doc gia co CCCD: " << cmnd << "\n";
    }
    cout << "----------------------------------------\n";
    cout << "1. Tim kiem tuan tu (Sequential Search):\n";
    cout << "   - So phep so sanh (steps): " << seqSteps << "\n";
    cout << "   - Thoi gian xu ly: " << timeSeq << " ms\n\n";
    cout << "2. Tim kiem bang Bang bam (Hash Table):\n";
    cout << "   - So phep so sanh (steps): " << hashSteps << "\n";
    cout << "   - Thoi gian xu ly: " << timeHash << " ms\n";
    cout << "========================================\n";
}


void ReaderHashTable::saveToFile(const std::string& filename) {
    std::ofstream outFile(filename, std::ios::binary);
    if (!outFile) return;
    for (int i = 0; i < TABLE_SIZE; i++) {
        ReaderNode* current = buckets[i];
        while (current != nullptr) {
            outFile.write(reinterpret_cast<char*>(&(current->data)), sizeof(Reader));
            current = current->next;
        }
    }
    outFile.close();
}

void ReaderHashTable::loadFromFile(const std::string& filename) {
    std::ifstream inFile(filename, std::ios::binary);
    if (!inFile) return;
    Reader r;
    while (inFile.read(reinterpret_cast<char*>(&r), sizeof(Reader))) {
        insertReader(r);
    }
    inFile.close();
}