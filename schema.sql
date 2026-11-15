-- Skema MySQL Lost & Found Locker (mengikuti RAT final).
-- Jalankan sekali:  mysql -u root -p < schema.sql

CREATE DATABASE IF NOT EXISTS lostfound
  CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;
USE lostfound;

CREATE TABLE IF NOT EXISTS users (
  nim            VARCHAR(15)  NOT NULL,
  nama           VARCHAR(100) NOT NULL,
  jurusan        VARCHAR(50)  NOT NULL,
  email          VARCHAR(100) NULL,
  no_hp          VARCHAR(20)  NOT NULL,
  password_hash  VARCHAR(255) NOT NULL,
  role           VARCHAR(10)  NOT NULL,
  failed_login   INT UNSIGNED NOT NULL DEFAULT 0,
  locked_until   DATETIME     NULL,
  PRIMARY KEY (nim),
  UNIQUE KEY uq_users_email (email)
) ENGINE=InnoDB;

CREATE TABLE IF NOT EXISTS categories (
  id_kategori    INT UNSIGNED NOT NULL AUTO_INCREMENT,
  nama_kategori  VARCHAR(50)  NOT NULL,
  PRIMARY KEY (id_kategori),
  UNIQUE KEY uq_categories_nama (nama_kategori)
) ENGINE=InnoDB;

CREATE TABLE IF NOT EXISTS locations (
  id_lokasi      INT UNSIGNED NOT NULL AUTO_INCREMENT,
  nama_gedung    VARCHAR(100) NOT NULL,
  detail         VARCHAR(150) NOT NULL DEFAULT '',
  PRIMARY KEY (id_lokasi)
) ENGINE=InnoDB;

CREATE TABLE IF NOT EXISTS lockers (
  id_locker      INT UNSIGNED NOT NULL AUTO_INCREMENT,
  nomor_loker    INT UNSIGNED NOT NULL,
  lokasi_unit    VARCHAR(50)  NOT NULL,
  status         VARCHAR(15)  NOT NULL,
  PRIMARY KEY (id_locker),
  UNIQUE KEY uq_lockers_nomor (nomor_loker)
) ENGINE=InnoDB;

CREATE TABLE IF NOT EXISTS items (
  id_item          INT UNSIGNED NOT NULL AUTO_INCREMENT,
  nim_penemu       VARCHAR(15)  NOT NULL,
  id_kategori      INT UNSIGNED NOT NULL,
  id_lokasi        INT UNSIGNED NOT NULL,
  nama_barang      VARCHAR(100) NOT NULL,
  ciri_ciri        TEXT         NOT NULL,
  tanggal_temuan   DATE         NOT NULL,
  data_verifikasi  TEXT         NOT NULL,
  status           VARCHAR(20)  NOT NULL,
  PRIMARY KEY (id_item),
  FOREIGN KEY (nim_penemu)  REFERENCES users (nim),
  FOREIGN KEY (id_kategori) REFERENCES categories (id_kategori),
  FOREIGN KEY (id_lokasi)   REFERENCES locations (id_lokasi)
) ENGINE=InnoDB;

CREATE TABLE IF NOT EXISTS lost_reports (
  id_laporan     INT UNSIGNED NOT NULL AUTO_INCREMENT,
  nim_pelapor    VARCHAR(15)  NOT NULL,
  id_kategori    INT UNSIGNED NOT NULL,
  id_lokasi      INT UNSIGNED NOT NULL,
  nama_barang    VARCHAR(100) NOT NULL,
  ciri_ciri      TEXT         NOT NULL,
  tanggal_hilang DATE         NOT NULL,
  status         VARCHAR(15)  NOT NULL,
  PRIMARY KEY (id_laporan),
  FOREIGN KEY (nim_pelapor) REFERENCES users (nim),
  FOREIGN KEY (id_kategori) REFERENCES categories (id_kategori),
  FOREIGN KEY (id_lokasi)   REFERENCES locations (id_lokasi)
) ENGINE=InnoDB;

CREATE TABLE IF NOT EXISTS claims (
  id_claim       INT UNSIGNED NOT NULL AUTO_INCREMENT,
  id_item        INT UNSIGNED NOT NULL,
  nim_pengklaim  VARCHAR(15)  NOT NULL,
  detail_claim   TEXT         NOT NULL,
  failed_count   INT UNSIGNED NOT NULL DEFAULT 0,
  status         VARCHAR(15)  NOT NULL,
  claimed_at     DATETIME     NULL,
  verified_at    DATETIME     NULL,
  PRIMARY KEY (id_claim),
  FOREIGN KEY (id_item)       REFERENCES items (id_item),
  FOREIGN KEY (nim_pengklaim) REFERENCES users (nim)
) ENGINE=InnoDB;

CREATE TABLE IF NOT EXISTS deposits (
  id_deposit         INT UNSIGNED NOT NULL AUTO_INCREMENT,
  id_item            INT UNSIGNED NOT NULL,
  id_locker          INT UNSIGNED NOT NULL,
  reserved_at        DATETIME     NULL,
  reserve_expires_at DATETIME     NULL,
  deposited_at       DATETIME     NULL,
  picked_up_at       DATETIME     NULL,
  status             VARCHAR(15)  NOT NULL,
  PRIMARY KEY (id_deposit),
  FOREIGN KEY (id_item)   REFERENCES items (id_item),
  FOREIGN KEY (id_locker) REFERENCES lockers (id_locker)
) ENGINE=InnoDB;

-- Tepat satu dari id_deposit (kode titip) atau id_claim (kode ambil) terisi.
CREATE TABLE IF NOT EXISTS access_codes (
  id_code        INT UNSIGNED NOT NULL AUTO_INCREMENT,
  id_deposit     INT UNSIGNED NULL,
  id_claim       INT UNSIGNED NULL,
  tipe           VARCHAR(10)  NOT NULL,
  code_hash      VARCHAR(255) NOT NULL,
  expires_at     DATETIME     NOT NULL,
  used_at        DATETIME     NULL,
  failed_count   INT UNSIGNED NOT NULL DEFAULT 0,
  status         VARCHAR(15)  NOT NULL,
  PRIMARY KEY (id_code),
  FOREIGN KEY (id_deposit) REFERENCES deposits (id_deposit),
  FOREIGN KEY (id_claim)   REFERENCES claims (id_claim),
  CHECK ((id_deposit IS NULL) <> (id_claim IS NULL))
) ENGINE=InnoDB;

CREATE TABLE IF NOT EXISTS locker_event_logs (
  id_log         INT UNSIGNED NOT NULL AUTO_INCREMENT,
  id_locker      INT UNSIGNED NOT NULL,
  id_deposit     INT UNSIGNED NULL,
  tipe_event     VARCHAR(20)  NOT NULL,
  waktu          DATETIME     NOT NULL,
  keterangan     VARCHAR(255) NOT NULL DEFAULT '',
  PRIMARY KEY (id_log),
  FOREIGN KEY (id_locker)  REFERENCES lockers (id_locker),
  FOREIGN KEY (id_deposit) REFERENCES deposits (id_deposit)
) ENGINE=InnoDB;
