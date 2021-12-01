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

double debit(double differences[], int largeur, int longueur){
    // calcul du débit pour chaque position x,y, sachant que nous considérons que 
	//l'altitude pour chaque couple (x,y) est la même, (L correspond a la 
	//longueur du glacier choisie dans le code python)
    double debit_total = 0.0;
    for(int i=0; i<longueur; i++){
        double diff = 0; 
        diff = differences[3*longueur + i]*largeur;
        debit_total += diff;
    } 
    return debit_total;
}

double scenario_debit(int longueur, int largeur, double * Hauteur, double * hauteur, double * Variation_neige, int temps){
	double Debit_scenario = 0.0;
    for(int i=0; i<longueur; i++){
		if ( Variation_neige[i] > 0 ){
			Debit_scenario += (Hauteur[i] - hauteur[i])*largeur;
		}else{
			Debit_scenario += (Hauteur[i]-hauteur[i]-Variation_neige[i])*largeur;
		}
    }
    return Debit_scenario/temps;
}

_Bool scenario_temp(double * values, double * temperature_scenario){
    // scenario 1: effet d'un changement de temperature sur chaque jours
	// fonction qui fait un changement de la temperature d'une valeur aleatoire entre -1 et 3 par jour
    srand(time(NULL));
    for(int i = 0; i<5*366; i++){
        double randDomain = RAND_MAX + 1.0;
        int ajout = (int) (rand() / randDomain * 5 - 1);
        temperature_scenario[i] = values[2*i+1] + 273.0 + ajout;
    }
    return true;
}

_Bool modele_fonte(double * hauteur, double * temperature, int time, int longueur, double pente){
	// l'enneigement initial ne change pas lorsque la temperature change 
	// donc on prend les valeurs de la hauteur calculee dans le code python 
	// auquel on applique le modele de fonte pour calculer la hauteur finale
	// calcul de la fonte avec les nouvelles valeurs de la temperature
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

double VolumeInitial(int longueur, int largeur, double pente, int epaisseur){
	double alpha = atan(pente/100);
	double longueurglace = longueur/cos(alpha);
	double volume = longueurglace * epaisseur * largeur;
	return volume;
}

double MasseInitiale(int longueur, int largeur, double pente, int epaisseur){
	double massevolumique = 917;
	return massevolumique*VolumeInitial(longueur, largeur, pente, epaisseur);
}

double VolumeFinal(double tableau[], int longueur, int largeur, double pente, int epaisseur){
	double resultat = 0.0;
	for (int i = 0; i<longueur; i++){
		double x = tableau[i];
		resultat += x*largeur;
	}
	return VolumeInitial(longueur, largeur, pente, epaisseur)-resultat;
}

double MasseFinale(double tableau[], int longueur, int largeur, double pente, int epaisseur){
	double massevolumique = 917;
	return massevolumique*VolumeFinal(tableau, longueur, largeur, pente, epaisseur);
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
	
	//tableau des données du code python
	double * Valeursdiff = malloc(4 * L * sizeof (double));
    lire_donnees_csv("donnees_fonte.csv", Valeursdiff, L, 4);
    
    //tableau avec les donnees meteoblue
    double values[2*5*366];
    lire_donnees_csv("donnees_finales.csv", values, 2, 5*366);

	double V = VolumeInitial(L, Larg, P, H);
	double M = MasseInitiale(L, Larg, P, H);
	double Vf = VolumeFinal(Valeursdiff, L, Larg, P, H);
	double Mf = MasseFinale(Valeursdiff, L, Larg, P, H);
	double pourcentageM = 1-Mf/M;
	
	printf("Le volume du glacier diminue de: %.3em^3, a: %.3em^3, ", V, Vf);
	printf("la masse totale du glacier diminue de: %0.3eKg3, a: %0.3eKg, ce qui correspond a une baisse de: %f%c sur 5 annees.\n\n", M, Mf, pourcentageM, 37);
    
    // somme des diff et diviser par nb de jours donne la moyenne
    float moyenne_debit_jours = debit(Valeursdiff, Larg, L)/(5*366);
    printf("Le debit moyen par jour est de: %0.2f m^3, ",moyenne_debit_jours);
    
    double temperature_scenario[5*366];
    scenario_temp(values, temperature_scenario);
    
    // faire un tableau avec la hauteur initiale et l'enneigement
    double Variation_neige[L];
    double hauteur[L];
    double Hauteur[L];
    for(int i=0;i<L;i++){
		Variation_neige[i]=Valeursdiff[2*L+i];
        hauteur[i]=Valeursdiff[1*L+i];
		Hauteur[i]=Valeursdiff[1*L+i];
    }
    
    // calculer la hauteur apres avoir applique le modele de fonte
    modele_fonte(hauteur, temperature_scenario, 5*366, L, P);
   
	double debit_scen = scenario_debit(L, Larg, Hauteur, hauteur, Variation_neige, 5*366);
	double pourcentageD = debit_scen/moyenne_debit_jours;
    printf("le debit moyen de notre scenario par jour est de: %0.2f m^3, soit une augementation de %f%c.\n\n", debit_scen, pourcentageD, 37);
    
    //tableau des differences avec le nouveau modele de fonte
    double difference_scen[L];
    for (int i=0; i<L; i++){
		difference_scen[i] = Hauteur[i] - hauteur[i] - Variation_neige[i];
	}
    
    double VolumeFinal_scen = VolumeFinal(difference_scen, L, Larg, P, H);
    double pourcentageVs = 1-VolumeFinal_scen/V;
    double pourcentageVfs = 1-VolumeFinal_scen/Vf;
    printf("Le volume final, avec le scenario augementant la temperature de maximum 3 degrees, est de: %0.3em^3, il y a donc une diminution de %f%c.", VolumeFinal_scen, pourcentageVs, 37);
    printf(" Soit %f%c de plus que selon notre modele de base\n", pourcentageVfs, 37);
    
    Fichiercsv("Vale.csv", difference_scen);
	free(Valeursdiff);
    
    return 0;
}
