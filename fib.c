/* Program: fib.c
 * Author: Saif Fadhel
 */

#include "bmp.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* struct for directions */
typedef enum {
    dir_none = 0, /* no direction is 0 */
    dir_up, /* up is 1 */
    dir_right, /* right is 2 */
    dir_down, /* down is 3 */
    dir_left  /* left is 4 */
} direction;

/* struct for rotations */
typedef enum { 
    rot_none = 0, /* no rotation is 0 */
    rot_right, /* right is 1*/
    rot_left /* left is 2 */
} rotate;

/* Function used to draw lines in one of four directions */
void draw(int y, int x, direction dir, int step, int w, int h, RGB* im, RGB c)
{
#define I(i, j) im[(i)*w + j]
    int j;
    if (dir == dir_up)
        for (j = 0; j < step; j++) 
            I(y + j, x) = c;
    if (dir == dir_down)
        for (j = 0; j < step; j++) 
            I(y - j, x) = c;
    if (dir == dir_right) 
        for (j = 0; j < step; j++) 
            I(y, x + j) = c;
    if (dir == dir_left)
        for (j = 0; j < step; j++) 
            I(y, x - j) = c;
#undef I 
}

/* Function used to draw fibonacci fractals */
int fib(int n, int x, int y, int step, RGB bc, RGB fc, int w, int h, RGB* image) 
{
    
    int i, j, k, z;
    unsigned long int next_int=0, first_int=0, second_int=1;

    unsigned long int fib_word_array_size=0; 
    unsigned long int previous_fib_word_length;

    direction choose_direction = dir_up;
    rotate choose_rotation = rot_none;

    /* Calculating the size of the fib_word array */
    i=1;
    while(i < n) {
        next_int = first_int + second_int;
        first_int = second_int;
        second_int = next_int;
        i++; 
    }
    i=0;
    fib_word_array_size=next_int;
    
    char* fib_word = malloc(fib_word_array_size*sizeof(char)+1);
    if (fib_word==NULL) return 0;
    
    /* Fib word is initialized with f3 since n>=3 */
    *fib_word='0';
    *(fib_word+1)='1';

    /* f2's word length */
    previous_fib_word_length=1;

    /* Starting at f3, each fibonacci word is computed until last one needed is found */
    if (n>3) 
    {
        for(k=4;k<=n;k++) 
        {
            for(j=0; j<previous_fib_word_length; j++) 
            {
                *(fib_word+strlen(fib_word))=*(fib_word+j);
            }
            previous_fib_word_length=strlen(fib_word)-previous_fib_word_length;
        }
    }

    /* Fills the .bmp image with all white pixels */
    for(z=0;z<w*h;z++) 
        image[z]=bc;

    /* Creates the image corresponding to the nth fibonacci word */
    for(k=1;k<=fib_word_array_size;k++) 
    {
    
        if(choose_rotation==rot_none)
        {
            switch (choose_direction) 
            {
                case 0: break;
                case 1: if (y+step>h) { return 0; } draw(y, x, dir_up, step, w, h, image, fc); y+=step; break;
                case 2: if (x+step>w) { return 0; } draw(y, x, dir_right, step, w, h, image, fc); x+=step; break;
                case 3: if (y-step<0) { return 0; } draw(y, x, dir_down, step, w, h, image, fc); y-=step; break;
                case 4: if (x-step<0) { return 0; } draw(y, x, dir_left, step, w, h, image, fc); x-=step; break;
            }
        }

        if(choose_rotation==rot_right) 
        {
            if(choose_direction==dir_up) {

                choose_direction=dir_right;
                if (x+step>w)
                    return 0; 
                draw(y, x, dir_right, step, w, h, image, fc); 
                x+=step;

            } else if (choose_direction==dir_right) {
                
                choose_direction=dir_down;
                if (y-step<0) 
                    return 0; 
                draw(y, x, dir_down, step, w, h, image, fc); 
                y-=step;

            } else if (choose_direction==dir_down) {

                choose_direction=dir_left;
                if (x-step<0)
                    return 0;
                draw(y, x, dir_left, step, w, h, image, fc); 
                x-=step;

            } else if (choose_direction==dir_left) {

                choose_direction=dir_up;
                if (y+step>h)
                    return 0;
                draw(y, x, dir_up, step, w, h, image, fc); 
                y+=step;

            }   
        }

        if(choose_rotation==rot_left) 
        {
            if(choose_direction==dir_up) {

                choose_direction=dir_left;
                if (x-step<0)
                    return 0;
                draw(y, x, dir_left, step, w, h, image, fc); 
                x-=step;

            } else if (choose_direction==dir_right) {
                
                choose_direction=dir_up;
                if (y+step>h)
                    return 0;
                draw(y, x, dir_up, step, w, h, image, fc);
                y+=step;

            } else if (choose_direction==dir_down) {

                choose_direction=dir_right;
                if (x+step>w) 
                    return 0; 
                draw(y, x, dir_right, step, w, h, image, fc); 
                x+=step;
            
            } else if (choose_direction==dir_left) {

                choose_direction=dir_down;
                if (y-step<0)
                    return 0;
                draw(y, x, dir_down, step, w, h, image, fc); 
                y-=step;

            }   
        }

        /* Checks if digit is 0 to know when to turn */
        if(*(fib_word+(k-1))=='0') {
            /* Checks if k is even or odd to know to turn left or right */
            if (k%2==0) {
                choose_rotation=rot_left;
            } else {
                choose_rotation=rot_right;
            }
        } else {
            choose_rotation=rot_none;
        }
    }

    free(fib_word);
    return fib_word_array_size;
}    