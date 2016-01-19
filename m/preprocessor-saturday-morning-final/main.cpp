#include <iostream>
#include<stdio.h>
#include<stdlib.h>
#include<strings.h>

using namespace std;

struct  header_file{

    char file_name[50];

};
typedef struct  header_file HF;

void makeMainFile(HF *incName,int sizeIncName);


void preprocessor(void){

    HF inc_name[10];
    int i=0;
    FILE *fp;
    fp=fopen("m.text" , "r+");
    if(fp==NULL)
		printf(" ERROR : can not open the file\n ");
	else{
        char c, str[8] , name[55],*p;
        while(!feof(fp)){
            c=fgetc(fp);
            if(c==35){
                    fgets(str , 8 , fp );//getting the word include from file

                    if(strcmp(str , "include")!=0)
                        cout<<"ERROR : wrong typing 'include' "<<endl;

                    else{

                        fseek(fp , 2, SEEK_CUR);
                        fgets(name,55,fp);
                        if((p=strchr(name,' ')) != NULL)
                            *p='\0';

                        int k=0;
                        char txt[]=".txt";
                        //don't forget .h!!!!
                        while(name[k]!=NULL)
                            k++;
                        for(int i=0 ; i < 5 ; i++){
                            name[k]=txt[i];
                            k++;
                        }
                        strcpy(inc_name[i].file_name,name);
                        i++;
                    }

            }
            else
                break;
        }
	}
	fclose(fp);
	makeMainFile(inc_name,i);

}

void makeMainFile(HF *incName,int sizeIncName){

    FILE *fp,*fNew , *f;
    fNew=fopen("mainF.txt","a");
    char line[500];
    for(int i=0; i<sizeIncName; i++){
        fp = fopen(incName[i].file_name,"r");
        if(fp==NULL)
            printf("ERROR: CAN NOT OPEN %s\n",incName[i].file_name);
        else{
            while(!feof(fp)){
                fgets(line,500,fp);
                fputs(line,fNew);
            }
            fputc('\n',fNew);
        }
        fclose(fp);
    }

    f = fopen("m.text","r+");
    if(f==NULL)
        printf("ERROR: CAN NOT OPEN m.txt\n");
    else{

        while(!feof(f)){
            fgets(line,500,f);
            if(line[0]!='#')
                fputs(line,fNew);
        }
    }
    fclose(f);
    fclose(fNew);
}

int main()
{
    preprocessor();

    return 0;
}

