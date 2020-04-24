#define FUSE_USE_VERSION 28
#define _GNU_SOURCE
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>
#include <sys/xattr.h>
#include <sys/wait.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>

char* dirpath = "/home/dwiki/Documents";

// Membuat log dengan pesan Warning
void createLogWarning(const char *log, const char *path){
    FILE *fp;
    fp = fopen("/home/dwiki/fs.log", "a");
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
    fp = fopen("/home/dwiki/fs.log", "a");
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
    fp = fopen("/home/dwiki/fs.log", "a");
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

int isRegFile(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

void caesar_chiper_encrypt (char text[1000], char newChar[1000])
{
    // char pathTitik[1000], pathTitik2[1000];
    // sprintf(pathTitik, "%s/.", dirpath);
    // sprintf(pathTitik2, "%s/..", dirpath);
    // // printf("%s\n", text);
    // if(strcmp(text, pathTitik) == 0 || strcmp(text, pathTitik2) == 0) return;

    // 9(ku@AW1[Lmvgax6q`5Y2Ry?+s F!^HKQiBXCUSe&0M.b%rI'7d)o 4~VfZ*{#:}ETt$3J-zpc]lnh8,GwP_ND|jO
	char key[100] = "9(ku@AW1[Lmvgax6q`5Y2Ry?+sF!^HKQiBXCUSe&0M.b%rI'7d)o4~VfZ*{#:}ETt$3J-zpc]lnh8,GwP_ND|jO";
    
    char sentence[1000] = "";
    strcpy(sentence, text);

    // char *hasil;
    // hasil = malloc(sizeof(char) * strlen(sentence));

    int in;
    for (in = 0 ; strlen(sentence) > in ; in++)
	{
		if(sentence[in] == '/')
		{
			// printf("/");
            newChar[in] = '/';
			continue;
		}
		char dif[2];
		dif[0] = sentence[in];
		// printf("%c", key[(strcspn(key,dif) + 10) % 87]);
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

void caesar_chiper_decrypt (char text[1000], char newChar[1000])
{
    // char pathTitik[1000], pathTitik2[1000];
    // sprintf(pathTitik, "%s/.", dirpath);
    // sprintf(pathTitik2, "%s/..", dirpath);
    // printf("%s\n", text);
    // if(strcmp(text, pathTitik) == 0 || strcmp(text, pathTitik2) == 0) return;

    // 9(ku@AW1[Lmvgax6q`5Y2Ry?+s F!^HKQiBXCUSe&0M.b%rI'7d)o 4~VfZ*{#:}ETt$3J-zpc]lnh8,GwP_ND|jO
	char key[100] = "9(ku@AW1[Lmvgax6q`5Y2Ry?+sF!^HKQiBXCUSe&0M.b%rI'7d)o4~VfZ*{#:}ETt$3J-zpc]lnh8,GwP_ND|jO";
    
    
    char sentence[1000] = "";
    strcpy(sentence, text);

    for (int in = 0 ; strlen(sentence) > in ; in++)
	{
		if(sentence[in] == '/')
		{
			// printf("/");
            newChar[in] = '/';
			continue;
		}
		
//		hasil[in] = key[(strcspn(key,dif) + 10) % 87];
//		printf("%c", hasil[in]);
		for (int i = 0 ; strlen(key) > i; i++)
		{
			if(key[i] == sentence[in])
			{
				int indTemp = i;
				if(indTemp < 10) indTemp += 87;
				// printf("%c", key[indTemp-10]);
                newChar[in] = key[indTemp-10];
				break;
			}
		}
	}
    // printf("%s", newChar);
}

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

static int xmp_getattr(const char *path, struct stat *stbuf){
    int res;
    char fpath[1000];
    char enc[1000] = "";

    bool encryptNeeded = false;
    char pathNow[1000] = "", pathEnc[1000] = "";

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
        // printf("%s / ", token);
        token = strtok(NULL, "/");
    }
    // printf("\n");

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

    // sprintf(fpath,"%s%s",dirpath,path);
    res = lstat(fpath, stbuf);
    if (res == -1)
        return -errno;
    createLogInfo1("GETATTR", path);
    return 0;
}

static int xmp_readlink(const char *path, char *buf, size_t size){
    char fpath[1000];
    if(strcmp(path,"/") == 0){
        path=dirpath;
        sprintf(fpath,"%s",path);
    } else{
        sprintf(fpath, "%s%s",dirpath,path);
    }

    int res;
    res = readlink(fpath, buf, size - 1);
    if (res == -1)
        return -errno;

    buf[res] = '\0';

    createLogInfo1("READLINK", path);
    return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi){
    char fpath[1000];
    bool encryptNeeded = false;

    char pathNow[1000] = "", pathEnc[1000] = "";

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
        // printf("%s / ", token);
        token = strtok(NULL, "/");
    }
    // printf("\n");

    if(strcmp(path,"/") == 0){
        path=dirpath;
        sprintf(fpath,"%s",path);
    } else{
        if(encryptNeeded)
        {
            char enc[1000] = "", checkFile[1000] = "", checkEnc[1000]= "";
        
            char* titik = strrchr(pathEnc, '.');

            if(titik !=NULL)
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
            char enc[1000] = "";
            caesar_chiper_decrypt(pathEnc, enc);
            sprintf(fpath, "%s%s%s",dirpath, pathNow, enc);
        }
    }

    int res = 0;
    DIR *dp;
    struct dirent *de;
    (void) offset;
    (void) fi;
    dp = opendir(fpath);

    if (dp == NULL)
        return -errno;
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
    closedir(dp);
    createLogInfo1("READDIR", path);
    return 0;
}

static int xmp_mknod(const char *path, mode_t mode, dev_t rdev){
    char fpath[1000];
    if(strcmp(path,"/") == 0){
        path=dirpath;
        sprintf(fpath,"%s",path);
    } else{
        sprintf(fpath, "%s%s",dirpath,path);
    }

	int res;

	/* On Linux this could just be 'mknod(path, mode, rdev)' but this
	   is more portable */
	if (S_ISREG(mode)) {
		res = open(fpath, O_CREAT | O_EXCL | O_WRONLY, mode);
		if (res >= 0)
			res = close(res);
	} else if (S_ISFIFO(mode))
		res = mkfifo(fpath, mode);
	else
		res = mknod(fpath, mode, rdev);
	if (res == -1)
		return -errno;

    createLogInfo1("MKNOD", path);
	return 0;
}

static int xmp_mkdir(const char *path, mode_t mode){
    char fpath[1000];
    if(strcmp(path,"/") == 0){
        path=dirpath;
        sprintf(fpath,"%s",path);
    } else{
        sprintf(fpath, "%s%s",dirpath,path);
    }

    char* token = strtok(path, "/");
    while(token != NULL)
    {
        if(strncmp(token, "encv1_", 6) == 0)
        {
            createLogInfo1("ENCRYPT1", path);
            break;
        }
        else if(strncmp(token, "encv2_", 6) == 0)
        {
            createLogInfo1("ENCRYPT2", path);
            break;
        }
        token = strtok(NULL, "/");
    }
    
    int res;
    res = mkdir(fpath, mode);
    
    if (res == -1)
        return -errno;
    createLogInfo1("MKDIR", path);
    return 0;
}

static int xmp_unlink(const char *path){
    char fpath[1000];
    if(strcmp(path,"/") == 0){
        path=dirpath;
        sprintf(fpath,"%s",path);
    } else{
        sprintf(fpath, "%s%s",dirpath,path);
    }

    int res;

    res = unlink(fpath);
    if (res == -1)
        return -errno;

    createLogWarning("UNLINK", path);
    return 0;
}

static int xmp_rmdir(const char *path){
    char fpath[1000];
    if(strcmp(path,"/") == 0){
        path=dirpath;
        sprintf(fpath,"%s",path);
    } else{
        sprintf(fpath, "%s%s",dirpath,path);
    }

    int res;

    res = rmdir(fpath);
    if (res == -1)
        return -errno;

    createLogWarning("RMDIR", path);
    return 0;
}

static int xmp_symlink(const char *from, const char *to){
    char fpathFrom[1000];
    char fpathTo[1000];
    if(strcmp(from,"/") == 0){
        from=dirpath;
        sprintf(fpathFrom,"%s",from);
    } else{
        sprintf(fpathFrom, "%s%s",dirpath,from);
    }

    if(strcmp(to,"/") == 0){
        to=dirpath;
        sprintf(fpathTo,"%s",to);
    } else{
        sprintf(fpathTo, "%s%s",dirpath,to);
    }

    int res;

    res = symlink(fpathFrom, fpathTo);
    if (res == -1)
        return -errno;

    createLogInfo2("SYMLINK", from, to);
    return 0;
}

static int xmp_rename(const char *from, const char *to){
    char fpathFrom[1000];
    char fpathTo[1000];
    if(strcmp(from,"/") == 0){
        from=dirpath;
        sprintf(fpathFrom,"%s",from);
    } else{
        sprintf(fpathFrom, "%s%s",dirpath,from);
    }

    if(strcmp(to,"/") == 0){
        to=dirpath;
        sprintf(fpathTo,"%s",to);
    } else{
        sprintf(fpathTo, "%s%s",dirpath,to);
    }

    int res;

    res = rename(fpathFrom, fpathTo);
    if (res == -1)
        return -errno;

    createLogInfo2("RENAME", from, to);

    bool en1 = false, en2 = false, en3 = false, en4 = false;
    char* token = strtok(from, "/");
    while(token != NULL)
    {
        if(strncmp(token, "encv1_", 6) == 0)
        {
            // createLogInfo1("ENCRYPT1", path);
            en1 = true;
            break;
        }
        else if(strncmp(token, "encv2_", 6) == 0)
        {
            // createLogInfo1("ENCRYPT2", path);
            en2 = true;
            break;
        }
        token = strtok(NULL, "/");
    }

    //false ke true encrypt, true ke false decrypt
    char* token1 = strtok(to, "/");
    while(token1 != NULL)
    {
        if(strncmp(token1, "encv1_", 6) == 0)
        {
            
            en3 = true;
            break;
        }
        else if(strncmp(token1, "encv2_", 6) == 0)
        {
            en4 = true;
            break;
        }
        token1 = strtok(NULL, "/");
    }

    if(!en1 && en3)
    {
        createLogInfo2("ENCRYPT1", from, to);
    }
    else if(en1 && !en3)
    {
        createLogInfo2("DECRYPT1", from, to);
    }
    else if(!en2 && en4)
    {
        createLogInfo2("ENCRYPT2", from, to);
        initEncrypt2(fpathTo);
    }
    else if(en2 && !en4)
    {
        createLogInfo2("DECRYPT2", from, to);
        initDecrypt2(fpathTo);
    }

    // if(strstr(from, "encv2_") != NULL && strstr(to, "encv2_") == NULL){
    //     initDecrypt2(fpathTo);
    //     createLogInfo2("DECRYPT2", from, to);
    // }

    // if(strstr(from, "encv2_") == NULL && (strstr(to, "encv2_") != NULL)){
    //     initEncrypt2(fpathTo);
    //     createLogInfo2("ENCRYPT2", from, to);
    // }
    return 0;
}

static int xmp_link(const char *from, const char *to){
    char fpathFrom[1000];
    char fpathTo[1000];
    if(strcmp(from,"/") == 0){
        from=dirpath;
        sprintf(fpathFrom,"%s",from);
    } else{
        sprintf(fpathFrom, "%s%s",dirpath,from);
    }

    if(strcmp(to,"/") == 0){
        to=dirpath;
        sprintf(fpathTo,"%s",to);
    } else{
        sprintf(fpathTo, "%s%s",dirpath,to);
    }

    int res;

    res = link(fpathFrom, fpathTo);
    if (res == -1)
        return -errno;

    createLogInfo2("LINK", from, to);
    return 0;
}

static int xmp_chmod(const char *path, mode_t mode){
    char fpath[1000];
    if(strcmp(path,"/") == 0){
        path=dirpath;
        sprintf(fpath,"%s",path);
    } else{
        sprintf(fpath, "%s%s",dirpath,path);
    }
    
    int res;

    res = chmod(fpath, mode);
    if (res == -1)
        return -errno;

    createLogInfo1("CHMOD", path);
    return 0;
}

static int xmp_chown(const char *path, uid_t uid, gid_t gid){
    char fpath[1000];
    if(strcmp(path,"/") == 0){
        path=dirpath;
        sprintf(fpath,"%s",path);
    } else{
        sprintf(fpath, "%s%s",dirpath,path);
    }

    int res;

    res = lchown(fpath, uid, gid);
    if (res == -1)
        return -errno;

    createLogInfo1("CHOWN", path);
    return 0;
}

static int xmp_truncate(const char *path, off_t size){
    char fpath[1000];
    if(strcmp(path,"/") == 0){
        path=dirpath;
        sprintf(fpath,"%s",path);
    } else{
        sprintf(fpath, "%s%s",dirpath,path);
    }

    int res;
    res = truncate(fpath, size);
    if (res == -1)
        return -errno;
    
    createLogInfo1("TRUNCATE", path);
    return 0;
}

static int xmp_utimens(const char *path, const struct timespec ts[2]){
	char fpath[1000];
    if(strcmp(path,"/") == 0){
        path=dirpath;
        sprintf(fpath,"%s",path);
    } else{
        sprintf(fpath, "%s%s",dirpath,path);
    }

    int res;
	struct timeval tv[2];

	tv[0].tv_sec = ts[0].tv_sec;
	tv[0].tv_usec = ts[0].tv_nsec / 1000;
	tv[1].tv_sec = ts[1].tv_sec;
	tv[1].tv_usec = ts[1].tv_nsec / 1000;

	res = utimes(fpath, tv);
	if (res == -1)
		return -errno;

    createLogInfo1("UTIMENS", path);
	return 0;
}

static int xmp_open(const char *path, struct fuse_file_info *fi){
    char fpath[1000];
    if(strcmp(path,"/") == 0){
        path=dirpath;
        sprintf(fpath,"%s",path);
    } else{
        sprintf(fpath, "%s%s",dirpath,path);
    }

    int res;

    res = open(fpath, fi->flags);
    if (res == -1)
        return -errno;

    close(res);

    createLogInfo1("OPEN", path);
    return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi){
    char fpath[1000];

    bool encryptNeeded = false;
    char pathNow[1000] = "", pathEnc[1000] = "";

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
        // printf("%s / ", token);
        token = strtok(NULL, "/");
    }
    // printf("\n");

    if(strcmp(path,"/") == 0){
        path=dirpath;
        sprintf(fpath,"%s",path);
    } else{
        if(encryptNeeded)
        {
            char enc[1000] = "", checkFile[1000] = "", checkEnc[1000]= "";
        
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
            char enc[1000] = "";
            caesar_chiper_decrypt(pathEnc, enc);
            sprintf(fpath, "%s%s%s",dirpath, pathNow, enc);
        }
    }

    int res = 0;
    int fd = 0 ;
    (void) fi;
    fd = open(fpath, O_RDONLY);

    if (fd == -1)
        return -errno;

    res = pread(fd, buf, size, offset);

    if (res == -1)
        res = -errno;

    close(fd);
    createLogInfo1("READ", path);
    return res;
}

static int xmp_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi){
	char fpath[1000];
    if(strcmp(path,"/") == 0){
        path=dirpath;
        sprintf(fpath,"%s",path);
    } else{
        sprintf(fpath, "%s%s",dirpath,path);
    }

    int fd;
    int res;

    (void) fi;
    fd = open(fpath, O_WRONLY);
    if (fd == -1)
        return -errno;

    res = pwrite(fd, buf, size, offset);
    if (res == -1)
        res = -errno;

    close(fd);
    createLogInfo1("WRITE", path);
    return res;
}

