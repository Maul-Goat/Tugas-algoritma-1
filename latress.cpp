#include <iostream>
#include <string>
using namespace std;

struct video
{
    string judul;
    int durasi; //dalam menit
    string status; // 'tersedia', 'dalam antrean', 'sedang diputar'
    video* kiri; ///PTB
    video* kanan;///PTB
};

// LINKEDLIST Playlist dan Riwayat
struct listNode
{
    video* Video;
    string jenisAksi; //Tambah, playlist, tonton, hapus
    listNode* next;
};

video* root = nullptr;
listNode* playlisthead  = nullptr; //Linkedlist playlist
listNode* riwayathead   = nullptr; //Linkedlist riwayat
listNode* undoAksi      = nullptr; //Linkedlist undo (Stack)

void tambahVideo(string judul, int durasi){
    video* newVideo = new video{judul,durasi,"tersedia",nullptr,nullptr};

    if (!root)
    {
        root = newVideo;
    } else{
        video* current = root;
        while (true)
        {
            if(judul < current->judul){
                if (!current->kiri)
                {
                    current->kiri = newVideo;
                    break;
                }
                current = current->kiri;
            } else if(judul > current->judul){
                if (!current->kanan)
                {
                    current->kanan = newVideo;
                    break;
                }
                current = current->kanan;
            } else {
                cout << "judul video sudah ada!\n";
                delete newVideo;
                return;
            }
        }
    }
    // Simpan aksi undo(tambah)
    listNode* undoNode = new listNode{newVideo, "tambah", undoAksi};
    undoAksi    = undoNode;
    cout << "Video berhasil ditambahkan!\n";
}

// Fungsi In-Order Traversal (Menampilkan Video Ascending)
void tampilkanDaftar(video* node){
    // Pengecekan ROOT kosong
    if(!root){
        cout << "Belum ada Daftar Video silahkan tambahkan terlebih dahulu\n";
    }
    if (!node) return;
    tampilkanDaftar(node->kiri);
    cout << "Judul: " << node->judul << " | Durasi: " << node->durasi << "m | Status: " << node->status << endl;
    tampilkanDaftar(node->kanan);
}

// Fungsi Cari Video Berdasarkan Judul (PTB Search)
video* cariVideo(string judul, video* node){
    if (!node) return nullptr;
    if (judul == node->judul) return node;
    if (judul < node->judul) return cariVideo(judul, node->kiri);
    return cariVideo(judul,node->kanan);
}

// Fungsi Tambah ke Playlist (Linkedlist)
void tambahkePlaylist(string judul){
    video* Video = cariVideo(judul,root);
    if(!Video){
        cout << "Video tidak ditemukan!\n";
        return;
    }
    if(Video->status != "tersedia"){
        cout << "Video tidak bisa ditambagkan (sudah di playlist/sedang diputar)!\n";
        return;
    }
    // Tambah ke Playlist
    listNode* newNode = new listNode{Video,"playlist",playlisthead};
    playlisthead = newNode;
    //update status
    if (!playlisthead->next) {
        Video->status = "sedang diputar";
    } else {
        Video->status = "dalam antrean";
    }
    // Simpan aksi undo(Playlist)
    listNode* undoNode = new listNode{Video, "playlist", undoAksi};
    undoAksi    = undoNode;
    cout << "Video Berhasil di tambahkan ke playlist!\n";
}

// Fungsi Tonton Video (Pindahkan ke Riwayat)
void tontonVideo(){
    if(!playlisthead){
        cout << "Playlist Kosong!\n";
        return;
    }
    video* Video = playlisthead->Video;
    Video->status= "tersedia";

    // Tambah Riwayat
    listNode* newNode = new listNode{Video,"tonton",riwayathead};
    riwayathead = newNode;
    // Hapus dari playlist
    listNode* temp = playlisthead;
    playlisthead = playlisthead->next;
    delete temp;
    // Simpan aksi undo (tonton)
    listNode* undoNode = new listNode{Video, "tonton", undoAksi};
    undoAksi = undoNode;
    cout << "Video \"" << Video->judul << "\"Selesai ditonton!\n";
}

// Fungsi Hpaus video dari BST
video* hapusVideoPTB(string judul, video* node){
    if (!node) return nullptr;
    if (judul < node->judul)
    {
        node->kiri = hapusVideoPTB(judul,node->kiri);
    } else if(judul > node->judul){
        node->kanan = hapusVideoPTB(judul,node->kanan);
    } else{
        if(!node->kiri) return node->kanan;
        if(!node->kanan) return node->kiri;
        video* temp = node->kanan;
        while(temp->kiri) temp = temp->kiri;
        node->judul = temp->judul;
        node->durasi = temp->durasi;
        node->kanan = hapusVideoPTB(temp->judul,node->kanan);
    }
    return node;
}

