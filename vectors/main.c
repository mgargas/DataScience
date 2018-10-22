#include <stdio.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_vector_double.h>
#include <time.h>
#include<stdio.h>
#include<string.h>
#include <time.h>
#include <stdlib.h>
#include <stdint-gcc.h>
#include <gsl/gsl_matrix.h>


void create_marks_csv(char *filename,double a[][5],int n,int m){

    printf("\n Creating %s.csv file",filename);

    FILE *fp;

    int i,j;

    filename=strcat(filename,".csv");

    fp=fopen(filename,"w+");

    fprintf(fp,"id, vector_size, dgemm_result, dgemm_time, ddot_result, ddot_time");

    for(i=0;i<m;i++){

        fprintf(fp,"\n%d",i+1);

        for(j=0;j<n;j++)

            fprintf(fp,",%g ",a[i][j]);

    }

    fclose(fp);

    printf("\n %sfile created",filename);

}


double randfrom(double min, double max)
{
    double range = (max - min);
    double div = RAND_MAX / range;
    return min + (rand() / div);
}

int main (void)
{
    srand(time(NULL));   // Initialization, should only be called once.
    int random;
    double csv_results[10][5];
    int size = 1;

    for(int i=0; i<10; i++) {

        double* a = malloc(size * sizeof(double));
        double* b = malloc(size * sizeof(double));
        for(int j=0; j<size; j++){
            a[j] = randfrom(-100,100);
            b[j] = randfrom(-100,100);
        }

        double dgemm_result[] = {0.00};

        double ddot_result = 0;

        //create matrixes
        gsl_matrix_view* A = malloc(sizeof(gsl_matrix_view_array(a, 1, size)));

        *A = gsl_matrix_view_array(a, 1, size);


        gsl_matrix_view* B = malloc(sizeof(gsl_matrix_view_array(b, size, 1)));
        *B = gsl_matrix_view_array(b, size, 1);


        gsl_matrix_view* C = malloc(sizeof(gsl_matrix_view_array(dgemm_result, 1, 1)));
        *C = gsl_matrix_view_array(dgemm_result, 1, 1);


        //create vectors
        gsl_vector_view* A1 = malloc(sizeof(gsl_vector_view_array(a, size)));
        *A1 = gsl_vector_view_array(a, size);

        gsl_vector_view* B1 = malloc(sizeof(gsl_vector_view_array(b, size)));
        *B1 = gsl_vector_view_array(b, size);

        /* Compute C = A B  as matrixes*/
        clock_t begin = clock();
        gsl_blas_dgemm(CblasNoTrans, CblasNoTrans,
                       1.0, &A->matrix, &B->matrix,
                       0.0, &C->matrix);
        clock_t end = clock();
        double time_spent_dgemm = (double) (end - begin) / CLOCKS_PER_SEC;


        /* Compute C = A B as vectors*/
        begin = clock();
        gsl_blas_ddot(&A1->vector, &B1->vector, &ddot_result);
        end = clock();
        double time_spent_ddot = (double) (end - begin) / CLOCKS_PER_SEC;


        csv_results[i][0] = size;
        csv_results[i][1] = ddot_result;
        csv_results[i][2] = time_spent_ddot*1000000;
        csv_results[i][3] = dgemm_result[0];
        csv_results[i][4] = time_spent_dgemm*1000000;

        //random = rand()%10^i;
        size = size * 8;


        free(a);
        free(b);
        free(A);
        free(B);
        free(C);
        free(A1);
        free(B1);
    }

    // export results to csv
    //double csv_tab[5] = {size, ddot_result, time_spent_ddot, dgemm_result[0], time_spent_dgemm};

    char str[100];

    printf("\n Enter the filename :");

    gets(str);

    create_marks_csv(str,csv_results,5,10);

    return 0;
}