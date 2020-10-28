  
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
void new_Quantization(int **Q, int QC)
{
    float Q_C;
    if(QC>50)
    {
        Q_C=(100-QC)/50;   //less compression
    }
    else
    {
        Q_C=QC/50;         //more compression
    }
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            Q[i][j]=(int) Q[i][j]*Q_C;
        }
    }
}
int main()
{
	int i, j;
	char filename[] = "LAND.bmp";
	int data = 0, offset, bpp = 0, width, height;
	long bmpsize = 0, bmpdataoff = 0;
	int** image,***img;
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
		image = (int**)malloc(height * sizeof(int*));

		for (i = 0; i < height; i++)
		{
			image[i] = (int*)malloc(width * sizeof(int));
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
				fread(&y, 1, 1, image_file);
				int red,green,blue;
				red=x;green=y;blue=z;
			                    // the Image is a
				// 24-bit BMP Image
				int gray;
                                                                              //gray  =red* 0.3 + green* 0.59 + blue * 0.11;
                                                                              gray  =red* 0.3 + green* 0.59 + blue * 0.11;

				//gray=(red+green+blue) /3;
                                                                              image[i][j] = gray;
			}
		}
	}

    int HEIGHT_OF_IMAGE,WIDTH_OF_IMAGE;
    HEIGHT_OF_IMAGE=height;
    WIDTH_OF_IMAGE=width;
    //Sample image value
   
  

    //int normalized_matrix1[HEIGHT_OF_IMAGE][WIDTH_OF_IMAGE];
    //int Decompressed_image1[HEIGHT_OF_IMAGE][WIDTH_OF_IMAGE];
    //int DCT_Coeff_matrix1[HEIGHT_OF_IMAGE][WIDTH_OF_IMAGE];
    int Quantization1[8][8]={{16,11,10,16,24,40,51,61},
    {12,12,14,19,26,58,60,55},
    {14,13,16,24,40,57,69,56},
    {14,17,22,29,51,87,80,62},
    {18,22,37,56,68,109,103,77},
    {24,35,55,64,81,104,113,92},
    {49,64,78,87,103,121,120,101},
    {72,92,95,8,112,100,103,99}};
    int Quantization_coeff=50;

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


    if(Quantization_coeff!=50)
    {
        new_Quantization(Quantization, Quantization_coeff);
    }

    for(int i=0;i<HEIGHT_OF_IMAGE;i++)
    {
        for(int j=0;j<WIDTH_OF_IMAGE;j++)
        {
             normalized_matrix[i][j]= image[i][j]-128;
        }
    }

    /*for(int i=0;i<HEIGHT_OF_IMAGE;i++)
    {
        for(int j=0;j<WIDTH_OF_IMAGE;j++)
        {
            printf("%d ",normalized_matrix[i][j]);
        }
        printf("\n");
    }
printf("\n");printf("\n");*/



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



    for(int i=0;i<HEIGHT_OF_IMAGE;i++)
    {
        for(int j=0;j<WIDTH_OF_IMAGE;j++)
        {
            DCT_Coeff_matrix[i][j]=DCT_Coeff_matrix[i][j]*Quantization[i%8][j%8];
        }
    }



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
   
    printf("hello");


    return 0;
}
