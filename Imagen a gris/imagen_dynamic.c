#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#define NUM_THREADS 50
int main()
{
    FILE *image, *outputImage, *lecturas;
    
    unsigned char*img = (unsigned char*) malloc(2782081*sizeof(unsigned char));
    unsigned char*oimg = (unsigned char*) malloc(2782081*sizeof(unsigned char));
    image = fopen("sample.bmp","rb");
    outputImage= fopen("img2_dd.bmp","wb");          //Imagen original a transformar
    
    long ancho;
    long alto;

    unsigned char xx[54];
    unsigned char r, g, b;  
    for(int i=0; i<54; i++) {
        xx[i] = fgetc(image);
        fputc(xx[i], outputImage);   //Copia cabecera a nueva imagen
    }
    ancho = (long)xx[21]*16777216+(long)xx[20]*65536+(long)xx[19]*256+(long)xx[18];
    alto = (long)xx[25]*16777216+(long)xx[24]*65536+(long)xx[23]*256+(long)xx[22];
    printf("largo img %li\n",alto);
    printf("ancho img %li\n",ancho);
    int i = 0;
    while(!feof(image)){
        *(img+i) = fgetc(image);
        i++;
    }
    
    int count = 0;
    for(int i=0;i<2782081;i+=3){
        
        b = *(img+i);
        g = *(img+i+1);
        r = *(img+i+2);
        unsigned char pixel = 0.21*r+0.72*g+0.07*b;
        *(oimg+i) = pixel;
        *(oimg+i+1) = pixel;
        *(oimg+i+2) = pixel;
        count+=3;
        if(count == ancho*3){
            count=0;
            i+=2;
        }
    }


    for(int i=0;i<2782080;i++){
        fputc(*(oimg+i), outputImage);
    }
    

    fclose(image);
    fclose(outputImage);
    return 0;
}
