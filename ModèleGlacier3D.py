import numpy as np
import matplotlib.pyplot as plt
import math
import json
import csv

fichiers = ["aletsch2016.json","aletsch2017.json","aletsch2018.json","aletsch2019.json","aletsch2020.json"]
#recuperer les donnees necessaires a notre simulation du fichier meteoblue json
def telecharger(dossier):
    with open(dossier, "r") as donnees_initiales:
        donnees_initiales = json.load(donnees_initiales)
        donnees_utiles = {"altitude": donnees_initiales.get("metadata").get("height")}
        donnees_utiles.update({ 'time': donnees_initiales.get("history_1h").get("time")})
        donnees_utiles.update({'precipitation': donnees_initiales.get("history_1h").get("precipitation")})
        donnees_utiles.update({'temperature': donnees_initiales.get("history_1h").get("temperature")})
    return donnees_utiles

Calculalt = telecharger(fichiers[0])

donnees_finales={'time':[]}
donnees_finales.update({'precipitation':[]})
donnees_finales.update({'temperature': []})

def dictionnaire(dossier):
    #changement d'unite temporelle, passer d'heures en jour, et du coup ne garder que la date 
    #for i in range(0, len(telecharger(dossier)["time"]), 24):
    #    donnees_finales['time'].append(telecharger(dossier)["time"][i][:10])
    doss = telecharger(dossier)
    #faire la somme des precipitations par jours
    for i in range(0, len(doss["precipitation"]), 24):
        somme = 0
        for j in range(i,i+24):
            somme += doss["precipitation"][j]
            donnees_finales["precipitation"].append(somme)

    #moyenne des temperatures par jours
    for i in range(0, len(doss["temperature"]), 24):
        somme = 0
        for j in range(i, i+24):
            somme += doss["temperature"][j]
            donnees_finales["temperature"].append(somme/24)
    return donnees_finales

for i in range(len(fichiers)):
    dictionnaire(fichiers[i])

##Dimension glacier
H = 200#float(input("Quelle est l'épaisseur du glacier?"))
P = 0.2#float(input("Quelle est la pente moyenne du glacier? En pourcentage"))/100
L = 1500#int(input("Quelle est la longueur du glacier?"))
Larg = 300#int(input("Quelle est la largeur du glacier?"))
tps = 1825#float(input("Temps de la simulation (en jour)"))
Alt = Calculalt["altitude"] #on suppose que le point des données est le sommet du glacier
alpha = P*100*math.pi/180 #radians
BaseG = Alt - (P*L)
BaseR = Alt - (P*(L+400)+H)
##Constantes
Patm = ...
V = 10/365 #m/jour
p = 917 #kg/m^3
g = 9.81 #constante pesanteur m/s^2
n = math.pow(10,14) #viscosité en Pascal*s
k = p*g/n
b = k*math.sin(alpha)/2
#mu = 2*b*H*n/(Patm+p*g*H)
Dt = 2.215 #Diffusion thermique
CL = 0.33*(10**6) #Chaleur Latente 
Tr = 273 + 0.0 #Température à la roche


##Création du graphique 3D
x1 = np.linspace(0, L+500, L+500)
x2 = np.linspace(0, Larg+200, Larg+200)
X1, X2 = np.meshgrid(x1, x2)

xx1 = np.linspace (100, L+100, L)
xx2 = np.linspace (100, Larg+100, Larg)
XX1, XX2 = np.meshgrid(xx1, xx2)

xxx2 = np.linspace(100, Larg+100, Larg)
xxx1 = np.linspace(100, L+500, L+500)
XXX1, XXX2 = np.meshgrid(xxx1, xxx2)

#niveau du sol
def roche(x,y):
    return P*(L+500)-P*x + BaseR

#niveau du glacier
def glacier(x,y):
    return Alt - P*(x-100)

Zr = roche(X1, X2)
Zg = glacier(XX1, XX2)

for i in range(len(Zg)):
    Zg[i][0]=Zr[i][100]
    Zg[i][-1]=Zr[i][L+100]
    
for i in range(len(Zg[1])):
    Zg[0][i]=Zr[0][i]
    Zg[-1][i]=Zr[Larg+100][i]


##Modèle de vitesse, Déformation interne
#la vitesse dépend uniquement de la proximité avec la roche.
#pour notre simulation nous allons négliger le contact sur les bords du glacier.
#la vitesse est égale sur la largeur et la longueur.
#ainsi la vitesse change avec la hauteur par rapport à la roche.
#nous pouvons mettre notre glacier à plat
xx = np.linspace (0, L, L)
yy = np.linspace (0, H, H)

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

deplacement = []
for i in range(H-1):
    deplacement.append(L + tps*vv[i])
deplacement.append(L)
dep = np.array(deplacement)

BaseGfinal = BaseG - P*(dep[-2]-L)

fig = plt.figure()
ax = fig.add_subplot(111)
ax.set_aspect('equal', adjustable='box')
ax.set_ylim(0, H + 100)
plt.plot(xx, w, label="Glacier à plat")
plt.plot(dep, yy, label=f"Glacier après {tps} jours")
plt.xlabel("longueur [m]")
plt.ylabel("hauteur [m]")
plt.legend()


##Modele d'enneigement
#coeff d'enneigement par rapport à l'altitude
Coeff = 0.00005
#valeurs enneigement par jour
#la neige a un coefficient de 10 par rapport à la précipitation
#pour l'avoir en mètre nous devons uniquement le diviser par 100
Valeursprecipitation = []
for i in range(len(donnees_finales["precipitation"])):
    Valeursprecipitation.append(donnees_finales["precipitation"][i]/100)

