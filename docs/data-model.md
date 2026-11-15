# Model Data: Array Paralel

Semua data dideklarasikan **sekali** di bagian atas `src/main.cpp`. Satu record adalah satu indeks yang sama di semua array dalam kelompoknya. Contoh: data barang temuan indeks `5` memakai `foundReporter[5]`, `foundName[5]`, `foundDate[5]`, sampai `foundStatus[5]`.

## Aturan emas

1. Jangan pernah mengubah satu array pasangan tanpa mengubah pasangan lain pada indeks yang sama.
2. Naikkan `userCount`, `foundCount`, `lostCount`, atau `claimCount` hanya setelah seluruh record selesai diisi.
3. MVP tidak menghapus data, jadi indeks tidak pernah bergeser.
4. Nomor yang dilihat pengguna adalah `indeks + 1`; gunakan `toDisplayNo` dan `toIndex`.
5. `searchResult` hanya tempat sementara berisi **indeks barang temuan**, bukan data barang baru.
6. `foundSecret` tidak boleh tampil di layar atau hasil pencarian.

## Pengguna

| Array             | Isi pada indeks `i`                          |
| ----------------- | -------------------------------------------- |
| `userName[i]`     | NIM/ID login (nama array lama dipertahankan) |
| `userPassword[i]` | password polos (latihan saja)                |
| `userPhone[i]`    | nomor telepon                                |
| `userFullName[i]` | nama lengkap mahasiswa                       |
| `userMajor[i]`    | jurusan mahasiswa                            |
| `userEmail[i]`    | email mahasiswa                              |
| `userRole[i]`     | peran `Mahasiswa` atau `Admin`               |
| `userFailed[i]`   | jumlah login salah berturut-turut            |
| `userLocked[i]`   | status terkunci                              |
| `userLockedUntil[i]` | waktu kunci berakhir (Unix timestamp)     |

Record pengguna valid berada pada indeks `0` sampai `userCount - 1`.

## Barang temuan

| Array                       | Isi pada indeks `i`                                                                                                                                                            |
| --------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| `foundReporter[i]`          | indeks pengguna pelapor/penemu                                                                                                                                                 |
| `foundName[i]`              | nama barang                                                                                                                                                                    |
| `foundCategory[i]`          | kategori                                                                                                                                                                       |
| `foundLocation[i]`          | lokasi ditemukan                                                                                                                                                               |
| `foundDate[i]`              | tanggal `YYYY-MM-DD`                                                                                                                                                           |
| `foundDescription[i]`       | deskripsi umum                                                                                                                                                                 |
| `foundSecret[i]`            | detail untuk verifikasi klaim, rahasia                                                                                                                                         |
| `foundStatus[i]`            | status, termasuk `Menunggu Dititipkan`, `Reservasi Batal`, `Tersedia`, `Dalam Klaim`, `Pengambilan Terkunci`, `Sudah Diambil`, `Perlu Verifikasi`, atau `Diserahkan ke Kampus` |
| `foundLocker[i]`            | indeks loker 0–8, atau `-1` bila tidak memakai loker                                                                                                                           |
| `foundDepositCode[i]`       | kode titip aktif, bila ada                                                                                                                                                     |
| `foundReservationExpiry[i]` | waktu kedaluwarsa reservasi sebagai Unix timestamp                                                                                                                             |
| `foundClaimant[i]`          | indeks pengguna pengklaim, atau `-1`                                                                                                                                           |
| `foundPickupCode[i]`        | kode ambil aktif, bila ada                                                                                                                                                     |
| `foundPickupExpiry[i]`      | waktu kedaluwarsa kode ambil sebagai Unix timestamp                                                                                                                            |
| `foundPickupFailed[i]`      | jumlah percobaan kode ambil salah                                                                                                                                              |
| `foundDepositFailed[i]`     | jumlah percobaan kode titip salah                                                                                                                                              |
| `foundReservedAt[i]`        | waktu reservasi loker dibuat                                                                                                                                                   |
| `foundDepositedAt[i]`       | waktu penitipan dikonfirmasi                                                                                                                                                   |
| `foundPickedUpAt[i]`        | waktu pengambilan selesai                                                                                                                                                      |
| `foundDepositCodeUsedAt[i]` | waktu kode titip terakhir dipakai                                                                                                                                              |
| `foundPickupCodeUsedAt[i]`  | waktu kode ambil terakhir dipakai                                                                                                                                              |

## Laporan hilang

| Array                | Isi pada indeks `i`     |
| -------------------- | ----------------------- |
| `lostReporter[i]`    | indeks pengguna pelapor |
| `lostName[i]`        | nama barang             |
| `lostCategory[i]`    | kategori                |
| `lostLocation[i]`    | lokasi terakhir         |
| `lostDate[i]`        | tanggal `YYYY-MM-DD`    |
| `lostDescription[i]` | deskripsi umum          |
| `lostStatus[i]`      | status laporan, `Dicari` |

