#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#define NUM_THREADS 14

void filtro_bn(FILE *image,FILE *outputImage);
void filtro_bn1(int lim_inf, int lim_sup, long ancho, long alto, unsigned char *arr_in,  unsigned char *arr_out, unsigned char *arr_filter);
int main()
{
    omp_set_num_threads(NUM_THREADS);
    FILE 
    *image1, *image2,*image3,*image4,*image5,*image6,*image7,*image8,*image9,*image10,
    *outputImage1,*outputImage2,*outputImage3,*outputImage4,*outputImage5,*outputImage6,*outputImage7,*outputImage8,*outputImage9,*outputImage10, 
    *lecturas;
    image1 = fopen("f1.bmp","rb"); 
    image2 = fopen("f2.bmp","rb"); 
    image3 = fopen("f3.bmp","rb"); 
    image4 = fopen("f4.bmp","rb"); 
    image5 = fopen("f5.bmp","rb"); 
    image6 = fopen("f6.bmp","rb"); 
    image7 = fopen("f7.bmp","rb"); 
    image8 = fopen("f8.bmp","rb");
    image9 = fopen("f9.bmp","rb");
    image10 = fopen("f10.bmp","rb");            //Imagen original a transformar
    outputImage1 = fopen("f1_out.bmp","wb");    //Imagen transformada
    outputImage2 = fopen("f2_out.bmp","wb"); 
    outputImage3 = fopen("f3_out.bmp","wb"); 
    outputImage4 = fopen("f4_out.bmp","wb"); 
    outputImage5 = fopen("f5_out.bmp","wb"); 
    outputImage6 = fopen("f6_out.bmp","wb"); 
    outputImage7 = fopen("f7_out.bmp","wb"); 
    outputImage8 = fopen("f8_out.bmp","wb"); 
    outputImage9 = fopen("f9_out.bmp","wb"); 
    outputImage10 = fopen("f10_out.bmp","wb"); 
    double t1,t2,tiempo;
    t1= omp_get_wtime();
    #pragma omp parallel num_threads(10) 
    {
    #pragma omp sections 
    {
        #pragma omp section
        filtro_bn(image1,outputImage1);
        #pragma omp section
        filtro_bn(image2,outputImage2);
        #pragma omp section
        filtro_bn(image3,outputImage3);
        #pragma omp section
        filtro_bn(image4,outputImage4);
        #pragma omp section
        filtro_bn(image5,outputImage5);
        #pragma omp section
        filtro_bn(image6,outputImage6);
        #pragma omp section
        filtro_bn(image7,outputImage7);
        #pragma omp section
        filtro_bn(image8,outputImage8);
        #pragma omp section
        filtro_bn(image9,outputImage9);
        #pragma omp section
        filtro_bn(image10,outputImage10);
    }
  }    
            
    t2=omp_get_wtime();
    tiempo = t2-t1;
        printf("tomo =(%lf) segundos\n",tiempo);
    return 0;
}

        

