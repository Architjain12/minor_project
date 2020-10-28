#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const int BYTES_PER_PIXEL = 3; /// red, green, & blue
const int FILE_HEADER_SIZE = 14;
const int INFO_HEADER_SIZE = 40;

unsigned char* createBitmapFileHeader (int height, int stride)
{
    int fileSize = FILE_HEADER_SIZE + INFO_HEADER_SIZE + (stride * height);

    static unsigned char fileHeader[] = {
        0,0,     /// signature
        0,0,0,0, /// image file size in bytes
        0,0,0,0, /// reserved
        0,0,0,0, /// start of pixel array
    };

    fileHeader[ 0] = (unsigned char)('B');
    fileHeader[ 1] = (unsigned char)('M');
    fileHeader[ 2] = (unsigned char)(fileSize      );
    fileHeader[ 3] = (unsigned char)(fileSize >>  8);
    fileHeader[ 4] = (unsigned char)(fileSize >> 16);
    fileHeader[ 5] = (unsigned char)(fileSize >> 24);
    fileHeader[10] = (unsigned char)(FILE_HEADER_SIZE + INFO_HEADER_SIZE);

    return fileHeader;
}

unsigned char* createBitmapInfoHeader (int height, int width)
{
    static unsigned char infoHeader[] = {
        0,0,0,0, /// header size
        0,0,0,0, /// image width
        0,0,0,0, /// image height
        0,0,     /// number of color planes
        0,0,     /// bits per pixel
        0,0,0,0, /// compression
        0,0,0,0, /// image size
        0,0,0,0, /// horizontal resolution
        0,0,0,0, /// vertical resolution
        0,0,0,0, /// colors in color table
        0,0,0,0, /// important color count
    };

    infoHeader[ 0] = (unsigned char)(INFO_HEADER_SIZE);
    infoHeader[ 4] = (unsigned char)(width      );
    infoHeader[ 5] = (unsigned char)(width >>  8);
    infoHeader[ 6] = (unsigned char)(width >> 16);
    infoHeader[ 7] = (unsigned char)(width >> 24);
    infoHeader[ 8] = (unsigned char)(height      );
    infoHeader[ 9] = (unsigned char)(height >>  8);
    infoHeader[10] = (unsigned char)(height >> 16);
    infoHeader[11] = (unsigned char)(height >> 24);
    infoHeader[12] = (unsigned char)(1);
    infoHeader[14] = (unsigned char)(BYTES_PER_PIXEL*8);

    return infoHeader;
}

void generateBitmapImage (unsigned char* image, int height, int width, char* imageFileName)
{
    int widthInBytes = width * BYTES_PER_PIXEL;

    unsigned char padding[3] = {0, 0, 0};
    int paddingSize = (4 - (widthInBytes) % 4) % 4;

    int stride = (widthInBytes) + paddingSize;

    FILE* imageFile = fopen(imageFileName, "wb");

    unsigned char* fileHeader = createBitmapFileHeader(height, stride);
    fwrite(fileHeader, 1, FILE_HEADER_SIZE, imageFile);

    unsigned char* infoHeader = createBitmapInfoHeader(height, width);
    fwrite(infoHeader, 1, INFO_HEADER_SIZE, imageFile);

    int i;
    for (i = 0; i < height; i++) {
        fwrite(image + (i*widthInBytes), BYTES_PER_PIXEL, width, imageFile);
        fwrite(padding, 1, paddingSize, imageFile);
    }
printf("IT is working!!!");
    fclose(imageFile);
}


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
printf("total_node is %d\n",total_node);
//printf("pix value %s",table[0].code);
//printf("pix value %s",table[1].code);
FILE *fp2=fopen("encoded_image.txt","rb");
//Reading the size of the file
fseek(fp2,0L,SEEK_END);
long int file_size=ftell(fp2);

rewind(fp2);



int **img;

img=(int**)malloc(sizeof(int*)*size_i);

for(int i=0;i<size_i;i++)
{
     img[i]=(int*)malloc(sizeof(int)*size_j);
}
printf("%d",size_i);
char *code_string;
code_string=(char*)malloc(sizeof(char)*file_size);
fread(code_string,file_size,1,fp2);
char bin_sub_string[Code_max_length];
//for(long int m=0;m<file_size;m++){
//printf("%c ",code_string[m]);
//}
int s=0;int ss=0;
int j=0;
i=0;
int flag,l;


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
printf("i=%d j=%d\n ",i,j);
int x,y;
//for(x=0;x<size_i;x++){
 //   for(y=0;y<size_j;y++)
 //       printf("%d ",img[x][y]);
//}

fclose(fp2);
unsigned char ***img1;
img1=(unsigned char***)malloc(size_i*sizeof(unsigned char**));
for(i=0;i<size_i;i++)
{
img1[i]=(unsigned char**)malloc(size_j*sizeof(unsigned char*));
for(j=0;j<size_j;j++){
img1[i][j]=(unsigned char*)malloc(3*sizeof(unsigned char));
}
}

for(i=0;i<size_i;i++){
    for(j=0;j<size_j;j++){
        int red=img[i][j]*(10/3);
        int green=img[i][j]*(100/58);
        int blue=img[i][j]*(100/11);

        img1[i][j][0]=red;
        img1[i][j][1]=green;
        img1[i][j][2]=blue;
    }
}
unsigned char* A = (unsigned char*)malloc(size_i * size_j * 3 * sizeof(unsigned char));
for (int i = 0; i < size_i; i++)
        for (int j = 0; j < size_j; j++)
            for (int k = 0; k < 3; k++)
                *(A + i*size_j*3 + j*3 + k) = img1[i][j][k];

char* imageFileName = (char*) "bitmapImage.bmp";
generateBitmapImage(A, size_i, size_j, imageFileName);
return 0;
}
