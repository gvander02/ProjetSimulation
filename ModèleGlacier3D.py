import numpy as np
import matplotlib.pyplot as plt
import math
import json
import csv

# recuperer les donnees necessaires a notre simulation du fichier meteoblue json

fichiers = ["aletsch2016.json", "aletsch2017.json","aletsch2018.json", "aletsch2019.json", "aletsch2020.json"]

def telecharger(dossier):
    with open(dossier, "r") as donnees_initiales:
        donnees_initiales = json.load(donnees_initiales)
        donnees_utiles = {"altitude": donnees_initiales.get(
            "metadata").get("height")}
        donnees_utiles.update(
            {'time': donnees_initiales.get("history_1h").get("time")})
        donnees_utiles.update(
            {'precipitation': donnees_initiales.get("history_1h").get("precipitation")})
        donnees_utiles.update(
            {'temperature': donnees_initiales.get("history_1h").get("temperature")})
    return donnees_utiles

Calculalt = telecharger(fichiers[0])

donnees_finales = {'time': []}
donnees_finales.update({'precipitation': []})
donnees_finales.update({'temperature': []})

def dictionnaire(dossier):
    # changement d'unite temporelle, passer d'heures en jour, garder que la date
    # for i in range(0, len(telecharger(dossier)["time"]), 24):
    #    donnees_finales['time'].append(telecharger(dossier)["time"][i][:10])
    doss = telecharger(dossier)
    # Somme des precipitations par jours
    for i in range(0, len(doss["precipitation"]), 24):
        somme = 0
        for j in range(i, i+24):
            somme += doss["precipitation"][j]
            donnees_finales["precipitation"].append(somme)

    # moyenne des temperatures par jours
    for i in range(0, len(doss["temperature"]), 24):
        somme = 0
        for j in range(i, i+24):
            somme += doss["temperature"][j]
            donnees_finales["temperature"].append(somme/24)
    return donnees_finales

for i in range(len(fichiers)):
    dictionnaire(fichiers[i])

# Dimension glacier-------------------------------------------------------------------------------------------
H = 200  # float(input("Quelle est l'épaisseur du glacier?"))
# float(input("Quelle est la pente moyenne du glacier? En pourcentage"))/100
P = 0.2
L = 2700  # int(input("Quelle est la longueur du glacier?"))
Larg = 300  # int(input("Quelle est la largeur du glacier?"))
tps = 1825#len()  # float(input("Temps de la simulation (en jour)"))
# on suppose que le point des données est le sommet du glacier
Alt = Calculalt["altitude"]
alpha = P*100*math.pi/180  # radians
BaseG = Alt - P*L
BaseR = Alt - P*(L+400) - H
BaseG2 = Alt - P*L - H
# Constantes
Patm = ...
V = 10/365  # m/jour
p = 917  # kg/m^3
g = 9.81  # constante pesanteur m/s^2
n = math.pow(10, 14)  # viscosité en Pascal*s
k = p*g/n
b = k*math.sin(alpha)/2
#mu = 2*b*H*n/(Patm+p*g*H)
Ct = 2.215  #Conductivité thermique
CL = 0.33*(10**6)  # Chaleur Latente
Tr = 273 + 0.0  # Température à la roche


# Création du graphique 3D------------------------------------------------------------------------------------
x1 = np.linspace(-100, L+400, L+500)
x2 = np.linspace(0, Larg+200, Larg+200)
X1, X2 = np.meshgrid(x1, x2)

xx1 = np.linspace(0, L, L)
xx2 = np.linspace(100, Larg+100, Larg)
XX1, XX2 = np.meshgrid(xx1, xx2)


# niveau du sol
def roche(x, y):
    return P*(L+500)-P*x + BaseR

# niveau du glacier


def glacier(x, y):
    return Alt - P*(x-100)


Zr = roche(X1, X2)
Zg = glacier(XX1, XX2)

for i in range(len(Zg)):
    Zg[i][0] = Zr[i][100]
    Zg[i][-1] = Zr[i][L+100]

for i in range(len(Zg[1])):
    Zg[0][i] = Zr[0][i]
    Zg[-1][i] = Zr[Larg+100][i]


# Modèle de vitesse, Déformation interne
# la vitesse dépend uniquement de la proximité avec la roche.
# pour notre simulation nous allons négliger le contact sur les bords du glacier.
# la vitesse est égale sur la largeur et la longueur.
# ainsi la vitesse change avec la hauteur par rapport à la roche.
# nous pouvons mettre notre glacier à plat
xx = np.linspace(0, L, L)
yy = np.linspace(0, H, H)

def vitesse(y):
    v = V + b*y*(2*H-y)*3600*24
    return v

W = [0]
for i in range(L-2):
    W.append(H)
W.append(0)
w = np.array(W)

