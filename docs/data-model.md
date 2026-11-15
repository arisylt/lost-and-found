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

## Penyimpanan: MySQL

Database adalah satu-satunya penyimpanan; tidak ada file teks. `loadAll()`
membaca semua tabel ke array saat program mulai, dan `saveAll()` menulis ulang
semua tabel dalam satu transaksi setelah setiap perubahan. Skema lengkap ada di
`schema.sql`.

| Tabel | Array tujuan / sumber |
| --- | --- |
| `users` | `userName`, `userPassword` (hash), `userPhone`, `userFailed`, `userLockedUntil`, `userFullName`, `userMajor`, `userEmail`, `userRole` |
| `lockers` | `lockerBroken` dari `status = 'Rusak'` |
| `items` | `foundReporter` (lewat `nim_penemu`), `foundCategory`/`foundLocation` (lewat join), `foundDate`, `foundName`, `foundDescription`, `foundSecret`, `foundStatus` |
| `lost_reports` | `lostReporter`, `lostCategory`, `lostLocation`, `lostDate`, `lostName`, `lostDescription`, `lostStatus` |
| `claims` | `claimItem`, `claimUser`, `claimDetail`, `claimFailed`, `claimStatus`, `claimClaimedAt`, `claimVerifiedAt` |
| `deposits` | `foundLocker`, `foundReservedAt`, `foundReservationExpiry`, `foundDepositedAt`, `foundPickedUpAt` |
| `access_codes` | `foundDepositCode`/`foundPickupCode` (hash), expiry, `used_at`, `failed_count`, `foundClaimant` |
| `locker_event_logs` | `logLocker`, `logItem`, `logType`, `logTime`, `logNote` |

Karena `saveAll()` menulis dengan id eksplisit (`id_item = indeks + 1`,
`id_claim = indeks klaim + 1`), memuat ulang tinggal membalik pemetaan itu.
Kategori, jurusan, dan lokasi tetap disimpan sebagai nama di array, sedangkan di
database berupa id yang di-join.

Status disimpan apa adanya dalam bahasa Indonesia (`Tersedia`, `Menunggu
Dititipkan`, `Disetujui`, `Diblokir`, `Dicari`, `Menunggu`/`Tersimpan`/`Diambil`
untuk deposit, `Aktif`/`Rusak` untuk loker) karena semua kolom status di
`schema.sql` bertipe `VARCHAR` tanpa enumerasi. Email kosong dan timestamp yang
belum ada menjadi `NULL`; hanya `access_codes.expires_at` dan
`locker_event_logs.waktu` yang memakai epoch karena `NOT NULL`.

Baris deposit tidak dihapus saat pengambilan selesai, sehingga `picked_up_at`
tetap tercatat. Loker dianggap kosong dari status barang (`Sudah Diambil` atau
`Reservasi Batal`), bukan dari nilai `id_locker`.

## Selisih yang masih terbuka

- `access_codes` hanya menyimpan kode yang masih aktif: begitu kode dipakai dan dihapus dari array, barisnya ikut hilang, jadi `used_at` paling lama hanya bertahan sampai kode tersebut hangus.
- `locations.detail` belum dipakai karena CLI hanya memilih nama lokasi.
- `deposits.status` bertipe `varchar(15)` dan memakai nilai pendek (`Menunggu`, `Tersimpan`, `Diambil`, `Periksa`, `Batal`), sedangkan status barang lengkap ada di `items.status varchar(20)`.
