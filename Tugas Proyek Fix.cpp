#include <iostream>
#include <vector>
#include <stack>
#include <fstream>
#include <string>
#include <limits>  
#include <sstream> 

using namespace std;

//Bagian Login
struct Pengguna {
    string username;
    string password;

	//Registrasi Akun 
    void registrasiAkunPengguna() {
        Pengguna penggunaBaru;
        cout << "\n=== Registrasi Pengguna ===\n";
        cout << "Username : ";
        cin.ignore(); 
        getline(cin, penggunaBaru.username);
        cout << "Password : ";
        getline(cin, penggunaBaru.password);

        ofstream file("data_pengguna.txt", ios::app);
        if (file.is_open()) {
            file << penggunaBaru.username << " " << penggunaBaru.password << endl;
            file.close();
            cout << "Akun pengguna berhasil dibuat!\n";
        } else {
            cout << "Gagal membuka file untuk menyimpan data pengguna.\n";
        }
    }

	//Login pengguna
    bool loginPengguna() {
        string inputUsername, inputPassword;
        cout << "\n=== Login Pengguna ===\n";
        cout << "Username : ";
        cin.ignore(); // Membersihkan buffer
        getline(cin, inputUsername);
        cout << "Password : ";
        getline(cin, inputPassword);

        ifstream file("data_pengguna.txt");
        if (file.is_open()) {
            string fileUsername, filePassword;
            while (file >> fileUsername >> filePassword) {
                if (fileUsername == inputUsername && filePassword == inputPassword) {
                    file.close();
                    cout << "Login berhasil! Selamat datang, " << inputUsername << ".\n";
                    return true;
                }
            }
            file.close();
        }
        cout << "Login gagal! Username atau password salah.\n";
        return false;
    }
};

//Identitas Motor
class Motor
{
private:
    string nama;
    string platNomor;
    double hargaSewa;
    bool tersedia;

public:
    Motor(string nama, string platNomor, double hargaSewa)
    {
        this->nama = nama;
        this->platNomor = platNomor;
        this->hargaSewa = hargaSewa;
        this->tersedia = true;
    }

    string getNama() const { return nama; }
    string getPlatNomor() const { return platNomor; }
    double getHargaSewa() const { return hargaSewa; }
    bool isTersedia() const { return tersedia; }
    void sewa() { tersedia = false; }
    void kembalikan() { tersedia = true; }
};

//Registrasi Penyewa
struct RegistrasiPenyewaan
{
    string namaPenyewa;
    string motor;
    string platNomor;
    double hargaSewa;
    int jumlahHari;
    double totalBiaya;
};

//Penyewaan Motor
class PenyewaanMotor
{
public:
    void tampilkanMotor();
    void hapusMotor(int indeks);
    void simpanDataMotor();
    void muatDataMotor();
    void registrasiPelanggan();
    void kembalikanMotor(int indeks);
    void tambahMotorKeStack(const Motor &motor);
    void pindahkanMotorDariStack();
    void tampilkanRiwayatPenyewaan();
    bool daftarMotorKosong();
    void tambahMotor(const Motor &motor);

    vector<Motor> daftarMotor;                   // Daftar motor
    vector<RegistrasiPenyewaan> daftarPenyewaan; // Riwayat penyewaan
    stack<Motor> stackMotorBaru;                 // Stack untuk motor baru
};

void PenyewaanMotor::tampilkanMotor()
{
    cout << "\nDaftar Motor:\n";
    for (size_t i = 0; i < daftarMotor.size(); ++i)
    {
        cout << i + 1 << ". " << daftarMotor[i].getNama()
             << " (Plat: " << daftarMotor[i].getPlatNomor()
             << ", Harga Sewa: Rp" << daftarMotor[i].getHargaSewa()
             << ", Tersedia: " << (daftarMotor[i].isTersedia() ? "Ya" : "Tidak") << ")\n";
    }
}

void PenyewaanMotor::hapusMotor(int indeks)
{
    if (indeks < 1 || indeks > daftarMotor.size())
    {
        cout << "Indeks tidak valid! Tidak ada motor pada indeks tersebut.\n";
        return;
    }
    daftarMotor.erase(daftarMotor.begin() + (indeks - 1));
    simpanDataMotor();
    cout << "Motor pada indeks " << indeks << " berhasil dihapus.\n";
}

void PenyewaanMotor::simpanDataMotor()
{
    ofstream file("daftar_motor.txt");
    if (file.is_open())
    {
        for (size_t i = 0; i < daftarMotor.size(); ++i)
        {
            const Motor &motor = daftarMotor[i];
            file << motor.getNama() << ","
                 << motor.getPlatNomor() << ","
                 << motor.getHargaSewa() << ","
                 << (motor.isTersedia() ? "1" : "0") << endl;
        }
        file.close();
        cout << "Data motor berhasil disimpan.\n";
    }
    else
    {
        cout << "Tidak dapat membuka file untuk menyimpan data motor.\n";
    }
}

