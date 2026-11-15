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
```
