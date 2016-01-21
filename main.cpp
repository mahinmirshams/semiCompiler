#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<iostream>
#define LINE_SIZE 500
#define T_SIZE 50
#define TEDDADEID 100
#define TEDADE_NUM 500


using namespace std;

struct  header_file{

    char file_name[50];

};
typedef struct  header_file HF;

void makeMainFile(HF *incName,int sizeIncName);
int counter =0;
void preprocessor(void){

    HF inc_name[10];
    int i=0;
    FILE *fp;
    fp=fopen("input.c" , "r+");
    if(fp==NULL)
		printf(" ERROR : can not open the file\n ");
	else{
        char c, str[8] , name[55],*p;
        while(!feof(fp)){
            c=fgetc(fp);
            if(c==35){

                    fgets(str , 8 , fp );
                    if(strcmp(str , "include")!=0)
                        cout<<"ERROR : wrong typing 'include' "<<endl;

                    else{
                        c=getc(fp);
                        while(c!=34)
                            c=getc(fp);
                        fgets(name,55,fp);
                        if((p=strchr(name,'"')) != NULL)
                            *p='\0';
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
          /*  while(1){
                if(!feof(fp)){
                    fgets(line,500,fp);
                    fputs(line,fNew);
                }
                else
                    break;
            }*/
            while(!feof(fp)){
                fgets(line,500,fp);
                fputs(line,fNew);
            }
            fputc('\n',fNew);
        }
        fclose(fp);
    }

    f = fopen("input.c","r+");
    if(f==NULL)
        printf("ERROR: CAN NOT OPEN m.txt\n");
    else{
        while(1){
            char *p;
            if(!feof(f)){
                fgets(line,500,f);
                if(line[0]!='#'&& ((p=strchr(line,'"')) == NULL)){
                    fputs(line,fNew);
                }
            }
            else
                break;
        }
      /*  char *p;
        while(!feof(f)){
            fgets(line,500,f);
            if(line[0]!='#' && ((p=strchr(line,'"')) == NULL)){
                fputs(line,fNew);
                puts(line);
            }
        }*/
    }
    fclose(f);
    fclose(fNew);
}

bool mainExist=false;
bool error=false;
bool returnExist = false;

struct tokenInfo{

    char token[T_SIZE];
    char attribute[T_SIZE];
    int lineNum;
    struct tokenInfo *next;
    struct tokenInfo *before;
};
typedef struct tokenInfo ST;

struct identifier{

    char name[T_SIZE];
    char type[10];
    char value[T_SIZE];
    struct identifier *next;
};
typedef struct identifier ID;

struct numbeer{
    char num[T_SIZE];
};
typedef struct numbeer NUM;

ID idTable[TEDDADEID];
int sizeIdTable=0;
int countBrace= 0;
int ifCounter=0;

NUM numberTable[TEDADE_NUM];
int sizeNumTable = 0;
FILE *fp_ir;
int lcounter =1;

void tokenizeString(int lineNum, char* line, ST *head);
int findingTheToken(char *tok);
void addingTokensToList(ST *head,char *tmp, int lineNum);
ST * checkElse(ST * mode);
ST  * checkIf(ST *mode);

void loadingTokensFromFile(ST *head){

    FILE *fp;
    fp=fopen("mainF.txt","r");
    char tmp[LINE_SIZE];
    int line=0;
    if(fp == NULL)
        printf("the file wasn't found");
    else{
        while(!feof(fp)){
            fgets(tmp,LINE_SIZE,fp);
            char *pos;
            if((pos=strchr(tmp,'\n'))!= NULL)
                *pos='\0';
            line++;
            tokenizeString(line,tmp,head);
        }
    }

}


void tokenizeString(int lineNum, char* line, ST *head){

    int i=0,j=0;
    char line2[T_SIZE];
    while(line[i]!=NULL){
        if(line[i]!=' ' && line[i]!='\t'){
                line2[j]=line[i];
                j++;
        }
        if(line[i]==' ' || line[i]=='\t'){
            if(j>0 && line2[j-1]!=' '){
                line2[j]=' ';
                j++;
            }
        }
        i++;
    }
    line2[j]='\0';

    const char delim[2]=" ";
    char *token;
    token=strtok(line2, delim);

    while(token != NULL){
        addingTokensToList(head,token,lineNum);
        token=strtok(NULL,delim);
    }
}

int findingTheToken(char *tok){
    if(strcmp(tok,"int")==0 || strcmp(tok,"char")==0 || strcmp(tok,"float")==0 || strcmp(tok,"bool")==0 )
       return 0;
    if(strcmp(tok,"void")==0 || strcmp(tok,"main")==0 || strcmp(tok,"NULL")==0 || strcmp(tok,"while")==0 || strcmp(tok,"true")==0 || strcmp(tok,"false")==0 || strcmp(tok,"if")==0 || strcmp(tok,"else")==0 || strcmp(tok,"return")==0)
        return 1;
    if(strcmp(tok,"+")==0 || strcmp(tok,"-")==0 || strcmp(tok,"*")==0 || strcmp(tok,"/")==0 || strcmp(tok,"<")==0 || strcmp(tok,">")==0 || strcmp(tok,"==")==0 || strcmp(tok,"!=")==0 || strcmp(tok,"&&")==0 || strcmp(tok,"||")==0 || strcmp(tok,"=")==0 || strcmp(tok,"<=")==0 || strcmp(tok,">=")==0)
        return 2;
    if(strcmp(tok,"(")==0 || strcmp(tok,")")==0 || strcmp(tok,"{")==0 || strcmp(tok,"}")==0 || strcmp(tok,";")==0 || strcmp(tok,",")==0)
        return 3;
    int i=0,ashar=0;
    bool isNum=true;
    while(tok[i]!=NULL && isNum){
        if((tok[i]<48 || tok[i]>57) && tok[i] != 46)
            isNum=false;
        if(tok[i]==46)
            ashar++;
        i++;
    }
    if(isNum && ashar<2)
        return 4;

    i=0;
    if(tok[i]==39){
        bool validChar=false;
        i++;
        while(tok[i]!=NULL)
            i++;
        if(tok[i-1]==39)
            validChar=true;
        if(validChar)
            return 5;
        else
            return 50;
    }
    i=0;
    bool validIdName=true;
    if(tok[i]<65 || tok[i]>122 || (tok[i]>90 && tok[i]<97))
        validIdName=false;
    i++;
    while(tok[i]!=NULL && validIdName){
        if(tok[i]<48 || tok[i]>122 || (tok[i]>90 && tok[i]<97) || (tok[i]>57 && tok[i]<65))
            validIdName=false;
        i++;
    }
    if(validIdName)
        return 6;
    else
        return 60;
}

int searchingId(char *id){
    //esme yeksan type mokhalef!
    for(int i=0;i<sizeIdTable;i++)
        if(strcmp(idTable[i].name,id)==0)
            return i;
    return -1;
}

ST * makingMode(ST *curMode){
    ST *newTok = (ST *)malloc(sizeof(ST));
    curMode->next=newTok;
    curMode->next->next=NULL;
    return newTok;
}

void updateIDtable(char *nameid, char *typeId,char *valueid){

    int index = searchingId(nameid);
    if(index == -1){
        strcpy(idTable[sizeIdTable].name,nameid);
        strcpy(idTable[sizeIdTable].type,typeId);
        strcpy(idTable[sizeIdTable].value,valueid);
        sizeIdTable++;
    }
    else{
        strcpy(idTable[index].value,valueid);
        strcpy(idTable[index].type,typeId);
    }
}

void addingTokensToList(ST *head,char *tmp, int lineNum){

    ST *start= head;
    while(start->next != NULL)
        start=start->next;
    int ti = findingTheToken(tmp);
    if(ti==1 || ti==0){
        ST *newT = makingMode(start);
        strcpy(newT->token,"keyword");
        strcpy(newT->attribute,tmp);
        newT->lineNum=lineNum;
        newT->before=start;
    }
    if(ti==2){
        ST *newT = makingMode(start);
        strcpy(newT->token,"operator");
        strcpy(newT->attribute,tmp);
        newT->lineNum=lineNum;
        newT->before=start;
    }
    if(ti==3){
        ST *newT = makingMode(start);
        strcpy(newT->token,"punctuation");
        strcpy(newT->attribute,tmp);
        newT->lineNum=lineNum;
        newT->before=start;
    }
    if(ti==4 || strcmp(tmp,"NULL")==0 ){
        ST *newT = makingMode(start);
        strcpy(newT->token,"number");
        strcpy(newT->attribute,tmp);
        newT->lineNum=lineNum;
        newT->before=start;
        strcpy(numberTable[sizeNumTable].num,tmp);
        sizeNumTable++;
    }
    if(ti==5){
        ST *newT = makingMode(start);
        strcpy(newT->token,"character");
        strcpy(newT->attribute,tmp);
        newT->lineNum=lineNum;
        newT->before=start;
        strcpy(numberTable[sizeNumTable].num,tmp);
        sizeNumTable++;
    }
    if(ti==6){
        ST *newT = makingMode(start);
        strcpy(newT->token,"identifier");
        strcpy(newT->attribute,tmp);
        newT->lineNum=lineNum;
        newT->before=start;
        updateIDtable(tmp,"unknown","NULL");
    }
    if(ti == 60)
        printf("line %d:\n%s is invlaid name for identifier!\n",lineNum,tmp);

}

ST * assignValue(ST *mode,int i){

    if(mode->next->next==NULL){
        error=true;
        printf("ERROR: LINE:%d expected ; after %s but there is %s\n",mode->lineNum,mode->attribute,mode->next->attribute);
        return NULL;
    }
    mode=mode->next;
    if(strcmp(mode->next->token,"number")==0 || strcmp(mode->next->token,"character")==0 || strcmp(mode->next->attribute,"true")==0 || strcmp(mode->next->attribute,"false")==0){
        mode=mode->next;
        if(mode->next==NULL){
            error=true;
            printf("ERROR: LINE:%d expected ; after %s\n",mode->lineNum,mode->attribute);
            return NULL;
        }
        if(strcmp(mode->next->attribute,";")==0){
            return mode->next;
        }
        if(strcmp(mode->next->attribute,"+")==0 || strcmp(mode->next->attribute,"-")==0 || strcmp(mode->next->attribute,"*")==0 || strcmp(mode->next->attribute,"/")==0){
            if(mode->next->next==NULL){
                error=true;
                printf("ERROR: LINE:%d expected ; after %s but there is %s\n",mode->next->lineNum,mode->attribute,mode->next->attribute);
                return NULL;
            }
            assignValue(mode,i);
        }
    }
    else if(strcmp(mode->next->token,"identifier")==0){
        mode=mode->next;
        int index=searchingId(mode->attribute);
        if(strcmp(idTable[index].type,"unknown")==0){
            error=true;
            printf("ERROR LINE:%d  %s didn't defined\n",mode->lineNum,mode->attribute);
        }
        if(mode->next==NULL){
            error=true;
            printf("ERROR: LINE:%d expected ; after %s\n",mode->lineNum,mode->attribute);
            return NULL;
        }
        if(strcmp(mode->next->attribute,"+")==0 || strcmp(mode->next->attribute,"-")==0 || strcmp(mode->next->attribute,"*")==0 || strcmp(mode->next->attribute,"/")==0){
            if(mode->next->next==NULL){
                error=true;
                printf("ERROR: LINE:%d expected ; after %s but there is %s\n",mode->next->lineNum,mode->attribute,mode->next->attribute);
                return NULL;
            }
            assignValue(mode,i);
        }
        else if(strcmp(mode->next->attribute,";")==0)
            return mode->next;
        else {
            error=true;
            printf("ERROR: LINE:%d expected ; after %s but there is %s\n",mode->lineNum,mode->attribute,mode->next->attribute);
            while(mode->next!=NULL){
                mode=mode->next;
                if(strcmp(mode->attribute,";")==0)
                        return mode;
            }
            return NULL;
        }
    }
    else{
        error=true;
        printf("ERROR: LINE:%d expected identifier or number or character after %s but there is %s\n",mode->lineNum,mode->attribute,mode->next->attribute);
        while(mode->next!=NULL){
            mode=mode->next;
            if(strcmp(mode->attribute,";")==0)
                return mode;
        }
        return NULL;
    }
}

ST*  typeDefiner(ST* mode,char* type){

    if(mode->next==NULL){
        error=true;
        printf("ERROR: LINE: %d : expected idenetifier after %s\n",mode->lineNum,mode);
        return NULL;
    }
    if(strcmp(mode->next->token,"identifier")!=0){
        error=true;
        printf("ERROR: LINE: %d : expexted an identifier after %s but there is a %s(%s)\n",mode->next->lineNum,mode->attribute,mode->next->token,mode->next->attribute);
        while(mode->next!=NULL){
                mode=mode->next;
                if(strcmp(mode->attribute,";")==0)
                    return mode;
        }
        return NULL;
    }
    else{
        updateIDtable(mode->next->attribute,type,"unknown");
        mode=mode->next;
        if(mode->next==NULL){
            error=true;
            printf("ERROR: LINE: %d : expected ; after %s\n", mode->lineNum,mode->attribute);//error type
            return NULL;
        }

        if(strcmp(mode->next->attribute,";")==0)
            return mode->next;

        else if(strcmp(mode->next->attribute,",")==0)
            typeDefiner(mode->next,type);

        else if(strcmp(mode->next->attribute,"=")==0){
            if(mode->next->next==NULL){
                    error=true;
                    printf("ERROR: LINE: %d : expected ; after %s but there is =\n", mode->next->lineNum,mode->attribute);
                    return NULL;
            }
            if(strcmp(mode->next->next->token,"number")==0 || strcmp(mode->next->next->token,"character")==0 || strcmp(mode->next->next->token,"identifier")==0 || strcmp(mode->next->next->attribute,"true")==0 ||strcmp(mode->next->next->attribute,"false")==0){
                mode=mode->next->next;
                if(strcmp(mode->token,"identifier")==0 && strcmp(idTable[searchingId(mode->attribute)].type,"unknown")==0){
                    error=true;
                    printf("ERROR: LINE: %d : %s was not defined\n",mode->lineNum,mode->attribute);
                }
                if(mode->next==NULL){
                    error=true;
                    printf("ERROR: LINE: %d : expected ; after %s\n",mode->lineNum,mode->attribute);//error type
                    return NULL;
                }
                if(strcmp(mode->next->attribute,";")==0)
                    return mode->next;
                if(strcmp(mode->next->attribute,",")==0)
                    mode=typeDefiner(mode->next,type);
                else if((strcmp(mode->next->attribute,";")!=0) && (strcmp(mode->next->attribute,",")!=0)){
                    error=true;
                    printf("ERROR : LINE: %d expected ; after %s but there is %s\n",mode->lineNum,mode->attribute,mode->next->attribute);
                    while(mode->next!=NULL){
                        mode=mode->next;
                        if(strcmp(mode->attribute,";")==0)
                            return mode;
                    }
                    return NULL;
                }

            }
        }
        else{
            error=true;
            printf("ERROR: LINE: %d : expected ; after %s but there is %s\n",mode->lineNum,mode->attribute,mode->next->attribute);
            mode=mode->next;
            while(mode->next!=NULL){
                mode=mode->next;
                if(strcmp(mode->attribute,";")==0)
                    return mode;
            }
            return NULL;
        }

    }
}

ST * checkIntMain(ST* mode){
    mainExist=true;
    mode=mode->next;
    if(strcmp(mode->next->attribute,"(")==0){
        mode=mode->next;
        if(strcmp(mode->next->attribute,")")==0){
            mode=mode->next;
            if(strcmp(mode->next->attribute,"{")==0){
                mode=mode->next;
                countBrace++;
                return mode;
            }
        }

    }

}

ST * checkKeyWord(ST* mode){
    int i=findingTheToken(mode->attribute);
    if(strcmp(mode->attribute,"void")==0 && mode->next != NULL && strcmp(mode->next->attribute,"main")==0){
        mode=checkIntMain(mode);
        return mode;
    }
    if(i==0){
        mode=typeDefiner(mode,mode->attribute);
        return mode;
    }
    if(strcmp(mode->attribute,"if")==0){
        mode=checkIf(mode);
        return mode;
    }
    if(strcmp(mode->attribute,"else")==0){
        mode=checkElse(mode);
        return mode;
    }
    if(strcmp(mode->attribute,"return")==0){
        returnExist = true;
        return mode;
    }
    else
        return mode;

}


ST * checkIdentifier(ST *mode){
    int index=searchingId(mode->attribute);
    if(strcmp(idTable[index].type,"unknown")==0){
        error=true;
        printf("ERROR LINE:%d  %s didn't defined\n",mode->lineNum,mode->attribute);
    }

        if(strcmp(mode->next->attribute,"=")==0){
            if(!mainExist){
                error=true;
                printf("not allowed to value a id outside main!%d\n",mode->lineNum);
                while(mode->next!=NULL){
                    mode=mode->next;
                    if(strcmp(mode->attribute,";")==0)
                        return mode;
                }
                return NULL;
            }
            mode=assignValue(mode,index);
            return mode;
        }
        if(strcmp(mode->next->attribute,";")==0)
            return mode->next;
}
ST  * checkIf(ST *mode){
    ifCounter++;
    if(mode->next==NULL){
        error=true;
        printf("ERROR LINE: %d expected ( after if\n",mode->lineNum);
        return NULL;
    }
    mode=mode->next;
    if(strcmp(mode->attribute,"(")==0){
        if(mode->next==NULL){
            error=true;
            printf("ERROR : LINE: %d expected identifier or number or character after ( \n",mode->lineNum);
            return NULL;
        }
        mode=mode->next;
        if(strcmp(mode->token,"identifier")==0 || strcmp(mode->token,"number")==0 || strcmp(mode->token,"character")==0){
            if(strcmp(mode->token,"identifier")==0){
                int i = searchingId(mode->attribute);
                if(strcmp(idTable[i].type,"unknown")==0){
                    error=true;
                    printf("ERROR LINE:%d  %s didn't defined\n",mode->lineNum,mode->attribute);
                }
            }
            if(mode->next==NULL){
                error=true;
                printf("ERROR : LINE: %d expected )",mode->lineNum);
                return NULL;
            }
            mode=mode->next;
            if(strcmp(mode->attribute,"==")==0 || strcmp(mode->attribute,">=")==0 || strcmp(mode->attribute,"<=")==0 || strcmp(mode->attribute,"!=")==0 || strcmp(mode->attribute,"&&")==0 || strcmp(mode->attribute,"||")==0){
                if(mode->next==NULL){
                    error=true;
                    printf("ERROR : LINE: %d expected identifier or number or character after %s \n",mode->lineNum,mode->attribute);
                    return NULL;
                }
                mode=mode->next;
                if(strcmp(mode->token,"identifier")==0 || strcmp(mode->token,"number")==0 || strcmp(mode->token,"character")==0){
                    if(strcmp(mode->token,"identifier")==0){
                        int i = searchingId(mode->attribute);
                        if(strcmp(idTable[i].type,"unknown")==0){
                            error=true;
                            printf("ERROR LINE:%d  %s didn't defined\n",mode->lineNum,mode->attribute);
                        }
                    }
                    if(mode->next==NULL){
                        error=true;
                        printf("ERROR : LINE: %d expected )\n",mode->lineNum,mode->attribute);
                        return NULL;
                    }
                    mode=mode->next;
                    if(strcmp(mode->attribute,")")==0){
                        if(mode->next==NULL){
                            error=true;
                            printf("ERROR : LINE: %d expected expration\n",mode->lineNum,mode->attribute);
                            return NULL;
                        }
                        mode=mode->next;
                        if(strcmp(mode->attribute,"{")==0){
                            countBrace++;
                            return mode;
                        }
                        else{
                            error=true;
                            printf("ERROR : LINE: %d expected { is %s\n",mode->lineNum,mode->attribute);
                            while(mode->next!=NULL){
                                if(strcmp(mode->attribute,"}")==0){
                                    countBrace--;
                                    return mode;
                                }
                                mode=mode->next;
                            }
                            return NULL;
                        }
                    }
                    else{
                        error=true;
                        printf("ERROR : LINE: %d expected ) but there is %s\n",mode->lineNum,mode->attribute);
                        while(mode->next!=NULL){
                            if(strcmp(mode->attribute,"}")==0){
                                countBrace--;
                                return mode;
                            }
                            mode=mode->next;
                        }
                    }
                }
                else{
                    error=true;
                    printf("ERROR : LINE: %d expected identifier or number or character but there is %s\n",mode->lineNum,mode->attribute);
                    while(mode->next!=NULL){
                        if(strcmp(mode->attribute,"}")==0){
                            countBrace--;
                            return mode;
                        }
                        mode=mode->next;
                    }
                }

            }
            else if(strcmp(mode->attribute,")")==0){
                if(mode->next==NULL){
                    printf("ERROR : LINE: %d expected expration\n",mode->lineNum,mode->attribute);
                    error=true;
                    return NULL;
                }
                mode=mode->next;
                if(strcmp(mode->attribute,"{")==0){
                    countBrace++;
                    return mode;
                }
                else{
                    error=true;
                    printf("ERROR : LINE: %d expected { %s\n",mode->lineNum,mode->attribute);
                    while(mode->next!=NULL){
                        if(strcmp(mode->attribute,"}")==0){
                            countBrace--;
                            return mode;
                        }
                        mode=mode->next;
                    }
                    return NULL;
                }
            }
            else{
                error=true;
               printf("ERROR : LINE: %d expected ) or operator but there is %s\n",mode->lineNum,mode->attribute);
                while(mode->next!=NULL){
                    if(strcmp(mode->attribute,"}")==0){
                        countBrace--;
                        return mode;
                    }
                    mode=mode->next;
                }
                return NULL;
            }
        }
        else{
            error=true;
            printf("ERROR : LINE: %d expected identifier or number or character after ( but there is %s\n",mode->lineNum,mode->token);
            while(mode->next!=NULL){
                if(strcmp(mode->attribute,"}")==0){
                    countBrace--;
                    return mode;
                }
                mode=mode->next;
            }
            return NULL;
        }
    }
    else{
        error=true;
        printf("ERROR : LINE: %d expected ( after if but there is %s\n",mode->lineNum,mode->attribute);
        while(mode->next!=NULL){
            if(strcmp(mode->attribute,"}")==0){
                countBrace--;
                return mode;
            }
            mode=mode->next;
        }
        return NULL;
    }
}
ST * checkElse(ST * mode){
    if(ifCounter<1){
        error=true;
        printf("ERROR : LINE:%d there wasn't any if condition\n",mode->lineNum);
        if(mode->next==NULL)
            return NULL;
        while(mode->next!=NULL){
            mode=mode->next;
            if(strcmp(mode->attribute,"{")==0)
                countBrace++;
            if(strcmp(mode->attribute,"}")==0){
                return mode;
            }
        }
        return NULL;

    }
    else if(mode->next==NULL){
        error=true;
        printf("ERROR : LINE:%d expected { after else\n",mode->lineNum);
        return NULL;
    }
    mode=mode->next;
    if(strcmp(mode->attribute,"{")==0){
        ifCounter--;
        countBrace++;
        return mode;
    }
    else{
        error =true;
        printf("ERROR : LINE:%d expected { after else but there is %s\n",mode->lineNum,mode->attribute);
        while(mode->next!=NULL){
            if(strcmp(mode->attribute,"{")==0)
                countBrace++;
            if(strcmp(mode->attribute,"}")==0){
                return mode;
            }
            mode=mode->next;
        }
        return NULL;
    }

}
void checkSyntax(ST *head){

    ST *start= head;
    if(start==NULL)
        return;

    if(start->next != NULL)
        start=start->next;
    else
        return;
    if(strcmp(start->attribute,"{")==0)
            countBrace++;
    else if(strcmp(start->token,"keyword")==0)
        start=checkKeyWord(start);
    else if(strcmp(start->token,"identifier")==0)
        start=checkIdentifier(start);
    else if(strcmp(start->attribute,"}")==0)
            countBrace--;
    checkSyntax(start);
}

int searchingNumber(char *num){

    for(int i=0;i<sizeNumTable;i++)
        if(strcmp(numberTable[i].num,num)==0)
            return i;
}

ST * IRassign(ST *mode){

    if(strcmp(mode->next->next->token,"number")==0 && strcmp(mode->next->next->next->attribute,";")==0){
        int i =searchingNumber(mode->next->next->attribute);
        int j=searchingId(mode->attribute);
        fprintf(fp_ir,"T_%d := %s\nT%d := T_%d\n",i,mode->next->next->attribute,j,i);
        strcpy(numberTable[i].num,"empty");
        return mode->next->next->next;
    }
    if(strcmp(mode->next->next->token,"character")==0 && strcmp(mode->next->next->next->attribute,";")==0){
        int i =searchingNumber(mode->next->next->attribute);
        int j=searchingId(mode->attribute);
        fprintf(fp_ir,"T_%d := %s\nT%d := T_%d\n",i,mode->next->next->attribute,j,i);
        strcpy(numberTable[i].num,"empty");
        return mode->next->next->next;
    }
    if((strcmp(mode->next->next->attribute,"true")==0 || strcmp(mode->next->next->attribute,"false")==0)&& strcmp(mode->next->next->next->attribute,";")==0){
        int j=searchingId(mode->attribute);
        fprintf(fp_ir,"T%d := %s\n",j,mode->next->next->attribute,"true");
        return mode->next->next->next;
    }
    if(strcmp(mode->next->next->token,"number")==0 && strcmp(mode->next->next->next->token,"operator")==0 && strcmp(mode->next->next->next->next->token,"number")==0){
        int i = searchingNumber(mode->next->next->attribute);
        int j = searchingNumber(mode->next->next->next->next->attribute);
        int z = searchingId(mode->attribute);
        fprintf(fp_ir,"T_%d := %s\nT_%d := %s\nT%d := T_%d %s T_%d\n",i,mode->next->next->attribute,j,mode->next->next->next->next->attribute,z,i,mode->next->next->next->attribute,j);
        strcpy(numberTable[i].num,"empty");
        strcpy(numberTable[j].num,"empty");
        return mode->next->next->next->next->next;
    }
    if(strcmp(mode->next->next->token,"number")==0 && strcmp(mode->next->next->next->token,"operator")==0 && strcmp(mode->next->next->next->next->token,"identifier")==0){
        int i = searchingNumber(mode->next->next->attribute);
        int j = searchingId(mode->next->next->next->next->attribute);
        int z = searchingId(mode->attribute);
        fprintf(fp_ir,"T_%d := %s\nT%d := T_%d %s T%d\n",i,mode->next->next->attribute,z,i,mode->next->next->next->attribute,j);
        strcpy(numberTable[i].num,"empty");
        return mode->next->next->next->next->next;
    }
    if(strcmp(mode->next->next->token,"identifier")==0 && strcmp(mode->next->next->next->token,"operator")==0 && strcmp(mode->next->next->next->next->token,"number")==0){
        int i = searchingNumber(mode->next->next->next->next->attribute);
        int j = searchingId(mode->next->next->attribute);
        int z = searchingId(mode->attribute);
        fprintf(fp_ir,"T_%d := %s\nT%d := T%d %s T_%d\n",i,mode->next->next->next->next->attribute,z,j,mode->next->next->next->attribute,i);
        strcpy(numberTable[i].num,"empty");
        return mode->next->next->next->next->next;
    }
    if(strcmp(mode->next->next->token,"identifier")==0 && strcmp(mode->next->next->next->token,"operator")==0 && strcmp(mode->next->next->next->next->token,"identifier")==0){
        int i = searchingId(mode->next->next->next->next->attribute);
        int j = searchingId(mode->next->next->attribute);
        int z = searchingId(mode->attribute);
        fprintf(fp_ir,"T%d := T%d %s T_%d\n",z,j,mode->next->next->next->attribute,i);
        return mode->next->next->next->next->next;
    }
    if(strcmp(mode->next->next->token,"identifier")==0 && strcmp(mode->next->next->next->attribute,";")==0){
        int i = searchingId(mode->next->next->attribute);
        int j = searchingId(mode->attribute);
        fprintf(fp_ir,"T%d := T%d\n",j,i);
        return mode->next->next->next;
    }
}

ST * ifIR(ST * mode){
    //mode=if
    mode=mode->next->next;
    ST *tmp = mode;
    bool elseExist=false;
    while(strcmp(tmp->attribute,"}")!=0)
        tmp=tmp->next;
    if(strcmp(tmp->next->attribute,"else")==0){
        elseExist=true;
        tmp=tmp->next;
    }
    if(strcmp(mode->token,"identifier")==0 && strcmp(mode->next->next->token,"identifier")==0){
        int i=searchingId(mode->attribute);
        int j=searchingId(mode->next->next->attribute);
        if(elseExist){
            fprintf(fp_ir,"\nif T%d %s T%d goto L%d else goto L%d\nL%d\n",i,mode->next->attribute,j,lcounter,lcounter+1,lcounter);
            mode=mode->next->next->next->next->next;
            while(strcmp(mode->attribute,"}")!=0){
                if(strcmp(mode->token,"identifier")==0 && strcmp(mode->next->attribute,"=")==0)
                    mode=IRassign(mode)->next;
                else
                    mode=mode->next;
            }
            mode=mode->next->next->next;
            fprintf(fp_ir,"\nL%d\n",lcounter+1);
            while(strcmp(mode->attribute,"}")!=0){
                if(strcmp(mode->token,"identifier")==0 && strcmp(mode->next->attribute,"=")==0)
                    mode=IRassign(mode)->next;
                else
                    mode=mode->next;
            }
            fprintf(fp_ir,"\n");
            lcounter += 2;
            return mode;
        }
        if(!elseExist){
            fprintf(fp_ir,"\nif T%d %s T%d goto L%d\nL%d\n",i,mode->next->attribute,j,lcounter,lcounter);
            lcounter += 1;
            mode=mode->next->next->next->next->next;
            while(strcmp(mode->attribute,"}")!=0){
                if(strcmp(mode->token,"identifier")==0 && strcmp(mode->next->attribute,"=")==0)
                    mode=IRassign(mode)->next;
                else
                    mode=mode->next;
            }
            fprintf(fp_ir,"\n");
            return mode;
        }
    }

    if(strcmp(mode->token,"identifier")==0 && (strcmp(mode->next->next->token,"number")==0 || strcmp(mode->token,"character")==0)){
        int i=searchingId(mode->attribute);
        int j=searchingNumber(mode->next->next->attribute);
        if(elseExist){
            fprintf(fp_ir,"\nT_%d := %s",j,mode->next->next->attribute);
            fprintf(fp_ir,"\nif T%d %s T_%d goto L%d else goto L%d\nL%d\n",i,mode->next->attribute,j,lcounter,lcounter+1,lcounter);
            mode=mode->next->next->next->next->next;
            while(strcmp(mode->attribute,"}")!=0){
                if(strcmp(mode->token,"identifier")==0 && strcmp(mode->next->attribute,"=")==0)
                    mode=IRassign(mode)->next;
                else
                    mode=mode->next;
            }
            mode=mode->next->next->next;
            fprintf(fp_ir,"\nL%d\n",lcounter+1);
            while(strcmp(mode->attribute,"}")!=0){
                if(strcmp(mode->token,"identifier")==0 && strcmp(mode->next->attribute,"=")==0)
                    mode=IRassign(mode)->next;
                else
                    mode=mode->next;
            }
            fprintf(fp_ir,"\n");
            lcounter += 2;
            return mode;
        }
        if(!elseExist){
            fprintf(fp_ir,"\nT_%d := %s",j,mode->next->next->attribute);
            fprintf(fp_ir,"\nif T%d %s T_%d goto L%d\nL%d\n",i,mode->next->attribute,j,lcounter,lcounter);
            lcounter += 1;
            mode=mode->next->next->next->next->next;
            while(strcmp(mode->attribute,"}")!=0){
                if(strcmp(mode->token,"identifier")==0 && strcmp(mode->next->attribute,"=")==0)
                    mode=IRassign(mode)->next;
                else
                    mode=mode->next;
            }
            fprintf(fp_ir,"\n");
            return mode;
        }

    }
    if((strcmp(mode->token,"number")==0 || strcmp(mode->token,"character")==0) && strcmp(mode->next->next->token,"identifier")==0){
        int i=searchingNumber(mode->attribute);
        int j=searchingId(mode->next->next->attribute);
        if(elseExist){
            fprintf(fp_ir,"\nT_%d := %s",i,mode->attribute);
            fprintf(fp_ir,"\nif T_%d %s T%d goto L%d else goto L%d\nL%d\n",i,mode->next->attribute,j,lcounter,lcounter+1,lcounter);
            mode=mode->next->next->next->next->next;
            while(strcmp(mode->attribute,"}")!=0){
                if(strcmp(mode->token,"identifier")==0 && strcmp(mode->next->attribute,"=")==0)
                    mode=IRassign(mode)->next;
                else
                    mode=mode->next;
            }
            mode=mode->next->next->next;
            fprintf(fp_ir,"\nL%d\n",lcounter+1);
            while(strcmp(mode->attribute,"}")!=0){
                if(strcmp(mode->token,"identifier")==0 && strcmp(mode->next->attribute,"=")==0)
                    mode=IRassign(mode)->next;
                else
                    mode=mode->next;
            }
            fprintf(fp_ir,"\n");
            lcounter += 2;
            return mode;
        }
        if(!elseExist){
            fprintf(fp_ir,"\nT_%d := %s",i,mode->attribute);
            fprintf(fp_ir,"\nif T_%d %s T%d goto L%d\nL%d\n",i,mode->next->attribute,j,lcounter,lcounter);
            lcounter +=1;
            mode=mode->next->next->next->next->next;
            while(strcmp(mode->attribute,"}")!=0){
                if(strcmp(mode->token,"identifier")==0 && strcmp(mode->next->attribute,"=")==0)
                    mode=IRassign(mode)->next;
                else
                    mode=mode->next;
            }
            fprintf(fp_ir,"\n");
            return mode;
        }
    }

}

void IRgeneration(ST *head){

    if(head->next==NULL)
        return;
    ST *mode=head->next;
    if(strcmp(mode->token,"identifier")==0 && strcmp(mode->next->attribute,"=")==0)
        mode=IRassign(mode);
    else if(strcmp(mode->attribute,"if")==0)
        mode=ifIR(mode);
    else if((strcmp(mode->attribute,"int") || strcmp(mode->attribute,"void"))&& strcmp(mode->next->attribute,"main")==0){
            fprintf(fp_ir,"\nPROCEDURE MAIN\n\nBEGIN\n\n");
            mode=mode->next->next->next->next;
    }
    else if((strcmp(mode->attribute,"return")==0)){
        fprintf(fp_ir,"\nRETURN\n\nCALL MAIN\n");
        mode->next=NULL;
    }
    IRgeneration(mode);
}

int main()
{
    preprocessor();
    ST *head = (ST *) malloc(sizeof(ST));
    head->next = NULL;
    loadingTokensFromFile(head);
    checkSyntax(head);
    if(!returnExist){
        printf("ERROR : didn't retrun anything\n");
        error=true;
    }
    /*if(countBrace>0)
        printf("you didin't close {\n");
    else if(countBrace<0)
        printf("you didn't open {\n");*/
    fp_ir=fopen("IR_GENERATION.txt","a");
    if(fp_ir==NULL)
        printf("can not open IR_GENERATION file");
    else if(!error)
        IRgeneration(head);
    return 0;
}
