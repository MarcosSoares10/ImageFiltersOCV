#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <string>
#include <algorithm>
#include <math.h>
#include "QueueElement.h"
using namespace std;
using namespace cv;


void mediana3x3(Mat image)
{
    int aux[9];
    int c = 0;
    for (int i = 1; i < image.size().height - 1; i++)
    {
        for (int j = 1; j < image.size().width -1; j++)
        {
            c=0;
            for (int x = i - 1; x <= i + 1; x ++)
            {
                for(int y = j - 1; y <= j + 1; y++)
                {
                    aux[c] = image.at<unsigned char>(x,y);
                    c+=1;

                }
            }
            sort(aux, aux+9);
            image.at<unsigned char>(i,j) = aux[(int)9/2];
        }
    }

}

void average3x3(Mat image)
{
    int aux[9];
    int c = 0;
    for (int i = 1; i < image.size().height - 1; i++)
    {
        for (int j = 1; j < image.size().width -1; j++)
        {
            c=0;
            for (int x = i - 1; x <= i + 1; x ++)
            {
                for(int y = j - 1; y <= j + 1; y++)
                {
                    aux[c] = image.at<unsigned char>(x,y);
                    c+=1;

                }
            }

            image.at<unsigned char>(i,j) = (int)((aux[0] + aux[1] + aux[2] +
                                                  aux[3] + aux[4] + aux[5] +
                                                  aux[6] + aux[7] + aux[8])/9);
        }
    }

}

void gaussian3x3(Mat image)
{
    int temp[3][3]= {1,2,1,
                     2,4,2,
                     1,2,1
                    };

    int somaDosPixels = 0;
    for (int i = 1; i < image.size().height - 1; i++ )
    {
        for (int j = 1; j < image.size().width - 1; j++)
        {
            somaDosPixels = (image.at<unsigned char>(i-1,j-1)* temp[0][0])+
                            (image.at<unsigned char>(i-1,j)* temp[0][1])+
                            (image.at<unsigned char>(i-1,j+1)* temp[0][2])+
                            (image.at<unsigned char>(i,j-1)* temp[1][0])+
                            (image.at<unsigned char>(i,j)* temp[1][1])+
                            (image.at<unsigned char>(i,j+1)* temp[1][2])+
                            (image.at<unsigned char>(i+1,j-1)* temp[2][0])+
                            (image.at<unsigned char>(i+1,j)* temp[2][1])+
                            (image.at<unsigned char>(i+1,j+1)* temp[2][2]);

            image.at<unsigned char>(i,j) = (int)somaDosPixels/22;
            somaDosPixels = 0;
        }
    }
}

void gaussianrgb3x3(Mat image)
{
    int temp[3][3]= {1,2,1,
                     2,4,2,
                     1,2,1
                    };

    int somaDosPixels = 0;
    for(int c = 0; c<image.channels(); c++)
    {
        for (int i = 1; i < image.size().height - 1; i++ )
        {
            for (int j = 1; j < image.size().width - 1; j++)
            {
                somaDosPixels = (image.at<unsigned char>(i-1,j-1,c)* temp[0][0])+
                                (image.at<unsigned char>(i-1,j,c)* temp[0][1])+
                                (image.at<unsigned char>(i-1,j+1,c)* temp[0][2])+
                                (image.at<unsigned char>(i,j-1,c)* temp[1][0])+
                                (image.at<unsigned char>(i,j,c)* temp[1][1])+
                                (image.at<unsigned char>(i,j+1,c)* temp[1][2])+
                                (image.at<unsigned char>(i+1,j-1,c)* temp[2][0])+
                                (image.at<unsigned char>(i+1,j,c)* temp[2][1])+
                                (image.at<unsigned char>(i+1,j+1,c)* temp[2][2]);

                image.at<unsigned char>(i,j,c) = (int)somaDosPixels/22;
                somaDosPixels = 0;
            }
        }
    }
}



void sobel3x3(Mat image)
{
    int tempimage[image.size().height][image.size().width];

    int tempx[3][3] = { { 1,   0,  -1},
        { 2,   0,  -2},
        { 1,   0,  -1}
    };

    int tempy[3][3] = { { 1,   2,   1},
        { 0,   0,   0},
        {-1,  -2,  -1}
    };



    ///////// OPERADOR PREWIT
    /*  int tempx[3][3] = { { 1,   0,  -1},
                          { 1,   0,  -1},
                          { 1,   0,  -1}};

      int tempy[3][3] = { { 1,   1,   1},
                          { 0,   0,   0}
                          {-1,  -1,  -1}};*/

    int somax=0,somay=0;

    for (int i = 1; i < image.size().height - 1; i++)
    {
        for (int j = 1; j < image.size().width -1; j++)
        {
            somax=0;
            somay=0;
            for (int x = -1; x <= 1; x++)
            {
                for (int y = -1; y <= 1; y++)
                {
                    somax= somax + image.at<unsigned char>((i+x),(j+y))*tempx[1+x][1+y];
                    somay = somay + image.at<unsigned char>((i+x),(j+y))*tempy[1+x][1+y];

                }
            }
            tempimage[i][j] = (int) (sqrt(pow(somax,2) + pow(somay,2)) );
            if (tempimage[i][j] < 0)
                tempimage[i][j] = 0;
            else if (tempimage[i][j] > 255)
                tempimage[i][j] = 255;

        }


    }
    for (int x = 1; x < image.size().height - 1; x++)
    {
        for (int z = 1; z < image.size().width -1; z++)
        {
            image.at<unsigned char>(x,z) = tempimage[x][z];
        }
    }
}




int main2()
{
    Mat imgsrc = imread("00.jpg");
    Mat gray;
    cvtColor(imgsrc, gray, cv::COLOR_BGR2GRAY);

    // Mat filteredMediana = gray.clone();
    //  Mat filteredGaussian = gray.clone();
    // Mat filteredAverage = gray.clone();


    // mediana3x3(filteredMediana);
    //  gaussian3x3(filteredGaussian);
    //  average3x3(filteredAverage);




    // Mat sobel = filteredMediana.clone();
    // sobel3x3(sobel);
    //    Mat teste = gray.clone();
    //  TESTE(teste);
    // imshow ("Imagem ", teste);

    //  imshow ("Imagem original", imgsrc);

    // imshow ("Deteccao de bordas com operador de sobel", sobel);
    //imshow ("Imagem Filtrada - Median Filter", filteredMediana);
    //imshow ("Imagem Filtrada - Gaussian Filter", filteredGaussian);
    //imshow ("Imagem Filtrada - Average Filter", filteredAverage);

    cvWaitKey();
    return 0;
}






