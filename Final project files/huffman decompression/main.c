#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const int BYTES_PER_PIXEL = 3; /// red, green, & blue
const int FILE_HEADER_SIZE = 14;
const int INFO_HEADER_SIZE = 40;
//create file header which contains meta-data of image
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
//create info header which contains all the information of image
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
//this function uses file header and info header to generate actual image
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
//now we will create our table structure using the table.txt which contains
//the pixel values along with their codes from Huffman compression
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
          i++;
     }
     else
    {
          strcpy(table[k++].code,ch);
          i++;
    }

}
int total_node=k;
fclose(fp);
printf("total_node is %d\n",total_node);
FILE *fp2=fopen("encoded_image.txt","rb");
//Reading the size of the file
fseek(fp2,0L,SEEK_END);
long int file_size=ftell(fp2);

rewind(fp2);//sets the pointer back to the start of the file.



int **img;

img=(int**)malloc(sizeof(int*)*size_i);

for(int i=0;i<size_i;i++)
{
     img[i]=(int*)malloc(sizeof(int)*size_j);
}
printf("%d",size_i);
char *code_string;
code_string=(char*)malloc(sizeof(char)*file_size);//we are storing all the codes in the file in this character array
fread(code_string,file_size,1,fp2);
char bin_sub_string[Code_max_length];//This will be the substring that will check if it matches any code in the table
                                     //then we will pick that pixel value and store it in image array

int s=0;int ss=0;
int j=0;
i=0;
int flag,l;
while(ss<file_size)
{
    bin_sub_string[s]=code_string[ss];
    flag=1;l=0;
    bin_sub_string[s+1]='\0';
    while(flag && l<total_node)
    {

        if(strcmp(table[l].code,bin_sub_string)==0)//we will compare the codes from table and this substring if they match
                                                   //we will pick the pixel value and store it in image array
        {
            img[i][j]=table[l].pix_val;
            j++;
            flag=0;
            if(j==size_j){i++; j=0;}//we switch to the next row of image array
        }
        l++;
    }
    if (flag==0){s=0; memset(bin_sub_string,0,Code_max_length);ss++;}//we found one match now to look for another we set
                                                                     //substring to empty and start matching again
    else{s++;ss++;
    }
}
printf("i=%d j=%d\n ",i,j);
int x,y;
fclose(fp2);

unsigned char ***img1;//our final image array
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
        int red=img[i][j];
        int green=img[i][j];
        int blue=img[i][j];

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

/*for (int k = 0; k < 3; k++)
    for (int i = 0; i < size_j; i++)
        for (int j = 0; j < size_i; j++)
            *(A + i*size_i*3 + j*3 + k) = img1[k][i][j];*/

char* imageFileName = (char*) "bitmapImage.bmp";
generateBitmapImage(A, size_i, size_j, imageFileName);//create the actual image from image array(img1)
return 0;
}
