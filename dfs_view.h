#ifndef DFS_VIEW_H
#define DFS_VIEW_H

#include "tree_adt.h"

// Menampilkan seluruh hierarki pohon secara mendalam (Depth-First)
inline void displayTreeDFS(TaxonNode* node, int depth = 0) {
    if (node == NULL) return;

    for (int i = 0; i < depth; i++) {
        cout << "   ";
    }
    cout << "|-- [" << node->string_tingkat << "] " << node->string_nama << endl;

    displayTreeDFS(node->firstChild, depth + 1);
    displayTreeDFS(node->nextSibling, depth);
}

// Helper rekursif untuk mengumpulkan simpul tingkat "Spesies" pada sub-pohon
inline void collectSpeciesDFS(TaxonNode* node, int& count) {
    if (node == NULL) return;

    if (toLowerStr(node->string_tingkat) == "spesies") {
        count++;
        cout << "  " << count << ". " << node->string_nama
             << " (Genus: " << (node->parent ? node->parent->string_nama : "-") << ")" << endl;
    }

    collectSpeciesDFS(node->firstChild, count);
    collectSpeciesDFS(node->nextSibling, count);
}

// Fitur pencarian & filter spesies menggunakan DFS
inline void searchSpeciesDFS(TaxonNode* root, string kategori) {
    TaxonNode* target = findNode(root, kategori);
    if (target == NULL) {
        cout << "[!] Kategori taksonomi \"" << kategori << "\" tidak ditemukan.\n";
        return;
    }

    cout << "\n=== HASIL FILTER SPESIES (DFS) UNTUK [" << target->string_tingkat
         << ": " << target->string_nama << "] ===\n";

    int count = 0;
    if (toLowerStr(target->string_tingkat) == "spesies") {
        count = 1;
        cout << "  1. " << target->string_nama << endl;
    } else {
        // Mulai dari firstChild agar tidak merembet ke nextSibling kategori induk
        collectSpeciesDFS(target->firstChild, count);
    }

    if (count == 0) {
        cout << "  (Belum ada data spesies di bawah kategori ini)\n";
    } else {
        cout << "Total ditemukan: " << count << " spesies.\n";
    }
}

#endif