#include <stdio.h>
#include <conio.h>
#include<stdlib.h>
void DCT(int **NM,int row,int column, int **Final_matrix)
{
    float DCT_matrix[8][8]={{0.3536,0.3536,0.3536,0.3536,0.3536,0.3536,0.3536,0.3536},
    {0.4904,0.4157,0.2778,0.0975,-0.0975,-0.2778,-0.4157,-0.4904},
    {0.4619,0.1913,-0.1913,-0.4619,-0.4619,-0.1913,0.1913,0.4619},
    {0.4157,-0.0975,-0.4904,-0.2778,0.2778,0.4904,0.0975,-0.4157},
    {0.3536,-0.3536,-0.3536,0.3536,0.3536,-0.3536,-0.3536,0.3536},
    {0.2778,-0.4904,0.0975,0.4157,-0.4157,-0.0975,0.4904,-0.2778},
    {0.1913,-0.4619,0.4619,-0.1913,-0.1913,0.4619,-0.4619,0.1913},
    {0.0975,-0.2778,0.4157,-0.4904,0.4904,-0.4157,0.2778,-0.0975}};

    int i, j, k;
    int matrix_1[8][8];
    for(i = row; i < row+8; i++)
    {
        for (j = column; j < column+8; j++)
        {
            matrix_1[i%8][j%8] = 0;
            for (k = row; k < row+8; k++)
            {
                matrix_1[i%8][j%8] += DCT_matrix[i%8][k%8]*NM[k][j];
             }
        }
    }
    for(i = row; i < row+8; i++)
    {
        for (j = column; j < column+8; j++)
        {
            Final_matrix[i][j] = 0;
            for (k = row; k < row+8; k++)
            {
                Final_matrix[i][j] += matrix_1[i%8][k%8]*DCT_matrix[j%8][k%8];
            }
        }
    }
}
    void DCT_reverse(int **NM,int row,int column, int **Final_matrix)
{
    float DCT_matrix[8][8]={{0.3536,0.3536,0.3536,0.3536,0.3536,0.3536,0.3536,0.3536},
    {0.4904,0.4157,0.2778,0.0975,-0.0975,-0.2778,-0.4157,-0.4904},
    {0.4619,0.1913,-0.1913,-0.4619,-0.4619,-0.1913,0.1913,0.4619},
    {0.4157,-0.0975,-0.4904,-0.2778,0.2778,0.4904,0.0975,-0.4157},
    {0.3536,-0.3536,-0.3536,0.3536,0.3536,-0.3536,-0.3536,0.3536},
    {0.2778,-0.4904,0.0975,0.4157,-0.4157,-0.0975,0.4904,-0.2778},
    {0.1913,-0.4619,0.4619,-0.1913,-0.1913,0.4619,-0.4619,0.1913},
    {0.0975,-0.2778,0.4157,-0.4904,0.4904,-0.4157,0.2778,-0.0975}};

    int i, j, k;
    int matrix_1[8][8];
    for(i = row; i < row+8; i++)
    {
        for (j = column; j < column+8; j++)
        {
            matrix_1[i%8][j%8] = 0;
            for (k = row; k < row+8; k++)
            {
                matrix_1[i%8][j%8] += DCT_matrix[k%8][i%8]*NM[k][j];
            }
        }
    }


    for(i = row; i < row+8; i++)
    {
        for (j = column; j < column+8; j++)
        {
            Final_matrix[i][j] = 0;
            for (k = row; k < row+8; k++)
            {
                Final_matrix[i][j] += matrix_1[i%8][k%8]*DCT_matrix[k%8][j%8];
            }
        }
    }
}


void new_Quantization(int **Q, int QC)
{
    float Q_C;
    if(QC>50)
    {
        Q_C=(float)(100-QC)/50;   //less compression
    }
    else
    {
        Q_C=(float)QC/50;         //more compression
    }
    printf("hhhhd");
    printf("%d\n  ",Q_C);
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            Q[i][j]=(int) Q[i][j]*Q_C;
            printf("%d ",Q[i][j]);
        }
    }
}

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