static int xmp_statfs(const char *path, struct statvfs *stbuf){
    char fpath[1000];
    if(strcmp(path,"/") == 0){
        path=dirpath;
        sprintf(fpath,"%s",path);
    } else{
        sprintf(fpath, "%s%s",dirpath,path);
    }
    
    int res;

    res = statvfs(fpath, stbuf);
    if (res == -1)
        return -errno;

    createLogInfo1("STATFS", path);
    return 0;
}

static int xmp_create(const char* path, mode_t mode, struct fuse_file_info* fi) {
    char fpath[1000];
    if(strcmp(path,"/") == 0){
        path=dirpath;
        sprintf(fpath,"%s",path);
    } else{
        sprintf(fpath, "%s%s",dirpath,path);
    }

    (void) fi;

    int res;
    res = creat(fpath, mode);
    if(res == -1)
	return -errno;

    close(res);
    createLogInfo1("CREAT", path);
    return 0;
}

#ifdef HAVE_SETXATTR
static int xmp_setxattr(const char *path, const char *name, const char *value, size_t size, int flags){
	char fpath[1000];
    if(strcmp(path,"/") == 0){
        path=dirpath;
        sprintf(fpath,"%s",path);
    } else{
        sprintf(fpath, "%s%s",dirpath,path);
    }

    int res = lsetxattr(fpath, name, value, size, flags);
	if (res == -1)
		return -errno;

    createLogInfo1("SETXATTR", path);
	return 0;
}

