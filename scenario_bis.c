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

double debit(double differences[], int largeur, int l_incr, int longueur){
    // calcul du débit pour chaque position x,y, sachant que nous considérons que 
	//l'altitude pour chaque couple (x,y) est la même, (L correspond a la 
	//longueur du glacier choisie dans le code python)
    double debit_total = 0.0;
    for(int i=0; i<l_incr; i++){
        double diff = 0; 
        diff = differences[3*l_incr + i]*largeur*longueur/l_incr;
        debit_total += diff;
    } 
    return debit_total;
}

double scenario_debit(int l_incr, int largeur, double * Hauteur, double * hauteur, double * Variation_neige, int temps, int longueur){
	double Debit_scenario = 0.0;
    for(int i=0; i<l_incr; i++){
		if ( Variation_neige[i] > 0 ){
			Debit_scenario += (Hauteur[i] + Variation_neige[i] - hauteur[i])*largeur*longueur/l_incr;
		}else{
			Debit_scenario += (Hauteur[i]-hauteur[i])*largeur*longueur/l_incr;
		}
    }
    return Debit_scenario/temps;
}

_Bool scenario_temp(double * values, double * temperature_scenario, int temps){
    //scenario 1: effet d'un changement de temperature sur chaque jours
    //selon des articles les vagues de chaleures seront plus intense et plus longues.
	//fonction qui fait un changement de la temperature d'une valeur aleatoire entre -1 et +3 par jour
    srand(time(NULL));
    for(int i = 0; i<temps; i++){
        double randDomain = RAND_MAX + 1.0;
        int ajout = (int) (rand() / randDomain * 5 - 1);
        temperature_scenario[i] = values[2*i+3] + 273.0 + ajout;
	}
	return true;
}

_Bool scenario_temp2(double * temperature_scenario, int tps){
	//on considere une vague de chaleur à partir de 0 degré pour une altitude a plus de 3600m
	//pour pas avoir des mesures exagérées nous faisons deux niveau de vague de chaleur
	srand(time(NULL));
	int temps = tps;
	while (temps > 1){	
		double randDomain = RAND_MAX + 1.0;
        int ajout = (int) (rand() / randDomain * 5);
		if (temperature_scenario[tps-temps] > 273 && temperature_scenario[tps-temps] < 276){
			for (int j = 0; j<10; j++){
				if (temperature_scenario[tps-temps+j]<273){ 
					temperature_scenario[tps-temps+j] = temperature_scenario[tps-temps] + 0.5*ajout;
				}else{ 
					temperature_scenario[tps-temps+j] = temperature_scenario[tps-temps+j] + 0.5*ajout;
				}
			}
        }
        temps -= 10;
        if (temperature_scenario[tps-temps] > 276){
			for (int j = 0; j<10; j++){
				if (temperature_scenario[tps-temps+j]<276){ 
					temperature_scenario[tps-temps+j] = temperature_scenario[tps-temps] + 0.5*ajout;
				}else{ 
					temperature_scenario[tps-temps+j] = temperature_scenario[tps-temps+j] + 0.5*ajout;
				}
			}
			temps -= 10;
        }
        temps -= 1;
    }
    
    return true;
}

