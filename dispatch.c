#include "utils.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#include <unistd.h>
#include <pthread.h>
// Compilation : gcc -Wall -std=c99 exemple.c utils.c
// Ceci est un exemple d'utilisation de util.c

struct processus {
    int pid;
    int arr;
    int unit[8];
};

struct file {
    int quantum;
    int use[8];
};

struct file f[3];

struct processus p[4];

int compare (const void * a, const void * b) {
   struct processus *p1 = (struct processus * )a;
   struct processus *p2 = (struct processus * )b;
   if(p1->arr == p2->arr) {
       return p1->pid = p2->pid;
   }
   return p1->arr - p2->arr;
}

int gestionEntree(char const *argv[]) {

    int c, i = 0, j = 0, e = 0, nombre = 0, neg = 0;
    const char *fileName = "test2.txt";

    FILE *file;
    file = fopen(fileName, "r");/*argv[3]*/
    if (file) {

        while ((c = getc(file)) != EOF) {

            if (c == '\n') {
                p[i].unit[j-2] = (neg) ? -nombre : nombre;
                i++;
                j = nombre = e = neg = 0;

            } else if (c == ' ') {

                if(j == 0) {
                    p[i].pid = nombre;
                } else if (j == 0) {
                    p[i].arr = nombre;
                } else {
                    p[i].unit[j-2] = (neg) ? -nombre : nombre;
                }
                nombre = e = neg = 0;
                j++;

            } else if (isdigit((int)c)) {
                nombre = (nombre*(10*e))+((int)c-48);
                //printf("%d ",nombre);
                e++;

            } else if (c == '-') {
                neg = 1;
            }
            //printf("Unit: %d\n",p[i].unit[j-2]);
        }
        p[i].unit[j-2] = (neg) ? -nombre : nombre;
        fclose(file);
    }
    if(i > 0) {
        qsort(p,2,sizeof(struct processus),compare);
    }

    return i;
}

void creationQueue() {
}

int main (int argc, char const *argv[]) {
    if(1/*argc == 4*/) {
        int q1 = 45;
        f[0].quantum = 5;/*atoi(argv[1])*/
        f[1].quantum = 10;/*atoi(argv[2])*/
        int sizeP = gestionEntree(argv);
        printf("%d\n",p[0].pid);

        for(int i = 0 ; i < sizeP ; i++){
            printf("PID : %d \t|\t ", p[i].pid);
            printf("ARR : %d \t|\t Info :", p[i].arr);

            int sizeU = sizeof(p[0].unit)/sizeof(int);
            for(int j = 0 ; j < sizeU ; j++) {
                if(p[i].unit[j] != 0) {
                    printf(" %d",p[i].unit[j]);
                }
            }
            printf("\n");
        }

        creationQueue();



        //Vérification de l'entrée
        if(0) {
            print_erreur(1);
        } else if(0) {
            print_erreur(2);
        } else if (0){
            print_erreur(3);
        } else {
            /*printf("\n");
            int sizeP = sizeof(p) / sizeof(struct processus);

            for (int i = 0 ; i < sizeP ; i++) {
                int sizeU = sizeof(p[i].unit) / sizeof(int);
                for(int j = 0 ; j < sizeU ; j++) {
                    print_element(p[i].pid,p[i].arr,p[i].unit[j]);
                }
            }*/
        }
        //free(p);
        return 0;
    } else {
        print_erreur(3);
        return 1;
    }
}
