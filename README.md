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

```c
void caesar_chiper_encrypt (char text[1000], char newChar[1000])
{
	char key[100] = "9(ku@AW1[Lmvgax6q`5Y2Ry?+sF!^HKQiBXCUSe&0M.b%rI'7d)o4~VfZ*{#:}ETt$3J-zpc]lnh8,GwP_ND|jO";
    
    char sentence[1000] = "";
    strcpy(sentence, text);

    int in;
    for (in = 0 ; strlen(sentence) > in ; in++)
	{
		if(sentence[in] == '/')
		{
            newChar[in] = '/';
			continue;
		}
		char dif[2];
		dif[0] = sentence[in];
        newChar[in] = key[(strcspn(key,dif) + 10) % 87];
//		for (i = 0 ; strlen(key) > i; i++)
//		{
//			if(key[i] == sentence[in])
//			{
//				printf("%c", key[(i+10) % 87]);
//				break;
//			}
//		}
	}

}
```
Untuk melakukan enkrip dengan caesar chiper, pertama kita harus mencari dahulu karakter per karakter yang ingin di enkrip di key. Cara mencarinya bisa dengan menggunakan fungsi `strcspn` dari `string.h` yang me-return index dimana huruf tersebut ditemukan pertama kali di key. Index tersebut ditambahkan dengan pergeseran key sebesar yang kita mau, lalu di mod dengan total panjangnya key untuk menghindari overflow. Dari itu kita dapatkan `(strcspn(key,dif) + 10) % 87`. Opsi lain adalah dengan menggunakan linear searching (line yang dicomment), dengan mencari satu per satu karakter dari key untuk di match dengan yang ingin di enkripkan untuk mendapatkan indexnya. Kita lakukan looping ini selama masih ada karakter yang ingin di enkrip.

```c
void caesar_chiper_decrypt (char text[1000], char newChar[1000])
{
	char key[100] = "9(ku@AW1[Lmvgax6q`5Y2Ry?+sF!^HKQiBXCUSe&0M.b%rI'7d)o4~VfZ*{#:}ETt$3J-zpc]lnh8,GwP_ND|jO";
    
    char sentence[1000] = "";
    strcpy(sentence, text);

    for (int in = 0 ; strlen(sentence) > in ; in++)
	{
		if(sentence[in] == '/')
		{
            newChar[in] = '/';
			continue;
		}
		for (int i = 0 ; strlen(key) > i; i++)
		{
			if(key[i] == sentence[in])
			{
				int indTemp = i;
				if(indTemp < 10) indTemp += 87;
                newChar[in] = key[indTemp-10];
				break;
			}
		}
	}
}
```
Untuk dekripnya hampir sama dengan enkrip. Bedanya disini adalah jika dienkrip kita menambahkan 10, didekrip kita mengurangi 10 sehingga menjadi karakter semula lagi. `indTemp += 87` digunakan untuk agar tidak keluar dari array key, dan kondisinya tergantung seberapa besar perpindahan keynya.

Sekarang kita bisa menuliskan tambahan kode untuk dekripsi path pada fungsi `xmp_getattr`, `xmp_readdir`, dan `xmp_read` untuk membaca fpath :

```c
    char* token = strtok(path, "/");
    while(token != NULL)
    {
        if(encryptNeeded)
        {
            strcat(pathEnc, "/");
            strcat(pathEnc, token);
        }
        else if (!encryptNeeded)
        {
            strcat(pathNow, "/");
            strcat(pathNow, token);
        }
        if(strncmp(token, "encv1_", 6) == 0) //perlu encrypt
        {
            encryptNeeded = true;
        }
        token = strtok(NULL, "/");
    }
```
pada perulangan ini akan mendeteksi apakah perlu diadakan enkripsi tipe 1. Caranya dengan mengecek pathnya. Kita akan memasukkan path kedalam `pathNow` satu per satu. Untuk setiap directory, dicek apakah namanya berisi "encv1_" di awalnya. Jika iya maka perlu dienkrip. Setelah itu sisa dari path setelah "encv1_" itu akan dimasukkan ke string yang berbeda dengan path sebelumnya.

```c
if(encryptNeeded)
    {
        char  checkFile[1000] = "", checkEnc[1000]= "";
        
        char* titik = strrchr(pathEnc, '.');
        if(titik != NULL)
        {
            strncpy(checkFile, pathEnc, titik-pathEnc);
            caesar_chiper_decrypt(checkFile, checkEnc);
        }
        
        
        char checkDoc[1000] = "";
        strcat(checkDoc, dirpath);
        strcat(checkDoc, pathNow);
        strcat(checkDoc, checkEnc);
        if(titik != NULL) strcat(checkDoc, titik);

        if(isRegFile(checkDoc))
        {
            sprintf(fpath, "%s%s%s%s",dirpath, pathNow, checkEnc, titik);
        }
        else
        {
            caesar_chiper_decrypt(pathEnc, enc);
            sprintf(fpath, "%s%s%s",dirpath, pathNow, enc);
        }
    }
    else
    {
        caesar_chiper_decrypt(pathEnc, enc);
	    sprintf(fpath,"%s%s%s",dirpath, pathNow,enc);
    }