Varneige = []

Ts = []  #Température surface
for i in range(len(donnees_finales["temperature"])):
    TempJ = []
    for j in range(L):
        TempJ.append(273 + donnees_finales["temperature"][i]+ 6.5*P*j/1000)#P ou math.tan(alpha)
    Ts.append(TempJ)

E = []#on commence l'année avec un enneigement initiale de (2m si en hiver)
for i in range(L):
    E.append(2.0)
    
def neige(jour):
    if jour > 4: #4 = nbrs jour pour transformer en glace:
        for i in range(jour-4):
            for j in range(L):
                if Ts[i][j]<=273:
                    valeur = Valeursprecipitation[i]/10 + E[j] - Coeff*P*j
                    if valeur > 0:
                        del E[j]
                        E.insert(j,valeur)
                else:
                    valeur = E[j] - Valeursprecipitation[i] 
                    del E[j]
                    E.insert(j, valeur)
        for i in range(4):
            for j in range(L):
                if Ts[i][j]<=273:
                    valeur = Valeursprecipitation[jour-4+i]/10 + E[j] - Coeff*P*j
                    if valeur > 0:
                        del E[j]
                        E.insert(j,valeur)
                else:
                    valeur = E[j] - Valeursprecipitation[i] 
                    del E[j]
                    E.insert(j, valeur)
    else:
        for i in range(jour):
            for j in range(L):
                if Ts[i][j]<=273:
                    valeur = Valeursprecipitation[i]/10 + E[j] - Coeff*P*j
                    if valeur > 0:
                        del E[j]
                        E.insert(j,valeur)
                else:
                    valeur = E[j] - Valeursprecipitation[i] 
                    del E[j]
                    E.insert(j, valeur)

    return E
neige(tps)


dp = int(dep[-2])
x = np.linspace (0, L, L)
xxx = np.linspace(0, dp, dp)
yyy = np.linspace(BaseR, BaseGfinal, H)
#niveau du sol y
y = P*L-P*xxx + BaseR

#hauteur glacier au temps t0
Z = [BaseR + P*L]
for i in range(L-2):
    Z.append(P*L-P*i + BaseG)
Z.append(BaseR)

#niveau de la neige
for i in range(L):
    Varneige.append(E[i]+Z[i])

##Modele de fonte
htt = []
for i in range(L):
    htt.append(BaseG + P*L - P*i)

def Fonte(temps):
    for i in range(temps):
        for j in range(L):
            if Ts[i][j]>Tr:
                hauteur = htt[j]**(2) - (2*Dt*(Ts[i][j]-Tr)*3600*24)/(p*CL)
                fonte = hauteur**(1/2)
                del htt[j]
                htt.insert(j, fonte)
    return htt
Fonte(tps)
Pentefinal = (htt[0]-htt[-1])/L
Xmax = int(dep[-2]-L)
ht =[Z[0]]
for i in range(L-1):
    total = htt[i]+Varneige[i]-Z[i]
    ht.append(total)
k = ht[-1]
for i in range(Xmax):
    ht.append(k - Pentefinal*i)

h = np.array(ht)
e = np.array(Varneige)
z = np.array(Z)

del deplacement[-1]
deplacement.append(deplacement[-1])
dep2 = np.array(deplacement)

#taille égale des axes 
fig = plt.figure()
ax = fig.add_subplot(111)
ax.set_aspect('equal', adjustable='box')
plt.plot(x, e, label="Enneigement", color='darkblue')
plt.plot(x, z, label="Glacier au temps t0", color='blue')
plt.plot(xxx, y, label="Roche", color="black")
plt.plot(xxx, h, label="Glacier au temps t", color = "red")
plt.plot(dep2, yyy, color = "red")
plt.xlabel("longueur [m]")
plt.ylabel("hauteur [m]")
plt.legend()


##Graphique 3D
ZgT = []
for i in range(Larg):
    ZgT.append(ht)
for i in range(Larg):
    ZgT[i][0] = ht[1] #htt[0] + Varneige[0] - Z[0]
    ZgT[i][-1] = ht[-2]# htt[-1] + Varneige[-1] - Z[-1]
Zgt = np.array(ZgT)

fig = plt.figure()
ax = plt.axes(projection='3d')
ax.set_xlim(0,L + 500)
ax.set_ylim(-100,Larg + 300)
ax.set_zlim(BaseR-100, Alt) 
tailleR = 0.5
tailleG = 5
ax.plot_wireframe(X1, X2, Zr, tailleR, color='black')
ax.plot_wireframe(XX1, XX2, Zg, tailleG, color='blue')
ax.plot_wireframe(XXX1, XX2, Zgt, tailleR, color='red')

plt.show()

#Envoyer les données csv pour le calcul en C.
liste_difference = []
liste_diffinal = []
for i in range(L):
    diffinal = Z[i] - ht[i]
    diff = Varneige[i] - ht[i]
    liste_diffinal.append(diffinal)
    liste_difference.append(diff)
del liste_difference[0]
del liste_diffinal[0]
del Z[0]
#csv_columns = ['Hauteur perdue dans la fonte']
with open('donnees_fonte.csv', 'w') as csv_file:
    writer = csv.writer(csv_file)
    writer.writerow(liste_difference)
    writer.writerow(Z)
    writer.writerow(E)
    writer.writerow(liste_diffinal)
    
with open('donnees_finales.csv', 'w') as csv_file:
    writer = csv.writer(csv_file)
    writer.writerow(donnees_finales)