vv = []
for i in range(H):
    vv.append(vitesse(i))
vv = np.array(vv)

vv1 = []
for i in range(H):
    vv1.append(vitesse(H-i))
vv1 = np.array(vv1)

deplacement1 = []
for i in range(H-1):
    deplacement1.append(L + tps*vv1[i])
deplacement1.append(L)
dep1 = np.array(deplacement1)

deplacement = []
for i in range(H-1):
    deplacement.append(L + tps*vv[i])
deplacement.append(L)
dep = np.array(deplacement)

fig = plt.figure()
ax = fig.add_subplot(111)
ax.set_aspect('equal', adjustable='box')
ax.set_ylim(0, H + 100)
plt.plot(xx, w, label="Glacier à plat", color = 'lightblue')
plt.plot(dep, yy, ':', label=f"Glacier après {tps} jours sans effondrement", color = 'green')
plt.plot(dep1, yy, label=f"Glacier après {tps} jours et après effondrement", color = 'red')
plt.xlabel("longueur [m]")
plt.ylabel("hauteur [m]")
plt.legend()


#Modele d'enneigement, Accumulation---------------------------------------------------//
Coeff = 0.00005  # coeff d'enneigement par rapport à l'altitude (plus réaliste)
dp = int(dep[-2])
dp1 = int(dep1[-2])
del deplacement[-1]
#del deplacement[-1]
#del deplacement[-1]
del deplacement1[-1]
BaseGfinal = BaseG - P*(dep1[-1]-L)
BaseRfinal = BaseR - P*(dep1[0]-L)
"""
valeurs enneigement par jour:
la hauteur de neige qui tombe a un coefficient de 10 par rapport à la précipitation(eau)
pour passer de millimètre en mètre nous devons uniquement le diviser par 100
"""
Valeursprecipitation = []
for i in range(len(donnees_finales["precipitation"])):
    Valeursprecipitation.append(donnees_finales["precipitation"][i]/100)

Ts = []  #Température surface
for i in range(len(donnees_finales["temperature"])):
    TempJ = []
    for j in range(L):
        TempJ.append(273 + donnees_finales["temperature"][i] + 6.5*P*j/1000)
    Ts.append(TempJ)

E = []  #on commence l'année avec un enneigement initiale de (2m si en hiver)
for i in range(L):
    E.append(2.0)

nbrsj = 4 #nbrs jour pour transformer en glace:
def neige(jour):
    if jour > nbrsj:
        for i in range(jour-nbrsj):
            for j in range(L):
                if Ts[i][j] <= 273:
                    valeur = Valeursprecipitation[i]/10 + E[j] - Coeff*P*j
                    if valeur > 0:
                        del E[j]
                        E.insert(j, valeur)
                else:
                    valeur = E[j] - Valeursprecipitation[i]
                    del E[j]
                    E.insert(j, valeur)
                    
        for i in range(nbrsj):
            for j in range(L):
                if Ts[i][j] <= 273:
                    valeur = Valeursprecipitation[jour-nbrsj+i] + E[j] - Coeff*P*j
                    if valeur > 0:
                        del E[j]
                        E.insert(j, valeur)
                else:
                    valeur = E[j] - Valeursprecipitation[i]
                    del E[j]
                    E.insert(j, valeur)
                    
    else:
        for i in range(jour):
            for j in range(L):
                if Ts[i][j] <= 273:
                    valeur = Valeursprecipitation[i]/10 + E[j] - Coeff*P*j
                    if valeur > 0:
                        del E[j]
                        E.insert(j, valeur)
                else:
                    valeur = E[j] - Valeursprecipitation[i]
                    del E[j]
                    E.insert(j, valeur)

    return E
neige(tps)

#Graphique 2D----------------------------------------------------------------//
x = np.linspace(0, L, L)
xxx = np.linspace(0, dp, dp)
xxx1 = np.linspace(0, dp1, dp1)


# niveau du sol y
y = P*L-P*xxx + BaseR

# hauteur glacier au temps t0
Z = [BaseR + P*L]
for i in range(L-2):
    Z.append(P*L-P*i + BaseG)
Z.append(BaseR)

# niveau de la neige
Varneige = []
VraiZ =[]
for i in range(L):
    VraiZ.append(P*L-P*i + BaseG)
for i in range(L):
    Varneige.append(E[i]+VraiZ[i])

# Modele de fonte, Ablation------------------------------------------------------------//
htt = VraiZ
#for i in range(L):
#    htt.append(BaseG + P*L - P*i)

for i in range(tps):
    for j in range(L):
        if Ts[i][j] > Tr:
            hauteur = htt[j]**(2) - (2*Ct*(Ts[i][j]-Tr)*3600*24)/(p*CL)
            fonte = hauteur**(1/2)
            del htt[j]
            htt.insert(j, fonte)

