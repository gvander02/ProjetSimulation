#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

// lire les donnees du fichier csv (jour, precipitation, temperature), et les mettre dans un tableau malloc
void lire_donnees_csv(char * nom_csv, double * values, int colonnes, int lignes){
    FILE * file = fopen(nom_csv, "r");
    if (file == NULL){
        fprintf(stderr, "File %s not found.\n", nom_csv);
        //return false;
    }
    int y = 0;
    char buffer[100000];
    // fgets(buffer, 100000, file);
    while (fgets(buffer, 100000, file) != NULL){
        int x=0;
        char * start = buffer;
        while (true) {
            values[y*colonnes + x]= atof(start);
            start=strchr(start, ',');
            if (start == NULL) break;
            start+=1;
            x+=1;
            if (x>= colonnes) break;
        } 
        y+=1;
        if (y>= lignes) break;
    }
    fclose(file);
    //return true;
}

// calcul du débit pour chaque position x,y, sachant que nous considérons que 
//l'altitude pour chaque couple (x,y) est la même, (1500 correspond a la 
//longueur du glacier choisie dans le code python)
double debit(double * differences, int y){ //y est la largeur de notre echantillon de glacier
    double debit_total = 0.0;
    for(int i=0; i<1500; i++){
        double diff = 0; 
        diff = differences[i]*y;
        debit_total+=diff;
    } 
    return debit_total;
}

// scenario 1: effet d'un changement de temperature sur chaque jours

// fonction qui fait un changement de la temperature d'une valeur aleatoire entre -1 et 3 par jour
bool scenario_temp(double *values, double *temperature_scenario){
    srandom(time(NULL));
    for(int i = 0; i<366; i++){
        double randomDomain = RAND_MAX + 1.0;
        int ajout = (int) (random() / randomDomain * 5 - 1);
        temperature_scenario[i] = values[3*i+2] + 273.0 + ajout;
        //printf("%f, ", temperatures_scen[i]);
    }
    return true;
}
// l'enneigement initial ne change pas lorsque la temperature change 
// donc on prend les valeurs de la hauteur calculee dans le code python 
// auquel on applique le modele de fonte pour calculer la hauteur finale
// calcul de la fonte avec les nouvelles valeurs de la temperature
bool modele_fonte(double * hauteur, double *hauteur_fonte, double * temperature, int time){
    double Tr = 273.0; //temperature de la roche
    double Dt = 2.215; // diffusion thermique 
    double p = 900.0; // masse volumnique glace
    double cl = 0.33*pow(10,6); // chaleur latente de fusion de la glace
    double P = 0.2; //pente 
    double temperature_h[1500];
    for (int i = 0; i<366; i++){
        for (int j = 0; j<1500; j++){
            temperature_h[j]=temperature[i]+(6.5*P*j)/1000;
            if (temperature[j]>Tr){
                double diff= hauteur[j]*hauteur[j]-(2*Dt*(temperature_h[j]-Tr)*3600*24)/(p*cl);
                double fonte = sqrt(diff);
                hauteur_fonte[j]=fonte;
            }
            else hauteur_fonte[j]=hauteur[j];
        }
    }
    return true;

}

_Bool readCsv(char * filename, double * values, int sizeX, int sizeY) {
    FILE * file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "File %s not found.", filename);
		return "false";
    }

    int y = 0;
    char buffer[1000000];
    while (fgets(buffer, 1000000, file) != NULL) {
        int x = 0;
        char * start = buffer;
        while ("true") {
            values[y * sizeX + x] = atof(start);
            start = strchr(start, ',');
            if (start == NULL) break;
            start += 1;

            x += 1;
            if (x >= sizeX) break;
        }

        y += 1;
        if (y >= sizeY) break;
    }

    fclose(file);
    return "true";
}

int main(){
    double * calculs = calloc(9000, sizeof (double));
    readCsv("donnees_fonte.csv", calculs, 4, 1500);
    // faire un tableau avec le calcul des differences
    // for(int i=0; i<6000; i++){
    //     printf("%f ,", calculs[i]);
    // }
    // creer un tableau avec les donnees meteoblue
    double values[3*366];
    lire_donnees_csv("donnees_finales.csv", values, 3, 366);
    // printf("données finales:\n");
    // for (int i=0; i<3*366; i++) {
    //     printf("%f, ", values[i]);
    // }
    double * difference= calloc(1500, sizeof(double));
    for(int i=0; i<10; i++){
        printf("%f\n", calculs[i]);
    }
    // somme des diff et diviser par nb de jours donne la moyenne
    float moyenne_debit_jours = debit(calculs,300)/366;
    printf("le débit moyen par jour est de: %f m3\n",moyenne_debit_jours);
    double temperature_scenario[366];
    scenario_temp(values, temperature_scenario);
    // for(int i = 0; i<366; i++){
    //     printf("%f, ", temperature_scenario[i]);
    // }
    // faire un tableau avec la hauteur initiale 
    double hauteur[1500];
    for(int i=0;i<1500;i++){
        hauteur[i]=calculs[1*1500+i];
    }
    // calculer la hauteur apres avoir applique le modele de fonte
    double hauteur_fonte[1500];
    modele_fonte(hauteur, hauteur_fonte, temperature_scenario, 366);
    for(int i=0; i<1500; i++){
        printf("%f ,", hauteur[i]-hauteur_fonte[i]);
    }
}
