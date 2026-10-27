#include <iostream>
#include <string>
#include <cctype>
#include <fstream>
#include <ctime>

using namespace std;

// Haris
// Haris
// ============ data ============

const int MAX_USERS = 100;
const int MAX_FOUND = 200;
const int MAX_LOST = 200;
const int MAX_CLAIMS = 400;
const int MAX_LOGIN_ATTEMPTS = 3;
const int MAX_CLAIM_ATTEMPTS = 3;
const int MIN_PASSWORD_LENGTH = 6;

const std::string STATUS_AVAILABLE = "Tersedia";
const std::string STATUS_RETURNED = "Dikembalikan";

const int REG_OK = 0;
const int REG_EMPTY_FIELD = 1;
const int REG_USERNAME_HAS_SPACE = 2;
const int REG_DUPLICATE = 3;
const int REG_PASSWORD_SHORT = 4;
const int REG_PHONE_INVALID = 5;
const int REG_FULL = 6;

const int LOGIN_NOT_FOUND = -1;
const int LOGIN_WRONG_PASSWORD = -2;
const int LOGIN_LOCKED = -3;

const int ITEM_OK = 0;
const int ITEM_EMPTY_FIELD = 1;
const int ITEM_INVALID_DATE = 2;
const int ITEM_FUTURE_DATE = 3;
const int ITEM_FULL = 4;

const int CLAIM_APPROVED = 0;
const int CLAIM_WRONG_DETAIL = 1;
const int CLAIM_TOO_MANY_ATTEMPTS = 2;
const int CLAIM_ITEM_NOT_FOUND = 3;
const int CLAIM_NOT_AVAILABLE = 4;
const int CLAIM_OWN_ITEM = 5;
const int CLAIM_STORAGE_FULL = 6;

std::string userName[MAX_USERS], userPassword[MAX_USERS], userPhone[MAX_USERS];
int userFailed[MAX_USERS];
bool userLocked[MAX_USERS];
int userCount = 0;

int foundReporter[MAX_FOUND];
std::string foundName[MAX_FOUND], foundCategory[MAX_FOUND],
    foundLocation[MAX_FOUND], foundDate[MAX_FOUND], foundDescription[MAX_FOUND],
    foundSecret[MAX_FOUND], foundStatus[MAX_FOUND];
int foundCount = 0;

int lostReporter[MAX_LOST];
std::string lostName[MAX_LOST], lostCategory[MAX_LOST], lostLocation[MAX_LOST],
    lostDate[MAX_LOST], lostDescription[MAX_LOST];
int lostCount = 0;

int claimItem[MAX_CLAIMS], claimUser[MAX_CLAIMS], claimFailed[MAX_CLAIMS];
int claimCount = 0;

int searchResult[MAX_FOUND];
int searchResultCount = 0;

// FUNGSI: loadDemoData   (flowchart: "Data kosong?" -> "Isi contoh")
// TUJUAN: menyediakan dua pengguna dan dua barang temuan agar program dapat
// dicoba tanpa registrasi.
// TODO:
//  [ ] 1. Hanya isi data ini saat userCount, foundCount, dan lostCount masih 0.
//  [ ] 2. Isi dua pengguna pada indeks 0 dan 1, termasuk password, nomor,
//  gagal=0, dan locked=false.
//  [ ] 3. Isi dua barang temuan pada indeks 0 dan 1;
//  reporter harus menunjuk indeks pengguna yang valid. [ ] 4. Isi semua array
//  pasangan untuk setiap barang, beri STATUS_AVAILABLE, lalu atur userCount dan
//  foundCount.
// CONTOH: user "andi" dan barang "Payung" dapat dipakai untuk mencoba menu
// klaim. HATI-HATI: satu record memakai indeks yang sama di semua array; jangan
// menaikkan count sebelum semua pasangan terisi. SELESAI JIKA: [ ] M-UI-1
// lulus. JANGAN ubah nama, parameter, atau tipe return fungsi ini tanpa lapor
// semua pemilik modul.
void loadDemoData()
{
  if (userCount == 0 && foundCount == 0 && lostCount == 0)
  {

    userName[0] = "Azhri";
    userPassword[0] = "Azhri123";
    userPhone[0] = "081234567890";
    userFailed[0] = 0;
    userLocked[0] = false;

    userName[1] = "Arya";
    userPassword[1] = "Arya123";
    userPhone[1] = "089876543210";
    userFailed[1] = 0;
    userLocked[1] = false;

    foundReporter[0] = 0;
    foundName[0] = "Payung Hitam";
    foundCategory[0] = "Aksesori";
    foundLocation[0] = "Kantin Utama";
    foundDate[0] = "2026-10-25";
    foundDescription[0] = "Payung lipat warna hitam merk standar";
    foundSecret[0] = "Ada stiker kecil bergambar bintang di pegangannya";
    foundStatus[0] = STATUS_AVAILABLE;

    foundReporter[1] = 1;
    foundName[1] = "Dompet Kulit Coklat";
    foundCategory[1] = "Dompet";
    foundLocation[1] = "Parkiran Fakultas";
    foundDate[1] = "2026-10-26";
    foundDescription[1] = "Dompet berisi kartu identitas dan beberapa kartu penting";
    foundSecret[1] = "Ada foto pelajar di dalam selipan kartu";
    foundStatus[1] = STATUS_AVAILABLE;

    userCount = 2;
    foundCount = 2;
  }
}

// ============ string_utils ============

// FUNGSI: trim   (flowchart: "Teks masuk" -> "Buang spasi tepi")
// TUJUAN: menghapus spasi di awal dan akhir teks.
// Penjelasan: Fungsi ini mengembalikan teks tanpa spasi di awal dan akhir.
// CONTOH: "  buku  " -> "buku".
std::string trim(std::string s)
{
  int left = 0;
  int right = s.length();

  while (left < right && s[left] == ' ')
  {
    left++;
  }

  while (right > left && s[right - 1] == ' ')
  {
    right--;
  }

  return s.substr(left, right - left);
}

