#include <iostream> // untuk input output
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include "VariadicTable.h" // untuk table
#include <cstdlib>

using namespace std;

struct Buku {
    int id, stok;
    float rating, harga;
    string namaBuku, penulis, sinopsis, tahunTerbit;
    Buku *next;
};

Buku *head, *tail;
string beliBuku;
string chartNamaBuku[5]; // array buat keranjang
int chartJumPembelian[5]; // array buat jumlah buku yang dimasukkan keranjang
float chartHargaBuku[5]; // array buat harga buku yang dimasukkan keranjang
int jumlahBarangBeli;
int i = 0;

// Inisialisasi head dan tail
void init() {
    head = NULL;
    tail = NULL;
}

// Validasi ada tidaknya data
int isEmpty() {
    if (tail == NULL)
        return 1; // true (alias data kosong)
    else
        return 0; // false (sudah ada data)
}

// Untuk memasukkan data dari csv ke SLL
void readData() {
    ifstream rfile;
    rfile.open("list.csv");
    if (!rfile) {
        cout << "File not open\n";
        //return;
    }

    string line;
    const char delim = ',';

    while (getline(rfile, line)) {
        istringstream ss(line);

        Buku *baru;
        baru = new Buku;
        ss >> baru->id;
        ss.ignore(10, delim);
        getline(ss, baru->namaBuku, delim);
        ss >> baru->harga;
        ss.ignore(10, delim);
        ss >> baru->stok;
        ss.ignore(10, delim);
        ss >> baru->rating;
        ss.ignore(10, delim);
        getline(ss, baru->penulis, delim);
        getline(ss, baru->tahunTerbit, delim);
        getline(ss, baru->sinopsis, delim);

        if (isEmpty() == 1) {
            head = tail = baru;
            tail->next = NULL;
        } else {
            baru->next = head;
            head = baru;
        }
    }
}

// Menampilkan tabel daftar data buku
void display_data() {
    Buku *cur; // butuh bantuan pointer cur 
    cur = head; // cur diarah kan ke head

    if (isEmpty() == 0) {

        VariadicTable<std::string, std::string, std::string, float, int, float> vt(
            {"Nama Buku", "Penulis", "Tahun Terbit", "Harga", "Stok", "Rating"}, 10);
        while (cur != NULL) {
            if (cur->stok == 0){
                cur = cur->next;
                continue; // validasi ketika stok berjumlah 0, maka data tidak ditampilkan
            }
            vt.addRow(cur->namaBuku, cur->penulis, cur->tahunTerbit, cur->harga, cur->stok, cur->rating);
            cur = cur->next; // untuk mengarahkan pointer cur ke data selanjutnya
        }
        vt.print(cout);
    } else {
        cout << "Data Masih Kosong\n";
    }
}

// Lihat isi buku
string liatIsiBuku() {
    string pilihanBuku;
    cout << "Lihat buku apa: ";
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //buat hapus buffer dari inputan
    getline(cin, pilihanBuku);

    cout << endl;
    system("clear");

    Buku *cur;
    cur = head;

    while (cur != NULL) {
        if (pilihanBuku == cur->namaBuku) {
            cout << "============================================" << endl;
            cout << "\tInfo Buku: " << pilihanBuku << endl;
            cout << "============================================" << endl;

            cout << "Nama Buku: " << cur->namaBuku << endl;
            cout << "Penulis: " << cur->penulis << endl;
            cout << "Tahun Terbit: " << cur->tahunTerbit << endl;
            cout << "Harga: " << cur->harga << " K" << endl;
            cout << "Jumlah Stok: " << cur->stok << endl;
            cout << "Rating: " << cur->rating << endl;

            cout << endl;
            cout << "Sinopsis: " << cur->sinopsis << endl;
        }
        cur = cur->next;
    }
    beliBuku = pilihanBuku;
    return pilihanBuku;
}

