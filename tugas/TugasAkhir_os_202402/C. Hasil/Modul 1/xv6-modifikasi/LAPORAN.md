# 📝 Laporan Tugas Akhir

**Mata Kuliah**: Sistem Operasi
**Semester**: Genap / Tahun Ajaran 2024–2025
**Nama**: `<Syukron Nur Fadillah>`
**NIM**: `<240202885>`
**Modul yang Dikerjakan**:
`(Modul 1 – System Call dan Instrumentasi Kernel
Menambahkan system call baru (getpinfo() dan getreadcount()), serta membaca informasi proses dari kernel ke user space.)`

---

## 📌 Deskripsi Singkat Tugas

Tuliskan deskripsi singkat dari modul yang Anda kerjakan. Misalnya:

* **Modul 1 – System Call dan Instrumentasi Kernel**:
Menambahkan dua system call baru, yaitu getpinfo() untuk mendapatkan informasi proses yang sedang aktif (PID, penggunaan memori, dan nama proses), dan getreadcount() untuk menghitung berapa kali sistem call read() dipanggil sejak sistem dinyalakan.
---

## 🛠️ Rincian Implementasi

Tuliskan secara ringkas namun jelas apa yang Anda lakukan:

### Contoh untuk Modul 1:
*Menambahkan dua system call baru: getpinfo() dan getreadcount() di file sysproc.c
*Mendaftarkan syscall di syscall.c, syscall.h, usys.S, dan user.h
*Menambahkan struktur struct pinfo di file proc.h untuk menyimpan informasi proses (PID, memori, nama)
*Menambahkan variabel global readcount di kernel untuk menghitung jumlah pemanggilan read()
*Memodifikasi syscall sys_read() untuk menambahkan counter readcount++
*Membuat dua program user-level untuk menguji syscall:
  - ptest.c untuk menampilkan daftar proses yang aktif
  - rtest.c untuk menguji dan menampilkan jumlah pemanggilan read()
---

## ✅ Uji Fungsionalitas

Tuliskan program uji apa saja yang Anda gunakan:
-ptest: untuk menguji system call getpinfo(), yaitu menampilkan daftar proses aktif beserta PID, ukuran memori, dan nama proses.
-rtest: untuk menguji system call getreadcount(), yaitu menghitung berapa kali read() dipanggil sejak sistem dinyalakan.--

## 📷 Hasil Uji

Lampirkan hasil uji berupa screenshot atau output terminal. Contoh:

### 📍 Contoh Output `ptest`:

```
$ ptest
PID     MEM     NAME
1       12288   init
2       16384   sh
3       12288   ptest
```

### 📍 Contoh Output `rtest`:

```
$ rtest
Read Count Sebelum: 12
```

### 📍 Contoh Output `chmodtest`:

```
Write blocked as expected
```

Jika ada screenshot:

```
![hasil cowtest](<img width="1919" height="1079" alt="Screenshot 2025-07-28 201040" src="https://github.com/user-attachments/assets/e899c293-7660-439a-8f56-b49dc30d2d8a" />
)
```

---

## ⚠️ Kendala yang Dihadapi

Tuliskan kendala (jika ada), misalnya:

✅ Kendala yang Ditemui:
*Duplikasi struct pinfo
  struct pinfo didefinisikan ulang di pinfo.h dan proc.h, menyebabkan error saat kompilasi (redefinition of 'struct pinfo').
  ✔️ Solusi: Pindahkan definisi struct pinfo hanya ke pinfo.h dan hapus dari proc.h.

*Gagal include pinfo.h
  Error: fatal error: pinfo.h: No such file or directory
  ✔️ Solusi: Pastikan file pinfo.h dibuat dan disimpan di direktori proyek yang sama atau di path yang dikenali compiler.

*Struktur pinfo tidak dikenali di ptest.c
  Error: storage size of 'info' isn't known
  Penyebab: pinfo.h belum di-include di ptest.c, sehingga struktur tidak dikenali saat kompilasi.
  ✔️ Solusi: Tambahkan #include "pinfo.h" di awal ptest.c.

*Akses tidak tepat pada ptable di sys_getpinfo
  Error: menggunakan -> pada objek biasa, dan . pada pointer (ptable->lock vs ptable.lock)
  ✔️ Solusi: Pastikan kamu menggunakan . untuk objek dan -> untuk pointer dengan konsisten.

*Referensi field yang tidak ada di struct pinfo
  Error: ‘struct pinfo’ has no member named 'inuse'
  Penyebab: Struktur pinfo belum lengkap atau salah versi digunakan.
  ✔️ Solusi: Gunakan definisi struct pinfo yang benar seperti yang ada di pinfo.h.

*Program ptest tidak menampilkan output
  Penyebab: Kemungkinan inuse[i] tidak di-set, atau syscall getpinfo() gagal diam-diam.
  ✔️ Solusi: Tambahkan pengecekan getpinfo() di ptest.c dan debug dengan mencetak status panggilannya.

*Error di Makefile karena lupa menambahkan _ptest ke UPROGS
  Akibatnya binary tidak dibangun, dan tidak bisa dijalankan dari shell xv6.
  ✔️ Solusi: Tambahkan user/_ptest ke UPROGS di Makefile.
---

## 📚 Referensi

Tuliskan sumber referensi yang Anda gunakan, misalnya:

* Buku xv6 MIT: [https://pdos.csail.mit.edu/6.828/2018/xv6/book-rev11.pdf](https://pdos.csail.mit.edu/6.828/2018/xv6/book-rev11.pdf)
* Repositori xv6-public: [https://github.com/mit-pdos/xv6-public](https://github.com/mit-pdos/xv6-public)
* Stack Overflow, GitHub Issues, diskusi praktikum

---