void filtro_bn(FILE *image,FILE *outputImage){
        long ancho;
        long alto;
        int i,j;
        int k,l;
        unsigned char r, g, b;               //Pixel
        unsigned char xx[54];
        for(int i=0; i<54; i++) {
            xx[i] = fgetc(image);
        fputc(xx[i], outputImage);   //Copia cabecera a nueva imagen
        }
        ancho = (long)xx[20]*65536+(long)xx[19]*256+(long)xx[18];
        alto = (long)xx[24]*65536+(long)xx[23]*256+(long)xx[22];
        int seccion_alto= alto/4;
        int n=0;
        long aux_ancho = ancho*3;
        if(aux_ancho%4!=0){
            aux_ancho = aux_ancho/4;
            aux_ancho = ((int)aux_ancho)*4+4;
            n = aux_ancho-ancho*3;
        }
        printf("largo img %li\n",alto);
        printf("ancho img %li\n",ancho);
        unsigned char* arr_in = (unsigned char*)malloc((ancho+n)*alto*3*sizeof(unsigned char));
        unsigned char* arr_out = (unsigned char*)malloc((ancho+n)*alto*3*sizeof(unsigned char));
        unsigned char* arr_filter = (unsigned char*)malloc((ancho+n)*alto*3*sizeof(unsigned char));
        while(!feof(image)){
            *(arr_in + j) = fgetc(image);
            *(arr_out + j) = *(arr_in + j);
            *(arr_filter + j) = *(arr_out + j);
            j++;
        }
        printf("%d\n", j);
        #pragma omp parallel num_threads(4) 
        {
        #pragma omp sections 
        {
            #pragma omp section 
                filtro_bn1(0, seccion_alto,ancho,alto,arr_in, arr_out,arr_filter);
            #pragma omp section
                filtro_bn1(seccion_alto, 2*seccion_alto,ancho,alto,arr_in,arr_out,arr_filter);
            #pragma omp section
                filtro_bn1(2*seccion_alto, 3*seccion_alto,ancho,alto,arr_in,arr_out,arr_filter);
            #pragma omp section
                filtro_bn1(3*seccion_alto, alto,ancho,alto,arr_in,arr_out,arr_filter);
            
        }
        }
        for(i = 0; i < (ancho+n)*alto*3; i++){
            fputc(*(arr_filter + i), outputImage);
        }
        free(arr_filter);
        free(arr_in);
        free(arr_out);
        fclose(image);
        fclose(outputImage);
        }


void filtro_bn1(int lim_inf, int lim_sup, long ancho, long alto, unsigned char *arr_in,  unsigned char *arr_out, unsigned char *arr_filter){
    long aux_ancho = ancho*3;
    int n=0;
    if(aux_ancho%4!=0){
        aux_ancho = aux_ancho/4;
        aux_ancho = ((int)aux_ancho)*4+4;
        n = aux_ancho-ancho*3;
    }
    unsigned char r, g, b;  
    int i;
    int k,l;
    int j= 0;
    int count = 0;

    for(i = (ancho+n)*lim_inf*3; i < (ancho+n)*lim_sup*3; i+=3){
            b = *(arr_in + i);
            g = *(arr_in + i + 1);
            r = *(arr_in + i + 2);
            unsigned char pixel = 0.21*r+0.72*g+0.07*b;
            *(arr_in + i) = pixel;
            *(arr_in + i + 1) = pixel;
            *(arr_in + i + 2) = pixel;
            count += 3;
            if(count == ancho*3){
                i+=n;
                count = 0;
            }
    }
//---------------------------------------------------------------------------------------------------------------------------------------------

    for(i = lim_inf; i < lim_sup; i++){
            for(j = 0; j < (ancho*3); j+= 3){
                *(arr_out + (i*((ancho*3)+n))+j) = *(arr_in + (i*((ancho*3)+n))+ (ancho*3) - j);
                *(arr_out + (i*((ancho*3)+n))+j + 1) = *(arr_in + (i*((ancho*3)+n))+ (ancho*3 )- j - 1);
                *(arr_out + (i*((ancho*3)+n))+j + 2) = *(arr_in + (i*((ancho*3)+n))+ (ancho*3) - j - 2);
            }
        }
        

    // -------------------------------------------------------------------------------------------------------------------------------------------    
    unsigned char aux = 0;
    int lim=9;
    count=0;
    //printf("%d",lim/2);
    for(i = lim_inf; i < lim_sup; i++){
        for(j = 0; j< (ancho*3); j+= 3){
            aux=0.0;
            for(k=-(lim/2);k<=lim/2;k++){
                for(l=-(lim*3/2);l<=lim*3/2;l+=3){
                    aux += *(arr_out + ((i+k+1)*((ancho*3)+n))+j+l+1)/(lim*lim);
                }
            }
            *(arr_filter + (i*((ancho*3)+n))+j) = aux;
            *(arr_filter + (i*((ancho*3)+n))+j + 1) = aux;
            *(arr_filter + (i*((ancho*3)+n))+j + 2) = aux;
        }
    }
}