int main()
{
    int Quantization1[8][8]={{16,11,10,16,24,40,51,61},
    {12,12,14,19,26,58,60,55},
    {14,13,16,24,40,57,69,56},
    {14,17,22,29,51,87,80,62},
    {18,22,37,56,68,109,103,77},
    {24,35,55,64,81,104,113,92},
    {49,64,78,87,103,121,120,101},
    {72,92,95,8,112,100,103,99}};
    int Quantization_coeff=10;
	int i, j;
	char filename[] = "sample4.bmp";
	int data = 0, offset, bpp = 0, width, height;
	long bmpsize = 0, bmpdataoff = 0;
	int** image,***img;
	unsigned char ***img1;
	int HEIGHT_OF_IMAGE,WIDTH_OF_IMAGE;
	unsigned char ***image_rgb;
	int temp = 0;

	// Reading the BMP File
	FILE* image_file;

	image_file = fopen(filename, "rb");
	if (image_file == NULL)
	{
		printf("Error Opening File!!");
		exit(1);
	}
	else
	{

		// Set file position of the
		// stream to the beginning
		// Contains file signature
		// or ID "BM"
		offset = 0;

		// Set offset to 2, which
		// contains size of BMP File
		offset = 2;

		fseek(image_file, offset, SEEK_SET);

		// Getting size of BMP File
		fread(&bmpsize, 4, 1, image_file);

		// Getting offset where the
		// pixel array starts
		// Since the information is
		// at offset 10 from the start,
		// as given in BMP Header
		offset = 10;

		fseek(image_file, offset, SEEK_SET);

		// Bitmap data offset
		fread(&bmpdataoff, 4, 1, image_file);

		// Getting height and width of the image
		// Width is stored at offset 18 and
		// height at offset 22, each of 4 bytes
		fseek(image_file, 18, SEEK_SET);

		fread(&width, 4, 1, image_file);

		fread(&height, 4, 1, image_file);

		// Number of bits per pixel
		fseek(image_file, 2, SEEK_CUR);

		fread(&bpp, 2, 1, image_file);

		// Setting offset to start of pixel data
		fseek(image_file, bmpdataoff, SEEK_SET);

		// Creating Image array

    HEIGHT_OF_IMAGE=height;
    WIDTH_OF_IMAGE=width;

image_rgb=(unsigned char***)malloc(HEIGHT_OF_IMAGE*sizeof(unsigned char**));
for(i=0;i<HEIGHT_OF_IMAGE;i++)
{
image_rgb[i]=(unsigned char**)malloc(WIDTH_OF_IMAGE*sizeof(unsigned char*));
for(j=0;j<WIDTH_OF_IMAGE;j++){
image_rgb[i][j]=(unsigned char*)malloc(3*sizeof(unsigned char));
}
}


img1=(unsigned char***)malloc(HEIGHT_OF_IMAGE*sizeof(unsigned char**));
for(i=0;i<HEIGHT_OF_IMAGE;i++)
{
img1[i]=(unsigned char**)malloc(WIDTH_OF_IMAGE*sizeof(unsigned char*));
for(j=0;j<WIDTH_OF_IMAGE;j++){
img1[i][j]=(unsigned char*)malloc(3*sizeof(unsigned char));
}
}


		// int image[height][width]
		// can also be done
		// Number of bytes in
		// the Image pixel array
		int numbytes = (bmpsize - bmpdataoff) / 3;

		// Reading the BMP File
		// into Image Array
		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
                                                                               unsigned char x,y,z;
				//fread(&temp, 3, 1, image_file);
				fread(&x, 1, 1, image_file);
				fread(&y, 1, 1, image_file);
				fread(&z, 1, 1, image_file);
				int red,green,blue;
				red=x;green=y;blue=z;
			                    // the Image is a
				// 24-bit BMP Image
				//int gray;
                                                                              //gray  =red* 0.3 + green* 0.59 + blue * 0.11;
                                                                              //gray  =red* 0.3 + green* 0.59 + blue * 0.11;

				//gray=(red+green+blue) /3;
                image_rgb[i][j][0] = red;
                image_rgb[i][j][1] = green;
                image_rgb[i][j][2] = blue;
			}
		}
	}


	image = (int**)malloc(height * sizeof(int*));
    for (i = 0; i < height; i++)
    {
        image[i] = (int*)malloc(width * sizeof(int));
    }

    for(int xyz=0;xyz<3;xyz++)
    {
        for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
			    image[i][j]=image_rgb[i][j][xyz];
			}
		}


printf("d");
    //Sample image value



    //int normalized_matrix1[HEIGHT_OF_IMAGE][WIDTH_OF_IMAGE];
    //int Decompressed_image1[HEIGHT_OF_IMAGE][WIDTH_OF_IMAGE];
    //int DCT_Coeff_matrix1[HEIGHT_OF_IMAGE][WIDTH_OF_IMAGE];


    int **normalized_matrix;
    int **DCT_Coeff_matrix;
    int **Decompressed_image;
    int **Quantization;

    normalized_matrix=(int**)malloc(sizeof(int*)*HEIGHT_OF_IMAGE);
    for(i=0;i<HEIGHT_OF_IMAGE;i++)
    {
            normalized_matrix[i]=(int*)malloc(sizeof(int)*WIDTH_OF_IMAGE);
     }


    DCT_Coeff_matrix=(int**)malloc(sizeof(int*)*HEIGHT_OF_IMAGE);
    for(i=0;i<HEIGHT_OF_IMAGE;i++)
    {
            DCT_Coeff_matrix[i]=(int*)malloc(sizeof(int)*WIDTH_OF_IMAGE);
     }


    Decompressed_image=(int**)malloc(sizeof(int*)*HEIGHT_OF_IMAGE);
    for(i=0;i<HEIGHT_OF_IMAGE;i++)
    {
            Decompressed_image[i]=(int*)malloc(sizeof(int)*WIDTH_OF_IMAGE);
     }


    Quantization=(int**)malloc(sizeof(int*)*8);
    for(i=0;i<8;i++)
    {
           Quantization[i]=(int*)malloc(sizeof(int)*8);
    }

    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
           Quantization[i][j]=Quantization1[i][j];
        }
    }

