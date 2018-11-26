#include "utils.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#define FILE_MAX = 16;
// Compilation : gcc -Wall -std=c99 exemple.c utils.c
// Ceci est un exemple d'utilisation de util.c
typedef struct processus {
    int pid;
    int tpsArrive;
    int tpsCPU[16];
    int tpsTotal;
} Processus;

typedef struct file {
    int quantum;
    Processus *ordreProcessus[16];
    int nbPro;
    int num;
} File;


File f[3];

int compare (const void * a, const void * b) {
    Processus *p1 = (Processus * )a;
    Processus *p2 = (Processus * )b;
    if(p1->tpsArrive == p2->tpsArrive) {
        return p1->pid - p2->pid;
    }
    return p1->tpsArrive - p2->tpsArrive;
}

void afficher(Processus *p, int nbPro) {

    for(int i = 0 ; i < nbPro ; i++){
        printf("PID : %d \t|\t ", p[i].pid);
        printf("ARR : %d \t|\t Info :", p[i].tpsArrive);
        int sizeU = sizeof(p[i].tpsCPU) / sizeof(int);
        for(int j = 0 ; j < sizeU ; j++) {
            if(p[i].tpsCPU[j] != 0) {
                printf(" %d",p[i].tpsCPU[j]);
            }
        }
        printf("\n");
    }
}

void gestionEntree(Processus *p, const char *fileName) {
    int c, i = 0, j = 0, e = 0, nombre = 0, neg = 0, nbTotal = 0;

    FILE *fichier;
    fichier = fopen(fileName, "r");
    //printf("Q1 : %d | Q2 = %d\n", q1,q2);
    if (fichier) {
        while ((c = getc(fichier)) != EOF) {

            if (c == '\n') {
                p[i].tpsCPU[j-2] = nbTotal + nombre;
                i++;
                j = nombre = e = neg = nbTotal = 0;
            } else if (c == ' ') {
                if(j == 0) {
                    p[i].pid = nombre;
                    j++;
                } else if (j == 1) {
                    p[i].tpsArrive = nombre;
                    j++;
                } else if(neg) {
                    p[i].tpsCPU[j-2] = (neg) ? -nombre : nombre;
                    j++;
                    nbTotal = 0;
                } else {
                    nbTotal += nombre;
                }
                nombre = e = neg = 0;
            } else if (isdigit((int)c)) {
                nombre = (nombre*(10*e))+((int)c-48);
                e++;
            } else if (c == '-') {
                p[i].tpsCPU[j-2] = nbTotal;
                nbTotal = 0;
                j++;
                neg = 1;
            }

        }
        p[i].tpsCPU[j-2] = nbTotal+nombre;
        fclose(fichier);
    }
}

void creationQueue(File *f, int nFile, Processus *p, int nbPro) {
    if(nbPro > 0) {
        for(int i = 0 ; i < nbPro ; i++) {
            f[0].ordreProcessus[i] = &p[i];
            f[0].nbPro++;
        }
    }
    printf("\n");
}

void changementFile(File *file) {
    /*File *nextFile = (file == f+1) ? &f[2] : &f[1];
    (unsigned) int t = &file->ordreProcessus[0];
    printf("p: %p\n",nextFile->ordreProcessus);
    printf("p: %p\n",&f[1].ordreProcessus);
    printf("------------------p4 : %p\n",p);
    nextFile->ordreProcessus[nextFile->nbPro] = file->ordreProcessus[file->nbPro];
    printf("------------------p4 : %p\n",nextFile->ordreProcessus);
    printf("pa: %d\n",f[1].ordreProcessus[0]);*/
}

int defiler(File *f) {
    int i = 0, j = 0;
    while(f->ordreProcessus[i] != NULL ) {
        f->ordreProcessus[i] = f->ordreProcessus[i+1];
        i++;
    }
    f->nbPro--;
    return i;
}

bool avancerTpsCPU(Processus *p) {
    int i = 0;
    printf("p = %d\n",p->tpsCPU[1]);
    while(p->tpsCPU[i] != 0 ) {
        p->tpsCPU[i] = p->tpsCPU[i+1];
        i++;
    }
    printf("p = %d\n",p->tpsCPU[0]);
    return i == 0;
}