Pentefinal = (htt[0]-htt[-1])/L
Xmin = int(V*tps)
Xmax = int(dep[-2]-L)

ht = [Z[0]]
HT = []
for i in range(1,L):
    ht.append(htt[i]+E[i])
for i in range(L):
    HT.append(htt[i]+E[i])
dernier_point = ht[-1]
for i in range(1,Xmin+2):
    ht.append(dernier_point - Pentefinal*i)
    
pente_v = []
for i in range(1,Xmax+1):
    pente_v.append(dernier_point - Pentefinal*i)

h = np.array(ht)
e = np.array(Varneige)
z = np.array(Z)
dep2 = np.array(deplacement)
dep1 = np.array(deplacement1)

haut = int(math.sin(alpha)*V*tps/2)
yyy = np.linspace(BaseR-haut, pente_v[-1], H-1)
yyy1 = np.linspace(BaseRfinal, int(ht[-1])-1, H-1)
xxx2 = np.linspace(L, dp, Xmax)

# taille égale des axes
fig = plt.figure()
ax = fig.add_subplot(111)
ax.set_aspect('equal', adjustable='box')
plt.plot(x, e, label="Enneigement", color='darkblue')
plt.plot(x, z, label="Glacier au temps t0", color='lightblue')
plt.plot(xxx, y, label="Roche", color="black")
plt.plot(xxx1, h, label="Glacier au temps t", color="red")
plt.plot(dep1, yyy1, color="red")
plt.plot(dep2, yyy, ':', label = f'glacier après {tps} jours', color="green")
plt.plot(xxx2, pente_v, ':', color = 'green')
plt.xlabel("longueur [m]")
plt.ylabel("hauteur [m]")
plt.legend()


# Graphique 3D---------------------------------------------------------------//
ZgT = []
distance = int(L+V*tps)
for i in range(Larg):
    ZgT.append(ht)
Zgt = np.array(ZgT)

for i in range(len(ZgT)):
    Zgt[i][0] = Zr[i][200]

for i in range(distance):
    Zgt[0][i] = Zr[0][i]
    Zgt[-1][i] = Zr[0][i]

ZgT2 = []
for i in range(Larg):
    ZgT2.append(deplacement1)
Zgt2 = np.array(ZgT2)

xxx2 = np.linspace(100, Larg+100, Larg)
xxx1 = np.linspace(0, dp1, dp1)
XXX1, XXX2 = np.meshgrid(xxx1, xxx2)

yyy1 = np.linspace(100, Larg+100, Larg)
yyy2 = np.linspace(BaseG2-haut, pente_v[-1], H-1)
YYY1, YYY2 = np.meshgrid(yyy2, yyy1)


fig = plt.figure()
ax = plt.axes(projection='3d')
ax.set_xlim(0, L + 500)
ax.set_ylim(-100, Larg + 300)
ax.set_zlim(BaseR-100, Alt)
tailleR = 0.5
tailleG = 5
ax.plot_wireframe(X1, X2, Zr, tailleR, color='black')
ax.plot_wireframe(XX1, XX2, Zg, tailleG, color='lightblue')
ax.plot_wireframe(XXX1, XXX2, Zgt, tailleR, color='red')
ax.plot_wireframe(Zgt2, YYY2, YYY1, tailleG, color='red')
plt.show()

# Envoyer les données csv pour le calcul en C.-------------------------------//
liste_difference = []
liste_diffinal = []
for i in range(L):
    diffinal = VraiZ[i] - HT[i]
    liste_diffinal.append(diffinal)
    
    if Z[i]<Varneige[i]:
        diff = Varneige[i] - HT[i]
    else:
        diff = VraiZ[i] - ht[i]
    liste_difference.append(diff)
    
with open('donnees_fonte.csv', 'w') as csv_file:
    writer = csv.writer(csv_file)
    writer.writerow(liste_diffinal)#différance hauteur entre temps t0 et temps t
    writer.writerow(VraiZ)#hauteur du glacier au temps t0
    writer.writerow(E)#hauteur de neige et glace après temps t
    writer.writerow(liste_difference)#Valeurs de ce qui a fondu


#convertir le dictionnaire python en fichier csv
#header du fichier csv (nom de chaque colonnes)
csv_columns= ['precipitation', 'temperature']
#lignes du ficher: date, precipitation e temperature moyenne par jour
dict_data = []
for i in range(len(donnees_finales["temperature"])):
    dict_data.append([donnees_finales["precipitation"][i], donnees_finales["temperature"][i]])
    
with open('donnees_finales.csv', 'w') as csv_file:
    writer = csv.writer(csv_file)
    writer.writerow(csv_columns)
    writer.writerows(dict_data)
