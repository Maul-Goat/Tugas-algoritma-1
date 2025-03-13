#include <iostream>
using namespace std;
struct data{
    char nama[30], jurusan[30];
    int nim, tahun;
    float ipk = 0.0;
};
data d[50];
int totaldata = 0;
FILE *file;

void input(){
    file = fopen("mahasiswaData.dat", "ab");
    if(file==NULL){
        cout << " FILE ERROR" << endl;
        exit(1);
        }

    int n = 0;
    cout << "Masukkan jumlah data: ";
    cin.ignore();
    cin >> n;
    
    for (int i = totaldata; i < totaldata + n; ++i) {
        cout << "\nMasukkan data ke-" << i + 1 << endl;
        cout << "Masukkan nama\t\t: ";
        cin.ignore();
        cin.getline(d[i].nama, 30);
        cout << "Masukkan NIM\t\t: ";
        cin >> d[i].nim;
        cout << "Masukkan jurusan\t: ";
        cin.ignore();
        cin.getline(d[i].jurusan, 30);
        cout << "Masukkan tahun masuk\t: ";
        cin >> d[i].tahun;
        fwrite(&d[i], sizeof(struct data), 1, file);
    }
    totaldata += n;
    cout << endl;
    fclose(file);
}
void quickshort(int low, int high){
    if (low < high) {
        float pivot = d[high].nim; 
        int i = low - 1;

        for (int j = low; j < high; j++) {
            if (d[j].nim < pivot) {
                i++;
                swap(d[i], d[j]); 
            }
        }
        swap(d[i + 1], d[high]); 
        int pi = i + 1;

        quickshort(low, pi - 1); 
        quickshort(pi + 1, high); 
    }
}
void tampil(){ // Quick short
    file = fopen("mahasiswaData.dat", "rb");
    if(file==NULL){
        cout << " FILE ERROR" << endl;
        exit(1);
        }
    totaldata = 0;
    while(fread(&d[totaldata], sizeof(struct data), 1, file)){
        totaldata++;
    }
    quickshort(0, totaldata - 1);
    fclose(file);
        for(int i = 0; i < totaldata; i++){
            cout << "\nData mahasiswa ke-" << i + 1 << endl;
            cout << "Nama\t\t: " << d[i].nama << endl;
            cout << "NIM\t\t: " << d[i].nim << endl;
            cout << "Jurusan\t\t: " << d[i].jurusan << endl;
            cout << "Tahun Masuk\t: " << d[i].tahun << endl;
            cout << "IPK\t\t: " << d[i].ipk << endl;
        }
}
void searchnim(){ //Binary search + quick short
    file = fopen("mahasiswaData.dat", "rb");
    if(file==NULL){
        cout << " FILE ERROR" << endl;
        exit(1);
        }
    totaldata = 0;
    while(fread(&d[totaldata], sizeof(struct data), 1, file)){
        totaldata++;
    }
    quickshort(0, totaldata - 1);
    fclose(file);
    cout << "Masukkan NIM yang ingin dicari: ";
    int nim;
    cin >> nim;
    
    int left = 0;
    int right = totaldata - 1;
    bool found = false;
    
    while (left <= right) {
        int mid = left + (right - left) / 2;
        
        if (d[mid].nim == nim) {
            int start = mid;
            while (start > 0 && d[start-1].nim == nim) {
                start--;
            }
            int end = mid;
            while (end < totaldata - 1 && d[end+1].nim == nim) {
                end++;
            }
            for (int i = start; i <= end; i++) {
                if (!found) {
                    cout << "Data ditemukan!" << endl;
                    found = true;
                }
                cout << "\nData ke-" << i+1 << endl;
                cout << "Nama\t\t: " << d[i].nama << endl;
                cout << "NIM\t\t: " << d[i].nim << endl;
                cout << "Jurusan\t\t: " << d[i].jurusan << endl;
                cout << "Tahun Masuk\t: " << d[i].tahun << endl;
                cout << "IPK\t\t: " << d[i].ipk << endl;
            }
            break;
        }
        else if (d[mid].nim < nim) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }
    
    if (!found) {
        cout << "Mahasiswa dengan NIM " << nim << " tidak ditemukan." << endl;
    }
}
void serachjurusan(){
    file = fopen("mahasiswaData.dat", "rb");
    if(file==NULL){
        cout << " FILE ERROR" << endl;
        exit(1);
        }
    totaldata = 0;
    while(fread(&d[totaldata], sizeof(struct data), 1, file)){
        totaldata++;
    }
    fclose(file);
    string jurusan;
    cout << "Masukkan jurusan yang ingin dicari: ";
    cin.ignore();
    getline(cin, jurusan);
    bool found = false;
    for (int i = 0; i < totaldata; i++) {
        if (d[i].jurusan == jurusan) {
            cout << "\nData ke-" << i+1 << endl;
            cout << "Nama\t\t: " << d[i].nama << endl;
            cout << "NIM\t\t: " << d[i].nim << endl;
            cout << "Jurusan\t\t: " << d[i].jurusan << endl;
            cout << "Tahun Masuk\t: " << d[i].tahun << endl;
            cout << "IPK\t\t: " << d[i].ipk << endl;
            found = true;
        }
    }
    if (!found) {
        cout << "Mahasiswa dengan jurusan " << jurusan << " tidak ditemukan." << endl;
    }
}
void updateipk(){\
    int nim;
    cout << "Masukkan NIM yang ingin diupdate: ";
    cin >> nim;
    file = fopen("mahasiswaData.dat", "rb+");
    if (file == NULL) {
        cout << " FILE ERROR" << endl;
        exit(1);
    }
    totaldata = 0;
    while (fread(&d[totaldata], sizeof(struct data), 1, file)) {
        totaldata++;
    }
    bool found = false;
    for (int i = 0; i < totaldata; i++) {
        if (d[i].nim == nim) {
            cout << "\nData ditemukan!" << endl;
            cout << "Nama\t\t: " << d[i].nama << endl;
            cout << "NIM\t\t: " << d[i].nim << endl;
            cout << "Jurusan\t\t: " << d[i].jurusan << endl;
            cout << "Tahun Masuk\t: " << d[i].tahun << endl;
            cout << "IPK saat ini\t: " << d[i].ipk << endl;

            float newipk;
            while (true) {
                cout << "\nMasukkan IPK yang baru (0.0 - 4.0): ";
                cin >> newipk;
                if (newipk >= 0.0 && newipk <= 4.0) {
                    d[i].ipk = newipk;
                    break;
                } else {
                    cout << "IPK tidak valid! Harap masukkan nilai antara 0.0 dan 4.0." << endl;
                }
            }
            fseek(file, i * sizeof(struct data), SEEK_SET);
            fwrite(&d[i], sizeof(struct data), 1, file);
            found = true;
            cout << "IPK berhasil diupdate!" << endl;
            break;
        }
    }
    if (!found) {
        cout << "Mahasiswa dengan NIM " << nim << " tidak ditemukan." << endl;
        cout << "IPK tidak berhasil diupdate!" << endl;
    }
    fclose(file);
    
}
void loadData(){
    file = fopen("mahasiswaData.dat", "rb");
    if(file != NULL){
        totaldata = 0;
        while(fread(&d[totaldata], sizeof(struct data), 1, file)){
            totaldata++;
        }
        fclose(file);
    }
}
void hapusData() {
    file = fopen("mahasiswaData.dat", "wb"); 
    if (file == NULL) {
        cout << "FILE ERROR" << endl;
        exit(1);
    }
    fclose(file); 
    totaldata = 0; 
    cout << "\nSemua data berhasil dihapus!" << endl;
}
int main(){
    loadData();
    int pilih;
    do{
        cout << "\n================" << endl;
        cout << "  Menu Pilihan  " <<  endl;
        cout << "================" << endl;
        cout << "1. Input data" << endl;
        cout << "2. Tampil data" << endl;
        cout << "3. Seaching NIM" << endl;
        cout << "4. Seaching Jurusan" << endl;
        cout << "5. Update IPK" << endl;
        cout << "6. Hapus Data" << endl;
        cout << "7. Exit" << endl;
        cout << "Pilih Menu: ";
        cin >> pilih;
        switch(pilih){
            case 1 :
            input();
            break;
            case 2 :
            tampil();
            break;
            case 3 :
            cout << endl;
            system("cls");
            searchnim();
            break;
            case 4 :
            cout << endl;
            system("cls");
            serachjurusan();
            break;
            case 5 :
            cout << endl;
            system("cls");
            updateipk();
            break;
            case 6 :
            cout << endl;
            hapusData();
            break;
            case 7 :
            system("cls");
            cout << "Exiting Program..." << endl;
            exit(0);
            break;
            default :
            cout << "\nPilihan tidak valid..." << endl;
        }
    }while(pilih != 0);
}