## Percobaan klaim dan hasil pencarian

| Array             | Isi pada indeks `i`                            |
| ----------------- | ---------------------------------------------- |
| `claimItem[i]`    | indeks barang temuan                           |
| `claimUser[i]`    | indeks pengguna pengklaim                      |
| `claimFailed[i]`  | jumlah jawaban detail rahasia yang salah       |
| `claimStatus[i]`  | `Gagal`, `Disetujui`, `Selesai`, `Diblokir`, atau `Ditolak` |
| `claimDetail[i]`  | jawaban detail terakhir yang diajukan pengklaim |
| `claimClaimedAt[i]` | waktu klaim disetujui                        |
| `claimVerifiedAt[i]` | waktu verifikasi admin                     |
| `searchResult[i]` | indeks barang temuan yang cocok pencarian/list |

`claimItem`, `claimUser`, dan `claimFailed` adalah satu record. Satu pasangan item-pengguna harus mempunyai satu baris percobaan klaim.

## Format file

Storage memakai file teks di direktori kerja, satu record per baris, pemisah `|`. Semua teks perlu disanitasi: `|`, newline, dan carriage return diganti `/`. File yang tidak ada berarti jumlah record terkait `0`; baris dengan jumlah kolom tak dikenal dilewati. `loadAll` menerima format kolom lama maupun baru, jadi file sebelum perubahan tetap terbaca.

| File | Urutan field | Contoh satu baris valid |
| --- | --- | --- |
| `users.txt` | `nim|password|phone|failed|locked(0/1)|fullName|major|email|role|lockedUntil` | `12345|6b3b...9f|08123456789|0|0|Andi|Informatika|andi@example.com|Mahasiswa|0` |
| `found_items.txt` | `reporterIndex|name|category|location|date|description|secret|status|cadangan|locker|depositCode|reservationExpiry|claimant|pickupCode|pickupExpiry|pickupFailed|depositFailed|reservedAt|depositedAt|pickedUpAt|depositCodeUsedAt|pickupCodeUsedAt` | `0|Payung|Aksesoris|Kantin|2025-08-17|Payung hitam|Pegangan kayu|Tersedia||0||0|-1||0|0|0|0|0|0|0|0` |
| `lost_reports.txt` | `reporterIndex|name|category|location|date|description|status` | `1|Dompet|Aksesoris|Perpustakaan|2025-08-16|Warna cokelat|Dicari` |
| `claims.txt` | `itemIndex|userIndex|failedCount|status|claimedAt|verifiedAt|detailClaim` | `0|1|2|Diblokir|0|0|jawaban pengklaim` |
| `lockers.txt` | satu baris per loker: `1` rusak, `0` siap | `0` |
| `locker_logs.txt` | `lockerIndex|itemIndex|tipeEvent|waktu|keterangan` | `2|5|RESERVASI|1730000000|Payung Hitam` |

Kolom ke-9 `found_items.txt` adalah kolom cadangan yang selalu ditulis kosong: nomornya dipertahankan agar file lama tidak bergeser. Kolom `password` dan kode akses berisi digest SHA-256 (64 karakter hex); nilai lama yang masih plaintext tetap diterima lalu di-upgrade saat berhasil dipakai.

Kategori, jurusan, dan lokasi disimpan sebagai nama dari daftar di bagian atas `src/main.cpp`, mengikuti tabel `categories`, `locations`, dan `majors` pada ERD. Panjang teks dibatasi mengikuti `varchar` di RAT agar tidak terpotong saat diimpor ke database.

## Mirror MySQL

Build dengan `-DUSE_MYSQL` menyalin seluruh array ke skema `schema.sql` setiap
`saveAll()` selesai (satu transaksi, `DELETE` lalu `INSERT`). File `.txt` tetap
sumber data utama; `loadAll` tidak membaca database. Nilai status disalin apa
adanya (`Disetujui`, `Diblokir`, `Dicari`, `Menunggu`, `Aktif`, ...) karena semua
kolom status di `schema.sql` bertipe `VARCHAR` tanpa enumerasi. Email kosong dan
timestamp yang belum ada menjadi `NULL`; hanya `access_codes.expires_at` dan
`locker_event_logs.waktu` yang memakai epoch karena `NOT NULL`.

## Selisih yang masih terbuka

- `access_codes` pada RAT adalah tabel tersendiri; di sini kode titip dan kode ambil hanya satu per barang, sehingga `used_at` menyimpan pemakaian kode terakhir.
- `locations.detail` belum dipakai karena CLI hanya memilih nama lokasi.
- `deposits.status` bertipe `varchar(15)`, sedangkan status barang `Menunggu Dititipkan` panjangnya 19 karakter dan disimpan pada `items.status varchar(20)`.
