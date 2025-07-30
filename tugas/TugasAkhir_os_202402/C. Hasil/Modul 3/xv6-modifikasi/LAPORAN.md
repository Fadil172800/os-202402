# 📝 Laporan Tugas Akhir

**Mata Kuliah**: Sistem Operasi
**Semester**: Genap / Tahun Ajaran 2024–2025
**Nama**: `<Syukron Nur Fadillah>`
**NIM**: `<240202885>`
**Modul yang Dikerjakan**:
`(Contoh: Modul 3A – Copy-On-Write (COW) Memory Allocation)`
`(Contoh: Modul 5A – Shared Memory (shmget dan shmrelease System Calls))`
---

## 📌 Deskripsi Singkat Tugas

Tuliskan deskripsi singkat dari modul yang Anda kerjakan. Misalnya:

* **Modul 3A – Copy-On-Write (COW) Memory Allocation**:
  Mengimplementasikan mekanisme Copy-On-Write saat proses melakukan fork(). Hal ini dilakukan dengan memodifikasi fork() agar menggunakan cowuvm() alih-alih menyalin seluruh memori. Saat terjadi page fault karena penulisan, trap handler mendeteksi bit PTE_COW, mengalokasikan halaman baru, menyalin konten, dan memperbarui page table. Fungsi allocuvm(), deallocuvm(), dan freevm() juga dimodifikasi agar sinkron dengan ref_count[]. Mekanisme ini menghemat memori dan meningkatkan efisiensi proses fork().

* **Modul 3B – Shared Memory via System Call (shmget dan shmrelease)**:
  Mengimplementasikan shared memory antar proses di XV6 melalui dua system call: shmget(int key) dan shmrelease(int key). Shared memory dikelola dalam array shmtab[] yang menyimpan key, pointer ke frame (alamat fisik), dan reference count. Proses yang memanggil shmget() akan mendapatkan page yang dipetakan ke virtual address tertentu (dekat USERTOP) melalui fungsi mappages(). Bila key sudah ada, page yang sama akan dibagikan ke proses pemanggil. shmrelease() digunakan untuk mengurangi reference count dan membebaskan frame jika tidak digunakan lagi. Implementasi ini memungkinkan beberapa proses untuk saling berbagi data secara efisien tanpa perlu menggunakan pipe atau file, serta dilengkapi mekanisme locking untuk menjaga konsistensi data antar proses.
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
*Menambahkan dua system call baru: sys_shmget() dan sys_shmrelease() di sysproc.c untuk mengelola shared memory.
*Menambahkan deklarasi prototipe fungsi void* shmget(int key) dan int shmrelease(int key) di user.h dan ulib.c.
*Menambahkan entri system call baru ke dalam syscall.c, usys.S, dan syscall.h untuk menghubungkan sistem call dengan kernel.
*Menambahkan array global shmtab[MAX_SHM] di sysproc.c untuk menyimpan informasi shared memory: key, frame, refcount, dan spinlock.
*Mengimplementasikan shmget() agar mengalokasikan frame baru bila key belum ada, atau mengembalikan virtual address yang sama jika key sudah ada.
*Menambahkan mekanisme mapping frame ke virtual memory user space menggunakan mappages() ke alamat (USERTOP - (i+1)*PGSIZE).
*Menambahkan fungsi shmrelease() untuk menurunkan reference count dan membebaskan frame jika tidak ada proses lain yang mengakses.
*Memastikan fungsi mappages() dikenali di sysproc.c dengan menambahkan int mappages(...) sebagai deklarasi manual (karena tidak pakai vm.h).
*Mendefinisikan USERTOP di memlayout.h untuk menentukan batas atas virtual address space proses user.
*Membuat locking menggunakan acquire() dan release() pada shmtab[i].lock untuk menjaga konsistensi antar proses.
*Menyesuaikan semua pemanggilan proc->pgdir menjadi myproc()->pgdir untuk mendukung konteks proses saat ini.
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
*shmtest: program uji untuk memastikan shared memory dapat digunakan oleh beberapa proses secara bersamaan.
  -Proses parent memanggil shmget(key) dan menulis data ke shared memory.
  -Proses child menggunakan shmget(key) yang sama dan dapat membaca data yang ditulis oleh parent.
  -Verifikasi keberhasilan mapping dengan mencetak alamat virtual yang dikembalikan oleh shmget().
  -Cek bahwa refcount bertambah saat child mengakses shared memory, dan berkurang saat shmrelease() dipanggil.
