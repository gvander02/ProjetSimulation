#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h> 
#include <stdbool.h>

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
		double x = tableau[3*3000+i];
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

int main(int argc, char * argv[]){
	double * Valeursdiff = malloc(4 * 3000 * sizeof (double));
    lire_donnees_csv("donnees_fonte.csv", Valeursdiff, 3000, 4);
    int L = 2500;
    int Larg = 300;
    int P = 20;
    int H = 200;

	double V = VolumeInitial(L, Larg, P, H);
	
	double M = MasseInitiale(L, Larg, P, H);

	double Vf = VolumeFinal(Valeursdiff, L, Larg, P, H);
	
	double Mf = MasseFinale(Valeursdiff, L, Larg, P, H);
	
	double pourcentageV = Vf/V;
	double pourcentageM = Mf/M;
	
	printf("le volume du glacier diminue de'%.3e'm^3 a '%.3e'm^3, ce qui correspond a une baisse de %f%c sur 5 annees\n", V, Vf, pourcentageV, 37);
	printf("la masse totale du glacier diminue de '%0.3e'Kg3 a '%0.3e'Kg, ce qui correspond a une baisse de %f%c sur 5 annees\n", M, Mf, pourcentageM, 37);
	
	Fichiercsv("Valeursdiff.csv", Valeursdiff);
	
	free(Valeursdiff);
	return 0;
}
