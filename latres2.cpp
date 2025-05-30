#include <iostream>
#include <string>
using namespace std;

enum Status { TERSEDIA, DALAM_ANTREAN, SEDANG_DIPUTAR }; 

// Node BST
struct Video {
    string judul;
    int durasi;
    Status status;
    Video* left;
    Video* right;
};

// Node stack, queue, history
struct Node {
    string action, judul;
    int durasi;
    Status oldStatus;
    Video* video;
    Node* next;
};

Video* root = nullptr;
Node* queueFront = nullptr;
Node* queueRear = nullptr;
Node* stackTop = nullptr;
Node* historyHead = nullptr;
Video* currentlyPlaying = nullptr;

// Node vidio baru
Video* createVideo(string judul, int durasi) {
    Video* v = new Video;
    v->judul = judul; v->durasi = durasi; v->status = TERSEDIA;
    v->left = v->right = nullptr;
    return v;
}

// Insert ke BST
Video* insertVideo(Video* node, string judul, int durasi) {
    if (!node) return createVideo(judul, durasi);
    if (judul < node->judul) node->left = insertVideo(node->left, judul, durasi);
    else if (judul > node->judul) node->right = insertVideo(node->right, judul, durasi);
    return node;
}

Video* searchVideo(Video* node, string judul) {
    if (!node || node->judul == judul) return node;
    return judul < node->judul ? searchVideo(node->left, judul) : searchVideo(node->right, judul);
}

// ubah status ke string
string getStatusString(Status s) {
    return s == TERSEDIA ? "Tersedia" : (s == DALAM_ANTREAN ? "Dalam Antrean" : "Sedang Diputar");
}

// Tampilkan vidio
void displayVideos(Video* node, int& counter) {
    if (node) {
        displayVideos(node->left, counter);
        if (node->judul != "") {
            cout << counter++ << ". " << node->judul << " (" << node->durasi 
                 << " menit) - " << getStatusString(node->status) << endl;
        }
        displayVideos(node->right, counter);
    }
}

void searchByKeyword(Video* node, string keyword) {
    if (node) {
        searchByKeyword(node->left, keyword);
        if (node->judul != "" && node->judul.find(keyword) != string::npos) {
            cout << "- " << node->judul << " (" << node->durasi 
                 << " menit) - " << getStatusString(node->status) << endl;
        }
        searchByKeyword(node->right, keyword);
    }
}

// tambah stack
void pushStack(string action, string judul, int durasi, Status status) {
    Node* n = new Node;
    n->action = action; n->judul = judul; n->durasi = durasi; 
    n->oldStatus = status; n->next = stackTop;
    stackTop = n;
}

// ambil stack
bool popStack(string& action, string& judul, int& durasi, Status& status) {
    if (!stackTop) return false;
    action = stackTop->action; judul = stackTop->judul; 
    durasi = stackTop->durasi; status = stackTop->oldStatus;
    Node* temp = stackTop;
    stackTop = stackTop->next;
    delete temp;
    return true;
}

// tambah vidio ke belakang
void enqueue(Video* video) {
    Node* n = new Node;
    n->video = video; n->next = nullptr;
    if (!queueRear) queueFront = queueRear = n;
    else { queueRear->next = n; queueRear = n; }
}

// ambil vidio dari depan
Video* dequeue() {
    if (!queueFront) return nullptr;
    Video* v = queueFront->video;
    Node* temp = queueFront;
    queueFront = queueFront->next;
    if (!queueFront) queueRear = nullptr;
    delete temp;
    return v;
}

void addHistory(string judul, int durasi) {
    Node* n = new Node;
    n->judul = judul; n->durasi = durasi; n->next = historyHead;
    historyHead = n;
}

//  tandai vidio
void markDeleted(Video* node, string judul) {
    if (node) {
        if (node->judul == judul) node->judul = "";
        markDeleted(node->left, judul);
        markDeleted(node->right, judul);
    }
}

