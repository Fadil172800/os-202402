# 📝 Laporan Tugas Akhir

**Mata Kuliah**: Sistem Operasi
**Semester**: Genap / Tahun Ajaran 2024–2025
**Nama**: `<Syukron Nur Fadillah>`
**NIM**: `<240202885>`
**Modul yang Dikerjakan**:
Modul 2 – Penjadwalan Proses (Process Scheduling) dan Modifikasi Kernel
Modul ini melibatkan:
  *Penambahan atribut priority ke dalam struktur proc
  *Modifikasi fungsi allocproc() untuk inisialisasi prioritas default
  *Implementasi syscall baru set_priority(int)
  *Penyesuaian fungsi scheduler() agar mempertimbangkan prioritas proses
  *Pengujian output eksekusi proses berdasarkan prioritas
---

## 📌 Deskripsi Singkat Tugas

Tuliskan deskripsi singkat dari modul yang Anda kerjakan. Misalnya:

Modul 2 – Penjadwalan Proses (Non-Preemptive Priority Scheduling)
Melakukan modifikasi terhadap kernel xv6 agar menggunakan algoritma penjadwalan Prioritas Non-Preemptive.
Perubahan utama meliputi:
  *Menambahkan field priority pada struktur proc
  *Menambahkan syscall baru set_priority(int) untuk mengatur prioritas proses dari ruang pengguna
  *Memodifikasi fungsi scheduler() agar hanya menjalankan proses dengan status RUNNABLE dan prioritas numerik tertinggi (angka terkecil)
  *Menambahkan program uji ptest.c untuk memastikan scheduler berjalan sesuai prioritas
  *Tujuan dari tugas ini adalah agar proses dengan angka prioritas lebih kecil dijalankan lebih dulu, dan proses lain harus menunggu hingga selesai (non-             preemptive).
---

## 🛠️ Rincian Implementasi

Tuliskan secara ringkas namun jelas apa yang Anda lakukan:

*Menambahkan field int priority di struktur proc pada file proc.h
*Menginisialisasi nilai default priority = 60 pada fungsi allocproc() di proc.c
*Menambahkan syscall baru set_priority(int) di:
  - sysproc.c → definisi fungsi sys_set_priority()
  - syscall.c → menambahkan extern sys_set_priority dan pemetaan di tabel syscalls[]
  - syscall.h → menambahkan definisi #define SYS_set_priority 24
  - usys.S dan user.h → deklarasi syscall untuk dapat dipanggil dari user space
*Memodifikasi fungsi scheduler() di proc.c:
  - Menambahkan logika untuk memilih proses RUNNABLE dengan nilai priority paling kecil
  - Mengganti proc = p dengan c->proc = p dan menyesuaikan cpu = mycpu() agar tidak terjadi error
*Menambahkan pemanggilan yield() di akhir sys_set_priority() agar proses langsung menyerahkan CPU bila perlu
*Membuat program uji ptest.c untuk menguji apakah proses dengan prioritas lebih tinggi benar dijalankan lebih dahulu
*Menambahkan sleep(1) pada ptest.c sebelum fork kedua agar hasil output sesuai prioritas (non-preemptive)
---

## ✅ Uji Fungsionalitas

Tuliskan program uji apa saja yang Anda gunakan,
*ptest:
Menguji apakah proses dengan prioritas lebih tinggi (angka lebih kecil) dijalankan terlebih dahulu dibanding proses dengan prioritas lebih rendah, sesuai dengan algoritma Non-Preemptive Priority Scheduling.
---

## 📷 Hasil Uji

Lampirkan hasil uji berupa screenshot atau output terminal. Contoh:

### 📍 Contoh Output `ptest`:

```
$ ptest
Child 2 selesai
Child 1 selesai
Parent selesai
$ 
```

### 📍 Contoh Output `shmtest`:

```
Child reads: A
Parent reads: B
```

### 📍 Contoh Output `chmodtest`:

```
Write blocked as expected
```

Jika ada screenshot:

```
![hasil cowtest](<img width="1919" height="1077" alt="Screenshot 2025-07-28 223219" src="https://github.com/user-attachments/assets/ca4b7ed9-7963-4f82-8c62-3d10816c5d15" />
)
```

---

## ⚠️ Kendala yang Dihadapi

Tuliskan kendala (jika ada), misalnya:

*Duplikasi fungsi sys_getpinfo
   Fungsi sys_getpinfo() tertulis dua kali dalam file sysproc.c, menyebabkan error saat kompilasi.
   -Solusi: Menghapus salah satu fungsi sys_getpinfo() yang duplikat, hanya mempertahankan versi yang lengkap dan benar.

*Literal ... disalin ke kode
   Baris placeholder ... dari dokumentasi disalin mentah ke file .c, menyebabkan error expected identifier.
   -Solusi: Menghapus semua baris ... yang bukan bagian dari sintaks C.

*Variabel proc dan cpu tidak dikenali di scheduler()
   Error muncul karena proc dan cpu digunakan tanpa deklarasi di dalam fungsi scheduler().
   -Solusi: Menambahkan struct cpu *c = mycpu(); dan mengganti proc = p; menjadi c->proc = p; agar sesuai dengan struktur xv6.

*Proses dengan prioritas lebih tinggi tidak selalu dijalankan lebih dulu
   Saat fork() dilakukan dalam urutan yang salah atau tanpa sinkronisasi, proses prioritas rendah bisa selesai lebih cepat.
   -Solusi: Mengubah urutan fork() di ptest.c sehingga proses prioritas tinggi (Child 2) dibuat lebih dulu. Ditambahkan juga sleep(1) untuk memberi waktu             scheduler.

*Fungsi sys_set_priority() tidak memanggil yield()
   Setelah prioritas diubah, sistem tidak langsung melakukan penjadwalan ulang, sehingga proses dengan prioritas lebih rendah tetap berjalan.
   -Solusi: Menambahkan yield(); di akhir sys_set_priority() agar proses menyerahkan CPU secara eksplisit.

*Tidak validasi nomor syscall
   SYS_set_priority langsung menggunakan nomor 24 tanpa mengecek apakah sudah digunakan.
   -Solusi: Mengecek isi syscall.h untuk memastikan nomor 24 belum digunakan oleh syscall lain.

Output proses tidak sesuai urutan prioritas
   Meski kode scheduler sudah benar, hasil cetakan bisa tidak sesuai ekspektasi karena proses tidak diberi waktu untuk bergiliran.
   -Solusi: Menambahkan sleep(1) di antara fork() agar proses memiliki waktu untuk masuk ke scheduler berdasarkan prioritasnya.
---

## 📚 Referensi

Tuliskan sumber referensi yang Anda gunakan, misalnya:

* Buku xv6 MIT: [https://pdos.csail.mit.edu/6.828/2018/xv6/book-rev11.pdf](https://pdos.csail.mit.edu/6.828/2018/xv6/book-rev11.pdf)
* Repositori xv6-public: [https://github.com/mit-pdos/xv6-public](https://github.com/mit-pdos/xv6-public)
* Stack Overflow, GitHub Issues, diskusi praktikum

---