string setBeliBuku() {
    cout << "Beli buku apa: ";
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    getline(cin, beliBuku);

    Buku *cur;
    cur = head;

    while (cur != NULL) {
        if (beliBuku == cur->namaBuku) {
            beliBuku = cur->namaBuku;
        }
        cur = cur->next;
    }
    return beliBuku;
}

// Mengurutkan data berdasarkan rating
void sorting() { // sorting menggunakan bubble short
    if (head == nullptr || head->next == nullptr)
        return;

    bool swapped;
    Buku *cur;
    Buku *last = nullptr;

    do {
        swapped = false;
        cur = head;
        while (cur->next != last) { 
            if (cur->rating < cur->next->rating) {
                swap(cur->rating, cur->next->rating);
                swap(cur->namaBuku, cur->next->namaBuku);
                swap(cur->penulis, cur->next->penulis);
                swap(cur->tahunTerbit, cur->next->tahunTerbit);
                swap(cur->harga, cur->next->harga);

                swapped = true;
            }
            cur = cur->next;
        }
        last = cur;
    } while (swapped);
}

// Checkout
void daftarKeranjang() {
    float totHargaPerBuku[5];

    if (i == 0) {
        cout << "Keranjang kosong" << endl;
    }
    for (int j = 0; j < i; j++) {
        totHargaPerBuku[j] = chartHargaBuku[j]  *  chartJumPembelian[j];

        cout << "=================================" << endl;
        cout << "BARANG KE-[" << j + 1 << "]" << endl;
        cout << "Buku: " << chartNamaBuku[j] << endl;
        cout << "Jumlah: " << chartJumPembelian[j] << endl;
        cout << "Harga : " << totHargaPerBuku[j] << " K" << endl;
        cout << "=================================" << endl;
        cout << endl;
        
    }


    cout<<endl;
    cout << "TOTAL BIAYA BUKU ANDA" <<endl;

    int total = 0;
    for(int j = 0; j < i; j++){
        total += totHargaPerBuku[j];
    }

    cout << "Total \t : "<< total << " K" <<endl;
    cout<<endl;
    cin.ignore();

}


void removeFromChart(int value){
    if (value < 1 || value >= i + 1) {
        cout << "Urutan tidak valid" << endl;
        return;
    }

    if(value - 1 != i) {
        for (int j = value - 1; j < i - 1; j++) {
            chartNamaBuku[j] = chartNamaBuku[j + 1];
            chartJumPembelian[j] = chartJumPembelian[j + 1];
            chartHargaBuku[j] = chartHargaBuku[j + 1];
        }
    }

    i--;
    cout << "Data di urutan " << value << " telah dihapus" << endl;
    return;
}