//printf("qs");
    if(Quantization_coeff!=50)
    {
        new_Quantization(Quantization, Quantization_coeff);
    }
//printf("qos");
    for(int i=0;i<HEIGHT_OF_IMAGE;i++)
    {
        for(int j=0;j<WIDTH_OF_IMAGE;j++)
        {
             normalized_matrix[i][j]= image[i][j]-128;
        }
    }
printf("\n Quantization");
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            printf("%d ",Quantization[i][j]);
        }
        printf("\n");
    }
printf("\n");printf("\n");



    for(int i=0;i<HEIGHT_OF_IMAGE;i=i+8)
    {
        for(int j=0;j<WIDTH_OF_IMAGE;j=j+8)
        {
            DCT(normalized_matrix,i,j,DCT_Coeff_matrix);
        }
    }

    /*for(int i=0;i<HEIGHT_OF_IMAGE;i++)
    {
        for(int j=0;j<WIDTH_OF_IMAGE;j++)
        {
            printf("%d ",DCT_Coeff_matrix[i][j]);
        }
        printf("\n");
    }
printf("\n");printf("\n");*/



    for(int i=0;i<HEIGHT_OF_IMAGE;i++)
    {
        for(int j=0;j<WIDTH_OF_IMAGE;j++)
        {
            DCT_Coeff_matrix[i][j]=(int) DCT_Coeff_matrix[i][j]/Quantization[i%8][j%8];
        }
    }

    /*for(int i=0;i<HEIGHT_OF_IMAGE;i++)
    {
        for(int j=0;j<WIDTH_OF_IMAGE;j++)
        {
            printf("%d ",DCT_Coeff_matrix[i][j]);
        }
        printf("\n");
    }*/



    //DECOMPRESS

printf("\ndecom");

printf("HEIGHT_OF_IMAGE=%d",HEIGHT_OF_IMAGE);
printf("WIDTH_OF_IMAGE=%d",WIDTH_OF_IMAGE);
int funny,zz=0;
    for(int i=0;i<HEIGHT_OF_IMAGE;i++)
    {
        for(int j=0;j<WIDTH_OF_IMAGE;j++)
        {
            funny=DCT_Coeff_matrix[i][j];
            DCT_Coeff_matrix[i][j]=DCT_Coeff_matrix[i][j]*Quantization[i%8][j%8];
            if(funny==DCT_Coeff_matrix[i][j])
            {
                zz++;
            }
        }
    }
    printf("zz=%d \n",zz);



    for(int i=0;i<HEIGHT_OF_IMAGE;i=i+8)
    {
        for(int j=0;j<WIDTH_OF_IMAGE;j=j+8)
        {
             //function DCT_reverse was never defiend
            DCT_reverse(DCT_Coeff_matrix,i,j,normalized_matrix);
            //random line added for now
            //DCT(DCT_Coeff_matrix,i,j,Decompressed_image);
        }
    }



    for(int i=0;i<HEIGHT_OF_IMAGE;i++)
    {
        for(int j=0;j<WIDTH_OF_IMAGE;j++)
        {
            Decompressed_image[i][j]=normalized_matrix[i][j]+128;
        }
    }

    /*for(int i=0;i<HEIGHT_OF_IMAGE;i++)
    {
        for(int j=0;j<WIDTH_OF_IMAGE;j++)
        {
            printf("%d ",Decompressed_image[i][j]);
        }
        printf("\n");
    }*/
printf("%d",xyz);

for(i=0;i<HEIGHT_OF_IMAGE;i++){
    for(j=0;j<WIDTH_OF_IMAGE;j++){
        img1[i][j][xyz]=Decompressed_image[i][j];
    }
}

}

unsigned char* A = (unsigned char*)malloc(HEIGHT_OF_IMAGE * WIDTH_OF_IMAGE * 3 * sizeof(unsigned char));
for (int k = 0; k < 3; k++)
    for (int i = 0; i < HEIGHT_OF_IMAGE; i++)
        for (int j = 0; j < WIDTH_OF_IMAGE; j++)
            *(A + i*WIDTH_OF_IMAGE*3 + j*3 + k) = img1[i][j][k];
for (int k = 0; k < 3; k++)
    for (int i = 0; i < HEIGHT_OF_IMAGE; i++)
        for (int j = 0; j < WIDTH_OF_IMAGE; j++)
        {
            if (i%7==0 || j%7==0)
            {
                continue;
            }
            else
            {
                int vi=i/7;
                int vj=j/7;
                *A[i][j][k]=*A[i-vi][j-vj][k];
            }
        }


char* imageFileName = (char*) "bitmapImage12.bmp";
generateBitmapImage(A, HEIGHT_OF_IMAGE, WIDTH_OF_IMAGE, imageFileName);

    printf("hello");


    return 0;
}
