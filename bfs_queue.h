#ifndef BFS_QUEUE_H
#define BFS_QUEUE_H

#include "tree_adt.h"

struct QueueNode {
    TaxonNode* treeNode;
    int level;
    QueueNode* next;
};

struct TaxonQueue {
    QueueNode* front;
    QueueNode* rear;
};

inline void initQueue(TaxonQueue& q) {
    q.front = NULL;
    q.rear = NULL;
}

inline bool isEmpty(const TaxonQueue& q) {
    return q.front == NULL;
}

inline void enqueue(TaxonQueue& q, TaxonNode* node, int level = 0) {
    QueueNode* newQNode = new QueueNode;
    newQNode->treeNode = node;
    newQNode->level = level;
    newQNode->next = NULL;

    if (isEmpty(q)) {
        q.front = newQNode;
        q.rear = newQNode;
    } else {
        q.rear->next = newQNode;
        q.rear = newQNode;
    }
}

inline TaxonNode* dequeue(TaxonQueue& q, int& levelOut) {
    if (isEmpty(q)) return NULL;
    QueueNode* temp = q.front;
    TaxonNode* data = temp->treeNode;
    levelOut = temp->level;

    q.front = q.front->next;
    if (q.front == NULL) {
        q.rear = NULL;
    }
    delete temp;
    return data;
}

// Menampilkan seluruh isi pohon secara melebar per tingkat (Level-Order BFS)
inline void displayLevelBFS(TaxonNode* root) {
    if (root == NULL) {
        cout << "[!] Pohon taksonomi masih kosong.\n";
        return;
    }

    TaxonQueue q;
    initQueue(q);

    // Masukkan seluruh root (jika ada lebih dari 1 Kingdom)
    TaxonNode* currRoot = root;
    while (currRoot != NULL) {
        enqueue(q, currRoot, 0);
        currRoot = currRoot->nextSibling;
    }

    int currentLevel = -1;
    while (!isEmpty(q)) {
        int lvl = 0;
        TaxonNode* curr = dequeue(q, lvl);

        if (lvl != currentLevel) {
            currentLevel = lvl;
            cout << "\n[Level " << currentLevel << " - " << curr->string_tingkat << "]\n";
        }
        cout << "  -> " << curr->string_nama;
        if (curr->parent != NULL) {
            cout << " (Induk: " << curr->parent->string_nama << ")";
        }
        cout << endl;

        // Masukkan semua anak langsung ke dalam antrean
        TaxonNode* child = curr->firstChild;
        while (child != NULL) {
            enqueue(q, child, lvl + 1);
            child = child->nextSibling;
        }
    }
}

// Fitur pencarian & filter spesies menggunakan BFS
inline void searchSpeciesBFS(TaxonNode* root, string kategori) {
    TaxonNode* target = findNode(root, kategori);
    if (target == NULL) {
        cout << "[!] Kategori taksonomi \"" << kategori << "\" tidak ditemukan.\n";
        return;
    }

    cout << "\n=== HASIL FILTER SPESIES (BFS) UNTUK [" << target->string_tingkat
         << ": " << target->string_nama << "] ===\n";

    TaxonQueue q;
    initQueue(q);
    enqueue(q, target, 0);

    int count = 0;
    while (!isEmpty(q)) {
        int lvl = 0;
        TaxonNode* curr = dequeue(q, lvl);

        if (toLowerStr(curr->string_tingkat) == "spesies") {
            count++;
            cout << "  " << count << ". " << curr->string_nama
                 << " (Genus: " << (curr->parent ? curr->parent->string_nama : "-") << ")" << endl;
        }

        TaxonNode* child = curr->firstChild;
        while (child != NULL) {
            enqueue(q, child, lvl + 1);
            child = child->nextSibling;
        }
    }

    if (count == 0) {
        cout << "  (Belum ada data spesies di bawah kategori ini)\n";
    } else {
        cout << "Total ditemukan: " << count << " spesies.\n";
    }
}

#endif