# 🏎️ FerzRP: Ultimate Drift (open.mp)

[![Platform](https://img.shields.io/badge/Platform-open.mp-orange.svg?style=flat-square)](https://open.mp/)
[![Language](https://img.shields.io/badge/Language-Pawn-blue.svg?style=flat-square)](https://github.com/pawn-lang/compiler)
[![Database](https://img.shields.io/badge/Database-SQLite-lightgrey.svg?style=flat-square)](https://www.sqlite.org/)
[![License](https://img.shields.io/badge/License-MIT-green.svg?style=flat-square)](LICENSE)

**FerzRP** adalah gamemode *Ultimate Drift* modern yang dibangun eksklusif untuk framework **open.mp**. Berbeda dengan gamemode drift tradisional, FerzRP menggunakan pendekatan **High-Performance Modular Architecture**, memastikan server tetap stabil bahkan dengan ratusan pemain dan ribuan objek kustom.

---

## 🌟 Fitur Utama (Highlight)

### 🚀 Arsitektur Modular Tinggi
Seluruh sistem tidak menumpuk di satu file `.pwn`, melainkan dipisah ke puluhan modul `.inc` di folder `modules/`. Ini memudahkan kolaborasi tim dan debugging:
- **Zero Circular Dependency**: Struktur include yang rapi.
- **Easy Maintenance**: Ubah satu fitur tanpa merusak fitur lainnya.

### 💨 Next-Gen Drift & Tandem System
Algoritma perhitungan poin yang adil dan dinamis:
- **Speed & Angle Scaling**: Semakin cepat dan tajam drift Anda, semakin besar poinnya.
- **Real-time Tandem Bonus**: Bonus poin x2 jika Anda melakukan drift berdampingan dengan pemain lain (Zoning system).
- **Drift Leaderboard**: Menampilkan skor tertinggi secara real-time di HUD.

### 🛠️ Visual Customization (Premium)
- **Sticker Editor**: Tempel stiker teks kustom di posisi mana pun pada body kendaraan. Data otomatis tersimpan di SQLite.
- **Dynamic Neon (Underglow)**: Lampu neon bawah mobil dengan offset yang menyesuaikan model kendaraan secara cerdas (SUV, Sedan, Supercar).
- **Persistence Visual**: Seluruh modifikasi visual (stiker & neon) tidak akan hilang saat server restart.

### 🛡️ Hardened Security
- **Anti-SQL Injection**: Seluruh query database difilter dengan `SQL_Escape`.
- **Brute Force Protection**: Pembatasan percobaan login (Kick otomatis setelah 5x salah).
- **Timer Race Condition Check**: Pengecekan koneksi pemain di setiap callback timer untuk mencegah bug dialog.

### ⛽ Realistic Ecosystem
- **Fuel Mechanics**: Bensin yang dikonsumsi secara real-time. Mesin akan mati jika bensin habis.
- **Daily Missions**: Sistem misi yang di-reset setiap hari secara otomatis (Score, Distance, Tandem).

---

## 🛠️ Struktur Folder & Modul

```text
ProjectRP/
├── drift.pwn              # File Utama (Entry Point)
├── README.md              # Dokumentasi ini
└── gamemodes/modules/     # Jantung dari Gamemode
    ├── auth.inc           # Sistem Login & Registrasi
    ├── database.inc       # Engine SQLite & Persistence logic
    ├── drift_system.inc   # Core logic perhitungan drift & tandem
    ├── visual.inc         # Logic stiker, neon, dan object attachment
    ├── tuning.inc         # Bengkel modifikasi visual (Dialog-based)
    ├── garage.inc         # Manajemen kendaraan pribadi & mod saving
    ├── clans.inc          # Sistem klan dinamis (Label klan & ranking)
    ├── fuel.inc           # Sistem bensin & konsumsi bahan bakar
    ├── tutorial.inc       # Alur tutorial untuk pemain baru
    └── hud.inc            # Desain & update TextDraw HUD (FiveM Style)
```

---

## ⌨️ Daftar Perintah (Commands)

### 🏎️ Kendaraan & Drift
- `/v [elegy/sultan/jester/flash/uranus]` - Spawn mobil drift instan.
- `/drift` - Menu teleportasi ke lokasi drift terbaik.
- `/duel [ghost_name]` - Tantang rekaman (ghost) pemain lain.
- `/rec [nama]` - Rekam aksi drift Anda sendiri.

### 🛠️ Kustomisasi & Toko
- `/shop` - Modifikasi neon, roda, dan nitro secara visual.
- `/sticker [teks]` - Tambahkan stiker teks unik ke kendaraan.
- `/fill` - Isi ulang bensin kendaraan Anda.
- `/mycars` - Daftar kendaraan pribadi yang Anda miliki.

### 👥 Sosial & Klan
- `/createclan [nama]` - Dirikan klan Anda sendiri (Butuh 10k Poin).
- `/invite [id]` - Ajak pemain lain bergabung ke klan Anda.
- `/claninfo` - Lihat statistik dan total poin klan.
- `/missions` - Periksa tantangan harian Anda.

---

## 💻 Cara Instalasi & Setup

1. **Unduh Framework**: Pastikan Anda memiliki server open.mp terbaru.
2. **Include**: Tempatkan folder `qawno/` yang berisi include open.mp di direktori root.
3. **Compile**:
   ```bash
   # Gunakan compiler PawnCC 3.10+
   pawncc gamemodes/drift.pwn -iincludes -oamx
   ```
4. **Configuration**: Edit `server.json` Anda:
   ```json
   {
     "gamemode": "drift",
     "mapname": "Nusantara Drift",
     "language": "Indonesian",
     "rcon_password": "ganti_dengan_rcon_anda"
   }
   ```
5. **Database**: Server akan otomatis membuat file `ferz_drift.db` saat pertama kali dijalankan.

---

## 🤝 Hubungi Penulis & Komunitas

Dapatkan dukungan teknis atau sekadar mengobrol dengan komunitas kami:

- **👾 Discord Community**: [Join FerzRP Community](https://discord.gg/f5QBgH8w9B)
- **📺 YouTube Channel**: [Ferzsampp](https://youtube.com/c/ferzsampp)
- **📸 Instagram Updates**: [@ferzchills](https://instagram.com/ferzchills)
- **💬 Lead Developer**: `ferzdevz`

---
*Dikembangkan dengan ❤️ untuk kemajuan komunitas SA-MP & open.mp Indonesia oleh **FerzDevZ**.*