void hapusVideo(string judul){
    video* Video = cariVideo(judul,root);
    if (!Video){
        cout << "Video tidak ditemukan!\n";
        return;
    }
    if(Video->status != "tersedia"){
        char konfirmasi;
        cout << "Video sedang diputar/dalam antean. Yakin ingin hapus? (y/t):";
        cin >> konfirmasi;
        if(konfirmasi != 'y')return;
    }
    // Hapus dari playlist (jika ada)
    listNode* prev = nullptr;
    listNode* curr = playlisthead;
    while (curr)
    {
        if(curr->Video->judul == judul){
            if (prev)
            {
                prev->next = curr->next;
            }else{
                playlisthead = curr->next;
            }
            delete curr;
            break;
        }
        prev = curr;
        curr = curr->next;
    }
    // Hapus dari PTB/BST
    root = hapusVideoPTB(judul,root);
    // Simpan aksi undo (hapus)
    listNode* undoNode = new listNode{Video, "hapus", undoAksi};
    undoAksi = undoNode;
    cout << "Video berhasil dihapus!\n";
}
// Fungsi Undo(Linkedlist sebagai stack)
void undoAksiTerakhir(){
    if(!undoAksi){
        cout << "Tidak ada aksi untuk undo!\n";
        return;
    }
    
    video* Video = undoAksi->Video;
    string jenisAksi = undoAksi->jenisAksi;
    listNode* temp = undoAksi;
    undoAksi = undoAksi->next;
    
    if(jenisAksi == "tambah"){
        root = hapusVideoPTB(Video->judul, root);
        cout << "Undo: Video \"" << Video->judul << "\" dibatalkan penambahannya!\n";
    }
    else if(jenisAksi == "playlist"){
        Video->status = "tersedia";
        // Hapus dari playlist
        listNode* prev = nullptr;
        listNode* curr = playlisthead;
        while(curr){
            if(curr->Video == Video){
                if(prev) prev->next = curr->next;
                else playlisthead = curr->next;
                delete curr;
                break;
            }
            prev = curr;
            curr = curr->next;
        }
        cout << "Undo: Video \"" << Video->judul << "\" dikeluarkan dari playlist!\n";
    }
    else if(jenisAksi == "tonton"){
        Video->status = "sedang diputar";
        // Kembalikan ke playlist
        listNode* newPlaylistNode = new listNode{Video,"tonton", playlisthead};
        playlisthead = newPlaylistNode;
        // Hapus dari riwayat
        listNode* prev = nullptr;
        listNode* curr = riwayathead;
        while(curr){
            if(curr->Video == Video){
                if(prev) prev->next = curr->next;
                else riwayathead = curr->next;
                delete curr;
                break;
            }
            prev = curr;
            curr = curr->next;
        }
        cout << "Undo: Video \"" << Video->judul << "\" dikembalikan ke playlist!\n";
    }
    else if(jenisAksi == "hapus"){
        // Kembalikan video ke BST
        tambahVideo(Video->judul, Video->durasi);
        cout << "Undo: Video \"" << Video->judul << "\" dikembalikan!\n";
    }
    
    delete temp;
}

// Menu Utama
int main(){
    int pilihan;
    string judul;
    int durasi;

    while (true)
    {
        cout << "\n=== IDLIX Tube ===\n";
        cout << "1. Tambah Video\n";
        cout << "2. Tampilkan Daftar Video\n";
        cout << "3. Tambah ke Playlist\n";
        cout << "4. Tonton Video\n";
        cout << "5. Tampilkan Riwayat\n";
        cout << "6. Hapus Video\n";
        cout << "7. Undo\n";
        cout << "8. Keluar\n";
        cout << "Pilih: ";
        cin >> pilihan;

        switch (pilihan)
        {
        case 1:
            cout << "Judul: "; 
            cin.ignore();
            getline(cin, judul);
            cout << "Durasi(menit): ";
            cin >> durasi;
            tambahVideo(judul,durasi);
            break;
        case 2:
            tampilkanDaftar(root);
            break;
        case 3:
            cout << "Judul: "; cin.ignore(); getline(cin,judul);
            tambahkePlaylist(judul);
            break;
        case 4:
            tontonVideo();
            break;
        case 5:{
            cout << "Riwayat Tontonan:\n";
            listNode* curr = riwayathead;
            while (curr)
            {
                cout << curr->Video->judul << "(" << curr->Video->durasi << "m)\n";
                curr = curr->next;
            }
            break;
        }
        case 6:
            cout << "Judul: "; cin.ignore(); getline(cin,judul);
            hapusVideo(judul);
            break;
        case 7:
            undoAksiTerakhir();
            break;
        case 8:
            return 0;
        default:
            cout << "Pilihan tidak valid!\n";
            break;
        }
    }
    
}