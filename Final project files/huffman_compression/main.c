// C Code for
// Image Compression
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
// function to calculate word length
int codelen(char* code)
{
	int l = 0;
	while (*(code + l) != '\0')//code[l]!='\0' then increment l
		l++;
	return l;
}

// function to concatenate the words
void strconcat(char* str, char* parentcode, char add)
{
	int i = 0;
	while (*(parentcode + i) != '\0')
	{
		*(str + i) = *(parentcode + i);
		i++;
	}
	if (add != '2')
	{
		str[i] = add;
		str[i + 1] = '\0';
	}
	else
		str[i] = '\0';
}

// function to find fibonacci number
int fib(int n)
{
	if (n <= 1)
		return n;
	return fib(n - 1) + fib(n - 2);
}

// Driver code
int main()
{
	int i, j;
	char filename[] = "MARBLES.bmp";
	int data = 0, offset, bits_per_pixel = 0, width, height;
	long bmpsize = 0, bmpdataoff = 0;
	int** image_matrix,***img;
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

		fread(&bits_per_pixel, 2, 1, image_file);

		// Setting offset to start of pixel data
		fseek(image_file, bmpdataoff, SEEK_SET);

		// Creating Image array
		image_matrix = (int**)malloc(height * sizeof(int*));

		for (i = 0; i < height; i++)
		{
			image_matrix[i] = (int*)malloc(width * sizeof(int));
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
                //we perform a weighted average to convert image to grey scale.
                //gray  =red* 0.3 + green* 0.59 + blue * 0.11;
                gray  =red* 0.3 + green* 0.59 + blue * 0.11;

				 //gray=(red+green+blue) /3;
                image_matrix[i][j] = gray;
			}
		}
	}

	// Finding the probability
	// of occurrence

	//first we create a histogram for image array containing each pixel value with its count.
	int hist[256];
	for (i = 0; i < 256; i++)
		hist[i] = 0;
	for (i = 0; i < height; i++)
		for (j = 0; j < width; j++)
			hist[image_matrix[i][j]] += 1;

	// Finding number of
	// non-zero occurrences

	//These will be the number of base nodes in our huffman tree,the tree will be created in a bottom up manner.
	int nodes = 0;
	for (i = 0; i < 256; i++)
		if (hist[i] != 0)
			nodes += 1;

	// Calculating minimum probability
	float p = 1.0, ptemp;//here p is set to maximum value of probability we will store the final minimum probability in p.
	for (i = 0; i < 256; i++)
	{
		ptemp = (hist[i] / (float)(height * width));
		if (ptemp > 0 && ptemp <= p)
			p = ptemp;
	}

	// Calculating max length
	// of code word possible

	//Here we have used a theorum from paper  “Maximal codeword lengths in Huffman codes” by  Y.S.Abu-Mostafa and R.J.McEliece.
	i = 0;
	while ((1 / p) > fib(i))
		i++;
	int maxcodelen = i - 3;

	// Defining Structures pixfreq
	struct pixfreq //This will store the information of each node of huffman tree
	{
		int pix, larrloc, rarrloc;
		float freq;
		struct pixfreq *left, *right;
		char code[maxcodelen];
	};

	// Defining Structures
	// huffcode
	struct huffcode //This will store the information of each node of
	{               //temporary tree which is sorted each time to give us the two minimum nodes
		int pix, arrloc;
		float freq;
	};

	// Declaring structs
	struct pixfreq* huff_tree; //Our huffman tree built by combining two nodes of minimum frequency.

	struct huffcode* huffcodes;//our updated(or sorted) tree that will give us the two minimum nodes
                               //each time any new node is inserted
	int totalnodes = 2 * nodes - 1;//the number of nodes in our final huffman tree
	huff_tree = (struct pixfreq*)malloc(sizeof(struct pixfreq) * totalnodes);
	huffcodes = (struct huffcode*)malloc(sizeof(struct huffcode) * nodes);

	// Initializing the huffcodes and huff_tree arrays
	j = 0;
	int totpix = height * width;//total pixel values in our array
	float tempprob;
	for (i = 0; i < 256; i++)
	{
		if (hist[i] != 0)
		{

			// pixel intensity value
			huffcodes[j].pix = i;
			huff_tree[j].pix = i;

			// location of the node
			// in the huff_tree array
			huffcodes[j].arrloc = j;

			// probability of occurrence
			tempprob = (float)hist[i] / (float)totpix;
			huff_tree[j].freq = tempprob;//storing probability of occurrence of pixel values as frequency of occurrence
			huffcodes[j].freq = tempprob;//minimum frequency two nodes will be chosen while combining the nodes

			// Declaring the child of leaf
			// node as NULL pointer
			huff_tree[j].left = NULL;
			huff_tree[j].right = NULL;

			// initializing the code
			// word as end of line
			huff_tree[j].code[0] = '\0';
			j++;
		}
	}

	// Sorting the histogram
	struct huffcode temphuff;

	// Sorting w.r.t probability
	// of occurrence(decreasing order)
	for (i = 0; i < nodes; i++)
	{
		for (j = i + 1; j < nodes; j++)
		{
			if (huffcodes[i].freq < huffcodes[j].freq)
			{
				temphuff = huffcodes[i];
				huffcodes[i] = huffcodes[j];
				huffcodes[j] = temphuff;
			}
		}
	}

	// Building Huffman Tree
	float sumprob;//it will store the sum of probabilities of two lowest frequency nodes
	int sumpix;//store the sum of their pixel values
	int n = 0, k = 0;
	int nextnode = nodes;

	// Since total number of
	// nodes in Huffman Tree
	// is 2*nodes-1
	while (n < nodes - 1)
	{

		// Adding the lowest two probabilities
		sumprob = huffcodes[nodes - n - 1].freq + huffcodes[nodes - n - 2].freq;
		sumpix = huffcodes[nodes - n - 1].pix + huffcodes[nodes - n - 2].pix;

		// Appending to the huff_tree Array
		huff_tree[nextnode].pix = sumpix;
		huff_tree[nextnode].freq = sumprob;
		//we set the left and right pointers to the previous lowest frequency nodes
		huff_tree[nextnode].left = &huff_tree[huffcodes[nodes - n - 2].arrloc];//picking the location of 2nd lowest node
		huff_tree[nextnode].right = &huff_tree[huffcodes[nodes - n - 1].arrloc];//picking the location of the lowest prob. node
		huff_tree[nextnode].code[0] = '\0';
		i = 0;

		// Sorting and Updating the
		// huffcodes array simultaneously

		// finding new position of the combined node
		while (sumprob <= huffcodes[i].freq)
			i++;

		// Inserting the new node
		// in the huffcodes array by right shifting the lesser values
		for (k = nodes; k >= 0; k--)
		{
			if (k == i)
			{
				huffcodes[k].pix = sumpix;
				huffcodes[k].freq = sumprob;
				huffcodes[k].arrloc = nextnode;
			}
			else if (k > i)

				// Shifting the nodes below
				// the new node by 1
				// For inserting the new node
				// at the updated position k
				huffcodes[k] = huffcodes[k - 1];//basically shifting the nodes to the right similar to insertion sort

		}
		n += 1;
		nextnode += 1;
	}

	// Assigning Code through
	// backtracking
	char left = '0';
	char right = '1';
	int index;
	for (i = totalnodes - 1; i >= nodes; i--)
	{
		if (huff_tree[i].left != NULL)
			strconcat(huff_tree[i].left->code, huff_tree[i].code, left);
		if (huff_tree[i].right != NULL)
			strconcat(huff_tree[i].right->code, huff_tree[i].code, right);
	}

	// Encode the Image
	int pix_val;
	int l;

	// Writing the Huffman encoded
	// Image into a text file
	FILE* imagehuff = fopen("encoded_image.txt", "wb");
	for (i = 0; i < height; i++)
		for (j = 0; j < width; j++)
		{
			pix_val = image_matrix[i][j];
			for (l = 0; l < nodes; l++)
				if (pix_val == huff_tree[l].pix)
					fprintf(imagehuff, "%s", huff_tree[l].code);
		}

	// Printing Huffman Codes
	printf("Huffmann Codes::\n\n");
	printf("pixel values -> Code\n\n");
	for (i = 0; i < nodes; i++) {
		if (snprintf(NULL, 0, "%d", huff_tree[i].pix) == 2)
			printf("	 %d	 -> %s\n", huff_tree[i].pix, huff_tree[i].code);
		else
			printf(" %d	 -> %s\n", huff_tree[i].pix, huff_tree[i].code);
	}

	// Calculating Average Bit Length
	float avgbitnum = 0;
	for (i = 0; i < nodes; i++)
		avgbitnum += huff_tree[i].freq * codelen(huff_tree[i].code);
	printf("Average number of bits:: %f", avgbitnum);

	char  a[100]="";
 char result[50];
        FILE* table = fopen("table1.txt", "a");
        //entering height in the table
        char temp1[50];
        itoa(height,temp1,10);
        strcat(temp1," ");
        fprintf(table,"%s",temp1);
        //printf("%s",temp1);
        //entering width in the table
        itoa(width,temp1,10);
        strcat(temp1," ");
        fprintf(table,"%s",temp1);
        //printf("%s",temp1);
        //Entering max code length in table
        itoa(maxcodelen,result,10);
        strcat(result," ");
        fprintf(table,"%s",result);
        //Entering nodes+1 in table
        char nod[50];
        itoa(nodes+1,nod,10);
        strcat(nod," ");
        fprintf(table,"%s",nod);

        for (l = 0; l < nodes; l++){
            sprintf(result,"%d",huff_tree[l].pix);
            strcat(result," ");
            strcat(result,huff_tree[l].code);
            strcat(result," ");
            fprintf(table,"%s",result);
        }
        fclose(table);
}
