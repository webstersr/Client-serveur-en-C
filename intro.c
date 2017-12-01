#include<stdio.h>
#include<stdlib.h>

void fctserveur (int sockdescr){

 // Fonction qui lit dans la socket une chaine de caractères (en fait un
 // bloc d'octets) et l'affiche à l'écran comme une chaine de caractères
 // ...

}


void finfils(int sig){

 // Handler pour la réception de SIGCHLD
 // ...

}

int main(int argc, char **argv){

 // structures pour la socket d'écoute et la socket de service :
 struct sockaddr_in serveur_ad, client_ad;
 // déclarations des autres variables nécessaires :
 // ...
 // Serveur :
 // récupère le numéro de port en argument (si présent !), se met en état de
 // recevoir des requêtes de clients, et sert chaque requête grâce à la
 // fonction fctserveur
 // ...
 // ...

 return 0;
}
