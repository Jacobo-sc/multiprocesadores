#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#define NUM_THREADS 100
int main()
{
    FILE *image, *outputImage, *lecturas;
    image = fopen("sample.bmp","rb");          //Imagen original a transformar
    outputImage = fopen("img2_dd.bmp","wb");   //Imagen transformada
    
    unsigned char r, g, b;               //Pixel
    int c=0;
    for(int i=0; i<54; i++) fputc(fgetc(image), outputImage);   //Copia cabecera a nueva imagen
    
    #pragma omp parallel
    {
    #pragma omp for schedule(guided) 
    for(int i=0;i<927361;i++)
        {                                        //Grises
        b = fgetc(image);
        g = fgetc(image);
        r = fgetc(image);
        unsigned char pixel = 0.21*r+0.72*g+0.07*b;
        fputc(pixel, outputImage);
        fputc(pixel, outputImage);
        fputc(pixel, outputImage);
        }
    }
    
    

    fclose(image);
    fclose(outputImage);
    return 0;
}
