#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main() {
//Reading the table of pixel values and their Codes
FILE *fp=fopen("table1.txt","r");

char ch[50];
int size_i,size_j;
//Reading the height
fscanf(fp,"%s",ch);
size_i=atoi(ch);
//Reading the width
fscanf(fp,"%s",ch);
size_j=atoi(ch);
//Reading max code length
fscanf(fp,"%s",ch);
int Code_max_length=atoi(ch);

//Reading no. of Nodes
fscanf(fp,"%s",ch);
int nodes=atoi(ch);


struct pix
{
    int pix_val;

    char code[Code_max_length];
};

struct pix* table;
table = (struct pix*)malloc(sizeof(struct pix) * nodes);

int i=0;
int k=0;

printf("maxLen and node is %d %d\n",Code_max_length,nodes);
while (fscanf(fp, "%s", ch) == 1)
{
     if(i%2==0)
     {
          table[k].pix_val= atoi(ch);
          //printf("pix value %s",ch);printf("\n");
          i++;
     }
     else
    {
          strcpy(table[k++].code,ch);
          //printf("pix code %s",ch);printf("\n");
          i++;
    }

}
int total_node=k;
fclose(fp);
//printf("total_node is %d\n",total_node);
//printf("pix value %s",table[0].code);
//printf("pix value %s",table[1].code);
FILE *fp2=fopen("encoded_image.txt","rb");
//Reading the size of the file
fseek(fp2,0L,SEEK_END);
long int file_size=ftell(fp2);

rewind(fp2);

//printf("total_node is %d %d\n",size_i,size_j);
//double (*variable)
//int img[size_i][size_j]={0};
int **img;

img=(int**)malloc(sizeof(int*)*size_i);

for(int i=0;i<size_i;i++)
{
     img[i]=(int*)malloc(sizeof(int)*size_j);
}

//printf("%d",size_i);
char *code_string;
code_string=(char*)malloc(sizeof(char)*file_size);
fread(code_string,file_size,1,fp2);
char bin_sub_string[Code_max_length];
printf("hello");

int s=0;int ss=0;
int j=0;
i=0;
int flag,l;
int fun=0;
//printf("total node %d\n",total_node);
//while(fun<total_node)
//{
//printf("%s\n",table[fun++].code);
//}
bin_sub_string[0]='0';
//bin_sub_string[1]=code_string[1];
bin_sub_string[1]='\0';
bin_sub_string[1]=code_string[0];
bin_sub_string[2]='\0';
//printf(" bin %s\n",bin_sub_string);


while(ss<file_size)
{
    //printf("while1");
    bin_sub_string[s]=code_string[ss];
    flag=1;l=0;
    bin_sub_string[s+1]='\0';
    while(flag && l<total_node)
    {
       //printf("while2");

        if(strcmp(table[l].code,bin_sub_string)==0)
        {
            img[i][j]=table[l].pix_val;
            //printf("if");
            j++;
            flag=0;
            if(j==size_j){i++; j=0;}
        }
        l++;
    }
    if (flag==0){s=0; memset(bin_sub_string,0,Code_max_length);ss++;}
    else{s++;ss++;
    //printf("%d%d ",s,ss);
    }
}
printf("\n\ni=%d j=%d\n ",i,j);
int x,y;
for(x=0;x<size_i;x++){
    for(y=0;y<size_j;y++)
        {printf("%d ",img[x][y]);}
    printf("\n");
}

fclose(fp2);

return 0;
}
