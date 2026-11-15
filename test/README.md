# Flowchart tests

Skrip Expect ini membagi `docs/flowchart.md` menurut alurnya. Setiap skrip membangun `build/lostfound-test`, memakai data unik berdasarkan PID, dan memeriksa keluaran penting.

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
| `11-legacy-data.exp` | File .txt format lama (users 5 kolom, items 8/16 kolom, claims 3 kolom) tetap terbaca lalu ditulis ulang ke format baru |
| `12-deposit-timestamps.exp` | Stempel reserved_at/deposited_at/picked_up_at dan used_at kode bertahan setelah restart |
| `13-claim-detail.exp` | Detail jawaban pengklaim tersimpan dan tampil di daftar klaim diblokir admin |
| `14-hashing.exp` | Password dan kode loker tersimpan sebagai digest SHA-256, plaintext lama di-upgrade saat login |
| `15-field-limits.exp` | NIM dan nama barang ditolak bila melebihi ukuran varchar pada RAT |

## Menjalankan

Setiap skrip memakai direktori sementara sendiri, jadi tidak membutuhkan database atau konfigurasi tambahan.

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
expect test/11-legacy-data.exp
expect test/12-deposit-timestamps.exp
expect test/13-claim-detail.exp
expect test/14-hashing.exp
expect test/15-field-limits.exp
```

`demo.exp` di root repo menjalankan demo penuh untuk presentasi (membangun
ulang `lostfound` lebih dulu agar tidak memakai binary lama).