// buat queue baru setealah dihapus
void rebuildQueue(string exclude = "") {
    Node* temp = queueFront;
    queueFront = queueRear = nullptr;
    currentlyPlaying = nullptr;
    
    while (temp) {
        if (temp->video->judul != exclude && temp->video->judul != "") {
            enqueue(temp->video);
            if (!currentlyPlaying) {
                currentlyPlaying = temp->video;
                temp->video->status = SEDANG_DIPUTAR;
            } else {
                temp->video->status = DALAM_ANTREAN;
            }
        }
        Node* del = temp;
        temp = temp->next;
        delete del;
    }
}

// masuk ke BST
void tambahVideo() {
    string judul; int durasi;
    cout << "\n=== TAMBAH VIDEO ===\nJudul: ";
    cin.ignore(); getline(cin, judul);
    cout << "Durasi (menit): "; cin >> durasi;
    
    if (searchVideo(root, judul)) {
        cout << "Error: Judul sudah ada!" << endl;
        return;
    }
    
    root = root ? insertVideo(root, judul, durasi) : createVideo(judul, durasi);
    cout << "Video berhasil ditambahkan!" << endl;
    pushStack("ADD", judul, durasi, TERSEDIA);
}

void tampilkanDaftarVideo() {
    cout << "\n=== DAFTAR VIDEO ===" << endl;
    if (!root) { cout << "Tidak ada video." << endl; return; }
    
    int counter = 1;
    displayVideos(root, counter);
    
    char pilihan;
    cout << "\nCari video (y/t)? ";
    cin >> pilihan;
    
    if (pilihan == 'y' || pilihan == 'Y') {
        string keyword;
        cout << "Kata kunci: ";
        cin.ignore(); getline(cin, keyword);
        cout << "\nHasil:\n";
        searchByKeyword(root, keyword);
    }
}

void tambahkanKePlaylist() {
    cout << "\n=== TAMBAH KE PLAYLIST ===\n";
    if (!root) { cout << "Tidak ada video." << endl; return; }
    
    string judul;
    cout << "Judul video: ";
    cin.ignore(); getline(cin, judul);
    
    Video* video = searchVideo(root, judul);
    if (!video || video->judul == "") {
        cout << "Video tidak ditemukan!" << endl;
        return;
    }
    
    if (video->status != TERSEDIA) {
        cout << "Video sudah di playlist!" << endl;
        return;
    }
    
    enqueue(video);
    pushStack("PLAYLIST", judul, video->durasi, TERSEDIA);
    
    if (!currentlyPlaying) {
        currentlyPlaying = video;
        video->status = SEDANG_DIPUTAR;
        cout << "Video ditambahkan dan langsung diputar!" << endl;
    } else {
        video->status = DALAM_ANTREAN;
        cout << "Video ditambahkan ke playlist!" << endl;
    }
}

void tontonVideo() {
    cout << "\n=== TONTON VIDEO ===" << endl;
    if (!currentlyPlaying) {
        cout << "Tidak ada video yang diputar!" << endl;
        return;
    }
    
    cout << "Menonton: " << currentlyPlaying->judul 
         << " (" << currentlyPlaying->durasi << " menit)\nSelesai!\n";
    
    pushStack("WATCH", currentlyPlaying->judul, currentlyPlaying->durasi, SEDANG_DIPUTAR);
    addHistory(currentlyPlaying->judul, currentlyPlaying->durasi);
    
    currentlyPlaying->status = TERSEDIA;
    dequeue();
    
    if (queueFront) {
        currentlyPlaying = queueFront->video;
        if (currentlyPlaying->judul != "") {
            currentlyPlaying->status = SEDANG_DIPUTAR;
            cout << "Berikutnya: " << currentlyPlaying->judul << endl;
        }
    } else {
        currentlyPlaying = nullptr;
        cout << "Playlist selesai!" << endl;
    }
}