```
Jika enkripsi diperlukan, maka kita akan memisahkan nama file dengan ekstensinya dengan `strrchr` yang fungsinya untuk mendeteksi karakter yang match yang paling akhir. Setelah itu kita akan mencheck apakah yang ingin di dekrip ini file atau directory. Jika file maka yang didekrip hanyalah namafile nya saja, ekstensinya tidak. Jika direktori, seluruhnya akan didekrip. fungsi mendekrip disini adalah mendapatkan nama path yang aslinya sehingga pada operasi selanjutnya dapat menggunakan path ini.

Jika enkripsi tidak diperlukan maka kita tidak perlu mendekripsi nama file/direktori nya.

Pada `xmp_readdir` harus ditambahkan kode lagi selain diatas, yaitu untuk melakukan enkripsi. Tambahan kodenya ada pada perulangan menbaca file-file dari folder yang dibuka.

```c
    while ((de = readdir(dp)) != NULL) {
        if(strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0) continue;
        struct stat st;
        memset(&st, 0, sizeof(st));
        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;

        if(encryptNeeded)
        {
            if(de->d_type == DT_DIR) //direktori
            {
                char enc[1000] = "";
                caesar_chiper_encrypt(de->d_name, enc);
		        res = (filler(buf, enc, &st, 0));
            }
            else if(de->d_type == DT_REG) //file reguler
            {
                char enc[1000] = "", fileLengkap[1000]="", fileName[1000] = "";

                strcpy(fileLengkap, de->d_name);
                char* titik = strrchr(fileLengkap, '.');

                if(titik != NULL) 
                    strncpy(fileName, fileLengkap, titik-fileLengkap);
                else
                    strcpy(fileName, fileLengkap);
                

                caesar_chiper_encrypt(fileName, enc);

                if(titik != NULL) strcat(enc, titik);


		        res = (filler(buf, enc, &st, 0));
            }
        }
        else
        {
            res = (filler(buf, de->d_name, &st, 0));
        }
        if(res!=0) break;
    }
```

Pertama, kita akan cek jika directory yang dibaca adalah "." dan "..". Direktori tersebut tidak perlu dienkripsi. Kita akan cek pula apakah enkripsi diperlukan atau tidak, dari pengecekan nama direktori diatas tadi. Jika perlu dienkrip, maka kita mengcek terlebih dahulu apakah dia file atau direktori.

Sama seperti diatas, file tidak perlu mengenkripsi ekstensinya sedangkan folder akan menenkripsi semua. Pada file kita perlu memisahkan nama dengan ekstensinya, dan hanya nama yang dienkripsi. Jika sudah, maka akan dimasukkan ke fungsi `filler()`

### 2. Enkripsi versi 2:

**Soal :**
```
a. Jika sebuah direktori dibuat dengan awalan “encv2_”, maka direktori tersebut
   akan menjadi direktori terenkripsi menggunakan metode enkripsi v2.
b. Jika sebuah direktori di-rename dengan awalan “encv2_”, maka direktori tersebut
   akan menjadi direktori terenkripsi menggunakan metode enkripsi v2.
c. Apabila sebuah direktori terenkripsi di-rename menjadi tidak terenkripsi, maka isi
   direktori tersebut akan terdekrip.
d. Setiap pembuatan direktori terenkripsi baru (mkdir ataupun rename) akan
   tercatat ke sebuah database/log berupa file.
e. Pada enkripsi v2, file-file pada direktori asli akan menjadi bagian-bagian kecil
   sebesar 1024 bytes dan menjadi normal ketika diakses melalui filesystem
   rancangan jasir. Sebagai contoh, file File_Contoh.txt berukuran 5 kB pada
   direktori asli akan menjadi 5 file kecil yakni: File_Contoh.txt.000,
   File_Contoh.txt.001, File_Contoh.txt.002, File_Contoh.txt.003, dan
   File_Contoh.txt.004.
f. Metode enkripsi pada suatu direktori juga berlaku kedalam direktori lain yang ada
   didalam direktori tersebut (rekursif).
