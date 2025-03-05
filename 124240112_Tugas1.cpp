#include <iostream>
using namespace std;
struct data{
    string nama;
    int nim;
    float ipk;
};
void tampil(int totaldata, data d[]);
void searchnim(int totaldata, data d[]);
void searchnama(int totaldata, data d[]);
void sortdecs(int totaldata, data d[]);
void sortasc(int low, int high, data d[]);
void menu(int totaldata, data d[]);

int main(){
    struct data d[15] ={
        {"Aditya Pratama", 124240001, 3.75},
        {"Budi Santoso", 124240002, 3.60},
        {"Cindy Maharani", 124240003, 3.80},
        {"Dimas Ramadhan", 124240004, 3.45},
        {"Erika Salsabila", 124240005, 3.90},
        {"Fajar Nugroho", 124240006, 3.50},
        {"Galih Permana", 124240007, 3.65},
        {"Hana Putri", 124240008, 3.85},
        {"Indra Wijaya", 124240009, 3.70},
        {"Joko Setiawan", 124240010, 3.55},
        {"Kartika Dewi", 124240011, 3.95},
        {"Luthfi Hidayat", 124240012, 3.40},
        {"Mega Pertiwi", 124240013, 3.75},
        {"Nanda Prasetyo", 124240014, 3.30},
        {"Oka Wibisono", 124240015, 3.85}
    };
    int totaldata = 15;

    menu(totaldata, d);
    
}
void menu(int totaldata, data d[]){
    int pilih;
    do{
        cout << "\n=================================" <<  endl;
        cout << "|           Menu Pilihan        |" <<  endl;
        cout << "=================================" <<  endl;
        cout << "1. Tampilkan Mahasiswa" << endl;
        cout << "2. Cari Berdasarkan NIM" << endl;
        cout << "3. Cari Berdasarkan Nama" << endl;
        cout << "4. Sorting IPK Mahasiswa (Asc)" << endl;
        cout << "5. Sorting IPK Mahasiswa (Desc)" << endl;
        cout << "6. Exit" << endl;
        cout << "Pilih Menu: ";
        cin >> pilih;
        cout << endl;

        switch(pilih){
            case 1 :
            tampil(totaldata, d);
            cout << endl;
            break;
            case 2 :
            searchnim(totaldata, d);
            system("pause");
            system("cls");
            break;
            case 3 :
            searchnama(totaldata,d);
            system("pause");
            system("cls");
            break;
            case 4 :
            sortasc(0, totaldata - 1, d);
            cout << "Data telah diurutkan berdasarkan IPK." << endl;
            tampil(totaldata, d);
            cout << endl;
            system("pause");
            system("cls");
            break;
            case 5 :
            sortdecs(totaldata, d);
            cout << "Data telah diurutkan berdasarkan IPK." << endl;
            tampil(totaldata, d);
            cout << endl;
            system("pause");
            system("cls");
            break;
            case 6 :
            cout << "Exiting program..." << endl;
            exit(0);
            break;
            default :
            cout << "Pilihan tidak ada dalam menu" << endl;
        }
    }while(pilih != 0);
}
void tampil(int totaldata, data d[]){ //menampilkan data
    cout << "Data Mahasiswa:";
    for(int i = 0; i < totaldata; i++){
        cout << "\nNO: " << i+1 << ",\t";
        cout << "Nama: " << d[i].nama << ",\t";
        cout << "NIM: " << d[i].nim << ",\t";
        cout << "IPK: " << d[i].ipk;
    }
}
void searchnim(int totaldata, data d[]){ //mencari nim
    int nim;
    cout << "Masukkan NIM yang ingin dicari: ";
    cin >> nim;
    bool found = false;
    for (int i = 0; i < totaldata; i++) {
        if (d[i].nim == nim) {
            cout << "\nMahasiswa ditemukan!" << endl;
            cout << "Nama\t: " << d[i].nama << endl;
            cout << "NIM\t: " << d[i].nim << endl;
            cout << "IPK\t: " << d[i].ipk << endl;
            found = true;
            break;
        }
    }
    if (!found) {
        cout << "Data tidak ditemukan." << endl;
    }
}
void searchnama(int totaldata, data d[]) { //mencari nama
    string nama; 
    cout << "Masukkan nama yang ingin dicari: ";
    cin.ignore();
    getline(cin, nama);
    
    int left = 0;
    int right = totaldata - 1;
    bool found = false;
    
    while (left <= right) {
        int mid = left + (right - left) / 2;
        
        if (d[mid].nama == nama) {
            int start = mid;
            while (start > 0 && d[start-1].nama == nama) {
                start--;
            }
            int end = mid;
            while (end < totaldata - 1 && d[end+1].nama == nama) {
                end++;
            }
            for (int i = start; i <= end; i++) {
                if (!found) {
                    cout << "Data ditemukan!" << endl;
                    found = true;
                }
                cout << "\nData ke-" << i+1 << endl;
                cout << "Nama Lengkap\t: " << d[i].nama << endl;
                cout << "NIM\t\t: " << d[i].nim << endl;
                cout << "IPK\t\t: " << d[i].ipk << endl;
            }
            
            break;
        }
        else if (d[mid].nama < nama) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }
    
    if (!found) {
        cout << "Data tidak ditemukan." << endl;
    }
}
void sortasc(int low, int high, data d[]){ //sorting ascending ipk
    if (low < high) {
        float pivot = d[high].ipk; 
        int i = low - 1;

        for (int j = low; j < high; j++) {
            if (d[j].ipk < pivot) {
                i++;
                swap(d[i], d[j]); 
            }
        }
        swap(d[i + 1], d[high]); 
        int pi = i + 1;

        sortasc(low, pi - 1, d); 
        sortasc(pi + 1, high, d); 
    }
}
void sortdecs(int totaldata, data d[]){ //sorting descending ipk
    for (int i = 0; i < totaldata - 1; i++) {
        for (int j = 0; j < totaldata - i - 1; j++) {
            if (d[j].ipk < d[j + 1].ipk) {
                swap(d[j], d[j + 1]);
            }
        }
    }
}