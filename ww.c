#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include<ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include "ww.h"


int wrap(unsigned width, int file_in, int file_out ){
    char bufferArr[BUFFER_LEN];
    char* str = malloc(sizeof(char)*2);//word -> more than one letter
    char space[1] = {' '};
    char line[1] = {'\n'};
    ssize_t buffer;
    int rt = 0;// return value
    int isNewLine = 0;// boolean
    int newLineCount = 0;// check new line char 
    int charCounter = 0;// num of characters of the line 
    int wordLen = 0;// len of the word 
    int ptr = 0; // a helper pointer
    int length = 2;// var of length of the word array

    do{// read file
        buffer = read(file_in, bufferArr, BUFFER_LEN);
        int i;
        for(i=0; i<buffer;i++){
            //it is not space
            if(isspace(bufferArr[i]) == 0){
                //check the space in line
                if(newLineCount >= 2){
                    write(file_out, line, 1);
                    write(file_out, line, 1);
                    isNewLine = 0;// reset not a new line
                    charCounter = 0;
                }
                newLineCount = 0;

                //add word to word arr
                if(ptr< length){
                    str[ptr] = bufferArr[i];
                    ptr = ptr+1;
                    wordLen = wordLen+1;
                }
                else{
                    length = length*2;
                    char *str2 = realloc(str, sizeof(char)*length);
                    if(!str2) {
                        return 1;
                    }
                    str = str2;
                    str[ptr] = bufferArr[i];
                    ptr = ptr+1;
                    wordLen= wordLen+1;
                    //printf("HI");
                }

            }
            else{// it is a space
            //check if there is word
            //printf("HI");
                if(wordLen !=0){
                    charCounter = charCounter +wordLen;
                
                    if(isNewLine !=0){// not a new line
                        charCounter = charCounter+1;
                        if(charCounter> width){
                            // over the width
                            if(wordLen > width){
                                write(file_out, line, 1);// line
                                write(file_out, str, wordLen); //add word to line
                                write(file_out, line, 1); // new line
                                isNewLine = 0;
                                charCounter = 0;
                                rt = -1;
                            }

                            else{
                                write(file_out, line, 1);
                                write(file_out, str, wordLen);
                                charCounter = wordLen;
                            }
                            wordLen = 0; //reset
                            ptr = 0;

                        }
                    else{
                        write(file_out, space, 1);
                        write(file_out,str, wordLen);
                        wordLen = 0;// reset
                        ptr = 0;
                    }

                }
                else{
                    if(charCounter> width){
                        write(file_out, str, wordLen);
                        write(file_out, line, 1);
                        wordLen = 0;
                        ptr = 0;
                        isNewLine = 0;
                        charCounter = 0;
                        rt = -1;
                    }
                    else{
                        write(file_out, str, wordLen);
                        wordLen = 0;
                        ptr = 0;
                        isNewLine = 1;
                    }

                }
            }
            if(bufferArr[i] == '\n'){
                newLineCount = newLineCount +1;
                
            }
        }

    }


    }while(buffer!=0);
    // last loop for last line
    	if(wordLen != 0){
		charCounter = charCounter + wordLen;

		//if its not a new line
		if(isNewLine != 0){
			charCounter++; //add an extra space
			if(charCounter > width){
				if(wordLen > width){
					write(file_out, line, 1);
					write(file_out, str, wordLen);
					write(file_out, line, 1);
					isNewLine = 0;
					rt = -1;
					charCounter = 0;
				}
				else{
					write(file_out, line, 1);
					write(file_out, str, wordLen);
					write(file_out, line, 1);
					charCounter = wordLen;
				}
				wordLen = 0;
				ptr = 0;
			}
			else{
				write(file_out, space, 1);
				write(file_out, str, wordLen);
				write(file_out, line, 1);
				wordLen = 0;
				ptr = 0;
			}
		}
		else{
			if(line > width){
				write(file_out, str, wordLen);
				write(file_out, line, 1);
				wordLen = 0;
				ptr = 0;
				isNewLine = 0;
				rt = -1;
				charCounter = 0;
			}
			else{
				write(file_out, str, wordLen);
				write(file_out, line, 1);
				wordLen = 0;
				ptr = 0;
				isNewLine = 1;
			}
		}
	}
    free(str);
    return rt;
    
}


int main(int argc, char* argv[]){
    int w = atoi(argv[1]);
    assert(w>0);
    unsigned int width = (unsigned int)w;
    int fCheck= 0;
    int dCheck = 0;
    int file;
    int closeFile =0; 
    struct stat strBf;
    if(argc >3) return EXIT_FAILURE;
    if(width == 0) return EXIT_FAILURE;
    if(argc<3){
        fCheck = wrap(width,0,1);
    }
    else{
        dCheck = stat(argv[2], &strBf);
        if(dCheck!=0){
            perror("Not in directory");
        }
        if(S_ISDIR(strBf.st_mode)){
            DIR* dir = opendir(argv[2]);
            struct dirent* rd;
            char* prefix = "wrap.";
            do{
                rd = readdir(dir);
                if(rd!= NULL){
                //create a new file
                    unsigned long fnameLen = strlen(rd->d_name);
                    char* fname = malloc(sizeof(char)*(fnameLen+5));
                    char* newfname = malloc(sizeof(char)*(fnameLen+5));
                    int i = 0;
                    while(i<5){
                        fname[i] = rd->d_name[i];
                        i++;
                    }
                    int checkFileName = strcmp(fname, prefix);
                    if((checkFileName!=0) && (rd->d_name[0] != '.')){
                        int dirchange = chdir(argv[2]);
                        if(dirchange!=0){
                            perror("Cannot change dir");
                        }
                        file = open(rd->d_name,O_RDONLY);
                        if(file == -1){
                            perror("Cannot open file");
                        }
                        //create a file in wrap.fname
                        strcpy(newfname,prefix);
                        strcat(newfname, fname);
                        int newFile = open(newfname, O_RDWR | O_TRUNC | O_CREAT, 0666 );
                        if(newFile == -1){
                            perror("Cannot create file");
                        }
                        fCheck = fCheck + wrap(width, file, newFile);
                        closeFile = close(newFile);
                        if(closeFile !=0 ) perror("fail to close file");
                        closeFile = close(file);
                        if(closeFile !=0 ) perror("fail to close file");
                        dirchange = chdir("..");// return to dir


                        

                        
                    }
                    free(fname);



                }
            }while(rd != NULL);

            closeFile = closedir(dir);
            if(closeFile !=0 ) perror("fail to close Dir\n");


        }
        else{ //part1
            if(S_ISREG(strBf.st_mode)){
                file = open(argv[2], O_RDONLY);
                if(file ==-1 ) perror("fail to open file\n");
                fCheck = wrap(width,file,1);
                closeFile = close(file);
                if(closeFile !=0 ) perror("fail to close file\n");
            }
        }
    }
    if(fCheck != 0){
        perror("Error: the len of word is larger than width\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;

}
