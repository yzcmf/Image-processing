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
 * 
 */

int main(int argc, char** argv) {

    int x, y, t;

    char type[3] = {0};

    int i, j;

    FILE * fp1 = fopen("testing2.pgm", "r");//You can choose to use either testing 1 or testing 2,they are different size

    FILE * fp2 = fopen("testing_output.pgm", "w");

    FILE * fp3 = fopen("testing_sobel_edge_output.pgm", "w");
    
    FILE * fp4 = fopen("testing_sobel_edge_flipping_output.pgm", "w");
  
    FILE * fp5 = fopen("testing_sobel_edge_rotatingClockwise_output.pgm", "w");
    
    FILE * fp6 = fopen("testing_sobel_edge_rotatingCounterClockwise_output.pgm", "w");

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
    } // pix[][] memory 1 time


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
              
                // pix[][] memory 9 times
                
                /*Convolution for Y*/
                for (m = 0; m < 3; m++) {
                    for (n = 0; n < 3; n++) {
                        sumY = sumY + GY[m][n] * (*(*(pix + j + m) + i + n)); //multiply the corresponding units between GY and I
                    }
                }
               // pix[][] memory 9 times
                
                /*Edge strength*/
                SUM = sqrt(pow((double) sumX, 2) + pow((double) sumY, 2)); // Formula to calculate SUM
            }
            if (SUM > 255)SUM = 255; //any sum values above 255 ,let it be 255;
            if (SUM < 0) SUM = 0; //any sum values below zero ,let it be zero;
            *(*(pix + j) + i) = SUM; //Now give the total sum values to pix[j][i];
            
            // pix[][] memory 1 time
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
    // pix[][] memory 1 time
    
    //For the flipping part
    
     //print the header
    fprintf(fp4, "%s\n", type);
    fprintf(fp4, "%d %d \n", x, y);
    fprintf(fp4, "%d \n", t);
    
    //print the body
    for (j = 0; j < row; j++) {
        for (i = 0; i < column; i++) {
            fprintf(fp4, "%d ", *(*(pix + row-j-1) + i)); //print the pix[j][i]--flipping up/down;
        }
        fprintf(fp4, "\n");
    }
    
    // pix[][] memory 1 time
    
    fclose(fp4); //close file "testing_sobel_edge_flipping_output.pgm"

    //For rotating part
    
    int **pix_rotating; //define the double pointer pix_rotating


    pix_rotating = (int**) malloc(sizeof (int**)*column); //malloc the space for the double pointer pix_rotating and just use the column number we detected

    for (j = 0; j < column; j++) {
        pix_rotating[j] = (int*) malloc(sizeof (int*)*row);
    }//malloc the space for the  pointers array pix_rotating[column] and just use the column number we detected
    
    //For the rotatingClockwise part
    
    //print the header
    
    fprintf(fp5, "%s\n", type);
    fprintf(fp5, "%d %d \n", y , x);
    fprintf(fp5, "%d \n", t);
    
    //print the body
    
    for (j = 0; j < row; j++) {
        
        for (i = 0; i < column; i++) {
            
            *(*(pix_rotating + i) + row-1-j)=*(*(pix + j) + i);//for clockwise
        }
    }
    
    // pix[][] memory 1 time and pix_rotating[][] memory 1 time
    
    for (j = 0; j < column; j++) {
        
        for (i = 0; i < row; i++) {          
           
            fprintf(fp5, "%d ", *(*(pix_rotating + j) + i)); 
        }
        fprintf(fp5, "\n");
    }
    
    // pix_rotating[][] memory 1 time
    
    //pix_clockwise[i][x-1-j]=pix[j][i];//clockwise; 
    
    fclose(fp5); //close file "testing_sobel_edge_rotatingClockwise_output.pgm"
    
    
    //For the rotatingCounterClockwise part
    
    //print the header
    
    fprintf(fp6, "%s\n", type);
    fprintf(fp6, "%d %d \n", y, x);
    fprintf(fp6, "%d \n", t);
    
    //print the body
    
    for (j = 0; j < row; j++) {
        
        for (i = 0; i < column; i++) {
            
            *(*(pix_rotating + column-1-i) + j)=*(*(pix + j) + i);//for counterclockwise
        }
    }
    
    // pix[][] memory 1 time and pix_rotating[][] memory 1 time
    
    for (j = 0; j < column; j++) {
        
        for (i = 0; i < row; i++) {          
           
            fprintf(fp6, "%d ", *(*(pix_rotating + j) + i)); 
        }
        fprintf(fp6, "\n");
    }
    
    // pix_rotating[][] memory 1 time
    
    //pix_clockwise[y-1-i][j]=pix[j][i];//counterclockwise; 
       
    fclose(fp6); //close file "testing_sobel_edge_rotatingCounterClockwise_output.pgm"
    
    fclose(fp3); //close file "testing_sobel_edge_output.pgm"

    fclose(fp1); //close file for testing
    

    for (j = 0; j < row; j++) {
        free(pix[j]);
    } //free the pointers array pix[y]

    free(pix); //free the double pointer pix
    
    for (j = 0; j < column; j++) {
        free(pix_rotating[j]);
    } //free the pointers array pix_rotating[y]

    free(pix_rotating); //free the double pointer pix_rotating

    
    return (EXIT_SUCCESS);
}



/*  Memory calculation : Base on the malloc function for the image operations mainly  
  
 1. The pix[][] array : and take the testing image for example: row= 512, column=384;
     
    The memory for pix[][] is :row  * column * 4 (int type)= 512*384*4(bytes)=786432(bytes);
    
  2. The pix_rotating[][] array(for counterclockwise and clockwise):
 
     The memory for pix[][] is :column  * row * 4 (int type)= 384*512*4(bytes)=786432(bytes);
  
 3.  Based on my comments on the program: there are 24 times pix[][] and 4 times pix_rotating[][]
 
 4. The total memory for my program is : 786432 * 28(bytes) = 22020096(bytes)=21MB;
 
*/homework2.c
homework2.c
 
 
