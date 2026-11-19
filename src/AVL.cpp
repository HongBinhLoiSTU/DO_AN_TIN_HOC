#include "AVL.h"
#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;

// Định nghĩa constructor cho AVLNode
AVLNode::AVLNode(Book b) {
    data = b;
    left = nullptr;
    right = nullptr;
    height = 1;
}

int height(AVLNode* N) {
    return N == nullptr ? 0 : N->height;
}

AVLNode* rightRotate(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

AVLNode* leftRotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

int getBalance(AVLNode* N) {
    return N == nullptr ? 0 : height(N->left) - height(N->right);
}

AVLNode* insertAVL(AVLNode* node, Book book) {
    if (node == nullptr) return new AVLNode(book);

    if (strcmp(book.isbn, node->data.isbn) < 0)
        node->left = insertAVL(node->left, book);
    else if (strcmp(book.isbn, node->data.isbn) > 0)
        node->right = insertAVL(node->right, book);
    else
        return node; // Không cho phép trùng ISBN

    node->height = 1 + max(height(node->left), height(node->right));
    int balance = getBalance(node);

    // Left Left Case
    if (balance > 1 && strcmp(book.isbn, node->left->data.isbn) < 0)
        return rightRotate(node);

    // Right Right Case
    if (balance < -1 && strcmp(book.isbn, node->right->data.isbn) > 0)
        return leftRotate(node);

    // Left Right Case
    if (balance > 1 && strcmp(book.isbn, node->left->data.isbn) > 0) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Right Left Case
    if (balance < -1 && strcmp(book.isbn, node->right->data.isbn) < 0) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

AVLNode* searchAVL(AVLNode* root, const char* isbn) {
    if (root == nullptr || strcmp(root->data.isbn, isbn) == 0)
        return root;
    if (strcmp(isbn, root->data.isbn) < 0)
        return searchAVL(root->left, isbn);
    return searchAVL(root->right, isbn);
}

void inorderAVL(AVLNode* root) {
    if (root != nullptr) {
        inorderAVL(root->left);
        cout << "ISBN: " << root->data.isbn
            << " | Ten: " << root->data.title
            << " | So luong: " << root->data.available << "/" << root->data.quantity << "\n";
        inorderAVL(root->right);
    }
}

// Tìm node có giá trị nhỏ nhất (dùng cho việc xóa node có 2 con)
AVLNode* minValueNode(AVLNode* node) {
    AVLNode* current = node;
    while (current->left != nullptr)
        current = current->left;
    return current;
}

// Hàm xóa một cuốn sách khỏi cây AVL dựa vào ISBN
AVLNode* deleteAVL(AVLNode* root, const char* isbn) {
    // 1. Thực hiện xóa chuẩn của BST
    if (root == nullptr) return root;

    int cmp = strcmp(isbn, root->data.isbn);
    if (cmp < 0)
        root->left = deleteAVL(root->left, isbn);
    else if (cmp > 0)
        root->right = deleteAVL(root->right, isbn);
    else {
        // Node có 1 con hoặc không có con nào
        if ((root->left == nullptr) || (root->right == nullptr)) {
            AVLNode* temp = root->left ? root->left : root->right;

            // Trường hợp không có con
            if (temp == nullptr) {
                temp = root;
                root = nullptr;
            }
            else // Trường hợp có 1 con
                *root = *temp; // Sao chép dữ liệu của con lên node hiện tại
            delete temp;
        }
        else {
            // Node có 2 con: Lấy phần tử nhỏ nhất ở cây con bên phải (inorder successor)
            AVLNode* temp = minValueNode(root->right);

            // Copy dữ liệu sang node hiện tại
            root->data = temp->data;

            // Xóa node thế chỗ đó đi
            root->right = deleteAVL(root->right, temp->data.isbn);
        }
    }

    if (root == nullptr) return root;

    // 2. Cập nhật lại chiều cao của node hiện tại
    root->height = 1 + max(height(root->left), height(root->right));

    // 3. Kiểm tra độ cân bằng và xoay cân bằng lại cây
    int balance = getBalance(root);

    // Left Left Case
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    // Left Right Case
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // Right Right Case
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    // Right Left Case
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}


// Lưu cây AVL xuống file nhị phân (Duyệt tiền thứ tự Pre-order)
void saveAVLToFile(AVLNode* root, std::ofstream& outFile) {
    if (root == nullptr) return;
    outFile.write(reinterpret_cast<char*>(&(root->data)), sizeof(Book));
    saveAVLToFile(root->left, outFile);
    saveAVLToFile(root->right, outFile);
}

// Đọc cây AVL từ file nhị phân khi khởi động
AVLNode* loadAVLFromFile(const std::string& filename, AVLNode* root) {
    std::ifstream inFile(filename, std::ios::binary);
    if (!inFile) return root; // Nếu chưa có file thì bỏ qua
    Book b;
    while (inFile.read(reinterpret_cast<char*>(&b), sizeof(Book))) {
        root = insertAVL(root, b);
    }
    inFile.close();
    return root;
}