int main() {

    bool loop1 = true;
    init();
    readData();
    

    while (loop1) {
        system("clear");
        int pill;
        cout << "=================================" << endl;
        cout << "Selamat Datang Di Toko Buku Wota" << endl;
        cout << "=================================" << endl;
        cout << "1. Daftar Buku" << endl;
        cout << "2. Exit." << endl;
        cout << "Masukkan Pilihan anda: ";
        cin >> pill;

        if (pill == 1) { // Lihat daftar buku pada menu
            system("clear");

            int pill2;
            bool loop = true;

            while (loop) {
                system("clear");
                sorting();
                
                display_data();
                cout << endl;
                cout << "1. LIHAT DETAIL BUKU" << endl;
                cout << "2. MASUKKAN KERANJANG" << endl;
                cout << "3. LIHAT KERANJANG" << endl;
                cout << "4. KELUAR" << endl;
                cout << endl;

                cout << "Pilihan: ";
                cin >> pill2;
        

                if (pill2 == 1) { //Liat detail buku
                    liatIsiBuku();
                    cout << "Tekan enter untuk melanjutkan";
                    cin.ignore();
                    cin.get();
                } else if (pill2 == 2) { // Masukkan keranjang
                    bool foundDuplicate = false, found = false, stokCukup = true;
                    int tempStok = 0;
                    setBeliBuku();
                    float hargaBuku;

                    cout << "> Judul \t: " << beliBuku << endl;
                    cout << "> Jumlah barang : ";
                    cin >> jumlahBarangBeli;
                    cin.ignore();

                    Buku *cur; // buat ambil data stok
                    cur = head;
                    while (cur != NULL) {
                        if (beliBuku == cur->namaBuku) {
                            found = true;
                            tempStok = cur->stok;
                            hargaBuku = cur->harga;
                            break;
                        }
                        cur = cur->next;
                    }

                    if (!found) { // jika masukin data buku yang tidak ada di list
                        cout << "Buku tidak ditemukan" << endl;
                        cin.ignore();
                        cin.get();
                    } else {
                        for (int j = 0; j < i; j++) { // misal sudah masukin keranjang dan pengen nambah jumlah barang
                            if (beliBuku == chartNamaBuku[j]) {
                                foundDuplicate = true;
                                if (chartJumPembelian[j] + jumlahBarangBeli > tempStok) { 
                                    stokCukup = false;
                                    cout << "Stok tidak mencukupi" << endl;
                                    cin.ignore();
                                    cin.get();
                                    break; // Menghentikan eksekusi loop saat stok tidak mencukupi
                                } else {
                                    chartJumPembelian[j] += jumlahBarangBeli;
                                }
                            }
                        }

                        if (!foundDuplicate) {// jika tidak ada data duplikat
                            if (jumlahBarangBeli > tempStok) {
                                stokCukup = false;
                                cout << "Stok tidak mencukupi" << endl;
                                cin.ignore();
                                //cin.get();
                            } else {
                                chartNamaBuku[i] = beliBuku;
                                chartJumPembelian[i] = jumlahBarangBeli;
                                chartHargaBuku[i] = hargaBuku;

                                i++;
                            }
                        }

                        if (stokCukup) {
                            system("clear");
                            cout << "Buku berhasil dimasukkan ke keranjang" << endl;
                            cin.ignore();
                            //cin.get();
                        }
                    }
                } else if (pill2 == 3) {
                    system("clear");
                    daftarKeranjang();

                    int pill3;

                    cout << "1. CHECKOUT" << endl;
                    cout << "2. REMOVE BUKU DARI KERANJANG" << endl;
                    cout << "3. KEMBALI" << endl;
                    cout << endl;
                    cout << "Pilih: ";
                    cin >> pill3;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    if (pill3 == 1) { // Checkout
                        if (i != 0) {
                            for (int j = 0; j < i; j++) {
                                Buku* cur;
                                cur = head;
                                while (cur != NULL) {
                                    if (chartNamaBuku[j] == cur->namaBuku) {
                                        cur->stok -= chartJumPembelian[j];
                                        break;
                                    }
                                    cur = cur->next;
                                }
                            }

                            i = 0;
                            cout << "Checkout berhasil" << endl;
                            cout << "Tekan enter untuk melanjutkan...";
                            cin.ignore();

                            
                        } else {
                            cout << "Keranjang kosong" << endl;
                            cin.ignore();
                            cin.get();
                        }
                        
                    } else if (pill3 == 2) { // Hapus keranjang
                        int index;
                        cout << "Masukkan nomor buku yang ingin dihapus : ";
                        cin >> index;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');

                        removeFromChart(index);
                        cin.get();
                    } else if (pill3 == 3) { // Kembali ke menu sebelumnya
                        //
                    } else {
                        cout << "Angka yang anda inputkan diluar pilihan" << endl;
                        cin.ignore();
                        cin.get();
                    }

                } else if (pill2 == 4) {
                    loop = false;
                    system("clear");
                    cout << "Tekan enter untuk melanjutkan...";
                    cin.ignore();
                    cin.get();
                } else {
                    cout << "Pilihan anda salah" << endl;
                }
            }
        } else if (pill == 2) {
            loop1 = false;
            system("clear");
            cout << "Tekan enter untuk melanjutkan...";
            cin.ignore();
            cin.get();
        } else {
            cout << "Angka yang anda inputkan diluar pilihan" << endl;
        }
    }
}