// FUNGSI: toLower   (flowchart: "Teks" -> "Huruf kecil")
// TUJUAN: menyamakan kapitalisasi untuk perbandingan.
// Penjelasan: Fungsi ini mengubah setiap huruf dalam salinan teks menjadi huruf kecil.
// CONTOH: "BukuA" -> "bukua".
std::string toLower(std::string s)
{
  int length = s.length();
  for (int i = 0; i < length; i++)
  {
    s[i] = tolower(s[i]);
  }

  return s;
}

// FUNGSI: containsIgnoreCase   (flowchart: "Normalisasi" -> "Cari kata")
// TUJUAN: mencari keyword tanpa membedakan huruf besar dan kecil.
// Penjelasan: Fungsi ini mencari keyword tanpa membedakan huruf besar dan kecil.
// CONTOH: "Tas Biru", "biru" -> true.
bool containsIgnoreCase(std::string text, std::string keyword)
{
  text = toLower(text);
  keyword = toLower(keyword);

  if (keyword == "")
  {
    return true;
  }

  int keywordLength = keyword.length();
  int textLength = text.length();
  for (int i = 0; i + keywordLength <= textLength; i++)
  {
    bool match = true;

    for (int j = 0; j < keywordLength; j++)
    {
      if (text[i + j] != keyword[j])
      {
        match = false;
        break;
      }
    }

    if (match)
    {
      return true;
    }
  }
  return false;
}

// FUNGSI: sanitizeField   (flowchart: "Teks input" -> "Teks aman file")
// TUJUAN: mengganti pemisah file dan baris baru agar satu record tetap satu
// baris.
// Penjelasan: Fungsi ini mengganti karakter pemisah dan baris baru agar teks aman disimpan.
// CONTOH: "A|B\nC" -> "A/B/C".
std::string sanitizeField(std::string s)
{
  int length = s.length();
  for (int i = 0; i < length; i++)
  {
    if (s[i] == '|' || s[i] == '\n' || s[i] == '\r')
    {
      s[i] = '/';
    }
  }

  return s;
}

// FUNGSI: isAllDigits   (flowchart: "Teks" -> "Semua angka?")
// TUJUAN: memeriksa bahwa string tidak kosong dan semua karakternya angka.
// Penjelasan: Fungsi ini memastikan teks tidak kosong dan seluruh karakternya berupa angka.
// CONTOH: "08123" -> true; "08-123" -> false.
bool isAllDigits(std::string s)
{
  if (s == "")
  {
    return false;
  }

  int length = s.length();
  for (int i = 0; i < length; i++)
  {
    bool isDigit = isdigit(s[i]);

    if (!isDigit)
    {
      return false;
    }
  }

  return true;
}

// FUNGSI: splitLine   (flowchart: "Baris" -> "Field-field")
// TUJUAN: memecah satu baris file ke array parts untuk modul storage.
// Penjelasan: Fungsi ini memecah satu baris menjadi field berdasarkan delimiter hingga batas array.
// CONTOH: "1|andi|08", '|', parts, 4 -> 3 dan parts berisi "1", "andi", "08".
int splitLine(std::string line, char delimiter, std::string parts[],
              int maxParts)
{
  std::string part = "";
  int amount = 0;

  int length = line.length();
  for (int i = 0; i < length; i++)
  {
    if (line[i] == delimiter)
    {
      if (amount < maxParts)
      {
        parts[amount] = part;
        amount++;
      }

      part = "";
    }
    else
    {
      part += line[i];
    }
  }

  if (amount < maxParts)
  {
    parts[amount] = part;
    amount++;
  }

  return amount;
}

// ============ date_utils ============

