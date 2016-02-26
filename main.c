/*This is homewrok 1 of Yuxuan */ 

/*
 * File:   main.c
 * Author: zyx
 *
 * Created on February 17, 2016, 5:28 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*
 Now I'm going to illustrate why I think my code has the least meomery.
 I define the double pointer pix , and malloc it with its row number, in 
 that case, I can take the minium spcace it needs; Then define the pix[row]
which is the arrays of pointers which are poniters to each line numbers.
And it is the same way , ask the system give column numbers to miumim its
space, in that case, both rows and columes are minmum so that the mermory 
we need is minmum and those it is somewhat different from the two mension 
array because only each row is continous and differect columns are not.  
 */

int main(int argc, char** argv) {

    int x, y, t;

    char type[3] = {0};

    int i, j;

    FILE * fp1 = fopen("testing2.pgm", "r");

    FILE * fp2 = fopen("testing_output.pgm", "w");

    FILE * fp3 = fopen("testing_sobel_edge_output.pgm", "w");

    //read the header information from the original file and print it into output file

    fscanf(fp1, "%s", type); //read the header type value
    fprintf(fp2, "%s\n", type); //print the header type value

    fscanf(fp1, "%d %d ", &x, &y); //read the column and row value
    fprintf(fp2, "%d %d \n", x, y); //print the column and row value

    fscanf(fp1, "%d", &t); //read the threshold value
    fprintf(fp2, "%d \n", t); //print the threshold value

    //To infer the type is "P2".

    if (strcmp(type, "P2") != 0)
        printf("error. The program can't run well.");

    // redefine row and colum from y , x from the original file's header

    const unsigned int row = y;
    const unsigned int column = x;

    int **pix; //define the double pointer pix


    pix = (int**) malloc(sizeof (int**)*row); //malloc the space for the double pointer pix and just use the row number we detected

    for (j = 0; j < row; j++) {
        pix[j] = (int*) malloc(sizeof (int*)*column);
    }//malloc the space for the  pointers array pix[row] and just use the column number we detected


    //read from the original file and print to the output file 

    for (j = 0; j < row; j++) {
        for (i = 0; i < column; i++) {
            fscanf(fp1, "%d ", *(pix + j) + i);
            fprintf(fp2, "%d ", *(*(pix + j) + i));
        }
        fprintf(fp2, "\n");
    }


    fclose(fp2);

    //For sobel edge detection

    int GX[3][3] = {
        {1, 0, -1},
        { 2, 0, -2},
        { 3, 0, -3}
    }; //define sobel edge detection array GX 

    int GY[3][3] = {
        {1, 2, 1},
        { 0, 0, 0},
        {-1, -2, -1}
    }; //define sobel edge detection array GY

    int SUM;
    int sumX, sumY;
    int m, n;

    for (j = 0; j < row; j++) {
        for (i = 0; i < column; i++) {

            sumX = 0;
            sumY = 0;

            if (i == 0 | i == column - 1 | i == column - 2)//column 1 and last should be 0, and for using pointer, last second column should also be 0;
                SUM = 0;
            else if (j == 0 | j == row - 1 | j == row - 2)//row 1 and last should be 0, and for using pointer, last second row should also be 0;
                SUM = 0;
            else {
                /*Convolution for X*/
                for (m = 0; m < 3; m++) {
                    for (n = 0; n < 3; n++) {
                        sumX = sumX + GX[m][n] *(*(*(pix + j + m) + i + n)); //multiply the corresponding units between GX and I
                    }
                }
                /*Convolution for Y*/
                for (m = 0; m < 3; m++) {
                    for (n = 0; n < 3; n++) {
                        sumY = sumY + GY[m][n] * (*(*(pix + j + m) + i + n)); //multiply the corresponding units between GY and I
                    }
                }

                /*Edge strength*/
                SUM = sqrt(pow((double) sumX, 2) + pow((double) sumY, 2)); // Formula to calculate SUM
            }
            if (SUM > 255)SUM = 255; //any sum values above 255 ,let it be 255;
            if (SUM < 0) SUM = 0; //any sum values below zero ,let it be zero;
            *(*(pix + j) + i) = SUM; //Now give the total sum values to pix[j][i];
        }
    }

    //Print the sobel_edge_output

    //print the header
    fprintf(fp3, "%s\n", type);
    fprintf(fp3, "%d %d \n", x, y);
    fprintf(fp3, "%d \n", t);

    //print the body
    for (j = 0; j < row; j++) {
        for (i = 0; i < column; i++) {
            fprintf(fp3, "%d ", *(*(pix + j) + i)); //print the pix[j][i];
        }
        fprintf(fp3, "\n");
    }

    fclose(fp3); //close file "sobel_edge_output.pgm"

    fclose(fp1); //close file for testing

    for (j = 0; j < row; j++) {
        free(pix[j]);
    } // free the pointers array pix[y]

    free(pix); // free the double pointer pix

    return (EXIT_SUCCESS);
This is the fourth time to push.
This time I finish my code writing.
 
