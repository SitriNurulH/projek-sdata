#include <iostream>
#include <fstream>
#include <stack>
#include <queue>
#include <iomanip> 
using namespace std;

struct KTP {
    string nama;
    string nik;
    string alamat;
};

struct Barang {
    string kodeItem;
    string tanggalMasuk;
    string namaBrg;
};


struct Pembeli {
    string ktp;
    string waktuDatang;
    string kodeItem;
    int jumlahBeli;
    double totalHarga;
};


stack<Barang> gudang;

queue<Pembeli> antrianPembeli;

void simpanDataKTP(const KTP& data) {
    ofstream file("data_ktp.txt", ios::app);
    if (file.is_open()) {
        file << data.nama << "," << data.nik << "," << data.alamat << endl;
        file.close();
    } else {
        cout << "Gagal membuka file data_ktp.txt!" << endl;
    }
}

void tampilkanDataKTP() {
    ifstream file("data_ktp.txt");
    if (!file.is_open()) {
        cout << "Gagal membuka file data_ktp.txt!" << endl;
        return;
    }

    string line;
    cout << "\nData KTP Terdaftar:\n";
    cout << "=================================\n";
    cout << left << setw(20) << "Nama" << setw(20) << "NIK" << "Alamat" << endl;
    cout << "=================================\n";

    while (getline(file, line)) {
        size_t pos1 = line.find(",");
        size_t pos2 = line.find(",", pos1 + 1);
        
        string nama = line.substr(0, pos1);
        string nik = line.substr(pos1 + 1, pos2 - pos1 - 1);
        string alamat = line.substr(pos2 + 1);
        
        cout << left << setw(20) << nama << setw(20) << nik << alamat << endl;
    }

    cout << "=================================\n";
    file.close();
}

bool validasiKTP(const string& nik) {
    ifstream file("data_ktp.txt");
    string line;
    while (getline(file, line)) {
        size_t pos = line.find(",");
        if (pos != string::npos) {
            string storedNik = line.substr(pos + 1, line.find(",", pos + 1) - pos - 1);
            if (storedNik == nik) {
                return true;
            }
        }
    }
    return false;
}

void tambahBarang() {
    Barang item;
    cin.ignore();
    cout << "Masukkan kode item: ";
    getline(cin, item.kodeItem);
    cout << "Masukkan nama barang: ";
    getline(cin, item.namaBrg);
    cout << "Masukkan tanggal masuk (YYYY-MM-DD): ";
    cin >> item.tanggalMasuk;

    gudang.push(item);
    cout << "Barang berhasil ditambahkan ke gudang!\n";
}

void catatPembeli(const string& nikAdmin) {
    Pembeli pembeli;
    cout << "Masukkan NIK pembeli: ";
    cin >> pembeli.ktp;
    if (pembeli.ktp == nikAdmin) {
        cout << "NIK pembeli tidak boleh sama dengan NIK admin!\n";
        return;
    }
    if (!validasiKTP(pembeli.ktp)) {
        cout << "NIK tidak valid! Pembeli harus menggunakan NIK yang terdaftar.\n";
        return;
    }
    cout << "Masukkan waktu kedatangan (HH:MM): ";
    cin >> pembeli.waktuDatang;
    cout << "Masukkan kode item yang dibeli: ";
    cin >> pembeli.kodeItem;
    cout << "Masukkan jumlah barang yang dibeli: ";
    cin >> pembeli.jumlahBeli;
    cout << "Masukkan harga satuan barang: ";
    double hargaSatuan;
    cin >> hargaSatuan;

    pembeli.totalHarga = pembeli.jumlahBeli * hargaSatuan;
    antrianPembeli.push(pembeli);
    cout << "Pembeli berhasil dicatat ke antrian.\n";
}

void cetakStruk(const string& adminNik) {
    if (!antrianPembeli.empty()) {
        Pembeli pembeli = antrianPembeli.front();
        antrianPembeli.pop();

        cout << "\n=========== STRUK PEMBELIAN ===========\n";
        cout << "Admin           : " << adminNik << endl;
        cout << "NIK Pembeli     : " << pembeli.ktp << endl;
        cout << "Waktu Kedatangan: " << pembeli.waktuDatang << endl;
        cout << "Kode Item       : " << pembeli.kodeItem << endl;
        cout << "Jumlah Barang   : " << pembeli.jumlahBeli << endl;
        cout << "Total Harga     : Rp" << pembeli.totalHarga << endl;
        cout << "=======================================\n";
    } else {
        cout << "Tidak ada pembeli dalam antrian untuk mencetak struk.\n";
    }
}

void tambahNIK() {
    KTP dataBaru;
    cin.ignore();
    cout << "Masukkan nama: ";
    getline(cin, dataBaru.nama);
    cout << "Masukkan NIK: ";
    cin >> dataBaru.nik;
    cout << "Masukkan alamat: ";
    cin.ignore();
    getline(cin, dataBaru.alamat);

    if (validasiKTP(dataBaru.nik)) {
        cout << "NIK sudah terdaftar!\n";
    } else {
        simpanDataKTP(dataBaru);
        cout << "Data NIK berhasil ditambahkan ke sistem!\n";
    }
}

int main() {
    int pilihan;
    string adminNik;

    do {
        cout << "\nMenu:\n";
        cout << "1. Admin Sign In\n";
        cout << "2. Tambah Data NIK\n";
        cout << "3. Entri Item Barang\n";
        cout << "4. Catat Pembeli\n";
        cout << "5. Cetak Struk Pembelian\n";
        cout << "6. Lihat Data KTP Terdaftar\n"; 
        cout << "7. Keluar\n";
        cout << "Pilih menu: ";
        cin >> pilihan;

        switch (pilihan) {
            case 1: {
                cout << "Masukkan NIK Admin: ";
                cin >> adminNik;
                if (validasiKTP(adminNik)) {
                    cout << "Sign in berhasil sebagai admin!\n";
                } else {
                    cout << "NIK tidak valid!\n";
                    adminNik = "";
                }
                break;
            }
            case 2:
                tambahNIK();
                break;
            case 3:
                tambahBarang();
                break;
            case 4:
                if (!adminNik.empty()) {
                    catatPembeli(adminNik);
                } else {
                    cout << "Silakan sign in sebagai admin terlebih dahulu.\n";
                }
                break;
            case 5:
                if (!adminNik.empty()) {
                    cetakStruk(adminNik);
                } else {
                    cout << "Silakan sign in sebagai admin terlebih dahulu.\n";
                }
                break;
            case 6:
                tampilkanDataKTP();  
                break;
            case 7:
                cout << "Keluar dari program.\n";
                break;
            default:
                cout << "Pilihan tidak valid!\n";
        }
    } while (pilihan != 7);

    return 0;
}
