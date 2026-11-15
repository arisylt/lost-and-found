# Lost and Found Management System

Kerangka proyek C++ terminal untuk tim mahasiswa semester 1. Program akan mencatat barang temuan, laporan kehilangan, dan klaim barang. Kerangka ini sengaja berisi deklarasi data, kontrak fungsi, stub aman, dan TODO kecil agar setiap anggota dapat mengisi logika bisnis secara bertahap.

## Build dan jalankan

Seluruh program berada di `src/main.cpp` dan **MySQL adalah satu-satunya
penyimpanan**; tidak ada file `.txt` lagi. Siapkan database sekali, lalu
kompilasi dengan flag dari `mysql_config`.

```bash
mariadb -u root -p < schema.sql

g++ -std=c++11 -Wall -Wextra $(mysql_config --cflags) \
    src/main.cpp -o lostfound $(mysql_config --libs)
./lostfound
```

Konfigurasi koneksi dibaca dari environment (semua punya default):

| Variabel | Default | Isi |
| --- | --- | --- |
| `LF_DB_HOST` | `127.0.0.1` | host MySQL |
| `LF_DB_PORT` | `3306` | port |
| `LF_DB_USER` | `lf` | user database |
| `LF_DB_PASS` | kosong | password |
| `LF_DB_NAME` | `lostfound` | nama database |

Bila koneksi gagal, program berhenti dengan pesan `[DB]` dan daftar variabel di
atas. Pilih `3` pada menu tamu untuk keluar.

## Penyimpanan

Seluruh array di `main.cpp` adalah salinan kerja dari isi database: `loadAll()`
membacanya sekali saat program mulai, dan `saveAll()` menulis ulang semua tabel
dalam satu transaksi setiap ada perubahan. Skema ada di `schema.sql` (tabel
`users`, `categories`, `locations`, `lockers`, `items`, `lost_reports`, `claims`,
`deposits`, `access_codes`, `locker_event_logs`).

Konsekuensi yang perlu diketahui:

- Data di MySQL **tidak boleh diubah manual** saat program berjalan; perubahan
  akan tertimpa oleh `saveAll()` berikutnya.
- `saveAll()` menghapus lalu menulis ulang seluruh tabel. Untuk proyek ini
  ukurannya kecil; jika nanti besar, ganti dengan `INSERT ... ON DUPLICATE KEY
  UPDATE`.
- Password dan kode loker disimpan sebagai digest SHA-256 (64 karakter hex),
  tanpa salt. Cukup untuk latihan, belum layak untuk aplikasi nyata.
- Status disimpan apa adanya dalam bahasa Indonesia (`Tersedia`,
  `Menunggu Dititipkan`, `Disetujui`, `Diblokir`, `Dicari`, ...) karena kolom
  status di `schema.sql` bertipe `VARCHAR` tanpa enumerasi.
- Kolom `deposits.picked_up_at` tetap terisi setelah pengambilan karena baris
  deposit tidak dihapus; loker dianggap kosong dari status barang
  (`Sudah Diambil`), bukan dari `id_locker`.

## Uji otomatis

Skrip Expect di `test/` membangun `build/lostfound-test` dan memakai direktori sementara sendiri, sehingga data di root repo tidak tersentuh.

```bash
for f in test/*.exp; do expect "$f"; done
```

## Struktur kode

Semua kode ada dalam satu file: `src/main.cpp`. Setiap pemilik bekerja hanya pada bagian yang diberi banner namanya; library dan `using namespace std;` hanya ada sekali di bagian paling atas.

## Kepemilikan bagian

| Anggota | Bagian |
| --- | --- |
| A | data, string_utils, id_utils, storage |
| B | auth, auth_screen |
| C | date_utils, items, item_screen |
| D | input_helpers, claims, claim_screen, main() |

## Aturan Git

1. Jalankan `git pull --rebase` sebelum mulai bekerja.
2. Buat satu branch per fitur, misalnya `feature/auth-register`.
3. Hanya edit bagian milik sendiri; jika harus mengubah bagian anggota lain, beri tahu pemiliknya lebih dahulu.
4. Jangan memformat ulang seluruh `src/main.cpp`.
5. Buka PR kecil untuk pekerjaan yang siap diuji.
6. Review silang berputar: **A → B → C → D → A**.
7. Jangan merge langsung ke branch utama tanpa review.

## Disederhanakan dengan sengaja

- Fase 1 **tidak memakai** `.h`, `struct`, `vector`, class, pointer, referensi, atau template. Semua data ada pada array paralel global.
- Semua bagian sengaja digabung dalam satu `main.cpp`; banner kepemilikan menjaga pembagian kerja tetap jelas.
- Akun yang terkunci tidak memiliki jalur buka dari menu; kunci 15 menit tersimpan di `users.locked_until` dan tetap berlaku setelah program ditutup, jadi cara membukanya adalah mengedit database.
- Tidak ada penghapusan data; indeks array tidak bergeser. Database kosong hanya diisi satu akun admin bawaan (`admin` / `admin123`) agar menu admin bisa dipakai; mahasiswa mendaftar sendiri lewat menu Register.
- Password dan kode loker disimpan sebagai digest SHA-256 tanpa salt. Cukup untuk latihan, belum layak untuk aplikasi nyata; kode 6 digit juga hanya punya sejuta kemungkinan.

Baca `docs/data-model.md` sebelum mengubah array dan `docs/TASKS.md` sebelum mulai mengisi TODO.

azhri hytam
mudho