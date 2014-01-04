#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include "LUdecomp.h"

LUdecomp *LUdecompose(int N, const double **A){

  //********************************************
  //START OF CODE TO INITIALIZE THE STRUCT
  LUdecomp *toReturn =(LUdecomp*) malloc(sizeof(LUdecomp));

  toReturn->LU = (double**) malloc(N*sizeof(double*));   
  for (int i = 0; i < N; i++){
    toReturn->LU[i] = (double*) malloc(N*sizeof(double));
  }
 
  for (int i = 0; i < N; i++){
    for (int j = 0; j < N; j++){
    memcpy(&toReturn->LU[i][j], &A[i][j], sizeof(double)); 
    } 
 }  

 
 int n = 0; //p = initial pivot value; n = initial pivot row
 double p = 0.0;
 toReturn->mutate = (short*) malloc(N*sizeof(short)); 

 for (short i = 0; i < N; i++){ //initialize the mutate array
   toReturn->mutate[i] = i;
 }
 
 toReturn->N = N;
 toReturn->d = 1;
 
//END CODE OF INITIALIZING THE STRUCT
//***********************************************************
//START OF CONVERTING PDF PSEUDOCODE TO C

 for (int j = 0; j < N; j++){
   for (int i = 0; i <= j; i++){
     double sum = 0.0;
     for (int k = 0; k < i; k++){
       sum += (toReturn->LU[i][k] * toReturn->LU[k][j]);
     }
     toReturn->LU[i][j] = (toReturn->LU[i][j] - sum);
   }
   p = fabs(toReturn->LU[j][j]);
   n = j;
   for (int i = j + 1; i < N; i++){
     double sum = 0.0;
     for (int k = 0; k < j; k++){
       sum += toReturn->LU[i][k] * toReturn->LU[k][j];
     }
     toReturn->LU[i][j] = toReturn->LU[i][j] - sum;
     if (fabs(toReturn->LU[i][j]) > p){
       p = fabs(toReturn->LU[i][j]);
       n = i;
     }
   }
     if (p == 0.0){return NULL;}
     if (n != j){
    
       short mutswap = 0;
       
       double* temp = toReturn->LU[n];
	 toReturn->LU[n] = toReturn->LU[j];
	 toReturn->LU[j] = temp;
       
       mutswap = toReturn->mutate[n];
       toReturn->mutate[n] = toReturn->mutate[j];
       toReturn->mutate[j] = mutswap;
       toReturn->d = -1*toReturn->d;
     }
     for (int i = j+1; i < N; i++){
       toReturn->LU[i][j] = toReturn->LU[i][j]/toReturn->LU[j][j];
     }
   }
 
 

return toReturn;
}


void LUdestroy(LUdecomp* toReturn){
    for (int i = 0; i < toReturn->N; i++){       
      free(toReturn->LU[i]);
      
    }
    free(toReturn->LU);
    free(toReturn->mutate);   
    free(toReturn); 
    return;
}

    
void LUsolve(LUdecomp *decomp, const double *b, double *x){
  
  double y[decomp->N]; //initialize the y vector were going to use for the forward substitution
  


  //start of forward-solving part
  y[0] = b[decomp->mutate[0]];
  for (int i = 1; i < decomp->N; i++){
    double sum = 0.0;
    for (int j = 0; j < i; j++){
      sum += decomp->LU[i][j]*y[j];
    }
    y[i] = b[decomp->mutate[i]] - sum;
  }

  //start of back-solving part
  x[decomp->N-1] = y[decomp->N-1]/decomp->LU[decomp->N-1][decomp->N-1];

    for (int i = decomp->N-2; i>=0; i--){
      double sum = 0.0;
      for (int j = i + 1;  j < decomp->N; j++){
	sum += decomp->LU[i][j]*x[j];
      }
      x[i] = (1/decomp->LU[i][i])*(y[i] - sum);
    }
  

  return; 
}