static int xmp_getxattr(const char *path, const char *name, char *value, size_t size){
    char fpath[1000];
    if(strcmp(path,"/") == 0){
        path=dirpath;
        sprintf(fpath,"%s",path);
    } else{
        sprintf(fpath, "%s%s",dirpath,path);
    }
    
    int res = lgetxattr(fpath, name, value, size);
    if (res == -1)
        return -errno;
    
    createLogInfo1("GETXATTR", path);
    return res;
}

static int xmp_listxattr(const char *path, char *list, size_t size){
    char fpath[1000];
    if(strcmp(path,"/") == 0){
        path=dirpath;
        sprintf(fpath,"%s",path);
    } else{
        sprintf(fpath, "%s%s",dirpath,path);
    }

    int res = llistxattr(fpath, list, size);
    if (res == -1)
        return -errno;
    
    createLogInfo1("LISTXATTR", path);
    return res;
}

static int xmp_removexattr(const char *path, const char *name){
    char fpath[1000];
    if(strcmp(path,"/") == 0){
        path=dirpath;
        sprintf(fpath,"%s",path);
    } else{
        sprintf(fpath, "%s%s",dirpath,path);
    }

    int res = lremovexattr(fpath, name);
    if (res == -1)
        return -errno;
    
    createLogInfo1("REMOVEXATTR", path);
    return 0;
}
#endif /* HAVE_SETXATTR */


static struct fuse_operations xmp_oper = {
    .getattr    = xmp_getattr,
    .readlink   = xmp_readlink,
    .readdir    = xmp_readdir,
    .mknod      = xmp_mknod,
    .mkdir      = xmp_mkdir,
    .symlink    = xmp_symlink,
    .unlink     = xmp_unlink,
    .rmdir      = xmp_rmdir,
    .rename     = xmp_rename,
    .link       = xmp_link,
    .chmod      = xmp_chmod,
    .chown      = xmp_chown,
    .truncate   = xmp_truncate,
    .utimens    = xmp_utimens,
    .open       = xmp_open,
    .read       = xmp_read,
    .write      = xmp_write,
    .statfs     = xmp_statfs,
    .create     = xmp_create,
#ifdef HAVE_SETXATTR
    .setxattr       = xmp_setxattr,
    .getxattr       = xmp_getxattr,
    .listxattr      = xmp_listxattr,
    .removexattr    = xmp_removexattr,
#endif
};

int  main(int  argc, char *argv[]){
    umask(0);
    return fuse_main(argc, argv, &xmp_oper, NULL);
}