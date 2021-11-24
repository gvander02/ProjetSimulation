#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

// lire les donnees du fichier csv (jour, precipitation, temperature), et les mettre dans un tableau malloc
bool lire_donnees_csv(char * nom_csv, double * values, int colonnes, int lignes){
    FILE * file = fopen(nom_csv, "r");
    if (file == NULL){
        fprintf(stderr, "File %s not found.\n", nom_csv);
        return false;
    }
    int y = 0;
    char buffer[100000];
    fgets(buffer, 100000, file);
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
    return true;
}
// lire fichier donnee issues des calculs du code python
bool lire_calculs_python_csv(char * nom_csv, double * values, int index, int type){
    FILE * file = fopen(nom_csv, "r");
    if (file==NULL){
        fprintf(stderr, "File %s not found.\n", nom_csv);
        return false;
    }
    // int y=0;
    char buffer[100000];
    while(fgets(buffer, 100000, file)!= NULL){
        int x=0;
        char * start = buffer;
        while(true){
            values[x]=atof(start);
            start= strchr(start, ',');
            if (start == NULL) break;
            start+=1;
            x+=1;
            if (x>=index) break;
        }
        // y+=1;
        // if(y>= type) break;
    }
    fclose(file);
    return true;     
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
bool scenario_temp(double * values, double * temperatures_scen){
    srandom(time(NULL));
    for(int i = 0; i<366; i++){
        double randomDomain = RAND_MAX + 1.0;
        int ajout = (int) (random() / randomDomain * 5 - 1);
        temperatures_scen[i] = values[3*i+2] + 273.0 + ajout;
        //printf("%f, ", temperatures_scen[i]);
    }
    return true;
}
// l'enneigement initial ne change pas lorsque la temperature change 
// donc on prend les valeurs de la hauteur calculee dans le code python 
// auquel on applique le modele de fonte pour calculer la hauteur finale
// calcul de la fonte avec les nouvelles valeurs de la temperature
bool modele_fonte(double * hauteur, double * temperature, int time){
    double Tr = 273.0; //temperature de la roche
    double Dt = 2.215; // diffusion thermique 
    double p = 900.0; // masse volumnique glace
    double cl = 0.33*pow(10,6); // chaleur latente de fusion de la glace
    double P = 0.2; //pente 
    for (int i = 0; i<time; i++){
        for (int j = 0; j<1500; j++){
            if (temperature[j]<Tr){
                double diff= hauteur[j]*hauteur[j]-(2*Dt*(temperature[i]+(6.5*P*j)/1000-Tr)*3600*24);
                double fonte = sqrt(diff);
                hauteur[j]=fonte;
            }
            else break;
        }
    }
    return true;

}

int main(){
    double * calculs = calloc(6000, sizeof (double));
    lire_donnees_csv("donnees_fonte.csv", calculs, 1500, 4);
    // faire un tableau avec le calcul des differences
    double * difference= calloc(1500, sizeof(double));
    for(int i=0; i<10; i++){
        printf("%f ,", calculs[i]);
    }
    printf("différences:\n");
    for(int i=0; i<10; i++){
        difference[i]=calculs[i];
        printf("%f, ", difference[i]);
    }
    double values[3*366];
    lire_donnees_csv("donnees_finales.csv", values, 3, 366);
    printf("données finales:\n");
    for (int i=0; i<10; i++) {
        printf("%f, ", values[i]);
    }
    // somme des diff et diviser par nb de jours donne la moyenne
    //float moyenne_debit_jours = debit(difference,300)/366;
    //printf("le débit moyen par jour est de: %f m3\n",moyenne_debit_jours);
    // creer tableau avec les donnees de meteoblue
    // double values[3*366];
    // lire_donnees_csv("donnees_finales.csv", values, 3, 366);
    // for(int i=0; i<366; i++){
        // printf("%f, ", values[i]);
    // }
    //double temperature_scenario[366];
    //scenario_temp(values, temperature_scenario);
    // faire un tableau avec la hauteur initiale 
    //double hauteur[1500];
    //for(int i=0;i<1500;i++){
        //hauteur[i]=calculs[2*1500+i];
    //}
    // calculer la hauteur apres avoir applique le modele de fonte
    //modele_fonte(hauteur, temperature_scenario, 366);
    //for(int i=0; i<1500; i++){
        //printf("%f ,", hauteur[i]);
    //}

}
