#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <opencv2/opencv.hpp>
#include <time.h>

using namespace cv;

//#define NUM_THREADS 16

Mat src_img;
Mat res_img;

void* sobel_filter(void* arg);

int main(int argc, char* argv[]) {
    if (argc != 2) exit(EXIT_FAILURE);

    int NUM_THREADS;
    std::cin >> NUM_THREADS;
    int res;
    pthread_t thread[NUM_THREADS];
    void* thread_result;

    src_img = imread(argv[1]);
    cvtColor(src_img, res_img, COLOR_BGR2YUV);

    clock_t start = clock();

    for (int i = 0; i < NUM_THREADS; i++) {
        res = pthread_create(&(thread[i]), NULL, sobel_filter, (void*)i);
        if (res != 0) {
            perror("Thread creation failed\n");
            exit(EXIT_FAILURE);
        }
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        res = pthread_join(thread[i], &thread_result);
        if (res != 0) {
            perror("pthread_join failed");
            exit(EXIT_FAILURE);
        }
    }

    clock_t end = clock();

    imwrite("res.jpeg", res_img);
    std::cout << "All done. Sobel filter func has run in " << ((double) end - start) / ((double) CLOCKS_PER_SEC) << " second(s)" << std::endl;
    exit(EXIT_SUCCESS);
}

void* sobel_filter(void* arg) {
    long long thread_id = (long long)arg;
    printf("Thread func is running. Argument was %lld\n", thread_id);

    /*
    Mask:
    p11 p12 p13
    p21 p22 p23
    p31 p32 p33
    */
    uchar p11, p12, p13, \
          p21, p22, p23, \
          p31, p32, p33;

    uint Gx, Gy, G;

    for (long long i = thread_id; i < src_img.rows; i += 1) {
	    for (long long j = 0; j < src_img.cols; j++) {
            p11 = (i > 0 && j > 0)                               ? src_img.at<Vec3b>( i - 1, j - 1 )[1] : 0;
            p12 = (i > 0)                                        ? src_img.at<Vec3b>( i - 1, j     )[1] : 0;
            p13 = (i > 0 && j < src_img.cols - 1)                ? src_img.at<Vec3b>( i - 1, j + 1 )[1] : 0;
            p21 = (j > 0)                                        ? src_img.at<Vec3b>( i    , j - 1 )[1] : 0;
            p22 =                                                  src_img.at<Vec3b>( i    , j     )[1];
            p23 = (j < src_img.cols - 1)                         ? src_img.at<Vec3b>( i    , j + 1 )[1] : 0;
            p31 = (i < src_img.rows - 1 && j > 0)                ? src_img.at<Vec3b>( i + 1, j - 1 )[1] : 0;
            p32 = (i < src_img.rows - 1)                         ? src_img.at<Vec3b>( i + 1, j     )[1] : 0;
            p33 = (i < src_img.rows - 1 && j < src_img.cols - 1) ? src_img.at<Vec3b>( i + 1, j + 1 )[1] : 0;

            Gx = p11 + 2 * p12 + p13 - p31 - 2 * p32 - p31;
            Gy = p13 + 2 * p23 + p33 - p11 - 2 * p21 - p31;

            G = sqrt(Gx * Gx + Gy * Gy);
            if (G > 255) G = 255;

			res_img.at<Vec3b>(i, j)[0] = G;
			res_img.at<Vec3b>(i, j)[1] = G;
			res_img.at<Vec3b>(i, j)[2] = G;
		}
    }

    pthread_exit(NULL);
}