void PenyewaanMotor::muatDataMotor()
{
    ifstream file("daftar_motor.txt");
    if (file.is_open())
    {
        string line;
        while (getline(file, line))
        {
            size_t pos1 = line.find(",");
            size_t pos2 = line.find(",", pos1 + 1);
            size_t pos3 = line.find(",", pos2 + 1);
            string nama = line.substr(0, pos1);
            string platNomor = line.substr(pos1 + 1, pos2 - pos1 - 1);
            double hargaSewa;
            stringstream(line.substr(pos2 + 1, pos3 - pos2 - 1)) >> hargaSewa;
            bool tersedia = line.substr(pos3 + 1) == "1";

            Motor motor(nama, platNomor, hargaSewa);
            if (tersedia)
                motor.kembalikan();
            daftarMotor.push_back(motor);
        }
        file.close();
    }
    else
    {
        cout << "File daftar_motor.txt tidak ditemukan. Membuat daftar motor baru.\n";
    }
}

void PenyewaanMotor::registrasiPelanggan()
{
    string namaLengkap;
    int indeksMotor, jumlahHari;

    cout << "Masukkan Nama Lengkap: ";
    cin.ignore(); // Clear the input buffer
    getline(cin, namaLengkap);

    tampilkanMotor();

    cout << "Masukkan indeks motor yang ingin disewa: ";
    while (!(cin >> indeksMotor) || indeksMotor < 1 || indeksMotor > daftarMotor.size())
    {
        cin.clear();                                         // Clear the error flag
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer
        cout << "Indeks motor tidak valid, coba lagi: ";
    }

    cout << "Masukkan jumlah hari sewa: ";
    while (!(cin >> jumlahHari) || jumlahHari <= 0)
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Jumlah hari tidak valid, coba lagi: ";
    }

    if (indeksMotor > 0 && indeksMotor <= (int)daftarMotor.size())
    {
        Motor &motor = daftarMotor[indeksMotor - 1];
        if (motor.isTersedia())
        {
            motor.sewa();
            double totalBiaya = motor.getHargaSewa() * jumlahHari;

            RegistrasiPenyewaan penyewaan = {namaLengkap, motor.getNama(), motor.getPlatNomor(), motor.getHargaSewa(), jumlahHari, totalBiaya};
            daftarPenyewaan.push_back(penyewaan);

            ofstream file("registrasi_pelanggan.txt", ios::app);
            file << "Nama Penyewa: " << namaLengkap << endl;
            file << "Motor: " << motor.getNama() << endl;
            file << "Plat Nomor: " << motor.getPlatNomor() << endl;
            file << "Harga Sewa per Hari: Rp" << motor.getHargaSewa() << endl;
            file << "Jumlah Hari: " << jumlahHari << endl;
            file << "Total Biaya: Rp" << totalBiaya << endl;
            file << "------------------------\n";
            file.close();

            cout << "\n--- Detail Penyewaan ---\n";
            cout << "Nama Penyewa: " << namaLengkap << endl;
            cout << "Motor: " << motor.getNama() << endl;
            cout << "Plat Nomor: " << motor.getPlatNomor() << endl;
            cout << "Harga Sewa per Hari: Rp" << motor.getHargaSewa() << endl;
            cout << "Jumlah Hari: " << jumlahHari << endl;
            cout << "Total Biaya: Rp" << totalBiaya << endl;

            cout << "\nMotor berhasil disewa oleh " << namaLengkap << " untuk " << jumlahHari << " hari dengan total biaya Rp" << totalBiaya << "\n";
        }
        else
        {
            cout << "Motor tidak tersedia.\n";
        }
    }
    else
    {
        cout << "Indeks motor tidak valid!\n";
    }
}

void PenyewaanMotor::kembalikanMotor(int indeks)
{
    if (indeks > 0 && indeks <= (int)daftarMotor.size())
    {
        Motor &motor = daftarMotor[indeks - 1];
        if (!motor.isTersedia())
        {
            motor.kembalikan();
            cout << "Motor berhasil dikembalikan.\n";
        }
        else
        {
            cout << "Motor sudah tersedia.\n";
        }
    }
    else
    {
        cout << "Indeks motor tidak valid!\n";
    }
}

void PenyewaanMotor::tambahMotorKeStack(const Motor &motor)
{
    stackMotorBaru.push(motor);
    cout << "Motor berhasil ditambahkan ke stack.\n";
}

void PenyewaanMotor::pindahkanMotorDariStack()
{
    if (!stackMotorBaru.empty())
    {
        Motor motor = stackMotorBaru.top();
        stackMotorBaru.pop();
        daftarMotor.push_back(motor);
        cout << "Motor berhasil dipindahkan dari stack dan ditambahkan ke daftar motor.\n";
    }
    else
    {
        cout << "Tidak ada motor di stack untuk dipindahkan.\n";
    }
}

