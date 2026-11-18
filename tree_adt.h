#ifndef TREE_ADT_H

#define TREE_ADT_H

#include <iostream>

#include <string>

using namespace std;

// Struktur Node untuk Pohon Taksonomi (Left-Child Right-Sibling)

struct TaxonNode {

string tingkat; // Kingdom, Filum, Kelas, Ordo, Famili, Genus, Spesies

string nama; // Contoh: Animalia, Chordata, Mammalia, Panthera leo

TaxonNode* parent; // Pointer ke induk klasifikasi

TaxonNode* firstChild; // Pointer ke anak pertama (sub-klasifikasi langsung)

TaxonNode* nextSibling; // Pointer ke saudara sejajar di tingkat yang sama

};

// 1. Fungsi Alokasi Node Baru

TaxonNode* createNode(string tingkat, string nama) {

TaxonNode* newNode = new TaxonNode;

newNode->tingkat = tingkat;

newNode->nama = nama;

newNode->parent = nullptr;

newNode->firstChild = nullptr;

newNode->nextSibling = nullptr;

return newNode;

}

// 2. Fungsi Menghubungkan Parent dengan Child (Insert Child)

void insertChild(TaxonNode* parent, TaxonNode* child) {

if (parent == nullptr || child == nullptr) return;

child->parent = parent;

// Jika parent belum memiliki anak sama sekali, jadikan sebagai firstChild

if (parent->firstChild == nullptr) {

parent->firstChild = child;

}

// Jika sudah punya anak, telusuri nextSibling hingga paling ujung

else {

TaxonNode* temp = parent->firstChild;

while (temp->nextSibling != nullptr) {

temp = temp->nextSibling;

}

temp->nextSibling = child;

}

}

// 3. Fungsi Dealokasi Memori (Post-Order Traversal)

void deleteTree(TaxonNode* root) {

if (root == nullptr) return;

// Hapus seluruh cabang anak dan saudara terlebih dahulu secara rekursif

deleteTree(root->firstChild);

deleteTree(root->nextSibling);

// Bebaskan memori node saat ini

delete root;

}

#endif