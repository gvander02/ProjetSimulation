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

_Bool modele_fonte(double * hauteur, double * temperature, int time, int l_incr, double pente, double * Hauteur, int H, double * precipitation, int longueur){
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
            }else{
				double valeur = precipitation[i]*10 - 0.0001*pente*j*longueur/l_incr;
				if (valeur > 0){
					hauteur[j] = hauteur[j] + valeur;
				}
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

double niveau_lac(int jours, double debiti, double debitf, double surface, double niveau_i){
    // evolution par jour du niveau du lac
    // nous avons mis des dimensions au hazard
    double affluent = debitf;
    double effluent = debiti;
    double niveau = niveau_i;
    double volume = surface*niveau_i;
    int larg_riv = 4;
    double h_i = 0.7; //hauteur de la rivière
    double vitesse = effluent/(larg_riv*h_i); //distance de l'eau par jour
    for (int i=0;i<jours;i++){
		volume = volume + affluent*1;
		double niveau_f = volume/surface;
		double h_f = (niveau_f - niveau + h_i);
		vitesse = vitesse * (h_f/h_i);
		if (niveau_f > niveau_i){
			effluent = h_f*larg_riv*vitesse;
			volume = volume - 1*effluent;
		}else{
			effluent = h_f*larg_riv*vitesse;
			volume = volume - 1*effluent;
		}
		printf("%f,  %f\n\n", vitesse, h_f);
		niveau = niveau_f;
	}
    return niveau;
}

int main(){
	int L_incr = 2000;

	//tableau des données du code python
	double * Valeursdiff = malloc(5 * L_incr * sizeof (double));
    lire_donnees_csv("donnees_fonte.csv", Valeursdiff, L_incr, 5);
    
    int L = Valeursdiff[4*L_incr+1];
    int Larg = Valeursdiff[4*L_incr+2];
    int P = Valeursdiff[4*L_incr+3]*100;
    int H = Valeursdiff[4*L_incr];
    int tps = 1827;
    
    //tableau avec les donnees meteoblue
    double * values = malloc(2*tps * sizeof (double));
    lire_donnees_csv("donnees_finales.csv", values, 2, tps);
	double V = VolumeInitial(L, Larg, P, H);
	double M = MasseInitiale(L, Larg, P, H);
	double Vf = VolumeFinal(Valeursdiff, L_incr, L, Larg, P, H);
	double Mf = MasseFinale(Valeursdiff, L_incr, L, Larg, P, H);
	double pourcentageM = 1-Mf/M;
	float moyenne_debit_jours = debit(Valeursdiff, Larg, L_incr, L)/(tps);
	
	printf("Selon le modele de base sur python, le volume du glacier diminue de: %.3em^3, a: %.3em^3, ", V, Vf);
	printf("la masse totale du glacier diminue de: %0.3eKg3, a: %0.3eKg, ce qui correspond a une baisse de: %f%c sur 5 annees.\n\n", M, Mf, pourcentageM, 37);
    printf("Nous avons fait un scenario qui simule la fonte du meme glacier, pendant 5 ans, environ 80 ans plus tard,");
    printf(" avec des temperatures qui devrait augementer et des vagues de chaleurs plus longues et plus intenses\n\n");
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
    modele_fonte(hauteur, temperature_scenario, tps, L_incr, P, Hauteur, H, precipitation, L);
    
	double debit_scen = scenario_debit(L_incr, Larg, Hauteur, hauteur, Variation_neige, tps, L);
	double pourcentageD = debit_scen/moyenne_debit_jours;
    printf("le debit moyen de notre scenario par jour serait de: %0.2f m^3, soit une augementation de %f%c.\n\n", debit_scen, pourcentageD, 37);
    
    // somme des diff et diviser par nb de jours donne la moyenne
    double niveau = niveau_lac(tps, moyenne_debit_jours, debit_scen, 3200000, 15);
    double delta_niveau = niveau - 15.5; 
    printf("Admettons un lac de 15 m de profondeur et une superficie de 3,2km^2, Ces dimensions sont prises pour un lac moyen de glacier."); 
    printf("En considerant comme unique affluent l'eau venant des glaciers, le niveau du lac augementerait de %f m en 2100, selon le scenario.\n\n", delta_niveau);
    
    
    //tableau des differences avec le nouveau modele de fonte
    double difference_scen[L_incr];
    for (int i=0; i<L_incr; i++){
		difference_scen[i] = Hauteur[i] - hauteur[i];
	}
    double VolumeFinal_scen = VolumeFinal(difference_scen, L_incr, L, Larg, P, H);
    double pourcentageVs = 1-VolumeFinal_scen/V;
    double Vfs = Vf - VolumeFinal_scen;
    printf("Le volume final, avec le scenario augementant la temperature et avec des vagues de chaleur, serait de: %0.3em^3, il y a donc une diminution de %f%c.", VolumeFinal_scen, pourcentageVs, 37);
    printf(" Soit %0.2em^3 de moins que selon notre modele de base\n\n", Vfs);
   	free(Valeursdiff);
   	free(values); 
    return 0;
}
