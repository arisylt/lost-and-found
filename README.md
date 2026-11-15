# Lost and Found Management System

Kerangka proyek C++ terminal untuk tim mahasiswa semester 1. Program akan mencatat barang temuan, laporan kehilangan, dan klaim barang. Kerangka ini sengaja berisi deklarasi data, kontrak fungsi, stub aman, dan TODO kecil agar setiap anggota dapat mengisi logika bisnis secara bertahap.

## Build dan jalankan

**Patokan utama:** seluruh program berada di `src/main.cpp`; kompilasi file itu dengan perintah berikut.

```bash
g++ -std=c++11 -Wall -Wextra src/main.cpp -o lostfound
./lostfound
```


Pilih `3` pada menu tamu untuk keluar.

## Format file data

Semua data disimpan sebagai file teks di direktori tempat program dijalankan, dengan pemisah `|`. File yang tidak ada dianggap kosong, dan format lama tetap terbaca.

| File | Kolom |
| --- | --- |
| `users.txt` | `username\|password\|phone\|failed\|locked\|fullName\|major\|email\|role\|lockedUntil` |
| `found_items.txt` | `reporter\|name\|category\|location\|date\|description\|secret\|status\|(cadangan)\|locker\|depositCode\|reservationExpiry\|claimant\|pickupCode\|pickupExpiry\|pickupFailed` |
| `lost_reports.txt` | `reporter\|name\|category\|location\|date\|description` |
| `claims.txt` | `item\|user\|failed\|status\|claimedAt\|verifiedAt` |
| `lockers.txt` | satu baris per loker: `1` rusak, `0` siap |
| `locker_logs.txt` | `locker\|item\|type\|time\|note`, maksimal 500 entri terakhir |

Kolom ke-9 `found_items.txt` adalah kolom cadangan yang selalu ditulis kosong; nomornya dipertahankan agar file lama tidak bergeser. Lokasi dan kategori disimpan sebagai nama dari daftar pilihan di bagian atas `main.cpp`.

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
- Password disimpan polos (apa adanya). Ini tidak aman untuk aplikasi nyata dan hanya boleh untuk latihan.
- Akun yang terkunci tidak memiliki jalur buka dari menu; kunci 15 menit tersimpan di `users.txt` dan tetap berlaku setelah program ditutup, jadi cara membukanya adalah mengedit data.
- Penyimpanan file teks selalu aktif; `loadAll`/`saveAll` menerima format kolom lama maupun baru.
- Tidak ada penghapusan data; indeks array tidak bergeser. Admin demo (`admin` / `admin123`) memiliki menu anomali, penanganan barang, klaim diblokir, status loker, log loker, dan logout.
- Password dan kode loker disimpan polos tanpa hashing. Ini keputusan yang masih terbuka untuk tim.

Baca `docs/data-model.md` sebelum mengubah array dan `docs/TASKS.md` sebelum mulai mengisi TODO.

azhri hytam
mudho