_Bool modele_fonte(double * hauteur, double * temperature, int time, int l_incr, double pente, double * Hauteur, int H, double * precipitation){
	// l'enneigement initial ne change pas lorsque la temperature change 
	// donc on prend les valeurs de la hauteur calculee dans le code python 
	// auquel on applique le modele de fonte pour calculer la hauteur finale
	// calcul de la fonte avec les nouvelles valeurs de la temperature
    double Tr = 273.0; //temperature de la roche
    double Dt = 2.215; // diffusion thermique 
    double p = 917.0; // masse volumnique glace
    double cl = 0.33*pow(10,6); // chaleur latente de fusion de la glace 
    double temperature_h[l_incr];
    for (int i = 0; i<time; i++){
		for (int j = 0; j<l_incr; j++){
            temperature_h[j]=temperature[i]+(6.5*pente*j)/1000;
            if (temperature_h[j]>Tr){
				double pluie = precipitation[i];
                double diff= pow((hauteur[j]-pluie),2)-(2*Dt*(temperature_h[j]-Tr)*3600*24)/(p*cl);
                double fonte = sqrt(diff);
                hauteur[j]=fonte;
            }
        }
    }
    for (int i = 0; i<l_incr; i++){
		hauteur[i] = hauteur[i] - H + Hauteur[i];
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

double VolumeFinal(double tableau[], int l_incr, int longueur, int largeur, double pente, int epaisseur){
	double resultat = 0.0;
	for (int i = 0; i<l_incr; i++){
		double x = tableau[i];
		resultat += x*largeur*longueur/l_incr;
	}
	return VolumeInitial(longueur, largeur, pente, epaisseur)-resultat;
}

double MasseFinale(double tableau[], int l_incr, int longueur, int largeur, double pente, int epaisseur){
	double massevolumique = 917;
	return massevolumique*VolumeFinal(tableau, l_incr, longueur, largeur, pente, epaisseur);
}

void Fichiercsv(char * fichier, double tableau[], int colonnes, int lignes){
	FILE * file = fopen(fichier, "w+");
		for (int y = 0; y < lignes; y++){
			for (int x = 0; x < colonnes; x++){
				double p = tableau[y*colonnes+x];
				if (x > 0) fprintf(file, ", ");
					fprintf(file, "%f", p);
			}
			fprintf(file, "\n");
		}
	fclose(file);
}

double niveau_lac(int jours, double debit, double surface, double niveau_i, int tps){
    // evolution par jour du niveau du lac
    // nous avons mis des dimensions au hazard
    double niveau = niveau_i;
    double affluent = debit/(3600*24);
    for(int i=0; i< tps; i++){
        double effluent = pow(niveau-15.01,2)*250*3600*24; //m^3/j
        double diff_vol = (affluent-effluent);
        niveau = niveau + diff_vol/surface;
    }
	return niveau;
}

_Bool Masse_norm(){
	for(int i=0; i< 100; i++){
	}
	return true;
}	

int main(){
	int L = 20000;
	int L_incr = 2000;
    int Larg = 1500;
    int P = 5;
    int H = 900;
    int tps = 1827;
		
	//tableau des données du code python
	double * Valeursdiff = malloc(4 * L_incr * sizeof (double));
    lire_donnees_csv("donnees_fonte.csv", Valeursdiff, L_incr, 4);
    
    //tableau avec les donnees meteoblue
    double * values = malloc(2*tps * sizeof (double));
    lire_donnees_csv("donnees_finales.csv", values, 2, tps);
	printf("%f", values[3]);
	double V = VolumeInitial(L, Larg, P, H);
	double M = MasseInitiale(L, Larg, P, H);
	double Vf = VolumeFinal(Valeursdiff, L_incr, L, Larg, P, H);
	double Mf = MasseFinale(Valeursdiff, L_incr, L, Larg, P, H);
	double pourcentageM = 1-Mf/M;
	
	printf("Selon le modele de base sur python, le volume du glacier diminue de: %.3em^3, a: %.3em^3, ", V, Vf);
	printf("la masse totale du glacier diminue de: %0.3eKg3, a: %0.3eKg, ce qui correspond a une baisse de: %f%c sur 5 annees.\n\n", M, Mf, pourcentageM, 37);
    printf("Nous avons fait un scenario qui simule aussi la fonte du meme glacier environ 80 ans plus tard,");
    printf(" avec des temperatures qui devrait augementer et des vagues de chaleurs plus longues et plus intenses\n\n");
    // somme des diff et diviser par nb de jours donne la moyenne
    float moyenne_debit_jours = debit(Valeursdiff, Larg, L_incr, L)/(tps);
    printf("Le debit moyen par jour est de: %0.2f m^3, ",moyenne_debit_jours);
    
    double temperature_scenario[tps];
    double precipitation[tps];
	for (int i=0;i<tps;i++){
		precipitation[i] = values[2*i+2]/1000;
	}
    scenario_temp(values, temperature_scenario, tps);
    scenario_temp2(temperature_scenario, tps);
    
    // faire un tableau avec la hauteur initiale et l'enneigement
    double Variation_neige[L_incr];
    double hauteur[L_incr];
    double Hauteur[L_incr];
    for(int i=0;i<L_incr;i++){
		Variation_neige[i]=Valeursdiff[2*L_incr+i];
        hauteur[i]= H;
		Hauteur[i]=Valeursdiff[1*L_incr+i];
    }
    // calculer la hauteur apres avoir applique le modele de fonte
    modele_fonte(hauteur, temperature_scenario, tps, L_incr, P, Hauteur, H, precipitation);
    
	double debit_scen = scenario_debit(L_incr, Larg, Hauteur, hauteur, Variation_neige, tps, L);
	double pourcentageD = debit_scen/moyenne_debit_jours;
    printf("le debit moyen de notre scenario par jour est de: %0.2f m^3, soit une augementation de %f%c.\n\n", debit_scen, pourcentageD, 37);
    
    //tableau des differences avec le nouveau modele de fonte
    double difference_scen[L_incr];
    for (int i=0; i<L_incr; i++){
		difference_scen[i] = Hauteur[i] - hauteur[i];
	}
    double VolumeFinal_scen = VolumeFinal(difference_scen, L_incr, L, Larg, P, H);
    double pourcentageVs = 1-VolumeFinal_scen/V;
    double Vfs = Vf - VolumeFinal_scen;
    printf("Le volume final, avec le scenario augementant la temperature et avec des vagues de chaleur, est de: %0.3em^3, il y a donc une diminution de %f%c.", VolumeFinal_scen, pourcentageVs, 37);
    printf(" Soit %0.2em^3 de moins que selon notre modele de base\n", Vfs);
    
    Fichiercsv("Vale.csv", difference_scen, L_incr, 1);
	free(Valeursdiff);
    
    return 0;
}
