# Flowchart tests

Skrip Expect ini membagi `docs/flowchart.md` menurut alurnya. Setiap skrip membangun `build/lostfound-test` (dengan flag MySQL), membuat database sendiri dari `schema.sql`, lalu memeriksa keluaran program dan isi tabelnya.

| Skrip | Alur yang dicakup |
| --- | --- |
| `01-authentication.exp` | Registrasi, 3 gagal login mengunci akun, login, logout |
| `02-deposit.exp` | Reservasi loker, PIN salah, PIN valid, penitipan selesai |
| `03-claim-pickup.exp` | Cari barang, klaim, kembali dengan `0`, lanjutkan klaim aktif, PIN ambil salah, pengambilan selesai |
| `04-lost-report.exp` | Pencarian tanpa hasil dan laporan barang hilang |
| `05-history.exp` | Riwayat titipan mahasiswa |
| `06-admin.exp` | Anomali loker, klaim diblokir, persetujuan admin, reset klaim aktif, status loker |
| `07-lock-restart.exp` | Kunci akun 15 menit dan kolom role tetap berlaku setelah program ditutup dan dibuka lagi |
| `08-locker-log.exp` | Urutan event log loker dari alur titip lalu ambil, dan log bertahan setelah restart |
| `09-claim-status.exp` | Status klaim: Diblokir, Disetujui admin, Selesai setelah pengambilan, bertahan setelah restart |
| `10-unique-email.exp` | Email ganda ditolak tanpa membuat akun setengah jadi, email beda berhasil |
| `11-deposit-timestamps.exp` | Stempel reserved_at/deposited_at/picked_up_at dan status deposit tersimpan di database |
| `12-claim-detail.exp` | Detail jawaban pengklaim tersimpan dan tampil di daftar klaim diblokir admin |
| `13-hashing.exp` | Password dan kode loker tersimpan sebagai digest SHA-256, bukan teks asli |
| `14-field-limits.exp` | NIM dan nama barang ditolak bila melebihi ukuran varchar pada RAT |

## Menjalankan

Setiap skrip memakai direktori sementara dan database sendiri, jadi data proyek
tidak tersentuh.

```sh
expect test/01-authentication.exp
expect test/02-deposit.exp
expect test/03-claim-pickup.exp
expect test/04-lost-report.exp
expect test/05-history.exp
expect test/06-admin.exp
expect test/07-lock-restart.exp
expect test/08-locker-log.exp
expect test/09-claim-status.exp
expect test/10-unique-email.exp
expect test/11-deposit-timestamps.exp
expect test/12-claim-detail.exp
expect test/13-hashing.exp
expect test/14-field-limits.exp
```

Database dibutuhkan oleh semua skrip. Nama database tiap skrip diturunkan dari
nama file (`lostfound_test_07_lock_restart`), dibuat ulang setiap kali dijalankan.

```sh
export LF_DB_HOST=127.0.0.1 LF_DB_PORT=3306 LF_DB_USER=root LF_DB_PASS=rahasia
export LF_DB_ADMIN_ARGS="-u root -prahasia"   # dipakai klien mariadb untuk setup
expect test/01-authentication.exp
```

Tanpa `LF_DB_ADMIN_ARGS`, klien memakai opsi default pengguna saat ini. Jika
server tidak dapat dihubungi, skrip berhenti dengan pesan yang jelas.

`demo.exp` di root repo menjalankan demo penuh untuk presentasi (membangun
ulang `lostfound` lebih dulu agar tidak memakai binary lama).
