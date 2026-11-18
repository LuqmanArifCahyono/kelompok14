#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>
#include "tree_adt.h"

using namespace std;

const string TINGKAT_TAKSON[7] = {
    "Kingdom", "Filum", "Kelas", "Ordo", "Famili", "Genus", "Spesies"
};

// Helper mengubah string ke huruf kecil untuk perbandingan case-insensitive
string toLowerStr(string s) {
    for (size_t i = 0; i < s.length(); i++) {
        s[i] = tolower(static_cast<unsigned char>(s[i]));
    }
    return s;
}

// Helper menghapus tanda kutip ganda di awal/akhir cell CSV (jika ada)
string stripQuotes(string s) {
    if (s.size() >= 2 && s[0] == '"' && s[s.size() - 1] == '"') {
        return s.substr(1, s.size() - 2);
    }
    return s;
}

// Mencari anak langsung dari parent agar tidak membuat node duplikat
TaxonNode* findDirectChild(TaxonNode* parent, const string& nama) {
    if (parent == nullptr) return nullptr;
    TaxonNode* child = parent->firstChild;
    while (child != nullptr) {
        if (toLowerStr(child->nama) == toLowerStr(nama)) {
            return child;
        }
        child = child->nextSibling;
    }
    return nullptr;
}

// Menyisipkan 1 jalur taksonomi lengkap (7 tingkat) menggunakan createNode & insertChild dari tree_adt.h
void insertTaxonPath(TaxonNode*& root, const string data[7], int& totalNode) {
    TaxonNode* current = nullptr;

    // Level 0: Kingdom (Root atau sibling dari Root)
    if (root == nullptr) {
        root = createNode(TINGKAT_TAKSON[0], data[0]);
        totalNode++;
        current = root;
    } else {
        TaxonNode* temp = root;
        TaxonNode* prev = nullptr;
        while (temp != nullptr) {
            if (toLowerStr(temp->nama) == toLowerStr(data[0])) {
                current = temp;
                break;
            }
            prev = temp;
            temp = temp->nextSibling;
        }
        if (current == nullptr) {
            current = createNode(TINGKAT_TAKSON[0], data[0]);
            totalNode++;
            prev->nextSibling = current;
        }
    }

    // Level 1 s.d. 6: Filum hingga Spesies
    for (int i = 1; i < 7; i++) {
        TaxonNode* existing = findDirectChild(current, data[i]);
        if (existing == nullptr) {
            TaxonNode* newChild = createNode(TINGKAT_TAKSON[i], data[i]);
            insertChild(current, newChild);
            totalNode++;
            current = newChild;
        } else {
            current = existing;
        }
    }
}

// Input jalur taksonomi baru dari pengguna via CLI (menggunakan createNode & insertChild)
void inputTaksonBaru(TaxonNode*& root, int& totalNode) {
    string data[7];
    cout << "\n--- Tambah Data Klasifikasi Baru ---\n";
    cin.ignore(1000, '\n');
    for (int i = 0; i < 7; i++) {
        cout << "Masukkan " << TINGKAT_TAKSON[i] << " : ";
        getline(cin, data[i]);
    }
    insertTaxonPath(root, data, totalNode);

    // Tampilkan verifikasi singkat dari node yang terhubung (tingkat, nama, parent)
    TaxonNode* curr = root;
    while (curr != nullptr && toLowerStr(curr->nama) != toLowerStr(data[0])) {
        curr = curr->nextSibling;
    }
    for (int i = 1; i < 7 && curr != nullptr; i++) {
        curr = findDirectChild(curr, data[i]);
    }
    if (curr != nullptr) {
        cout << "[OK] Simpul akhir tersimpan: [" << curr->tingkat << "] " << curr->nama
             << " (Parent: " << (curr->parent ? curr->parent->nama : "-") << ")\n";
    }
    cout << "[OK] Klasifikasi berhasil ditambahkan! Total simpul saat ini: "
         << totalNode << " node.\n";
}

// Menghapus seluruh pohon dari memori menggunakan deleteTree() dari tree_adt.h
void resetPohon(TaxonNode*& root, int& totalNode) {
    if (root == nullptr) {
        cout << "[INFO] Pohon sudah kosong.\n";
        return;
    }
    deleteTree(root);
    root = nullptr;
    totalNode = 0;
    cout << "[OK] Seluruh simpul pohon berhasil dihapus dari memori (deleteTree).\n";
}

// Memuat data awal dari file CSV ke dalam General Tree (LCRS)
bool loadFromCSV(TaxonNode*& root, const string& filename, int& totalBaris, int& totalNode) {
    ifstream file(filename.c_str());
    if (!file.is_open()) {
        return false;
    }

    string line;
    bool firstLine = true;
    while (getline(file, line)) {
        if (line.empty()) continue;
        if (!line.empty() && line[line.size() - 1] == '\r') {
            line.erase(line.size() - 1);
        }

        stringstream ss(line);
        string cell;
        string cols[7];
        int idx = 0;

        while (getline(ss, cell, ',') && idx < 7) {
            cols[idx++] = stripQuotes(cell);
        }

        if (idx == 7) {
            // Lewati baris pertama jika berupa header CSV
            if (firstLine && toLowerStr(cols[0]) == "kingdom") {
                firstLine = false;
                continue;
            }
            firstLine = false;
            insertTaxonPath(root, cols, totalNode);
            totalBaris++;
        }
    }
    file.close();
    return true;
}

int main() {
    TaxonNode* root = nullptr;
    const string CSV_FILE = "taxonomy.csv";
    int totalBaris = 0;
    int totalNode = 0;

    if (loadFromCSV(root, CSV_FILE, totalBaris, totalNode)) {
        cout << "[INFO] Berhasil memuat " << totalBaris << " baris data dari "
             << CSV_FILE << " (" << totalNode << " node dibuat).\n";
    } else {
        cout << "[!] File " << CSV_FILE << " tidak ditemukan.\n";
    }

    int pilihan;

    do {
        cout << "\n======================================================\n";
        cout << "  SISTEM KLASIFIKASI TAKSONOMI BIOLOGI (LCRS TREE ADT)\n";
        cout << "  Total Simpul Aktif: " << totalNode << " node\n";
        cout << "======================================================\n";
        cout << "1. Tambah Jalur Taksonomi Baru (createNode & insertChild)\n";
        cout << "2. Hapus / Reset Seluruh Pohon (deleteTree)\n";
        cout << "0. Keluar Program\n";
        cout << "Pilih menu [0-2]: ";
        cin >> pilihan;

        switch (pilihan) {
            case 1:
                inputTaksonBaru(root, totalNode);
                break;
            case 2:
                resetPohon(root, totalNode);
                break;
            case 0:
                deleteTree(root);
                root = nullptr;
                totalNode = 0;
                cout << "[INFO] Memori pohon dibersihkan (deleteTree). Program selesai.\n";
                break;
            default:
                cout << "[!] Pilihan menu tidak valid.\n";
        }
    } while (pilihan != 0);

    return 0;
}
