#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <string.h>
#include "fonctions.h"

#define MAX_CONNEXION_SIM 5
#define TAMPON_TAILLE_MAX 512

/**
 * La fonction traite une requete client
 * @param {int} pSocket : socket sur lequelle on lit la requete
 * @param {TableauVoyage} *pTableau : Tableau contenant la liste des voyages
 * @return void
 */
void traitement_tmpRequete_client(int pSocket, TableauVoyage *pTableau);

int main(int argc,char*argv[]){

    /*Si il n'y a pas qu'un et unique argument on appel la fonction usage*/
	if(argc !=2) usage(argv[0]);
        
	const int PORT_SOURCE = atoi(argv[1]);
	int tmpIdSocketEcoute, tmpServices;
        unsigned int *tmpTailleAdresseClient = (unsigned int*)malloc(sizeof(unsigned int));
	struct sigaction tmpGestionSignal;
	struct sockaddr_in tmpAdresseServeur, tmpAdresseClient;
        TableauVoyage *tmpTableau=NULL;
                
	
	*tmpTailleAdresseClient = sizeof(struct sockaddr_in);

	/*Gestion des signaux à intercepter*/
	/*--On utilise la fonction recupere_mort_fils*/
	tmpGestionSignal.sa_handler = recupere_mort_fils;
	/*--On évite l'interruption de accept par SIGCHLD*/
	tmpGestionSignal.sa_flags = SA_RESTART;
	/*--On précise que notre gestionnaire s'applique aux signaux SICHLD*/
	if(sigaction(SIGCHLD, &tmpGestionSignal, NULL)==-1) erreur("sigaction");

	/*Création d'une socket qui va nous servir à écouter*/
	tmpIdSocketEcoute = socket(AF_INET,SOCK_STREAM,0);
	if(tmpIdSocketEcoute == -1) erreur("socket");

	/*configuration de l'adresse du serveur*/
	tmpAdresseServeur.sin_family = AF_INET;
	tmpAdresseServeur.sin_port = htons(PORT_SOURCE);
	tmpAdresseServeur.sin_addr.s_addr = htonl(INADDR_ANY);
	/*liaison de l'id de la socket du serveur à l'adresse de la socket du serveur*/
	if(bind(tmpIdSocketEcoute,(struct sockaddr *)&tmpAdresseServeur, sizeof(struct sockaddr_in)) == -1) erreur("bind");
	/*Mise en écoute de la socket serveur*/
	if(listen(tmpIdSocketEcoute,MAX_CONNEXION_SIM) == -1) erreur("listen");

	/*tant que l'on reçoit des requêtes de connexions, on crée des services*/
	while(1){
	tmpServices = accept(tmpIdSocketEcoute, (struct sockaddr *)&tmpAdresseClient, tmpTailleAdresseClient);
	if(tmpServices == -1) erreur("accept");

        /*Parsage et instanciation de la base de données*/
        /*tmpTableau=remplir_tableau_voyage(tmpTableau);
        sleep(3);*/
        
        /*Propagation de celle-ci dans les fils*/
	switch(fork()){
		case -1:
		erreur("fork");
		case 0:
		/*Connexion réussie, traitement des demandes client*/
		/*--On ferme la socket d'écoute inutile dans le processus fils*/
                    close(tmpIdSocketEcoute);
                /*--Traitements du serveur*/
                    traitement_tmpRequete_client(tmpServices,tmpTableau);
		/*--On tue le service*/
                close(tmpServices);
		exit(0);
		default:
		/*--On ferme le service inutile dans le père*/
		close(tmpServices);
		}
	}

        close(tmpIdSocketEcoute);
	exit(0);
}


/**
 * La fonction traite une requete client
 * @param {int} pSocket : socket sur lequelle on lit la requete
 * @param {TableauVoyage} *pTableau : Tableau contenant la liste des voyages
 * @return void
 */
void traitement_tmpRequete_client(int pSocket, TableauVoyage *pTableau){
    int *taille_tampon=(int*)malloc(sizeof(int));
    read(pSocket,taille_tampon,sizeof(int));
    char tmpRequete[*taille_tampon];

    read(pSocket,tmpRequete,*taille_tampon);
	    puts("Requete du serveur");
	    puts(tmpRequete);
   // afficher_table(pTableau);
}
