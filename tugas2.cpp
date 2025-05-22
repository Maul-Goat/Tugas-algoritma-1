#include <iostream>
#include <string>
using namespace std;

struct Pesanan {
    string nama;
    string jenisRoti;
    double totalHarga;
};

struct Node { // queue & stack
    Pesanan data;
    Node* next;
};

Node* front = NULL; 
Node* rear = NULL;
Node* top = NULL;

void enqueue(string nama, string jenisRoti, double totalHarga) { // tambah pesanan
    Node* newNode = new Node;
    newNode->data.nama = nama;
    newNode->data.jenisRoti = jenisRoti;
    newNode->data.totalHarga = totalHarga;
    newNode->next = NULL;

    if (rear == NULL) {
        front = rear = newNode;
    } else {
        rear->next = newNode;
        rear = newNode;
    }
    cout << "Pesanan atas nama " << nama << " ditambahkan ke antrean.\n";
}

Pesanan dequeue() { // menghapus pesanan
    if (front == NULL) {
        cout << "Antrean kosong!" << endl;
    }
    Node* temp = front;
    Pesanan data = temp->data;
    front = front->next;
    if (front == NULL) {
        rear = NULL;
    }
    delete temp;
    return data;
}

void cancelLast() { // batalkan pesanan
    if (rear == NULL) {
        cout << "Antrean kosong, tidak ada pesanan untuk dibatalkan!\n";
        return;
    }
    if (front == rear) {
        delete front;
        front = rear = NULL;
        cout << "Pesanan terakhir dibatalkan.\n";
        return;
    }
    Node* current = front;
    while (current->next != rear) {
        current = current->next;
    }
    delete rear;
    rear = current;
    rear->next = NULL;
    cout << "Pesanan terakhir dibatalkan.\n";
}

void displayQueue() { // tampilkan pesanan
    if (front == NULL) {
        cout << "Antrean kosong.\n";
        return;
    }
    cout << "\nDaftar Pesanan dalam Antrean:\n";
    cout << "-----------------------------\n";
    Node* current = front;
    int index = 1;
    while (current != NULL) {
        cout << "Pelanggan NO " << index << "\nNama: " << current->data.nama
             << "\nJenis Roti: " << current->data.jenisRoti
             << "\nTotal Harga: Rp" << current->data.totalHarga << "\n\n";
        current = current->next;
        index++;
    }
    cout << "-----------------------------\n";
}

bool isQueueEmpty() { // cek queue kosong atau tidak
    return front == NULL;
}

void push(Pesanan pesanan) { // tambah pesanan
    Node* newNode = new Node;
    newNode->data = pesanan;
    newNode->next = top;
    top = newNode;
}

void displayHistory() { // tampil history
    if (top == NULL) {
        cout << "Riwayat pesanan kosong.\n";1
        return;
    }
    cout << "\nRiwayat Pesanan yang Sudah Dilayani:\n";
    cout << "------------------------------------\n";
    Node* current = top;
    int index = 1;
    while (current != NULL) {
        cout << "Pelanggan NO " << index << "\nNama: " << current->data.nama
             << "\nJenis Roti: " << current->data.jenisRoti
             << "\nTotal Harga: Rp" << current->data.totalHarga << "\n\n";
        current = current->next;
        index++;
    }
    cout << "------------------------------------\n";
}

int main() {
    int pilihan;
    string nama, jenisRoti;
    double totalHarga;

    do {
        cout << "=========================================================" << endl;
        cout << "|    Sistem Manajemen Pesanan Toko Roti \"Manis Lezat\"   |" << endl;
        cout << "=========================================================" << endl;
        cout << "| 1. Ambil Antrean                                      |" << endl;
        cout << "| 2. Layani Pembeli                                     |" << endl;
        cout << "| 3. Tampilkan Pesanan                                  |" << endl;
        cout << "| 4. Batalkan Pesanan                                   |" << endl;
        cout << "| 5. Tampilkan History Pesanan                          |" << endl;
        cout << "| 6. Keluar                                             |" << endl;
        cout << "=========================================================\n";
        cout << "Pilih opsi: ";
        cin >> pilihan;
        cin.ignore(); 

        switch (pilihan) {
            case 1: 
                cout << "Masukkan nama pelanggan: ";
                getline(cin, nama);
                cout << "Masukkan jenis roti: ";
                getline(cin, jenisRoti);
                cout << "Masukkan total harga: Rp";
                cin >> totalHarga;
                enqueue(nama, jenisRoti, totalHarga);
                break;

            case 2: 
                if (!isQueueEmpty()) {
                    Pesanan pesanan = dequeue();
                    push(pesanan);
                    cout << "Pesanan atas nama " << pesanan.nama << " telah dilayani.\n";
                } else {
                    cout << "Antrean kosong!\n";
                }
                break;

            case 3: 
                displayQueue();
                break;

            case 4: 
                cancelLast();
                break;

            case 5: 
                displayHistory();
                break;

            case 6: 
                cout << "Terima kasih telah menggunakan aplikasi ini!\n";
                break;

            default:
                cout << "Pilihan tidak valid!\n";
        }
    } while (pilihan != 6);

    while (front != NULL) {
        Node* temp = front;
        front = front->next;
        delete temp;
    }
    while (top != NULL) {
        Node* temp = top;
        top = top->next;
        delete temp;
    }

    return 0;
}