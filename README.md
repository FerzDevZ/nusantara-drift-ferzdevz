# 🏎️ FerzRP: Ultimate Drift (open.mp)

**FerzRP** adalah gamemode *Ultimate Drift* modern yang dibangun di atas framework **open.mp**. Gamemode ini dirancang dengan arsitektur modular tinggi, performa optimal, dan fitur visual premium untuk memberikan pengalaman drifting terbaik di SA-MP/open.mp.

---

## ✨ Fitur Unggulan

- **🚀 Modular Architecture**: Seluruh logic dipisah ke dalam modul `.inc` (Tuning, Garage, Clan, Mission, dll) untuk kemudahan pengembangan.
- **💨 Advanced Drift System**: Perhitungan poin drift berbasis kecepatan, sudut (angle), dan bonus **Tandem** real-time.
- **🛠️ Sticker & Visual Editor**: Sistem stiker kendaraan kustom dan lampu neon (Underglow) yang tersimpan permanen di database.
- **🏁 Daily Missions**: Tantangan harian untuk mendapatkan poin drift ekstra.
- **🛡️ Security Hardening**: Proteksi SQL Injection, anti-brute force, dan sistem autentikasi aman.
- **⛽ Realistic Fuel System**: Manajemen bensin dengan indikator HUD dan sistem kontrol mesin otomatis.
- **👥 Dynamic Clan System**: Buat kelompok, undang anggota, dan kumpulkan skor klan tertinggi.

---

## 🛠️ Tech Stack & Dependencies

- **Platform**: [open.mp](https://open.mp/)
- **Language**: Pawn 3.10.x
- **Database**: Native SQLite (Tanpa plugin eksternal)
- **HUD**: Modern FiveM-Style TextDraws
- **Security**: SHA256 Hashing for Passwords

---

## 📁 Struktur Projek

```text
gamemodes/
├── drift.pwn            # Main entry & Core callbacks
└── modules/             # Modul Fitur (Modularized)
    ├── auth.inc         # Login/Register System
    ├── database.inc     # SQLite Engine & Persistence
    ├── tuning.inc       # Visual Modification Shop
    ├── visual.inc       # Object Attachment & Stickers
    ├── fuel.inc         # Advanced Vehicle Mechanics
    └── [others]...      # Ghost Duel, Clans, Missions, dll.
```

---

## 🚀 Cara Menjalankan

1. Download dan instal server package **open.mp**.
2. Masukkan file `gamemodes/drift.pwn` dan seluruh isi folder `modules/` ke direktori server Anda.
3. Pastikan folder `qawno/` sudah dikonfigurasi dengan include open.mp terbaru.
4. Compile `drift.pwn`.
5. Jalankan server dan selamat drifting!

---

## 📝 Konten & Lisensi

Projek dikembangkan oleh **FerzDevZ** (Nusantara Drift).
- **YouTube**: [Ferzsampp](https://youtube.com/c/ferzsampp)
- **Instagram**: [ferzchills](https://instagram.com/ferzchills)
- **Discord**: [ferzdevz](https://discord.gg/ferzdevz)

---
*Dibuat dengan ❤️ untuk komunitas SA-MP Indonesia.*