*shmrelease test: menguji apakah shared memory benar-benar dibebaskan setelah tidak digunakan oleh proses manapun.
  -Setelah shmrelease() terakhir, entry shmtab yang sesuai menjadi kosong kembali (key = 0, frame = 0, refcount = 0).
*Semua pengujian dilakukan dengan mencetak hasil di console dan memverifikasi melalui print manual serta observasi kernel log jika ada panic atau kesalahan          mapping.
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
SeaBIOS (version 1.14.0-2)
iPXE (http://ipxe.org) 00:03.0 CA00 PCI2.10 PnP PMM+1FF8F4C0+1FECF4C0 CA00
Booting from Hard Disk..xv6...
cpu1: starting 1
cpu0: starting 0
sb: size 1000 nblocks 941 ninodes 200 nlog 30 logstart 2 inodestart 32 bmap start 58
init: starting sh

$ shmtest
Child reads: A
Parent reads: B
$
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
![hasil shmtest](<img width="1919" height="1079" alt="MODUL 3B" src="https://github.com/user-attachments/assets/e1634444-f228-494b-a5f2-acae9489774a" />
>
---

## ⚠️ Kendala yang Dihadapi

Tuliskan kendala (jika ada), 
-MODUL 3A
* **`walkpgdir` undefined reference**: karena fungsi dideklarasikan `static` di `vm.c`, sehingga tidak bisa dipanggil dari `trap.c`. Solusinya: tambahkan `extern` dan hilangkan `static` agar dapat diakses lintas file.
* **`ref_count` dan `decref` undeclared**: perlu deklarasi `extern` di `trap.c` agar dikenali saat kompilasi.
* **`cowuvm` implicit declaration**: karena belum ditambahkan ke `proc.c`, solusinya tambahkan `extern` declaration di atas `proc.c`.
* **Salah urutan `incref()` dan `decref()`**: jika lupa memanggil `incref` setelah `kalloc()`, maka `decref` bisa menyebabkan free page yang belum dialokasikan.
* **Kernel panic saat `page fault` tidak valid**: perlu validasi `PTE_P` dan `PTE_COW` sebelum melakukan COW.

-MODUL 3B
Berikut adalah dokumentasi **Kendala yang Dihadapi** untuk **Modul 3B – Shared Memory:
* **`USERTOP` undeclared**: error karena `USERTOP` belum didefinisikan di `memlayout.h`. Solusinya adalah menambahkan `#define USERTOP 0xA0000` di file `memlayout.h`.
* **`mappages` undefined reference**: error saat linking karena `mappages` tidak memiliki deklarasi eksternal. Solusi: tambahkan `int mappages(...)` sebagai deklarasi di atas `sysproc.c`.
* **`kmem` multiple definition**: error karena `kmem` didefinisikan dua kali, yaitu di `vm.c` dan `kalloc.c`. Solusinya: hapus definisi `kmem` dari `vm.c`, karena aslinya memang berada di `kalloc.c`.
* **`freerange`, `end` undeclared in vm.c**: error saat memanggil fungsi atau variabel yang tidak dikenal. Solusinya: pastikan `freerange()` dan `end` dideklarasikan secara `extern` jika dipakai lintas file, atau cukup digunakan dalam file asal (`kalloc.c`).
* **Error implicit declaration function**: terjadi karena tidak semua fungsi memiliki prototipe (declaration). Solusinya: pastikan semua fungsi seperti `freerange`, `mappages`, `shmget`, dll., dideklarasikan sebelum dipanggil.
* **Shared memory tidak ter-mapping dengan benar**: terjadi saat `frame` `NULL` atau `key` tidak dicocokkan dengan benar. Solusinya: periksa kembali urutan logika `shmtab`, locking, dan `mappages`.
---

Silakan sesuaikan jika ada kendala tambahan yang Anda alami selama implementasi.

---

## 📚 Referensi

Tuliskan sumber referensi yang Anda gunakan, misalnya:

* Buku xv6 MIT: [https://pdos.csail.mit.edu/6.828/2018/xv6/book-rev11.pdf](https://pdos.csail.mit.edu/6.828/2018/xv6/book-rev11.pdf)
* Repositori xv6-public: [https://github.com/mit-pdos/xv6-public](https://github.com/mit-pdos/xv6-public)
* Stack Overflow, GitHub Issues, diskusi praktikum

---