// FUNGSI: isLeapYear   (flowchart: "Tahun" -> "Habis dibagi?" -> "Kabisat/tidak")
// TUJUAN: menentukan apakah sebuah tahun merupakan tahun kabisat.
// Penjelasan: Fungsi ini memakai aturan tahun habis dibagi 400 atau habis dibagi 4 tetapi bukan 100.
// CONTOH: 2024 -> true; 2025 -> false.
bool isLeapYear(int year)
{
  return (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
}

// FUNGSI: daysInMonth   (flowchart: "Bulan+tahun" -> "Jumlah hari")
// TUJUAN: mengembalikan jumlah hari pada bulan tertentu.
// Penjelasan: Fungsi ini memilih jumlah hari sesuai bulan dan memakai isLeapYear untuk Februari.
// CONTOH: 2, 2024 -> 29; 4, 2025 -> 30.
int daysInMonth(int month, int year)
{
  switch (month)
  {
  case 1:
  case 3:
  case 5:
  case 7:
  case 8:
  case 10:
  case 12:
    return 31;

  case 4:
  case 6:
  case 9:
  case 11:
    return 30;

  case 2:
    if (isLeapYear(year))
    {
      return 29;
    }
    return 28;

  default:
    return 0;
  }
}

// FUNGSI: isValidDate   (flowchart: "Tanggal" -> "Format?" -> "Bulan/hari?")
// TUJUAN: memeriksa format YYYY-MM-DD dan batas hari tiap bulan.
// Penjelasan: Fungsi ini memeriksa format YYYY-MM-DD serta batas hari pada setiap bulan.
// CONTOH: "2025-02-30" -> false; "2025-02-28" -> true.
bool isValidDate(std::string date)
{
  if (date.length() != 10)
    return false;

  if (date[4] != '-' || date[7] != '-')
    return false;

  for (int i = 0; i < 10; i++)
  {
    if (i == 4 || i == 7)
    {
      continue;
    }

    if (date[i] < '0' || date[i] > '9')
    {
      return false;
    }
  }

  // convertion
  // ascii '0' = 48
  int year = (date[0] - '0') * 1000 + (date[1] - '0') * 100 +
             (date[2] - '0') * 10 + (date[3] - '0');
  int month = (date[5] - '0') * 10 + (date[6] - '0');
  int day = (date[8] - '0') * 10 + (date[9] - '0');

  if (year < 1)
    return false;

  if (month < 1 || month > 12)
    return false;

  if (day < 1 || day > daysInMonth(month, year))
  {
    return false;
  }

  return true;
}

// FUNGSI: getTodayDate   (flowchart: "Jam komputer" -> "YYYY-MM-DD")
// TUJUAN: mengembalikan tanggal lokal hari ini dengan format yang sama seperti
// input laporan.
// Penjelasan: Fungsi ini mengambil tanggal lokal saat ini dalam format YYYY-MM-DD.
// CONTOH: pada 2025-08-17 -> "2025-08-17".
std::string getTodayDate()
{
  time_t now = time(0);
  tm *local = localtime(&now);
  char result[11];
  strftime(result, sizeof(result), "%Y-%m-%d", local);
  return std::string(result);
}

// FUNGSI: isFutureDate   (flowchart: "Tanggal valid?" -> "Bandingkan hari ini")
// TUJUAN: menentukan apakah tanggal laporan lebih besar dari tanggal hari ini.
// Penjelasan: Fungsi ini membandingkan tanggal valid dengan tanggal hari ini untuk mendeteksi tanggal masa depan.
// CONTOH: jika hari ini "2025-08-17", "2025-08-18" -> true.
bool isFutureDate(std::string date)
{
  if (isValidDate(date) == false)
    return false;

  std::string today = getTodayDate();

  return date > today;
}

// ============ id_utils ============

// FUNGSI: toDisplayNo   (flowchart: "Indeks" -> "+1" -> "Nomor layar")
// TUJUAN: mengubah indeks mulai 0 menjadi nomor yang ramah pengguna mulai 1.
// Penjelasan: Fungsi ini mengubah indeks array mulai 0 menjadi nomor tampilan mulai 1.
// CONTOH: 0 -> 1.
int toDisplayNo(int index) { return index + 1; }

// FUNGSI: toIndex   (flowchart: "Nomor layar" -> "-1" -> "Indeks")
// TUJUAN: mengubah nomor layar mulai 1 kembali menjadi indeks array mulai 0.
// Penjelasan: Fungsi ini mengubah nomor tampilan mulai 1 kembali menjadi indeks array mulai 0.
// CONTOH: 1 -> 0.
int toIndex(int displayNo) { return displayNo - 1; }

// ============ input_helpers ============

// FUNGSI: readLine   (flowchart: "Prompt" -> "getline" -> "Teks")
// TUJUAN: menampilkan prompt lalu membaca satu baris lengkap, termasuk spasi.
// CONTOH: prompt "Nama: ", input "Tas Biru" -> "Tas Biru".
std::string readLine(std::string prompt)
{
  cout << prompt;
  std::string result;
  getline(cin, result);
  return result;
}

// FUNGSI: readInt   (flowchart: "Input teks" -> "Angka valid?" -> "Dalam
// batas?") TUJUAN: membaca bilangan dalam batas tanpa memakai konversi yang
// belum dipelajari.
// CONTOH: input "2" dengan batas 1..3 -> 2; input "dua" -> diminta ulang.
int readInt(std::string prompt, int minValue, int maxValue)
{
  while (true)
  {
    std::string text = readLine(prompt);
    if (!isAllDigits(text))
    {
      cout << "Masukkan angka yang benar.\n";
    }
    else
    {
      int result = 0;
      for (int i = 0; i < (int)text.length(); i++)
      {
        result = result * 10 + (text[i] - '0');
      }
      if (result >= minValue && result <= maxValue)
      {
        return result;
      }
      cout << "Masukkan angka dari " << minValue << " sampai " << maxValue
           << ".\n";
    }
  }
}

// FUNGSI: pauseScreen   (flowchart: "Tampil pesan" -> "Enter" -> "Lanjut")
// TUJUAN: memberi pengguna waktu membaca hasil sebelum kembali ke menu.
// CONTOH: setelah pesan berhasil, Enter kembali ke menu.
void pauseScreen() { readLine("Tekan Enter untuk lanjut..."); }

// FUNGSI: printHeader   (flowchart: "Judul" -> "Garis" -> "Judul")
// TUJUAN: membuat judul layar konsisten pada setiap menu.
// CONTOH: "Menu Pengguna" tampil di antara dua garis.
void printHeader(std::string title)
{
  std::cout << "\n==============================\n";
  std::cout << title << "\n";
  std::cout << "==============================\n";
}

// Haris
// ============ storage ============

// FUNGSI: loadAll   (flowchart: "Buka file" -> "Baris valid?" -> "Isi array")
// TUJUAN: memuat users, found_items, lost_reports, dan claim_attempts dari data/*.txt.
// TODO:
//  [ ] 1. Untuk tiap file, coba buka dengan ifstream; jika gagal dibuka, biarkan count terkait 0 dan lanjut.
//  [ ] 2. Baca satu baris per while, pecah dengan splitLine, dan cek jumlah field tepat.
//  [ ] 3. Cek count belum mencapai MAX sebelum mengisi seluruh array pasangan pada indeks count.
//  [ ] 4. Lewati baris rusak tanpa crash; baru naikkan count setelah satu record lengkap terisi.
//  [ ] 5. Jangan panggil loadDemoData di sini; main yang memutuskan setelah semua file selesai.
// CONTOH: users.txt tidak ada -> userCount tetap 0 dan program tetap membuka menu.
// HATI-HATI: file belum ada bukan error; indeks di file dimulai dari 0, nomor layar dimulai dari 1.
// SELESAI JIKA: [ ] M-STORAGE-1 lulus.
// JANGAN ubah nama, parameter, atau tipe return fungsi ini tanpa lapor semua pemilik modul.
void loadAll()
{
  // Penyimpanan file bersifat opsional; stub ini sengaja aman saat data/*.txt belum ada.
}

// FUNGSI: saveAll   (flowchart: "Array" -> "Tulis tiap record" -> "Tutup file")
// TUJUAN: menyimpan seluruh data saat layar UI berhasil mengubah data.
// TODO:
//  [ ] 1. Buka tiap file output dan loop dari 0 sampai count terkait.
//  [ ] 2. Tulis satu record per baris dengan pemisah '|', mengikuti urutan yang tertulis di data-model.md.
//  [ ] 3. Tulis bool locked sebagai 0 atau 1.
//  [ ] 4. Pastikan teks sudah sanitizeField sebelum masuk array atau sebelum ditulis.
//  [ ] 5. Tutup file setelah selesai; fungsi ini tidak boleh cout/cin.
// CONTOH: user indeks 0 menjadi "andi|rahasia|08123|0|0".
// HATI-HATI: jangan menulis foundSecret ke layar, tetapi tetap wajib ditulis ke found_items.txt untuk data klaim.
// SELESAI JIKA: [ ] M-STORAGE-2 lulus.
// JANGAN ubah nama, parameter, atau tipe return fungsi ini tanpa lapor semua pemilik modul.
void saveAll()
{
  // Penyimpanan file bersifat opsional; tidak ada data yang berubah pada stub ini.
}

// ============ auth ============

// FUNGSI: findUserByUsername   (flowchart: "Username" -> "Loop pengguna" ->
// "Indeks/-1") TUJUAN: mencari indeks pengguna dengan username tanpa membedakan
// huruf besar/kecil.
// Penjelasan: Fungsi ini mencari indeks pengguna berdasarkan username tanpa membedakan huruf besar dan kecil.
// CONTOH: data "Andi", input "andi" -> 0.
int findUserByUsername(std::string username)
{
  username = toLower(username);

  for (int i = 0; i < userCount; i++)
  {
    if (toLower(userName[i]) == username)
    {
      return i;
    }
  }

  return -1;
}

// FUNGSI: registerUser   (flowchart: "Input daftar" -> "Valid?" -> "Simpan
// array") TUJUAN: memvalidasi lalu menambahkan satu pengguna baru ke array
// paralel.
// Penjelasan: Fungsi ini memvalidasi data pendaftaran lalu menambahkan pengguna baru ke array.
// CONTOH: "andi", "rahasia", "08123" -> REG_OK lalu userCount bertambah satu.
int registerUser(std::string username, std::string password,
                 std::string phone)
{
  if (username == "" || password == "" || phone == "")
    return REG_EMPTY_FIELD;

  int length = username.length();
  for (int i = 0; i < length; i++)
  {
    if (username[i] == ' ')
    {
      return REG_USERNAME_HAS_SPACE;
    }
  }

  if (findUserByUsername(username) != -1)
    return REG_DUPLICATE;

  if (password.length() < MIN_PASSWORD_LENGTH)
    return REG_PASSWORD_SHORT;

  if (isAllDigits(phone) == false)
    return REG_PHONE_INVALID;

  if (userCount >= MAX_USERS)
    return REG_FULL;

  userName[userCount] = username;
  userPassword[userCount] = password;
  userPhone[userCount] = phone;
  userFailed[userCount] = 0;
  userLocked[userCount] = false;

  userCount++;

  return REG_OK;
}

// FUNGSI: loginUser   (flowchart: "Input login" -> "Valid?" -> "Gagal 3x?")
// TUJUAN: memeriksa username+password dan mengunci akun setelah 3x salah.
// Penjelasan: Fungsi ini memverifikasi login dan mengunci akun setelah terlalu banyak password salah.
// CONTOH: password salah 3x -> panggilan ke-3 mengembalikan LOGIN_LOCKED;
// panggilan ke-4 dengan password benar tetap LOGIN_LOCKED. HATI-HATI: indeks 0
// adalah login SUKSES; jangan memakai "if (hasil)" untuk mengecek. Bandingkan
// dengan >= 0. Dilarang cout di sini.
int loginUser(std::string username, std::string password)
{
  int index = findUserByUsername(username);
  if (index == -1)
    return LOGIN_NOT_FOUND;

  if (userLocked[index] == true)
    return LOGIN_LOCKED;

  if (userPassword[index] == password)
  {
    userFailed[index] = 0;
    return index;
  }
  else
  {
    userFailed[index] += 1;
  }

  if (userFailed[index] >= MAX_LOGIN_ATTEMPTS)
  {
    userLocked[index] = true;
    return LOGIN_LOCKED;
  }
  else
  {
    return LOGIN_WRONG_PASSWORD;
  }

  return LOGIN_NOT_FOUND;
}

// Muflih
// ============ items ============

/*
 * FILE:         src/items/items.cpp
 * TUJUAN:       Mengelola laporan temuan, laporan hilang, dan hasil pencarian
 * tanpa layar. BERGANTUNG:   models/data.cpp, utils/string_utils.cpp,
 * utils/date_utils.cpp DIPAKAI OLEH: ui/item_screen.cpp dan ui/claim_screen.cpp
 * URUTAN KERJA: addFoundItem, addLostReport, listItemsByReporter,
 * searchAvailableItems KONSEP YANG DIPERLUKAN: array, string, for, if/else,
 * return
 */

// FUNGSI: addFoundItem   (flowchart: "Input barang" -> "Valid?" -> "Simpan")
// TUJUAN: memvalidasi dan menambah satu laporan barang temuan.
// TODO:
//  [ ] 1. Cek name, category, location, dateFound, dan secretDetail tidak
//  kosong; description boleh kosong. Jika ada yang kosong -> ITEM_EMPTY_FIELD.
//  [ ] 2. Jika isValidDate(dateFound) false -> ITEM_INVALID_DATE.
//  [ ] 3. Jika isFutureDate(dateFound) true -> ITEM_FUTURE_DATE.
//  [ ] 4. Jika foundCount sudah MAX_FOUND -> ITEM_FULL.
//  [ ] 5. Isi semua array found pada indeks foundCount; sanitizeField pada
//  semua teks dan set foundStatus menjadi STATUS_AVAILABLE. [ ] 6. Naikkan
//  foundCount hanya setelah semua pasangan terisi, lalu return ITEM_OK.
// CONTOH: tanggal masa depan -> ITEM_FUTURE_DATE dan foundCount tidak berubah.
// HATI-HATI: description satu-satunya teks opsional; satu record selalu memakai
// indeks sama di semua array paralel. SELESAI JIKA: [ ] M-ITEM-1, M-ITEM-2, dan
// M-ITEM-3 lulus. JANGAN ubah nama, parameter, atau tipe return fungsi ini
// tanpa lapor semua pemilik modul.
int addFoundItem(int reporterIndex, std::string name, std::string category,
                 std::string location, string dateFound,
                 std::string description, std::string secretDetail)
{
  (void)reporterIndex; // hapus komentar ini setelah dipakai
  (void)name;          // hapus komentar ini setelah dipakai
  (void)category;      // hapus komentar ini setelah dipakai
  (void)location;      // hapus komentar ini setelah dipakai
  (void)dateFound;     // hapus komentar ini setelah dipakai
  (void)description;   // hapus komentar ini setelah dipakai
  (void)secretDetail;  // hapus komentar ini setelah dipakai
  return ITEM_FULL;
}

// FUNGSI: addLostReport   (flowchart: "Input hilang" -> "Valid?" -> "Simpan")
// TUJUAN: memvalidasi dan menambah satu laporan kehilangan.
// TODO:
//  [ ] 1. Cek name, category, location, dan dateLost tidak kosong; description
//  boleh kosong. [ ] 2. Jika tanggal tidak valid -> ITEM_INVALID_DATE; jika
//  masa depan -> ITEM_FUTURE_DATE. [ ] 3. Jika lostCount sudah MAX_LOST ->
//  ITEM_FULL. [ ] 4. Isi seluruh array lost pada indeks lostCount dengan
//  sanitizeField untuk semua teks. [ ] 5. Naikkan lostCount lalu return
//  ITEM_OK.
// CONTOH: lokasi kosong -> ITEM_EMPTY_FIELD.
// HATI-HATI: jangan memakai array found untuk laporan hilang dan jangan cout di
// sini. SELESAI JIKA: [ ] M-ITEM-1 dan M-ITEM-3 lulus. JANGAN ubah nama,
// parameter, atau tipe return fungsi ini tanpa lapor semua pemilik modul.
int addLostReport(int reporterIndex, std::string name, std::string category,
                  std::string location, string dateLost,
                  std::string description)
{
  (void)reporterIndex; // hapus komentar ini setelah dipakai
  (void)name;          // hapus komentar ini setelah dipakai
  (void)category;      // hapus komentar ini setelah dipakai
  (void)location;      // hapus komentar ini setelah dipakai
  (void)dateLost;      // hapus komentar ini setelah dipakai
  (void)description;   // hapus komentar ini setelah dipakai
  return ITEM_FULL;
}

// FUNGSI: searchAvailableItems   (flowchart: "Filter" -> "Loop temuan" ->
// "searchResult") TUJUAN: mengisi hasil pencarian hanya dengan barang berstatus
// Tersedia.
// TODO:
//  [ ] 1. Set searchResultCount=0 lalu loop i dari 0 sampai kurang dari
//  foundCount. [ ] 2. Lewati item jika foundStatus[i] bukan STATUS_AVAILABLE.
//  [ ] 3. Jika keyword tidak kosong, cocokkan ke foundName ATAU
//  foundDescription memakai containsIgnoreCase. [ ] 4. Jika category atau
//  location tidak kosong, cocokkan masing-masing tanpa membedakan huruf
//  besar/kecil. [ ] 5. Jika semua filter lolos, simpan i ke
//  searchResult[searchResultCount], lalu tambah count; return count setelah
//  loop.
// CONTOH: keyword "tas" menemukan "Tas Biru" tetapi tidak pernah menampilkan
// detail rahasia. HATI-HATI: string filter kosong berarti filter diabaikan;
// JANGAN pernah memakai foundSecret untuk pencarian atau layar. SELESAI JIKA: [
// ] M-ITEM-4 dan M-ITEM-5 lulus. JANGAN ubah nama, parameter, atau tipe return
// fungsi ini tanpa lapor semua pemilik modul.
int searchAvailableItems(std::string keyword, std::string category,
                         std::string location)
{
  (void)keyword;  // hapus komentar ini setelah dipakai
  (void)category; // hapus komentar ini setelah dipakai
  (void)location; // hapus komentar ini setelah dipakai
  searchResultCount = 0;
  return 0;
}

// FUNGSI: listItemsByReporter   (flowchart: "User" -> "Loop temuan" ->
// "searchResult") TUJUAN: mengisi hasil bersama dengan semua barang temuan
// milik seorang pengguna.
// TODO:
//  [ ] 1. Set searchResultCount=0 dan loop semua found item.
//  [ ] 2. Jika foundReporter[i] sama dengan reporterIndex, simpan i ke
//  searchResult dan tambah count. [ ] 3. Return searchResultCount setelah loop.
// CONTOH: pengguna indeks 0 dengan dua temuan -> return 2.
// HATI-HATI: yang disimpan adalah indeks asli barang, bukan nomor tampilan
// indeks+1. SELESAI JIKA: [ ] M-ITEM-6 lulus. JANGAN ubah nama, parameter, atau
// tipe return fungsi ini tanpa lapor semua pemilik modul.
int listItemsByReporter(int reporterIndex)
{
  (void)reporterIndex; // hapus komentar ini setelah dipakai
  searchResultCount = 0;
  return 0;
}

// Mudhofar
// ============ claims ============

/*
 * FILE:         src/claims/claims.cpp
 * TUJUAN:       Memproses percobaan klaim barang tanpa layar atau file.
 * BERGANTUNG:   models/data.cpp dan utils/string_utils.cpp
 * DIPAKAI OLEH: ui/claim_screen.cpp
 * URUTAN KERJA: getFailedClaimCount, getFinderContact, submitClaim
 * KONSEP YANG DIPERLUKAN: array, string, for, if/else, return
 */

// FUNGSI: getFailedClaimCount   (flowchart: "Item+user" -> "Loop klaim" ->
// "Jumlah") TUJUAN: mengambil jumlah jawaban klaim salah untuk pasangan item
// dan pengguna.
// TODO:
//  [ ] 1. Loop i dari 0 sampai kurang dari claimCount.
//  [ ] 2. Jika claimItem[i] dan claimUser[i] keduanya cocok, return
//  claimFailed[i]. [ ] 3. Jika tidak ada baris pasangan tersebut, return 0.
// CONTOH: belum pernah mencoba klaim -> 0.
// HATI-HATI: pasangan item sama tetapi user berbeda harus memiliki hitungan
// terpisah. SELESAI JIKA: [ ] M-CLAIM-1 lulus. JANGAN ubah nama, parameter,
// atau tipe return fungsi ini tanpa lapor semua pemilik modul.
int getFailedClaimCount(int itemIndex, int userIndex)
{
  (void)itemIndex; // hapus komentar ini setelah dipakai
  (void)userIndex; // hapus komentar ini setelah dipakai
  return 0;
}

// FUNGSI: getFinderContact   (flowchart: "Item" -> "Reporter" -> "Kontak")
// TUJUAN: membuat teks "username - nomor telepon" milik penemu barang.
// TODO:
//  [ ] 1. Ambil reporterIndex dari foundReporter[itemIndex].
//  [ ] 2. Return userName[reporterIndex] + " - " + userPhone[reporterIndex].
// CONTOH: penemu andi dengan 08123 -> "andi - 08123".
// HATI-HATI: fungsi ini hanya dipanggil UI setelah CLAIM_APPROVED dan itemIndex
// sudah valid. SELESAI JIKA: [ ] M-CLAIM-3 lulus. JANGAN ubah nama, parameter,
// atau tipe return fungsi ini tanpa lapor semua pemilik modul.
std::string getFinderContact(int itemIndex)
{
  (void)itemIndex; // hapus komentar ini setelah dipakai
  return "";
}

// FUNGSI: submitClaim   (flowchart: "Pilih item" -> "Boleh klaim?" -> "Detail
// benar?") TUJUAN: menyetujui klaim benar atau mencatat jawaban salah untuk
// satu item dan pengguna.
// TODO:
//  [ ] 1. Jika itemIndex < 0 atau >= foundCount -> return CLAIM_ITEM_NOT_FOUND.
//  [ ] 2. Jika foundStatus[itemIndex] bukan STATUS_AVAILABLE -> return
//  CLAIM_NOT_AVAILABLE. [ ] 3. Jika foundReporter[itemIndex] sama dengan
//  claimantIndex -> return CLAIM_OWN_ITEM. [ ] 4. Jika
//  getFailedClaimCount(itemIndex, claimantIndex) sudah >= MAX_CLAIM_ATTEMPTS ->
//  return CLAIM_TOO_MANY_ATTEMPTS. [ ] 5. Bandingkan
//  toLower(trim(secretAnswer)) dengan toLower(trim(foundSecret[itemIndex])).
//  Jika sama, set status Dikembalikan dan return CLAIM_APPROVED. [ ] 6. Jika
//  salah, cari baris pasangan klaim. Bila ada, tambah claimFailed; bila tidak
//  ada dan claimCount penuh -> CLAIM_STORAGE_FULL; selain itu buat baris baru
//  dengan failed=1. [ ] 7. Return CLAIM_WRONG_DETAIL sesudah kesalahan berhasil
//  dicatat.
// CONTOH: jawaban salah ketiga -> CLAIM_WRONG_DETAIL, panggilan berikutnya ->
// CLAIM_TOO_MANY_ATTEMPTS. HATI-HATI: urutan cek wajib persis; ubah semua data
// satu record klaim di indeks yang sama dan jangan cout/saveAll di sini.
// SELESAI JIKA: [ ] M-CLAIM-1, M-CLAIM-2, M-CLAIM-3, dan M-CLAIM-4 lulus.
// JANGAN ubah nama, parameter, atau tipe return fungsi ini tanpa lapor semua
// pemilik modul.
int submitClaim(int claimantIndex, int itemIndex, std::string secretAnswer)
{
  (void)claimantIndex; // hapus komentar ini setelah dipakai
  (void)itemIndex;     // hapus komentar ini setelah dipakai
  (void)secretAnswer;  // hapus komentar ini setelah dipakai
  return CLAIM_ITEM_NOT_FOUND;
}

// Haris
// ============ auth_screen ============

/*
 * FILE:         src/ui/auth_screen.cpp
 * TUJUAN:       Menampilkan alur daftar dan login serta menerjemahkan kode auth ke pesan Indonesia.
 * BERGANTUNG:   ui/input_helpers.cpp, auth/auth.cpp, storage/storage.cpp
 * DIPAKAI OLEH: main.cpp
 * URUTAN KERJA: showRegisterScreen, showLoginScreen
 * KONSEP YANG DIPERLUKAN: string, int, if/else, fungsi, return
 */

// FUNGSI: showRegisterScreen   (flowchart: "Input daftar" -> "registerUser" -> "Pesan")
// TUJUAN: meminta data pendaftaran, menampilkan pesan hasil, dan menyimpan jika sukses.
// TODO:
//  [ ] 1. Tampilkan header lalu ambil username, password, dan nomor dengan readLine.
//  [ ] 2. Panggil registerUser dan simpan kode hasilnya.
//  [ ] 3. Dengan if/else, tampilkan pesan Indonesia untuk setiap REG_*; jangan memakai switch.
//  [ ] 4. Jika kode REG_OK, panggil saveAll tepat sekali dan tampilkan pesan berhasil.
//  [ ] 5. Panggil pauseScreen sebelum kembali ke main.
// CONTOH: password lima karakter -> pesan password minimal 6 karakter dan tidak memanggil saveAll.
// HATI-HATI: layar menyimpan input dengan readLine; modul auth tidak boleh memakai cout/cin/file.
// SELESAI JIKA: [ ] M-AUTH-1 dan M-AUTH-2 lulus.
// JANGAN ubah nama, parameter, atau tipe return fungsi ini tanpa lapor semua pemilik modul.
void showRegisterScreen()
{
  // Stub layar: isi setelah registerUser selesai dikerjakan.
}

// FUNGSI: showLoginScreen   (flowchart: "Input login" -> "loginUser" -> "Masuk/-1")
// TUJUAN: meminta kredensial dan mengembalikan indeks pengguna bila login berhasil.
// TODO:
//  [ ] 1. Tampilkan header dan ambil username serta password melalui readLine.
//  [ ] 2. Panggil loginUser(username, password).
//  [ ] 3. Jika hasil >= 0, tampilkan selamat datang, panggil saveAll karena gagal login mungkin berubah, lalu return hasil.
//  [ ] 4. Jika hasil LOGIN_NOT_FOUND, LOGIN_WRONG_PASSWORD, atau LOGIN_LOCKED, tampilkan pesan Indonesia dengan if/else, panggil saveAll bila failed/locked berubah, pauseScreen, lalu return -1.
// CONTOH: username tidak ada -> pesan akun tidak ditemukan dan return -1.
// HATI-HATI: indeks 0 sukses; selalu cek hasil >= 0, bukan if(hasil).
// SELESAI JIKA: [ ] M-AUTH-3 dan M-AUTH-4 lulus.
// JANGAN ubah nama, parameter, atau tipe return fungsi ini tanpa lapor semua pemilik modul.
int showLoginScreen()
{
  // Stub aman: -1 berarti kembali ke menu tamu.
  return -1;
}

// Muflih
// ============ item_screen ============

/*
 * FILE:         src/ui/item_screen.cpp
 * TUJUAN:       Menampilkan layar laporan barang dan laporan pengguna.
 * BERGANTUNG:   ui/input_helpers.cpp, items/items.cpp, utils/id_utils.cpp,
 * storage/storage.cpp DIPAKAI OLEH: main.cpp dan ui/claim_screen.cpp URUTAN
 * KERJA: showReportFoundScreen, showReportLostScreen, showMyReportsScreen
 * KONSEP YANG DIPERLUKAN: string, int, if/else, for, fungsi
 */

// FUNGSI: showReportFoundScreen   (flowchart: "Input temuan" -> "addFoundItem"
// -> "Pesan") TUJUAN: meminta laporan barang temuan dan menyimpan jika
// penambahan berhasil.
// TODO:
//  [ ] 1. Ambil name, category, location, date, description, dan secret detail
//  dengan readLine. [ ] 2. Panggil addFoundItem(userIndex, ...). [ ] 3.
//  Tampilkan pesan tiap ITEM_* memakai if/else. [ ] 4. Jika ITEM_OK, panggil
//  saveAll dan jangan tampilkan secret detail pada pesan. [ ] 5. Panggil
//  pauseScreen.
// CONTOH: description kosong tetap dapat berhasil jika field wajib lain valid.
// HATI-HATI: detail rahasia hanya diminta untuk verifikasi klaim, tidak boleh
// dicetak. SELESAI JIKA: [ ] M-ITEM-1 sampai M-ITEM-3 lulus. JANGAN ubah nama,
// parameter, atau tipe return fungsi ini tanpa lapor semua pemilik modul.
void showReportFoundScreen(int userIndex)
{
  (void)userIndex; // hapus komentar ini setelah dipakai
}

// FUNGSI: showReportLostScreen   (flowchart: "Input hilang" -> "addLostReport"
// -> "Pesan") TUJUAN: meminta laporan kehilangan; prefillName dapat dipakai
// dari pencarian tanpa hasil.
// TODO:
//  [ ] 1. Jika prefillName tidak kosong, tampilkan dan tawarkan sebagai nama
//  awal; tetap gunakan readLine untuk nama akhir. [ ] 2. Ambil category,
//  location, date, dan description dengan readLine. [ ] 3. Panggil
//  addLostReport(userIndex, ...) lalu tampilkan pesan ITEM_* dengan if/else. [
//  ] 4. Jika ITEM_OK, panggil saveAll; kemudian pauseScreen.
// CONTOH: pencarian "dompet" kosong -> layar dibuka dengan prefillName
// "dompet". HATI-HATI: prefill bukan data yang otomatis tersimpan; pengguna
// tetap dapat mengubahnya. SELESAI JIKA: [ ] M-ITEM-1 dan M-ITEM-5 lulus.
// JANGAN ubah nama, parameter, atau tipe return fungsi ini tanpa lapor semua
// pemilik modul.
void showReportLostScreen(int userIndex, std::string prefillName)
{
  (void)userIndex;   // hapus komentar ini setelah dipakai
  (void)prefillName; // hapus komentar ini setelah dipakai
}

// FUNGSI: showMyReportsScreen   (flowchart: "User" -> "listItemsByReporter" ->
// "Daftar") TUJUAN: menampilkan semua barang temuan yang dilaporkan pengguna
// aktif.
// TODO:
//  [ ] 1. Panggil listItemsByReporter(userIndex).
//  [ ] 2. Jika hasil 0, tampilkan pesan belum ada laporan.
//  [ ] 3. Jika ada, loop searchResult dan tampilkan nomor toDisplayNo(index),
//  nama, lokasi, tanggal, dan status. [ ] 4. Jangan tampilkan foundSecret; lalu
//  pauseScreen.
// CONTOH: item indeks 0 tercetak sebagai nomor 1, bukan 0.
// HATI-HATI: searchResult menyimpan indeks asli; status boleh Dikembalikan
// tetap muncul di laporan saya. SELESAI JIKA: [ ] M-ITEM-6 lulus. JANGAN ubah
// nama, parameter, atau tipe return fungsi ini tanpa lapor semua pemilik modul.
void showMyReportsScreen(int userIndex)
{
  (void)userIndex; // hapus komentar ini setelah dipakai
}

// Mudhofar
// ============ claim_screen ============

/*
 * FILE:         src/ui/claim_screen.cpp
 * TUJUAN:       Menampilkan pencarian dan proses klaim barang kepada pengguna.
 * BERGANTUNG:   ui/input_helpers.cpp, items/items.cpp, claims/claims.cpp, utils/id_utils.cpp, storage/storage.cpp, ui/item_screen.cpp
 * DIPAKAI OLEH: main.cpp
 * URUTAN KERJA: showClaimFlowScreen
 * KONSEP YANG DIPERLUKAN: string, int, if/else, for, fungsi
 */

// FUNGSI: showClaimFlowScreen   (flowchart: "Cari" -> "Pilih" -> "Jawab detail" -> "Klaim")
// TUJUAN: mengarahkan pengguna dari pencarian barang sampai hasil klaim.
// TODO:
//  [ ] 1. Ambil keyword, category, dan location dengan readLine, lalu panggil searchAvailableItems.
//  [ ] 2. Jika hasil 0, tawarkan dengan readInt apakah ingin membuka showReportLostScreen(userIndex, keyword); lalu return.
//  [ ] 3. Loop searchResult dan tampilkan nomor toDisplayNo, nama, category, lokasi, tanggal, dan description; JANGAN tampilkan foundSecret.
//  [ ] 4. Minta nomor barang dengan readInt, ubah memakai toIndex, dan pastikan indeks itu benar-benar salah satu hasil pencarian.
//  [ ] 5. Ambil secretAnswer dengan readLine, panggil submitClaim, lalu ubah semua CLAIM_* menjadi pesan Indonesia dengan if/else.
//  [ ] 6. Jika CLAIM_APPROVED, tampilkan getFinderContact(itemIndex); setelah setiap perubahan berhasil dicatat, panggil saveAll; lalu pauseScreen.
// CONTOH: jawaban benar -> pesan disetujui dan "username - nomor telepon" penemu muncul.
// HATI-HATI: nomor layar adalah indeks+1; jangan mempercayai nomor yang tidak ada dalam searchResult dan jangan cetak detail rahasia.
// SELESAI JIKA: [ ] M-ITEM-5 dan M-CLAIM-1 sampai M-CLAIM-4 lulus.
// JANGAN ubah nama, parameter, atau tipe return fungsi ini tanpa lapor semua pemilik modul.
void showClaimFlowScreen(int userIndex)
{
  (void)userIndex; // hapus komentar ini setelah dipakai
}

// Ilham
// Ilham
// ============ main() ============

// FUNGSI: main   (flowchart: "Mulai" -> "loadAll" -> "Menu tamu/pengguna" -> "Keluar")
// TUJUAN: memulai data, mengarahkan layar, dan menjaga menu berjalan sampai pengguna keluar.
// TODO:
//  [*] 1. Panggil loadAll; jika userCount 0, panggil loadDemoData agar contoh dapat dipakai.
//  [*] 2. Ulangi menu tamu: Register, Login, Keluar; ambil pilihan dengan readInt.
//  [*] 3. Saat Login berhasil (hasil >= 0), ulangi menu pengguna: Lapor temuan, Klaim barang, Laporan saya, Logout.
//  [ ] 4. Panggil fungsi layar sesuai pilihan; layar yang sukses mengubah data bertugas memanggil saveAll.
//  [*] 5. Saat Logout kembali ke menu tamu; saat Keluar hentikan loop dan return 0.
// CONTOH: pilih 3 di menu tamu -> program berhenti normal.
// HATI-HATI: indeks 0 adalah login sukses; main tidak boleh berisi logika bisnis atau memakai cin/cout langsung.
// SELESAI JIKA: [ ] M-UI-1 lulus.
// JANGAN ubah nama, parameter, atau tipe return fungsi ini tanpa lapor semua pemilik modul.
int main()
{
  cout << "awokawok" << endl;
  cout << "Hai" << endl;
  loadAll();
  if (userCount == 0)
  {
    loadDemoData();
  }

  bool isRunning = true;
  while (isRunning)
  {
    printHeader("Lost and Found Management System");
    int guestChoice = readInt("1. Register\n2. Login\n3. Keluar\nPilih: ", 1, 3);

    if (guestChoice == 1)
    {
      showRegisterScreen();
    }
    else if (guestChoice == 2)
    {
      int userIndex = showLoginScreen();
      while (userIndex >= 0)
      {
        printHeader("Menu Pengguna");
        int userChoice = readInt("1. Lapor temuan\n2. Klaim barang\n3. Laporan saya\n4. Logout\nPilih: ", 1, 4);
        if (userChoice == 1)
        {
          showReportFoundScreen(userIndex);
        }
        else if (userChoice == 2)
        {
          showClaimFlowScreen(userIndex);
        }
        else if (userChoice == 3)
        {
          showMyReportsScreen(userIndex);
        }
        else
        {
          userIndex = -1;
        }
      }
    }
    else
    {
      isRunning = false;
    }
  }

  return 0;
}
// hari ini sangat cerah, bahkan saya ingin segera memeluk awan dan hujan yang tersebar
// harap hari-hari seperti ini selalu datang, dan nikmat yang aku dapatkan tidak berkurang sama sekali

// testing123
// yoks