```

**Jawaban :**

#### Enkripsi
```c
void encrypt2(char *filePath){
    char srcPath[1000];
    char destPath[1000];
    sprintf(srcPath, "%s", filePath);
    sprintf(destPath, "%s.", filePath);
    pid_t child = fork();
    int status;
    if(child == 0){
        pid_t ex = fork();
        if(ex == 0){
            char *argv[]={"split", "-a", "3", "-d","-b", "1024", srcPath, destPath, NULL};
            execv("/usr/bin/split", argv);
        } else {
            while((wait(&status)) > 0);
            char *argv[]={"rm", srcPath, NULL};
            execv("/bin/rm", argv);
        }
    }
    return;
}

void initEncrypt2(char *wPath){
    chdir(wPath);
    DIR *d;
    struct dirent *dir;
    struct stat myFile;
    d = opendir(".");
    if(d){
        while((dir = readdir(d)) != NULL){
            if (stat(dir->d_name, &myFile) < 0);
            else if (!S_ISDIR(myFile.st_mode))
            {
                char filePath[1000];
                sprintf(filePath, "%s/%s", wPath, dir->d_name);
                encrypt2(filePath);
            } else{
                if(strcmp(dir->d_name, "..") == 0 || strcmp(dir->d_name, ".") == 0)
                    continue;
                char newPath[1000];
                sprintf(newPath, "%s/%s", wPath, dir->d_name);
                initEncrypt2(newPath);
            }
        }
    }
    return;
}
```

#### Dekripsi

```c
void appendContent(char source[], char dest[]){
    int ch;
    FILE *fp1, *fp2;

    fp1 = fopen(source, "r");
    fp2 = fopen(dest, "a");
    
    if (!fp1) {
            printf("Unable to open source file to read!!\n");
            fclose(fp2);
            return ;
    }

    if (!fp2) {
            printf("Unable to open target file to write\n");
            return ;
    }

    while ((ch = fgetc(fp1)) != EOF) {
            fputc(ch, fp2);
    }

    fclose(fp1);
    fclose(fp2);

    remove(source);
    return ;
}

void decrypt2(char *filePath){
    int i = 0;
    FILE *fp = fopen(filePath, "w");
    fclose(fp);
    while(1){
        char curFile[1000];
        sprintf(curFile, "%s.%03d", filePath, i);
        struct stat buffer;
        if(stat(curFile, &buffer) != 0)
            break;
        
        appendContent(curFile, filePath);
        i++;
    }
}

void initDecrypt2(char *wPath){
    chdir(wPath);
    DIR *d;
    struct dirent *dir;
    struct stat myFile;
    d = opendir(".");
    if(d){
        while((dir = readdir(d)) != NULL){
            if (stat(dir->d_name, &myFile) < 0);
            else if (!S_ISDIR(myFile.st_mode))
            {
                char filePath[1000];
                sprintf(filePath, "%s/%s", wPath, dir->d_name);
                char *pch = strrchr(filePath, '.');
                char oldFilePath[1000];
                int i;
                for(i=0; i<pch-filePath; i++){
                    if(i == 0)
                        sprintf(oldFilePath, "%c", filePath[i]);
                    else
                        sprintf(oldFilePath, "%s%c", oldFilePath, filePath[i]);
                }
                decrypt2(oldFilePath);
            } else{
                if(strcmp(dir->d_name, "..") == 0 || strcmp(dir->d_name, ".") == 0)
                    continue;
                char newPath[1000];
                sprintf(newPath, "%s/%s", wPath, dir->d_name);
                initDecrypt2(newPath);
            }
        }
    }
    return;
}
```

**Penjelasan :**

####Enkripsi

Enkripsi berjalan secara sekuensial. Semua file dalam folder yang di-enkripsi akan dipecah menjadi beberapa file menggunakan "split". Perintah split yang digunakan diatur dengan argumen yang membantu menghasilkan output soal: size sebesar 1024 bytes dan diberi tiga digit angka di akhir nama file kecil. Ketika fungsi mendeteksi folder, maka akan dilakukan proses enkripsi yang sama terhadap semua file dalam folder tersebut.

####Dekripsi

Dekripsi dieksekusi serupa dengan enkripsi: sekuensial dan berlaku untuk semua file dalam folder. Dekripsi menggunakan fungsi bantuan appendContent. Fungsi ini merupakan modifikasi fungsi umum untuk menyalin sebuah file. Modifikasi berupa pergantian akses "write" ke "append". Akses ini memungkinkan untuk menyalin isi file kecil ke dalam satu file yang merupakan file awal sebelum dipecah. Penyalinan file dimulai dari file dari digit 000 sampai digit ddd tidak ditemukan (artinya penyalinan berakhir). Dekripsi bersifat rekursif.

### 3. Sinkronisasi direktori otomatis:

**Tidak selesai**

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