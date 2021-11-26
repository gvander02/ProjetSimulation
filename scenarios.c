#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

// lire les donnees du fichier csv (jour, precipitation, temperature), et les mettre dans un tableau malloc
_Bool lire_donnees_csv(char * nom_csv, double * values, int colonnes, int lignes){
    FILE * file = fopen(nom_csv, "r");
    if (file == NULL){
        fprintf(stderr, "File %s not found.\n", nom_csv);
        return false;
    }
    int y = 0;
    char buffer[100000];
    //while(fgets(buffer, 100000, file));
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

// calcul du débit pour chaque position x,y, sachant que nous considérons que 
//l'altitude pour chaque couple (x,y) est la même, (1500 correspond a la 
//longueur du glacier choisie dans le code python)
double debit(double * differences, int largeur, int longueur){
    double debit_total = 0.0;
    for(int i=0; i<longueur; i++){
        double diff = 0; 
        diff = differences[i]*largeur;
        debit_total += diff;
    } 
    return debit_total;
}

// scenario 1: effet d'un changement de temperature sur chaque jours
// fonction qui fait un changement de la temperature d'une valeur aleatoire entre -1 et 3 par jour
_Bool scenario_temp(double * values, double * temperature_scenario){
    srand(time(NULL));
    for(int i = 0; i<5*366; i++){
        double randDomain = RAND_MAX + 1.0;
        int ajout = (int) (rand() / randDomain * 5 - 1);
        temperature_scenario[i] = values[2*i+1] + 273.0 + ajout;
    }
    return true;
}
// l'enneigement initial ne change pas lorsque la temperature change 
// donc on prend les valeurs de la hauteur calculee dans le code python 
// auquel on applique le modele de fonte pour calculer la hauteur finale
// calcul de la fonte avec les nouvelles valeurs de la temperature
_Bool modele_fonte(double * hauteur, double * temperature, int time, int longueur, double pente){
    double Tr = 273.0; //temperature de la roche
    double Dt = 2.215; // diffusion thermique 
    double p = 900.0; // masse volumnique glace
    double cl = 0.33*pow(10,6); // chaleur latente de fusion de la glace 
    double temperature_h[longueur];
    for (int i = 0; i<366*5; i++){
        for (int j = 0; j<longueur; j++){
            temperature_h[j]=temperature[i]+(6.5*pente*j)/1000;
            if (temperature_h[j]>Tr){
                double diff= hauteur[j]*hauteur[j]-(2*Dt*(temperature_h[j]-Tr)*3600*24)/(p*cl);
                double fonte = sqrt(diff);
                hauteur[j]=fonte;
            }
        }
    }
    return true;
}

void Fichiercsv(char * fichier, double tableau[]){
	FILE * file = fopen(fichier, "w+");
		for (int y = 0; y < 4; y++){
			for (int x = 0; x < 3000; x++){
				double p = tableau[y*3000+x];
				if (x > 0) fprintf(file, ", ");
					fprintf(file, "%f", p);
			}
			fprintf(file, "\n");
		}
	fclose(file);
}

int main(){
	int L = 2700;
    int Larg = 300;
    int P = 20;
    int H = 200;

    // faire un tableau avec le calcul des differences   
    double * calculs = calloc(4*3000, sizeof (double));
    lire_donnees_csv("donnees_fonte.csv", calculs, L, 4);

    // creer un tableau avec les donnees meteoblue
    double values[2*5*366];
    lire_donnees_csv("donnees_finales.csv", values, 2, 5*366);
    printf("donnees finales:\n");
    
    double * difference = calloc(L, sizeof(double));
    
    // somme des diff et diviser par nb de jours donne la moyenne
    float moyenne_debit_jours = debit(calculs, Larg, L)/(5*366);
    printf("le debit moyen par jour est de: %0.2f m3\n",moyenne_debit_jours);
    
    double temperature_scenario[5*366];
    scenario_temp(values, temperature_scenario);
    // for(int i = 0; i<366; i++){
    //     printf("%f, ", temperature_scenario[i]);
    // }
    
    
    // faire un tableau avec la hauteur initiale 
    double hauteur[L];
    double Hauteur[L];
    for(int i=0;i<L;i++){
        hauteur[i]=calculs[1*L+i];
		Hauteur[i]=calculs[1*L+i];
    }
    
    // calculer la hauteur apres avoir applique le modele de fonte
    modele_fonte(hauteur, temperature_scenario, 5*366, L, P);
    for(int i=0; i<L; i++){
        printf("%f\n", Hauteur[i] - hauteur[i]);
    }
    
    Fichiercsv("calculsdiff.csv", calculs);
    free(calculs);
    return 0;
}
