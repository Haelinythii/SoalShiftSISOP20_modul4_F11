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
#include <stdlib.h>
#include <stdbool.h>

//tambah cuman folder encv1_

static  const  char *dirpath = "/home/dwiki/Documents";

int isRegFile(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
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

void writeToFile(char c[1000], char b[100])
{
    char a[1000];
    strcpy(a,b);
    FILE *f;
    f = fopen("/home/dwiki/debug.txt", "a+");
    // fwrite(a, sizeof(a), 1, f);
    fprintf(f, "%s ----> %s\n", c , a);
    fclose(f);
}

void encrypt_type_1 (char* folderName, char newFName[1000])
{
    
    if(strcmp(folderName, ".") == 0 || strcmp(folderName, "..") == 0) return;

    char oldName[1000];
    strcpy(oldName, folderName);

    int slash = 0;
    if (folderName[0] == '/') {
        folderName++;
        slash = 1;
    }
    int first = 1;

    bool encryptEnable = false;
    char* token = strtok(oldName, "/");
    while(token != NULL)
    {
        char newName[1000] = "";
        caesar_chiper_encrypt(token, newName);
        // strcat(newFName, "/");
        if (!first || slash) strcat(newFName, "/");
        first = 0;
        strcat(newFName, newName);
        token = strtok(NULL, "/");
    }

    
    // writeToFile(folderName, newFName);
	// strcat(newFName, newName);
}

static  int  xmp_getattr(const char *path, struct stat *stbuf)
{
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
    
    // printf("%s\n", path);
    writeToFile(path, enc);
	res = lstat(fpath, stbuf);

	if (res == -1)
		return -errno;

  	return 0;
}

  

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
	char fpath[1000];
    bool encryptNeeded = false;

    char pathNow[1000] = "", pathEnc[1000] = "";
    // strcpy(pathNow, dirpath);

    // char *ret;
    // ret = strstr(path, "encv1_");
    // if(ret != NULL)
    // {
    //     printf("%s\n", ret);

    // }
    // printf("%s\n", path);
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

	if(strcmp(path,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else 
    {
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
        

        

        
    // printf("%s\n", pathNow);

    }
    
    
    // bool encryptEnable = false;
    // char* token = strtok(fpath, "/");
    // while(token != NULL)
    // {
    //     printf("%s / ", token);
    //     token = strtok(NULL, "/");
    // }
    // printf("\n");
    

	int res = 0;
	DIR *dp;
	struct dirent *de;
  
	(void) offset;
	(void) fi;

                printf("%s\n", fpath);
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
	return 0;
}

  

static int xmp_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
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

	
	if(strcmp(path,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else 
    {
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
	return res;
}

// static int xmp_write(const char *path, const char *buf, size_t size,
// 		    off_t offset, struct fuse_file_info *fi)
// {
// 	int fd;
// 	int res;

// 	char fpath[1000];

// 	if(strcmp(path,"/") == 0)
// 	{
// 		path=dirpath;
// 		sprintf(fpath,"%s",path);
// 	}
// 	else sprintf(fpath, "%s%s",dirpath,path);

// 	(void) fi;
// 	fd = open(fpath, O_WRONLY);
// 	if (fd == -1)
// 		return -errno;

// 	res = pwrite(fd, buf, size, offset);
// 	if (res == -1)
// 		res = -errno;

// 	close(fd);
// 	return res;
// }

// static int xmp_truncate(const char *path, off_t size)
// {
//     int res;
//     char fpath[1000];
//     char name[1000];
//     sprintf(name,"%s",path);
//     sprintf(fpath, "%s%s",dirpath,name);
//     res = truncate(fpath, size);
//     if (res == -1)
//         return -errno;

//     return 0;
// }

static int xmp_rename(const char *from, const char *to)
{
	int res;

    char frompath[1000];
    char namef[1000];
    sprintf(namef,"%s",from);
    sprintf(frompath, "%s%s",dirpath,namef);

    char topath[1000];
    char namet[1000];
    sprintf(namet,"%s",to);
    sprintf(topath, "%s%s",dirpath,namet);

    char newPath[1000] = "";

    // if(strncmp(namet, "/encv1_", 7) == 0)
    // {
    //     encrypt_type_1(frompath, newPath);
    // }

	res = rename(frompath, topath);
	// res = rename(from, to);
	if (res == -1)
		return -errno;

	return 0;
}

// static int xmp_mkdir(const char *path, mode_t mode)
// {
// 	int res;

//     char fpath[1000];
//     char name[1000];
//     sprintf(name,"%s",path);
//     sprintf(fpath, "%s%s",dirpath,name);

// 	res = mkdir(fpath, 0750);
// 	if (res == -1)
// 		return -errno;

// 	return 0;
// }

// static int xmp_rmdir(const char *path)
// {
// 	int res;

//     char fpath[1000];
//     char name[1000];
//     sprintf(name,"%s",path);
//     sprintf(fpath, "%s%s",dirpath,name);

// 	res = rmdir(fpath);
// 	if (res == -1)
// 		return -errno;

// 	return 0;
// }

static struct fuse_operations xmp_oper = {
    .getattr    = xmp_getattr,
    .readdir    = xmp_readdir,
    .read       = xmp_read,
    // .truncate	= xmp_truncate,
    // .write		= xmp_write,
    .rename		= xmp_rename,
    // .mkdir		= xmp_mkdir,
    // .rmdir		= xmp_rmdir,
};

  

int  main(int  argc, char *argv[])

{
    umask(0);
    return fuse_main(argc, argv, &xmp_oper, NULL);
}

// void encrypt_type_1 (char* folderName, char newFName[1000])
// {
//     DIR *dirOpen;
// 	struct dirent *dirEntry;

//     //int res = 0;

//     dirOpen = opendir(folderName);

//     while ((dirEntry = readdir(dirOpen)) != NULL) 
//     {
//         if(strcmp(dirEntry->d_name, ".") == 0 || strcmp(dirEntry->d_name, "..") == 0) continue;

//         if(dirEntry->d_type == DT_DIR)
//         {
//             char oldName[1000], newName[1000];
//             strcpy(oldName, dirEntry->d_name);

//             caesar_chiper_encrypt(oldName, newName);
//             writeToFile(dirEntry->d_name, newName);
//             // xmp_rename(dirEntry->d_name, newName);

//             char frompath[1000];
//             char namef[1000];
//             sprintf(namef,"%s",dirEntry->d_name);
//             sprintf(frompath, "%s/%s",folderName,namef);

//             char topath[1000];
//             char namet[1000];
//             sprintf(namet,"%s",newName);
//             sprintf(topath, "%s/%s",folderName,namet);

//             // if(strncmp(namet, "/encv1_", 7) == 0)
//             // {
//             //     encrypt_type_1(frompath);
//             // }

// 	        rename(frompath, topath);
//         }
//         else if(dirEntry->d_type == DT_REG)
//         {

//         }
// 		struct stat st;
// 		memset(&st, 0, sizeof(st));
// 		// st.st_ino = dirEntry->d_ino;
// 		// st.st_mode = dirEntry->d_type << 12;
// 		// res = (filler(buf, dirEntry->d_name, &st, 0));
// 		// if(res!=0) break;
// 	}
// 	closedir(dirOpen);
// }