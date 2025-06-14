#include <iostream>
#include <conio.h>
#include <vector>
#include <string>
#include <cstring>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <random>
#include <ctime>
using namespace std;

#define MAKS_INAP 10
#define YEAR 2023

// CLASSESS
class Pelanggan
{
public:
    char id_pelanggan[12];
    string nama;
    string email;
    string alamat;
    char no_telepon[12];
    char ktp[16];
    char kelamin;
    int umur;
};

class TipeKamar
{
public:
    string nama_tipe;
    float harga;
    bool ac;
    bool heater;
    string tipe_kasur;
};

class Kamar
{
public:
    TipeKamar tipe;
    int no_kamar;
    bool status;
};

class MetodeBayar
{
public:
    double tax;
    string nama_metode;
};

class Booking
{
private:
    char id_booking[8];

public:
    void setIdBooking(const char *id)
    {
        strncpy(id_booking, id, sizeof(id_booking - 1));
        id_booking[sizeof(id_booking) - 1] = '\0';
    }
    const char *getIdBooking() const
    {
        return id_booking;
    }
    int no_kamar;
    string pemesan;
    int start_date;
    int end_date;
    string metode_bayar;
    int jml_pelanggan;
    double total_amount;
};

// GLOBAL VARIABLE
MetodeBayar method_bayar;
double total;
bool isRoomFound = false;
bool isOrderRoom = false;
double jumlah_bayar;
int nP;
int tipe, nomor_kamar;
int tanggal_mulai, tanggal_selesai, bulan_mulai, bulan_selesai;
unsigned int durasi_inap;
string logged_user;
vector<Pelanggan> list_pelanggan;
vector<Kamar> list_kamar;
vector<Kamar> filtered_kamar;
vector<string> list_admin = {"Billy", "Joel", "Noel", "Ocim"};
vector<TipeKamar> list_tipeKamar = {
    {"Standart", 140.0, true, false, "Single"},
    {"Deluxe", 220.4, true, true, "Double"},
    {"President Suite", 320.5, true, true, "King Size"},
};
vector<string> dd{"01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31"};
vector<string> mmm{"JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};
vector<MetodeBayar> list_methodBayar{
    {10.0, "DANA"},
    {8.0, "OVO"},
    {5.0, "TRANSFER BANK"},
    {2.0, "CASH"},
};
vector<Booking> list_historyBooking;

class ManajemenPelanggan
{
private:
    ofstream file_pelanggan_of;
    ifstream file_pelanggan_if;
    const string file_pelanggan_path = "data_pelanggan.txt";

public:
    void verifikasiUsia();
    void formIdentitas(int nP);
    void jumlahPelanggan();
    void storeDataUser(vector<Pelanggan> list_pelanggan);
};

class ManajemenKamar
{
private:
    ofstream file_kamar_of;
    ifstream file_kamar_if;
    const string file_kamar_path = "data_kamar.txt";

public:
    void tambahKamar();
    void bookingKamar();
    void listFiturKamar();
    void getAllOneTypeDataKamar(vector<Kamar> list_kamar, string nama_tipe);
    void getAllDataKamar();
    class Kamar getDataKamar(int nomor_kamar);
    void listTipeKamar(vector<TipeKamar> list_tipeKamar);
    void storeDataKamar(vector<Kamar> list_kamar);
    vector<Kamar> filterNomorKamar(vector<Kamar> &list_kamar, string &nama_tipe);
    unsigned int durasiMenginap();
    void prosesPembayaran();
    class MetodeBayar pilihMetodeBayar(vector<MetodeBayar> list_methodBayar);
    void ringkasanPembayaran();
    class Kamar prosesCariDataKamar(int rno);
    void cariDataKamar();
};

class ManajemenBooking : public Booking
{
private:
    ofstream file_booking_of;
    ifstream file_booking_if;
    const string file_booking_path = "data_history_booking.txt";

public:
    void cekHistoryBooking();
    void isiDataBooking(int start_date, int end_date, Kamar kamar, double total, vector<Pelanggan> list_pelanggan);
    void storeDataBooking(vector<Booking> list_historyBooking);
    vector<Booking> getAllDataBooking();
    void displayHistoryBooking(vector<Booking> list_booking);
};

struct Invoice
{
    const char *num_invoice[8];
    int lama_inap;
    int nomor_kamar;
    int base_kamar_harga;
    double tax;
    int jml_penginap;
    double total;
};

// FUNGSI PROTOTIPE
void welcome();
void proses();
void adminDashboard();
bool checkAdmin(string name);
string genRandom(const int len);
double hitungTotal(double tax, double amount, int durasi_inap);
void cetakInvoice();
void promoKamar();

int main()
{
    ManajemenPelanggan m_p;
    ManajemenKamar m_k;
    ManajemenBooking m_b;
    int option;

    welcome();

    cout << "	Masukkan nama anda : ";
    cin >> logged_user;

    if (!checkAdmin(logged_user))
    {
        do
        {
            proses();
            cout << endl;

            cout << "	Masukkan pilihan anda : ";
            cin >> option;

            switch (option)
            {
            case 1:
                m_p.verifikasiUsia();
                getch();
                m_p.jumlahPelanggan();
                m_p.formIdentitas(nP);
                m_k.bookingKamar();
                getch();
                break;
            case 2:
                m_k.listFiturKamar();
                getch();
                break;
            case 3:
                promoKamar();
                getch();
                break;
            case 4:
                if (!isOrderRoom)
                {
                    cout << "\nGagal cetak invoice" << endl;
                    cout << "Anda harus melakukan pemesanan kamar terlebih dahulu." << endl;
                    getch();
                }
                else
                {
                    cetakInvoice();
                    getch();
                }
                break;
            default:
                break;
            }
        } while (option != 5);
    }
    else
    {
        do
        {
            adminDashboard();
            cout << endl;
            cout << "	Masukkan pilihan anda : ";
            cin >> option;

            switch (option)
            {
            case 1:
                m_k.cariDataKamar();
                getch();
                break;
            case 2:
                m_k.tambahKamar();
                break;
            case 3:
                m_b.cekHistoryBooking();
                getch();
                break;
            default:
                break;
            }
        } while (option != 5);
    }

    return 0;
}

void welcome()
{
    system("cls");
    cout << endl;
    cout << "                           ========================" << endl;
    cout << "                 ===========================================" << endl;
    cout << "        ==============================================================" << endl;
    cout << "================================ THE VASA HOTEL  =========================== " << endl;
    cout << "                Jl. Mayjen HR. Muhammad No.31, Putat Gede" << endl;
    cout << "                  Kec. Sukomanunggal, Surabaya, Jawa Timur 60189" << endl;
    cout << "===============================================================================" << endl;
    cout << "===============================================================================" << endl;
    cout << endl;
}

void proses()
{
    system("cls");
    welcome();
    cout << "                     ====================================" << endl;
    cout << "                     |            Halaman Utama          |" << endl;
    cout << "                     ====================================" << endl;
    cout << endl;
    cout << "	1. Booking Kamar " << endl;
    cout << "	2. Fasilitas Kamar " << endl;
    cout << "	3. Promo Menarik " << endl;
    cout << "	4. Cetak Invoice " << endl;
    cout << "	5. Exit Program " << endl;
}

void adminDashboard()
{
    system("cls");
    welcome();
    cout << "                     ====================================" << endl;
    cout << "                     |            Dashboard Admin        |" << endl;
    cout << "                     ====================================" << endl;
    cout << endl;
    cout << "	Selamat Datang Admin - " << logged_user << endl;
    cout << endl;
    cout << "	1. Cari Data Kamar " << endl;
    cout << "	2. Tambah Data Kamar " << endl;
    cout << "	3. History Booking " << endl;
    cout << "	5. Exit Program " << endl;
}

bool checkAdmin(string name)
{
    bool is_admin = false;
    for (auto p : list_admin)
    {
        if (p.find(name) != string::npos)
        {
            is_admin = true;
            break;
        }
    }

    return is_admin;
}

string genRandom(int length)
{
    mt19937 generator(static_cast<unsigned int>(time(0)));

    const string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

    uniform_int_distribution<> distribution(0, characters.size() - 1);

    string random_str;
    for (int i = 0; i < length; ++i)
    {
        random_str += characters[distribution(generator)];
    }

    return random_str;
}

void ManajemenPelanggan::verifikasiUsia()
{
    system("cls");
    welcome();
    cout << "                     ====================================" << endl;
    cout << "                     |           Verifikasi Usia         |" << endl;
    cout << "                     ====================================" << endl;
    cout << endl;

    int verif_usia;
    cout << "Masukkan usia anda: ";
    cin >> verif_usia;

    if (verif_usia < 18)
    {
        cout << "Mohon maaf umur anda belum mencukupi untuk bisa mengakses fitur ini!!" << endl;
        getch();
        exit(0);
    }
    else
    {
        cout << "Selamat! Anda dapat mengakses fitur ini, tekan apapun untuk melanjutkan proses booking hotel" << endl;
    }
}

void ManajemenPelanggan::jumlahPelanggan()
{
    system("cls");
    welcome();
    cout << "                     ====================================" << endl;
    cout << "                     |           Jumlah Pelanggan        |" << endl;
    cout << "                     ====================================" << endl;
    cout << endl;

    cout << "Berapa jumlah orang yang akan menginap? (Maks 4 orang): ";
    cin >> nP;
}

void ManajemenPelanggan::formIdentitas(int nP)
{
    Pelanggan new_pelanggan;
    ManajemenPelanggan m_pelanggan;
    string random_id = genRandom(10);

    for (int i = 0; i < nP; ++i)
    {
        cout << endl;
        strcpy(new_pelanggan.id_pelanggan, random_id.c_str());
        cout << "Identitas orang ke " << i + 1 << endl;
        cout << "Nama              :";
        cin.ignore();
        getline(cin, new_pelanggan.nama);
        cout << "Email             :";
        getline(cin, new_pelanggan.email);
        cout << "Alamat            :";
        getline(cin, new_pelanggan.alamat);
        cout << "No Telepon        :";
        cin >> new_pelanggan.no_telepon;
        cout << "KTP               :";
        cin >> new_pelanggan.ktp;
        cout << "Kelamin (L/P)     :";
        cin >> new_pelanggan.kelamin;
        cout << "Usia              :";
        cin >> new_pelanggan.umur;

        list_pelanggan.push_back(new_pelanggan);
    }

    m_pelanggan.storeDataUser(list_pelanggan);
}

void ManajemenPelanggan::storeDataUser(vector<Pelanggan> list_pelanggan)
{
    file_pelanggan_of.open(file_pelanggan_path, ios::app);

    if (file_pelanggan_of.is_open())
    {
        for (const auto pelanggan : list_pelanggan)
        {
            file_pelanggan_of << "\n"
                              << pelanggan.id_pelanggan << "\t"
                              << pelanggan.nama << "\t"
                              << pelanggan.email << "\t"
                              << pelanggan.alamat << "\t\t"
                              << pelanggan.kelamin << "\t\t\t\t\t"
                              << pelanggan.ktp << "\t\t\t"
                              << pelanggan.no_telepon;
        }

        cout << "\nData user berhasil ditambahkan!" << endl;
        file_pelanggan_of.close();
    }
    else
    {
        cout << "\nTejadi kesalahan ketika membuka file!" << endl;
    }
}

void ManajemenKamar::tambahKamar()
{

    Kamar new_kamar;
    ManajemenKamar m_kamar;
    int opt_tipeKamar;

    system("cls");
    welcome();
    cout << "                     ====================================" << endl;
    cout << "                     |           Tambah Data Kamar       |" << endl;
    cout << "                     ====================================" << endl;
    cout << endl;

    cout << "List Tipe Kamar   :" << endl;
    for (int i = 0; i < list_tipeKamar.size(); i++)
    {
        cout << i + 1 << ". " << list_tipeKamar[i].nama_tipe;
        cout << endl;
    }
    cout << endl;
    cout << "Pilih Tipe Kamar  :";
    cin >> opt_tipeKamar;

    if (opt_tipeKamar >= 1 && opt_tipeKamar <= list_tipeKamar.size())
    {
        const TipeKamar tipeKamar_terpilih = list_tipeKamar[opt_tipeKamar - 1];

        cout << "Tipe Kamar        :" << tipeKamar_terpilih.nama_tipe;
        new_kamar.tipe = tipeKamar_terpilih;
        cout << "\nNomor Kamar       :";
        cin >> new_kamar.no_kamar;
        cout << "Status Kamar      :" << "Tersedia ";
        new_kamar.status = true;

        list_kamar.push_back(new_kamar);

        m_kamar.storeDataKamar(list_kamar);
    }
    else
    {
        cout << "\nOpsi tidak sesuai!";
    }
}

void ManajemenKamar::storeDataKamar(vector<Kamar> list_kamar)
{
    file_kamar_of.open(file_kamar_path, ios::app);

    if (file_kamar_of.is_open())
    {
        for (const auto kamar : list_kamar)
        {
            file_kamar_of << "\n"
                          << kamar.tipe.nama_tipe << "\t\t\t"
                          << kamar.no_kamar << "\t\t\t\t"
                          << kamar.status;
        }

        cout << "\nData kamar berhasil ditambahkan!" << endl;
        file_kamar_of.close();
    }
    else
    {
        cout << "\nTejadi kesalahan ketika membuka file!" << endl;
    }
}

void ManajemenKamar::listTipeKamar(vector<TipeKamar> list_tipeKamar)
{
    cout << "                     ====================================" << endl;
    cout << "                     |         Pilihan Tipe Kamar        |" << endl;
    cout << "                     ====================================" << endl;
    cout << endl;
    cout << "Tipe-Tipe Kamar  : \n";
    for (int i = 0; i < list_tipeKamar.size(); ++i)
    {
        cout << i + 1 << ". " << list_tipeKamar[i].nama_tipe << "                               " << " $" << list_tipeKamar[i].harga << endl;
    }
}

vector<Kamar> ManajemenKamar::filterNomorKamar(vector<Kamar> &list_kamar, string &nama_tipe)
{

    vector<Kamar> results;

    copy_if(list_kamar.begin(), list_kamar.end(), back_inserter(results),
            [&nama_tipe](const Kamar &kamar)
            {
                return kamar.tipe.nama_tipe == nama_tipe;
            });

    return results;
}

void ManajemenKamar::getAllOneTypeDataKamar(vector<Kamar> list_kamar, string nama_tipe)
{

    cout << "                     ======================================" << endl;
    cout << "                     |             Tipe " << nama_tipe << "        |" << endl;
    cout << "                     ======================================" << endl;
    cout << endl;
    file_kamar_if.open(file_kamar_path);

    Kamar kamar;

    if (file_kamar_if.is_open())
    {
        string line;
        string delimiter = "==============================================";
        string tipe;
        int no;
        int status;

        Kamar kamar;

        while (getline(file_kamar_if, line) && line != delimiter)
        {
        }

        while (getline(file_kamar_if, line))
        {
            istringstream iss(line);
            getline(iss, tipe, '\t');
            iss >> no >> status;

            kamar.no_kamar = no;
            kamar.status = (status == 1 ? true : false);
            kamar.tipe = (tipe == "Standart" ? list_tipeKamar[0] : tipe == "President Suite" ? list_tipeKamar[2]
                                                                                             : list_tipeKamar[1]);

            list_kamar.push_back(kamar);
        }

        filtered_kamar = filterNomorKamar(list_kamar, nama_tipe);

        file_kamar_if.close();
    }
    else
    {
        cout << "\nTejadi kesalahan ketika membuka file!" << endl;
    }
}

void ManajemenKamar::getAllDataKamar()
{
    file_kamar_if.open(file_kamar_path);

    Kamar kamar;

    if (file_kamar_if.is_open())
    {
        string line;
        string delimiter = "==============================================";
        string tipe;
        int no;
        int status;

        Kamar kamar;

        while (getline(file_kamar_if, line) && line != delimiter)
        {
        }

        while (getline(file_kamar_if, line))
        {
            istringstream iss(line);
            getline(iss, tipe, '\t');
            iss >> no >> status;

            kamar.no_kamar = no;
            kamar.status = (status == 1 ? true : false);
            kamar.tipe = (tipe == "Standart" ? list_tipeKamar[0] : tipe == "President Suite" ? list_tipeKamar[2]
                                                                                             : list_tipeKamar[1]);

            list_kamar.push_back(kamar);
        }

        file_kamar_if.close();
    }
    else
    {
        cout << "\nTejadi kesalahan ketika membuka file!" << endl;
    }
}

Kamar ManajemenKamar::getDataKamar(int no_kamar)
{
    Kamar selected;

    for (auto kamar : filtered_kamar)
    {
        if (kamar.no_kamar == no_kamar)
        {
            selected.no_kamar = kamar.no_kamar;
            selected.status = kamar.status;
            selected.tipe = kamar.tipe;
        }
    }

    return selected;
}

unsigned int ManajemenKamar::durasiMenginap()
{
    system("cls");
    welcome();
    cout << endl;

    cout << "Tanggal mulai: ";
    cin >> tanggal_mulai;
    cout << "Bulan mulai: ";
    cin >> bulan_mulai;
    cout << "Tanggal akhir: ";
    cin >> tanggal_selesai;
    cout << "Bulan akhir: ";
    cin >> bulan_selesai;

    if (tanggal_mulai > dd.size() && tanggal_selesai > dd.size())
    {
        cout << "\nInvalid tanggal" << endl;
    }
    else if (bulan_mulai > mmm.size() && bulan_selesai > mmm.size())
    {
        cout << "\nInvalid bulan" << endl;
    }

    unsigned int differ_day = tanggal_selesai - tanggal_mulai;

    return differ_day;
}

void ManajemenKamar::bookingKamar()
{
    TipeKamar tp_kamar;

    system("cls");
    welcome();
    listTipeKamar(list_tipeKamar);
    cout << "Silahkan Pilih tipe kamar : ";
    cin >> tipe;
    cout << endl;

    if (tipe >= 1 && tipe <= list_tipeKamar.size())
    {
        tp_kamar = list_tipeKamar[tipe - 1];

        system("cls");
        welcome();
        getAllOneTypeDataKamar(list_kamar, tp_kamar.nama_tipe);
        for (auto kamar : filtered_kamar)
            cout << "No. " << kamar.no_kamar << "\t";
        cout << endl;
        cout << endl;
        cout << "Silahkan pilih nomor kamar : ";
        cin >> nomor_kamar;

        durasi_inap = durasiMenginap();

        cout << "===============================" << endl;

        cout << "\nAnda akan menginap selama " << durasi_inap << " hari." << endl;
        isOrderRoom = true;
        getch();
        prosesPembayaran();
    }
    else
    {
        cout << "\nTipe Kamar tersebut tidak ada.";
    }
}

Kamar ManajemenKamar::prosesCariDataKamar(int rno)
{
    Kamar found_kamar;

    getAllDataKamar();

    if (list_kamar.size() > 1)
    {
        for (const auto &kamar : list_kamar)
        {
            if (kamar.no_kamar == rno)
            {
                found_kamar = kamar;
                isRoomFound = true;
            }
        }
    }

    return found_kamar;
}

void ManajemenKamar::cariDataKamar()
{
    Kamar kamar;
    int rno;

    system("cls");
    welcome();
    cout << "                     ====================================" << endl;
    cout << "                     |            Cari Data Kamar        |" << endl;
    cout << "                     ====================================" << endl;
    cout << endl;
    cout << "	Masukkan nomor kamar : ";
    cin >> rno;

    kamar = prosesCariDataKamar(rno);

    if (isRoomFound)
    {
        cout << "Kamar ditemukan." << endl;

        cout << "	- No. Kamar :" << kamar.no_kamar << endl;
        cout << "	- Tipe      :" << kamar.tipe.nama_tipe << endl;
        cout << "	- Fasilitas :" << endl;
        cout << "	        AC(Y/N)     :" << (kamar.tipe.ac ? "Y" : "N") << endl;
        cout << "	        Heater(Y/N) :" << (kamar.tipe.heater ? "Y" : "N") << endl;
        cout << "	        Tipe Kasur  :" << kamar.tipe.tipe_kasur << endl;
        cout << "	- Harga     : $" << kamar.tipe.harga << endl;
        cout << "	- Status    :" << kamar.status << endl;
    }
    else
    {
        cout << "Kamar dengan nomor " << rno << " tidak ditemukan." << endl;
    }
}

MetodeBayar ManajemenKamar::pilihMetodeBayar(vector<MetodeBayar> list_methodBayar)
{
    int opt;
    string method_str;
    MetodeBayar method;

    for (int i = 0; i < list_methodBayar.size(); ++i)
    {
        cout << i + 1 << ". " << list_methodBayar[i].nama_metode << endl;
    }
    cout << "Pilih metode pembayaran: ";
    cin >> opt;

    if (opt >= 1 && opt <= list_methodBayar.size())
    {
        method_str = list_methodBayar[opt - 1].nama_metode;
    }
    else
    {
        cout << "\nInvalid input metode pembayaran." << endl;
    }

    for (const auto mp : list_methodBayar)
    {
        if (mp.nama_metode.find(method_str) != string::npos)
        {
            method.nama_metode = mp.nama_metode;
            method.tax = mp.tax;
        }
    }

    return method;
}

void ManajemenKamar::prosesPembayaran()
{
    char choice;
    char bayar_lunas;
    ManajemenBooking m_b;
    Kamar kamar;

    do
    {
        system("cls");
        welcome();
        cout << "                     ====================================" << endl;
        cout << "                     |             Pembayaran           |" << endl;
        cout << "                     ====================================" << endl;
        cout << endl;

        method_bayar = pilihMetodeBayar(list_methodBayar);
        cout << "\nAnda memilih metode pembayaran berupa " << method_bayar.nama_metode
             << " , dengan biaya pajak sebesar: " << static_cast<double>(method_bayar.tax) << "%" << endl;
        getch();

        ringkasanPembayaran();
        cout << endl;

        cout << "\nMasukkan nominal pembayaran anda: $";
        cin >> jumlah_bayar;

        if (total > jumlah_bayar)
        {
            cout << endl;
            cout << "Pembayaranmu BELUM cukup, dan masih tersisa $" << (total - jumlah_bayar) << endl;
            cout << "Apakah anda ingin melakukan pelunasan secara langsung? (Y/N): ";
            cin >> bayar_lunas;
            if (bayar_lunas == 'y' || bayar_lunas == 'Y')
            {
                cout << "\nMasukkan nominal pembayaran anda: $";
                cin >> jumlah_bayar;
            }
            else
            {
                cout << "Pelunasan pembayaran akan dilakukan ketika anda check-in pada  " << tanggal_mulai << " " << bulan_mulai << " " << YEAR;
            }
            getch();
        }
        else if (jumlah_bayar == total)
        {
            cout << endl;
            cout << "Pembayaran anda LUNAS." << endl;
            cout << "Anda dapat langsung melakukan check-in pada" << tanggal_mulai << " " << bulan_mulai << " " << YEAR;
            getch();
        }
        else if (jumlah_bayar > total)
        {
            cout << endl;
            cout << "Pembayaran anda LUNAS. Dengan kembalian sebesar $" << (jumlah_bayar - total) << endl;
            cout << "Anda dapat langsung melakukan check-in pada" << tanggal_mulai << " " << bulan_mulai << " " << YEAR;
            getch();
        }

        kamar = getDataKamar(nomor_kamar);

        m_b.isiDataBooking(tanggal_mulai, tanggal_selesai, kamar, total, list_pelanggan);
        m_b.storeDataBooking(list_historyBooking);

    } while (choice == 'y' && choice == 'Y');
}

void ManajemenKamar::ringkasanPembayaran()
{
    Kamar kamar;

    system("cls");
    welcome();
    cout << "                     ====================================" << endl;
    cout << "                     |         Ringkasan Pembayaran      |" << endl;
    cout << "                     ====================================" << endl;
    cout << endl;

    kamar = getDataKamar(nomor_kamar);

    cout << "RINCIAN KAMAR - " << endl;
    cout << endl;
    cout << "	Lama Inap      : " << durasi_inap << " hari." << endl;
    cout << "	Nomor          : " << kamar.no_kamar << endl;
    cout << "	Tipe           : " << kamar.tipe.nama_tipe << endl;
    cout << "	Harga          : $" << kamar.tipe.harga << endl;
    cout << "	Fasilitas  -    " << endl;
    cout << "   \t\tAC (Y/N)       : " << (kamar.tipe.ac ? "Y" : "N") << endl;
    cout << "   \t\tHeater (Y/N)   : " << (kamar.tipe.heater ? "Y" : "N") << endl;
    cout << "   \t\tTipe Kasur     : " << kamar.tipe.tipe_kasur << endl;
    cout << "	Metode Bayar   : " << method_bayar.nama_metode << endl;
    cout << "	Pajak          : " << method_bayar.tax << "%" << endl;

    total = hitungTotal(method_bayar.tax, kamar.tipe.harga, durasi_inap);

    cout << "	Total biaya    : $" << total << endl;
}

double hitungTotal(double tax, double amount, int durasi_inap)
{
    double amount_total = 0.0;

    amount_total += amount * durasi_inap;
    amount_total += (amount_total * (tax / 100.0));

    return amount_total;
}

// ROHIM - DISPLAY FITUR KAMAR
void ManajemenKamar::listFiturKamar()
{
    system("cls");
    welcome();
    cout << "                     ====================================" << endl;
    cout << "                     |          Fitur-Fitur Kamar        |" << endl;
    cout << "                     ====================================" << endl;
    cout << endl;

    for (int i = 0; i < list_tipeKamar.size(); ++i)
    {
        cout << "Tipe Kamar: " << list_tipeKamar[i].nama_tipe << endl;
        cout << "Harga: Rp." << list_tipeKamar[i].harga << endl;
        cout << "Fasilitas: " << endl;
        cout << "- AC: " << (list_tipeKamar[i].ac ? "Tersedia" : "Tidak Tersedia") << endl;
        cout << "- Heater: " << (list_tipeKamar[i].heater ? "Tersedia" : "Tidak Tersedia") << endl;
        cout << "- Tipe Kasur: " << list_tipeKamar[i].tipe_kasur << endl;
        cout << "---------------------------------------" << endl;
    }
}

void ManajemenBooking::isiDataBooking(int start_date, int end_date, Kamar kamar, double total, vector<Pelanggan> list_pelanggan)
{
    Booking booking;

    setIdBooking(genRandom(12).c_str());

    booking.no_kamar = kamar.no_kamar;
    booking.pemesan = logged_user;
    booking.start_date = start_date;
    booking.end_date = end_date;
    booking.metode_bayar = method_bayar.nama_metode;
    booking.jml_pelanggan = list_pelanggan.size();
    booking.total_amount = total;

    list_historyBooking.push_back(booking);
}

void ManajemenBooking::storeDataBooking(vector<Booking> list_historyBooking)
{
    file_booking_of.open(file_booking_path, ios::app);

    const char *booking_id = getIdBooking();

    if (file_booking_of.is_open())
    {
        for (const auto history : list_historyBooking)
        {
            file_booking_of << "\n"
                            << booking_id << "\t\t\t"
                            << history.start_date << "\t\t\t\t\t "
                            << history.end_date << "\t\t\t\t\t  "
                            << history.no_kamar << "\t\t\t"
                            << history.total_amount << "\t\t"
                            << history.pemesan << "\t\t"
                            << history.jml_pelanggan << "\t\t";
        }

        cout << "\nData booking berhasil ditambahkan!" << endl;
        file_booking_of.close();
    }
    else
    {
        cout << "\nTejadi kesalahan ketika membuka file!" << endl;
    }
}

vector<Booking> ManajemenBooking::getAllDataBooking()
{
    vector<Booking> list_historyBooking;
    Booking booking;

    file_booking_if.open(file_booking_path);

    if (file_booking_if.is_open())
    {
        string line;
        string id_booking;
        int tgl_mulai;
        int tgl_selesai;
        int no_kamar;
        double total;
        string pemesan;
        int jml_pelanggan;

        while (getline(file_booking_if, line))
        {
            istringstream iss(line);
            iss >> id_booking >> tgl_mulai >> tgl_selesai >> no_kamar >> total >> pemesan >> jml_pelanggan;

            booking.setIdBooking(id_booking.c_str());
            booking.start_date = tgl_mulai;
            booking.end_date = tgl_selesai;
            booking.no_kamar = no_kamar;
            booking.total_amount = total;
            booking.pemesan = pemesan;
            booking.jml_pelanggan = jml_pelanggan;

            list_historyBooking.push_back(booking);
        }
    }
    else
    {
        cout << "\nTejadi kesalahan ketika membuka file!" << endl;
    }

    return list_historyBooking;
}

void ManajemenBooking::cekHistoryBooking()
{
    system("cls");
    welcome();
    cout << "                     ====================================" << endl;
    cout << "                     |           History Booking         |" << endl;
    cout << "                     ====================================" << endl;
    cout << endl;

    vector<Booking> list_booking = getAllDataBooking();

    displayHistoryBooking(list_booking);
}

void ManajemenBooking::displayHistoryBooking(vector<Booking> list_booking)
{
    for (const auto &booking : list_booking)
    {
        cout << "\n=========================" << endl;
        cout << "ID                  : " << booking.getIdBooking() << endl;
        cout << "Tanggal Mulai       : " << booking.start_date << endl;
        cout << "Tanggal Selesai     : " << booking.end_date << endl;
        cout << "Nomor Kamar         : " << booking.no_kamar << endl;
        cout << "Nama Pemesan        : " << booking.pemesan << endl;
        cout << "Jumlah Pelanggan    : " << booking.jml_pelanggan << endl;
    }
}

void cetakInvoice()
{
    ManajemenBooking m_bk;
    ManajemenKamar m_k;
    Kamar kamar;
    vector<Booking> list_booking = m_bk.getAllDataBooking();
    Booking last_book = list_booking[list_booking.size() - 1];

    kamar = m_k.prosesCariDataKamar(last_book.no_kamar);

    Invoice invoice;
    const char *id_invoice = last_book.getIdBooking();
    invoice.lama_inap = (last_book.end_date - last_book.start_date);
    invoice.nomor_kamar = last_book.no_kamar;
    invoice.base_kamar_harga = kamar.tipe.harga;
    invoice.tax = method_bayar.tax;
    invoice.jml_penginap = last_book.jml_pelanggan;
    invoice.total = total;

    system("cls");
    welcome();
    cout << "============================== Invoice ==============================" << endl;
    cout << "Nomor Invoice: " << id_invoice << endl;
    cout << "Pemesan: " << logged_user << endl;
    cout << "---------------------------------------------------------------------" << endl;

    cout << setw(30) << left << "No. Kamar"
         << setw(10) << right << "Jumlah Penginap"
         << setw(10) << right << "Harga Kamar($)"
         << setw(10) << right << "Lama Inap"
         << setw(10) << right << "Pajak(%)"
         << setw(15) << right << "Total" << endl;

    cout << "---------------------------------------------------------------------" << endl;

    cout << setw(30) << left << invoice.nomor_kamar
         << setw(10) << right << invoice.jml_penginap
         << setw(10) << right << invoice.base_kamar_harga
         << setw(10) << right << invoice.lama_inap
         << setw(10) << right << invoice.tax
         << setw(15) << right << invoice.total << endl;

    cout << "---------------------------------------------------------------------" << endl;

    cout << setw(55) << left << "Total amount"
         << setw(15) << right << fixed << setprecision(2) << invoice.total;
}

// NOEL - DISPLAY MENU PROMO
void promoKamar()
{
    system("cls");
    welcome();
    cout << "                    ==============================================\n";
    cout << "                    |                Promo Menarik               |\n";
    cout << "                    ==============================================\n\n";
    cout << "Dapatkan berbagai hadiah menarik apabila total booking mencapai ketentuan berikut:\n";
    cout << "1. Total booking mencapat RP. 50.000.000: Free 1 unit iPhone X\n";
    cout << "2. Total booking mencapai Rp. 40.000.000: Free 1 tiket Garuda first class Jakarta-Amsterdam PP\n";
    cout << "3. Total booking mencapai Rp. 30.000.000: Free 1 unit iPhone 6s\n";
}