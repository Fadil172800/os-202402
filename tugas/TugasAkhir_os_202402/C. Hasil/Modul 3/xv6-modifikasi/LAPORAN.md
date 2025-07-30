# 📝 Laporan Tugas Akhir

**Mata Kuliah**: Sistem Operasi
**Semester**: Genap / Tahun Ajaran 2024–2025
**Nama**: `<Syukron Nur Fadillah>`
**NIM**: `<240202885>`
**Modul yang Dikerjakan**:
`(Contoh: Modul 3A – Copy-On-Write (COW) Memory Allocation)`
`(Contoh: Modul 3B – Copy-On-Write (COW) Memory Allocation)`
---

## 📌 Deskripsi Singkat Tugas

Tuliskan deskripsi singkat dari modul yang Anda kerjakan. Misalnya:

* **Modul 3A – Copy-On-Write (COW) Memory Allocation**:
  Mengimplementasikan mekanisme Copy-On-Write saat proses melakukan fork(). Hal ini dilakukan dengan memodifikasi fork() agar menggunakan cowuvm() alih-alih menyalin seluruh memori. Saat terjadi page fault karena penulisan, trap handler mendeteksi bit PTE_COW, mengalokasikan halaman baru, menyalin konten, dan memperbarui page table. Fungsi allocuvm(), deallocuvm(), dan freevm() juga dimodifikasi agar sinkron dengan ref_count[]. Mekanisme ini menghemat memori dan meningkatkan efisiensi proses fork().

* **Modul 3B – Copy-On-Write (COW) Memory Allocation**:
---

## 🛠️ Rincian Implementasi

Tuliskan secara ringkas namun jelas apa yang Anda lakukan:

MODUL 3A:
*Menambahkan fungsi cowuvm() di vm.c sebagai pengganti copyuvm() untuk menduplikasi page table dengan mekanisme Copy-On-Write
*Mengubah fork() di proc.c agar menggunakan cowuvm() dan menyesuaikan penanganan error
*Menambahkan handler untuk page fault T_PGFLT di trap.c, termasuk pengecekan bit PTE_COW, alokasi halaman baru, dan update page table
*Menambahkan dan mengelola array ref_count[] untuk menghitung referensi halaman fisik di vm.c
*Memodifikasi kalloc() dan kfree() agar sinkron dengan ref_count[] menggunakan incref() dan decref()
*Mengubah fungsi allocuvm(), deallocuvm(), dan freevm() agar mengganti kfree() menjadi decref() setelah incref() digunakan
*Menambahkan deklarasi eksternal seperti extern pde_t* cowuvm(...) dan extern pte_t* walkpgdir(...) di file yang membutuhkannya
*Membuat program uji cowtest.c untuk memastikan memori bersama tidak disalin hingga terjadi penulisan (write) oleh child process

MODUL 3B
---

## ✅ Uji Fungsionalitas

Tuliskan program uji apa saja yang Anda gunakan
MODUL 3A:
*cowtest: untuk menguji fungsionalitas fork dengan mekanisme Copy-on-Write (COW)
*Program ini memastikan bahwa:
   -Page tidak disalin saat fork, kecuali ketika proses anak melakukan penulisan (write)
   -Page fault tertangani dengan benar dan memicu penyalinan halaman hanya saat diperlukan
   -Referensi ref_count[] meningkat saat berbagi page dan berkurang saat proses selesai

MODUL 3B:
---

## 📷 Hasil Uji

Lampirkan hasil uji berupa screenshot atau output terminal. Contoh:

### 📍 Contoh Output `cowtest`: MODUL 3A

```
$ cowtest
Child sees: Y
Parent sees: X
$ 
```

### 📍 Contoh Output `shmtest`: MODUL 3B

```
Child reads: A
Parent reads: B
```

### 📍 Contoh Output `chmodtest`:

```
Write blocked as expected
```

Jika ada screenshot:

```MODUL 3A
![hasil cowtest](<img width="1919" height="1079" alt="Screenshot 2025-07-30 104104 3a" src="https://github.com/user-attachments/assets/2c0e8899-88f6-4ab1-b8f9-686d45364462" />
)
```MODUL 3B

---

## ⚠️ Kendala yang Dihadapi

Tuliskan kendala (jika ada), misalnya:
-MODUL 3A
* **`walkpgdir` undefined reference**: karena fungsi dideklarasikan `static` di `vm.c`, sehingga tidak bisa dipanggil dari `trap.c`. Solusinya: tambahkan `extern` dan hilangkan `static` agar dapat diakses lintas file.
* **`ref_count` dan `decref` undeclared**: perlu deklarasi `extern` di `trap.c` agar dikenali saat kompilasi.
* **`cowuvm` implicit declaration**: karena belum ditambahkan ke `proc.c`, solusinya tambahkan `extern` declaration di atas `proc.c`.
* **Salah urutan `incref()` dan `decref()`**: jika lupa memanggil `incref` setelah `kalloc()`, maka `decref` bisa menyebabkan free page yang belum dialokasikan.
* **Kernel panic saat `page fault` tidak valid**: perlu validasi `PTE_P` dan `PTE_COW` sebelum melakukan COW.

-MODUL 3B
---

## 📚 Referensi

Tuliskan sumber referensi yang Anda gunakan, misalnya:

* Buku xv6 MIT: [https://pdos.csail.mit.edu/6.828/2018/xv6/book-rev11.pdf](https://pdos.csail.mit.edu/6.828/2018/xv6/book-rev11.pdf)
* Repositori xv6-public: [https://github.com/mit-pdos/xv6-public](https://github.com/mit-pdos/xv6-public)
* Stack Overflow, GitHub Issues, diskusi praktikum

---


