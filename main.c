#include <stdio.h>
#include <conio.h>
#include<stdlib.h>
void DCT(int **NM,int column,int row, int **Final_matrix)
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
    int HEIGHT_OF_IMAGE,WIDTH_OF_IMAGE,i,j;
    HEIGHT_OF_IMAGE=8;
    WIDTH_OF_IMAGE=8;
    //Sample image value
    int Image[8][8]={{154,123,123,123,123,123,123,136},
    {192,180,136,154,154,154,136,110},
    {254,198,154,154,180,154,123,123},
    {239,180,136,180,180,166,123,123},
    {180,154,136,167,166,149,136,136},
    {128,136,123,136,154,180,198,154},
    {123,105,110,149,136,136,180,166},
    {110,136,123,123,123,136,154,136}};

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
    for(i=0;i<HEIGHT_OF_IMAGE;i++)
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
    for(int i=0;i<WIDTH_OF_IMAGE;i++)
    {
        for(int j=0;j<HEIGHT_OF_IMAGE;j++)
        {
             normalized_matrix[i][j]= Image[i][j]-128;
        }
    }

    for(int i=0;i<WIDTH_OF_IMAGE;i++)
    {
        for(int j=0;j<HEIGHT_OF_IMAGE;j++)
        {
            printf("%d ",normalized_matrix[i][j]);
        }
        printf("\n");
    }
printf("\n");printf("\n");
    for(int i=0;i<WIDTH_OF_IMAGE;i=i+8)
    {
        for(int j=0;j<HEIGHT_OF_IMAGE;j=j+8)
        {
            DCT(normalized_matrix,i,j,DCT_Coeff_matrix);
        }
    }

    for(int i=0;i<WIDTH_OF_IMAGE;i++)
    {
        for(int j=0;j<HEIGHT_OF_IMAGE;j++)
        {
            printf("%d ",DCT_Coeff_matrix[i][j]);
        }
        printf("\n");
    }
printf("\n");printf("\n");
    for(int i=0;i<WIDTH_OF_IMAGE;i++)
    {
        for(int j=0;j<HEIGHT_OF_IMAGE;j++)
        {
            DCT_Coeff_matrix[i][j]=(int) DCT_Coeff_matrix[i][j]/Quantization[i%8][j%8];
        }
    }

    for(int i=0;i<WIDTH_OF_IMAGE;i++)
    {
        for(int j=0;j<HEIGHT_OF_IMAGE;j++)
        {
            printf("%d ",DCT_Coeff_matrix[i][j]);
        }
        printf("\n");
    }



    //DECOMPRESS

    void DCT_reverse(int **NM,int column,int row, int **Final_matrix)
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



    for(int i=0;i<WIDTH_OF_IMAGE;i++)
    {
        for(int j=0;j<HEIGHT_OF_IMAGE;j++)
        {
            DCT_Coeff_matrix[i][j]=DCT_Coeff_matrix[i][j]*Quantization[i%8][j%8];
        }
    }


    for(int i=0;i<WIDTH_OF_IMAGE;i=i+8)
    {
        for(int j=0;j<HEIGHT_OF_IMAGE;j=j+8)
        {
             //function DCT_reverse was never defiend
            DCT_reverse(DCT_Coeff_matrix,i,j,normalized_matrix);
            //random line added for now
            //DCT(DCT_Coeff_matrix,i,j,Decompressed_image);
        }
    }

    for(int i=0;i<WIDTH_OF_IMAGE;i++)
    {
        for(int j=0;j<HEIGHT_OF_IMAGE;j++)
        {
            Decompressed_image[i][j]=normalized_matrix[i][j]+128;
        }
    }

    for(int i=0;i<WIDTH_OF_IMAGE;i++)
    {
        for(int j=0;j<HEIGHT_OF_IMAGE;j++)
        {
            printf("%d ",Decompressed_image[i][j]);
        }
        printf("\n");
    }
    return 0;
}