void riwayatTontonan() {
    cout << "\n=== RIWAYAT TONTONAN ===" << endl;
    if (!historyHead) { cout << "Belum ada riwayat." << endl; return; }
    
    int counter = 1;
    Node* current = historyHead;
    while (current) {
        cout << counter++ << ". " << current->judul 
             << " (" << current->durasi << " menit)" << endl;
        current = current->next;
    }
}

void hapusVideo() {
    cout << "\n=== HAPUS VIDEO ===" << endl;
    if (!root) { cout << "Tidak ada video." << endl; return; }
    
    string judul;
    cout << "Judul yang dihapus: ";
    cin.ignore(); getline(cin, judul);
    
    Video* video = searchVideo(root, judul);
    if (!video || video->judul == "") {
        cout << "Video tidak ditemukan!" << endl;
        return;
    }
    
    if (video->status != TERSEDIA) {
        char konfirmasi;
        cout << "Video " << getStatusString(video->status) 
             << ". Tetap hapus (y/t)? ";
        cin >> konfirmasi;
        if (konfirmasi != 'y' && konfirmasi != 'Y') {
            cout << "Dibatalkan." << endl;
            return;
        }
    }
    
    pushStack("DELETE", video->judul, video->durasi, video->status);
    
    if (video->status != TERSEDIA) rebuildQueue(judul);
    
    markDeleted(root, judul);
    cout << "Video berhasil dihapus!" << endl;
}

void undoTindakanTerakhir() {
    cout << "\n=== UNDO ===" << endl;
    
    string action, judul; int durasi; Status oldStatus;
    
    if (!popStack(action, judul, durasi, oldStatus)) {
        cout << "Tidak ada tindakan untuk dibatalkan." << endl;
        return;
    }
    
    cout << "Membatalkan: " << action << " - \"" << judul << "\"" << endl;
    
    if (action == "ADD") {
        markDeleted(root, judul);
        cout << "Penambahan dibatalkan." << endl;
    }
    else if (action == "DELETE") {
        Video* video = searchVideo(root, judul);
        if (video) {
            video->judul = judul;
            video->status = TERSEDIA;
            cout << "Video dikembalikan." << endl;
        }
    }
    else if (action == "PLAYLIST") {
        Video* video = searchVideo(root, judul);
        if (video && video->judul != "") {
            video->status = TERSEDIA;
            rebuildQueue(judul);
            cout << "Dihapus dari playlist." << endl;
        }
    }
    else if (action == "WATCH") {
        Video* video = searchVideo(root, judul);
        if (video && video->judul != "") {
            if (historyHead && historyHead->judul == judul) {
                Node* temp = historyHead;
                historyHead = historyHead->next;
                delete temp;
            }
            
            Node* newNode = new Node;
            newNode->video = video;
            newNode->next = queueFront;
            
            if (currentlyPlaying) currentlyPlaying->status = DALAM_ANTREAN;
            
            queueFront = newNode;
            if (!queueRear) queueRear = newNode;
            
            currentlyPlaying = video;
            video->status = SEDANG_DIPUTAR;
            
            cout << "Video dikembalikan ke playlist." << endl;
        }
    }
}

int main() {
    int pilihan;
    
    do {
        cout << "\n=== IDLIX TUBE ===\n"
             << "1. Tambah Video\n2. Daftar Video\n3. Tambah ke Playlist\n"
             << "4. Tonton Video\n5. Riwayat\n6. Hapus Video\n7. Undo\n0. Keluar\n"
             << "Pilih: ";
        cin >> pilihan;
        
        switch (pilihan) {
            case 1: tambahVideo(); break;
            case 2: tampilkanDaftarVideo(); break;
            case 3: tambahkanKePlaylist(); break;
            case 4: tontonVideo(); break;
            case 5: riwayatTontonan(); break;
            case 6: hapusVideo(); break;
            case 7: undoTindakanTerakhir(); break;
            case 0: cout << "Terima kasih!" << endl; break;
            default: cout << "Pilihan tidak valid!" << endl;
        }
    } while (pilihan != 0);
    
    return 0;
}