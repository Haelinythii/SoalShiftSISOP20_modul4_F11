# SoalShiftSISOP20_modul4_F11

## Soal

```
Di suatu perusahaan, terdapat pekerja baru yang super jenius, ia bernama jasir. Jasir baru
bekerja selama seminggu di perusahan itu, dalam waktu seminggu tersebut ia selalu terhantui
oleh ketidak amanan dan ketidak efisienan file system yang digunakan perusahaan tersebut.
Sehingga ia merancang sebuah file system yang sangat aman dan efisien. Pada segi
keamanan, Jasir telah menemukan 2 buah metode enkripsi file. Pada mode enkripsi pertama,
nama file-file pada direktori terenkripsi akan dienkripsi menggunakan metode substitusi.
Sedangkan pada metode enkripsi yang kedua, file-file pada direktori terenkripsi akan di-split
menjadi file-file kecil. Sehingga orang-orang yang tidak menggunakan filesystem rancangannya
akan kebingungan saat mengakses direktori terenkripsi tersebut. Untuk segi efisiensi,
dikarenakan pada perusahaan tersebut sering dilaksanakan sinkronisasi antara 2 direktori,
maka jasir telah merumuskan sebuah metode agar filesystem-nya mampu mengsingkronkan
kedua direktori tersebut secara otomatis. Agar integritas filesystem tersebut lebih terjamin,
maka setiap command yang dilakukan akan dicatat kedalam sebuah file log.
(catatan: filesystem berfungsi normal layaknya linux pada umumnya)
(catatan: mount source (root) filesystem adalah direktori /home/[user]/Documents)
```

### 1. Enkripsi versi 1:

**Belum selesai**

### 2. Enkripsi versi 2:

**Belum selesai**

### 3. Sinkronisasi direktori otomatis:

**Belum selesai**

### 4. Log system:

**Soal :**
```
a. Sebuah berkas nantinya akan terbentuk bernama "fs.log" di direktori *home*
   pengguna (/home/[user]/fs.log) yang berguna menyimpan daftar perintah system
   call yang telah dijalankan.
b. Agar nantinya pencatatan lebih rapi dan terstruktur, log akan dibagi menjadi
   beberapa level yaitu INFO dan WARNING.
c. Untuk log level WARNING, merupakan pencatatan log untuk syscall rmdir dan unlink.
d. Sisanya, akan dicatat dengan level INFO.
e. Format untuk logging yaitu: [LEVEL]::[yy][mm][dd]-[HH]:[MM]:[SS]::[CMD]::[DESC ...]
```

**Jawaban :**

```c
// Membuat log dengan pesan Warning
void createLogWarning(const char *log, const char *path){
    FILE *fp;
    fp = fopen("/home/noradier/fs.log", "a");
    fputs("WARNING::", fp);
    char timestamp[1000];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(timestamp, "%02d%02d%02d-%02d:%02d:%02d::", (tm.tm_year + 1900)%100, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    fputs(timestamp, fp);
    fputs(log, fp);
    fputs("::", fp);
    fputs(path, fp);
    fputs("\n", fp);
    fclose(fp);
}

// Membuat log dengan pesan Info
void createLogInfo1(const char *log, const char *path){
    FILE *fp;
    fp = fopen("/home/noradier/fs.log", "a");
    fputs("INFO::", fp);
    char timestamp[1000];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(timestamp, "%02d%02d%02d-%02d:%02d:%02d::", (tm.tm_year + 1900)%100, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    fputs(timestamp, fp);
    fputs(log, fp);
    fputs("::", fp);
    fputs(path, fp);
    fputs("\n", fp);
    fclose(fp);
}

// Membuat log dengan pesan Info tapi menerima dua path, source dan destination
void createLogInfo2(const char *log, const char *source, const char *destination){
    FILE *fp;
    fp = fopen("/home/noradier/fs.log", "a");
    fputs("INFO::", fp);
    char timestamp[1000];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(timestamp, "%02d%02d%02d-%02d:%02d:%02d::", (tm.tm_year + 1900)%100, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    fputs(timestamp, fp);
    fputs(log, fp);
    fputs("::", fp);
    fputs(source, fp);
    fputs("::", fp);
    fputs(destination, fp);
    fputs("\n", fp);
    fclose(fp);
}
```

**Penjelasan :**
Setiap kali perintah rmdir dan unlink dipanggil, akan dimasukkan catatan ke dalam file fs.log ditandai dengan level WARNING. Selain itu, pemanggilan fungsi lain akan ditandai dengan level INFO. Isi pesan setelah tingkat level diikuti dengan timestamp yymmdd-HH:MM:SS. Timestamp kemudian diikuti dengan perintah yang dipanggil dan deskripsi perintah. Deskripsi perintah yang diisi berupa path dimana perintah di jalankan.