void* gestionCPU(File *file, Processus *p, int nbPro) {
    int i = 0, j = 0;
    int *tpsPro;

    printf("\nFILE : %d\t",file->num);
    printf("Processus : ");
    while(file->ordreProcessus[j] != NULL ) {
        printf("%d | ", file->ordreProcessus[j]->pid);
        j++;
    }
    printf("\n\n");
    j=0;

    while(file->ordreProcessus[0] != NULL) {
        tpsPro = &file->ordreProcessus[0]->tpsCPU[0];
        printf("Traitement de : %d\n",file->ordreProcessus[0]->pid);
        if(*tpsPro < 0) {
             avancerTpsCPU(file->ordreProcessus[0]);
             if(file == f+2 && *tpsPro > 0) {
                *tpsPro = 0;
            } else {
                if(file == f) {
                    f[1].ordreProcessus[f[1].nbPro] = file->ordreProcessus[0];
                    f[1].nbPro++;
                } else {
                    f[2].ordreProcessus[f[2].nbPro] = file->ordreProcessus[0];
                    f[2].nbPro++;
                }
                defiler(file);
            }
        } else if(*tpsPro <= file->quantum ) {
            printf("tpsPro : %d | Quantum : %d\n",*tpsPro, file->quantum );
            avancerTpsCPU(file->ordreProcessus[0]);
            defiler(file);
        } else {
            *tpsPro -= file->quantum;
            //printf("-tpsPro : %d\n",*tpsPro);
            if(file == f+2 && *tpsPro > 0) {
                *tpsPro = 0;
            } else {
                if(file == f) {
                    f[1].ordreProcessus[f[1].nbPro] = file->ordreProcessus[0];
                    f[1].nbPro++;
                } else {
                    f[2].ordreProcessus[f[2].nbPro] = file->ordreProcessus[0];
                    f[2].nbPro++;
                }
                defiler(file);
            }

        }
        printf("---------------NOUVEAU---------------\n");
        afficher(p, nbPro);
        printf("#####################################\n");
    }
}

void gestionQueue(File *f, Processus *p, int nbPro) {
    while(f[0].ordreProcessus[0] != NULL || f[1].ordreProcessus[0] != NULL || f[2].ordreProcessus[0] != NULL) {
        gestionCPU(&f[0],p,nbPro);
        gestionCPU(&f[1],p,nbPro);
        gestionCPU(&f[2],p,nbPro);

    }
}



int main (int argc, char const *argv[]) {

    if(1 == 1/*argc == 4*/) {

        f[0].quantum = atoi(argv[1]);
        f[1].quantum = atoi(argv[2]);
        f[0].nbPro = f[1].nbPro = f[2].nbPro = 0;
        f[0].num = 1;
        f[1].num = 2;
        f[2].num = 3;
        FILE *fichier;
        const char *fileName = argv[3];
        fichier = fopen(fileName, "r");
        char c;
        int nbPro = 1, tTotalCPU;
        struct processus* p;

        while ((c = getc(fichier)) != EOF) {
            if (c == '\n') {
                nbPro++;
            }
        }

        p = malloc(nbPro * sizeof(Processus));
        gestionEntree(p, fileName);
        afficher(p, nbPro);
        qsort(p,nbPro,sizeof(Processus),compare);
        tTotalCPU = p[0].tpsArrive;
        creationQueue(f, 0, p, nbPro);
        gestionQueue(f,p, nbPro);
        afficher(p, nbPro);

        //Vérification de l'entrée
        if(0) {
            print_erreur(1);
        } else if(0) {
            print_erreur(2);
        } else if (0){
            print_erreur(3);
        } else {
            printf("\n");
            int sizeP = sizeof(p) / sizeof(Processus);

            for(int i = 0 ; i < sizeP ; i++){
                print_element(p[i].pid,p[i].tpsArrive,0);
            }
        }
        return 0;
    } else {
        print_erreur(3);
        return 1;
    }
}