void PenyewaanMotor::tampilkanRiwayatPenyewaan()
{
    cout << "\nRiwayat Penyewaan:\n";
    if (daftarPenyewaan.empty())
    {
        cout << "Belum ada penyewaan motor.\n";
        return;
    }
    for (size_t i = 0; i < daftarPenyewaan.size(); ++i)
    {
        const RegistrasiPenyewaan &penyewaan = daftarPenyewaan[i];
        cout << "\nNama Penyewa: " << penyewaan.namaPenyewa << endl;
        cout << "Motor: " << penyewaan.motor << endl;
        cout << "Plat Nomor: " << penyewaan.platNomor << endl;
        cout << "Harga Sewa: Rp" << penyewaan.hargaSewa << endl;
        cout << "Jumlah Hari: " << penyewaan.jumlahHari << endl;
        cout << "Total Biaya: Rp" << penyewaan.totalBiaya << endl;
    }
}

bool PenyewaanMotor::daftarMotorKosong()
{
    return daftarMotor.empty();
}

void PenyewaanMotor::tambahMotor(const Motor &motor)
{
    daftarMotor.push_back(motor);
    cout << "Motor berhasil ditambahkan ke daftar motor.\n";
}


int main() {
    PenyewaanMotor penyewaan;
    penyewaan.muatDataMotor(); 
    Pengguna pengguna;

    cout << "=== Selamat Datang di Penyewaan Motor Online Lempuyangab ===\n";
    cout << endl;
    cout << "Silahkan Baca Syarat penyewaan Sebelum melakukan penyewaan : " << endl;
    cout << "1. Penyewa bersedia menjadikan Kartu Indentitas (KTP, SIM A, KTM, dll) sebagai jaminan penyewaan" << endl;
    cout << "2. Penyewa harus Memiliki SIM C" << endl;
    cout << "3. Penyewa Bersedia untuk diambil gambar saat motor diserahkan" << endl;
    cout << "4. Penyewa Bersedia mengganti kerusakan atau kehilangan motor yang di sewa" << endl;
    cout << "5. Penyewa Membayar uang muka minimal 50% dari total biaya sewa" << endl;
    cout << "6. Penyewa Melunasi uang muka pada saat serah terima kendaraan" << endl;
    cout << "7. Penyewa minimal berusia 18 tahun" << endl;
    
    cout << endl;
    cout << "Silahkan Registrasi Akun Terlebih Dahulu" << endl;
    
    int pilihanAwal;
    bool berhasilLogin = false;

    // Pilihan Login atau Registrasi
    do {
        cout << "\n1. Registrasi Akun\n";
        cout << "2. Login Akun\n";
        cout << "Pilih opsi (1/2): ";
        cin >> pilihanAwal;

        switch (pilihanAwal) {
            case 1:
                pengguna.registrasiAkunPengguna();
                break;
            case 2:
                berhasilLogin = pengguna.loginPengguna();
                break;
            default:
                cout << "Pilihan tidak valid. Silakan coba lagi.\n";
        }
    } while (!berhasilLogin);

    int pilihan;
    do
    {
        cout << "\nMenu Penyewaan Motor:\n";
        cout << "1. Tambah Motor\n";
        cout << "2. Tampilkan Daftar Motor\n";
        cout << "3. Hapus Motor\n";
        cout << "4. Registrasi Penyewaan\n";
        cout << "5. Kembalikan Motor\n";
        cout << "6. Tampilkan Riwayat Penyewaan\n";
        cout << "7. Keluar\n";
        cout << "Masukan pilihan: ";
        cin >> pilihan;

        switch (pilihan)
        {
        case 1:
        {
            string nama, platNomor;
            double hargaSewa;
            cout << "Masukkan Nama Motor: ";
            cin.ignore(); // Clear buffer
            getline(cin, nama);
            cout << "Masukkan Plat Nomor Motor: ";
            getline(cin, platNomor);
            cout << "Masukkan Harga Sewa per Hari: ";
            cin >> hargaSewa;
            Motor motor(nama, platNomor, hargaSewa);
            penyewaan.tambahMotor(motor);
            penyewaan.simpanDataMotor();
            break;
        }
        case 2:
            penyewaan.tampilkanMotor();
            break;
        case 3:
        {
            int indeks;
            cout << "Masukkan indeks motor yang ingin dihapus: ";
            cin >> indeks;
            penyewaan.hapusMotor(indeks);
            break;
        }
        case 4:
            penyewaan.registrasiPelanggan();
            break;
        case 5:
        {
            int indeks;
            cout << "Masukkan indeks motor yang ingin dikembalikan: ";
            cin >> indeks;
            penyewaan.kembalikanMotor(indeks);
            break;
        }
        case 6:
            penyewaan.tampilkanRiwayatPenyewaan();
            break;
        case 7:
            cout << "Terima kasih telah menggunakan sistem penyewaan motor!\n";
            break;
        default:
            cout << "Pilihan tidak valid. Coba lagi.\n";
        }
    } while (pilihan != 7);
}
