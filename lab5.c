/* 
 * File:   main.c
 * Author: zyx
 *
 * Created on February 17, 2016, 5:28 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * 
 */
int main(int argc, char** argv) {

    int x,y,t;
    
    int i,j;
    
    char type[3]={0};// for header type
    
    int pix[768][1024]={0};// define array for storing the original data
    
    int pix_flipping[768][1024]={0};// define array for storing flipping data
    
    int ** pix_shifting=malloc(sizeof (int**)*768);//malloc the space for the  pointer  pix_shifting  
    
    for (j = 0; j < 768; j++) {
        pix_shifting[j] = (int*) malloc(sizeof (int*)*1024);
        
    }//malloc the space for the  pointers array pix_shifting [row] 
   
    FILE * fp1=fopen("cat.pgm","r");
    
    FILE * fp2=fopen("cat_output.pgm","w");
    
    FILE * fp3=fopen("cat_flipping_output.pgm","w");
    
    FILE * fp4=fopen("cat_shifting_output.pgm","w");
    
    //read header information from "cat.pgm" and print the header information to "cat_output.pgm"
    
    fscanf(fp1,"%s",type);
    fprintf(fp2,"%s\n",type);
    
    fscanf(fp1,"%d %d ", &x, &y);
    fprintf(fp2,"%d %d \n", x, y);
    
    fscanf(fp1,"%d", &t);
    fprintf(fp2,"%d \n",t);
    

  //For the output copy of the original picture
    
    for(j=0;j<y;j++)
    {
     for(i=0;i<x;i++)
     {   
     fscanf(fp1,"%d ",&pix[j][i]);
     fprintf(fp2,"%d ",pix[j][i]);
     }
     fprintf(fp2,"\n");
    }
    fclose(fp2);// close the file pointer to the file "cat_output.pgm"
  
 //For the flipping of the original picture,problem 1;
    
    
    //print the header information to "cat_flipping_output.pgm"
    
    fprintf(fp3,"%s\n",type);
    fprintf(fp3,"%d %d\n", x, y);
    fprintf(fp3,"%d\n",t);
    
    for(j=0;j<y;j++)
    {
     for(i=0;i<x;i++)
     {   
     pix_flipping[j][i]=pix[j][x-i-1];//flipping and store them data into pix_flipping[768][1024];
     
     fprintf(fp3,"%d ",pix_flipping[j][i]);//print the flipping data to "cat_flipping_output.pgm"
     
     }
     fprintf(fp3,"\n");
    }
    
    fclose(fp3);// close the file pointer to the file "cat_flipping_output.pgm"
    
   
//For the shifting of the original picture,problem 2;
    
    //print the header information to "cat_shifting_output.pgm"
    
    fprintf(fp4,"%s\n",type);
    fprintf(fp4,"%d %d \n", x, y);
    fprintf(fp4,"%d \n",t);
    
    for(i=0;i<300;i++)
      for(j=0;j<y;j++)
        *(*(pix_shifting+j)+i)=0;// set the first 300 column all to zero
    
    for(j=0;j<y;j++)//row counter
    {
     for(i=0;i<x-300;i++)//column counter
     {   
     *(*(pix_shifting + j) + i+300)=pix[j][i];// shifting all the values 300 columns right   
     }
    }
   
    for(j=0;j<y;j++)
    {
     for(i=0;i<x;i++)
     {   
     fprintf(fp4,"%d ",*(*(pix_shifting + j) + i)); //print the shifting data to "cat_shifting_output.pgm"
     }
    
     fprintf(fp4,"\n");
  }
         
    
    
    fclose(fp4);// close the file pointer to the file "cat_shifting_output.pgm"
    fclose(fp1);// close the file pointer to the file "cat.pgm"


    for (j = 0; j < 768; j++) {
        free(pix_shifting[j]);
    }//free  the array pointers pix_shifting[row]
    
    
    free(pix_shifting);//free  the double pointer pix_shifting
    
    return (EXIT_SUCCESS);
}

 
 
 
