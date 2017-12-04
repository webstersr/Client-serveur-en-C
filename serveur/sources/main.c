#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/types.h>        
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>
#include <netinet/in.h>

#define MAX_CONNEXION_SIM 5

void mort_fils(){
	wait(NULL);
}

void erreur(char *p_fonction){
	perror(p_fonction);
	exit(errno);
}

void usage(char *p_nom_programme){
	fprintf(stderr,"%s prend 1 paramètre, c'est le numéro de port que prendra la socket du serveur\n", p_nom_programme);
	exit(-1);
}

void fct_serveur(int p_socket);

int main(int argc,char*argv[]){

    /*Si il n'y a pas qu'un et unique argument on appel la fonction usage*/
	if(argc !=2) usage(argv[0]);
        
	const int PORT_SOURCE = atoi(argv[1]);
	int id_socket_ecoute, tmp_services;
        unsigned int *taille_adresse_client = (unsigned int*)malloc(sizeof(unsigned int));
	struct sigaction gestion_signal;
	struct sockaddr_in adresse_serveur, adresse_client;
                
	
	*taille_adresse_client = sizeof(struct sockaddr_in);

	/*Gestion des signaux à intercepter*/
	/*--On utilise la fonction mort_fils*/
	gestion_signal.sa_handler = mort_fils;
	/*--On évite l'interruption de accept par SIGCHLD*/
	gestion_signal.sa_flags = SA_RESTART;
	/*--On précise que notre gestionnaire s'applique aux signaux SICHLD*/
	if(sigaction(SIGCHLD, &gestion_signal, NULL)==-1) erreur("sigaction");

	/*Création d'une socket qui va nous servir à écouter*/
	id_socket_ecoute = socket(AF_INET,SOCK_STREAM,0);
	if(id_socket_ecoute == -1) erreur("socket");

	/*configuration de l'adresse du serveur*/
	adresse_serveur.sin_family = AF_INET;
	adresse_serveur.sin_port = htons(PORT_SOURCE);
	adresse_serveur.sin_addr.s_addr = htonl(INADDR_ANY);
	/*liaison de l'id de la socket du serveur à l'adresse de la socket du serveur*/
	if(bind(id_socket_ecoute,(struct sockaddr *)&adresse_serveur, sizeof(struct sockaddr_in)) == -1) erreur("bind");
	/*Mise en écoute de la socket serveur*/
	if(listen(id_socket_ecoute,MAX_CONNEXION_SIM) == -1) erreur("listen");

	/*tant que l'on reçoit des requêtes de connexions, on crée des services*/
	while(1){
	tmp_services = accept(id_socket_ecoute, (struct sockaddr *)&adresse_client, taille_adresse_client);
	if(tmp_services == -1) erreur("accept");

	switch(fork()){
		case -1:
		erreur("fork");
		case 0:
		/*Connexion réussie, traitement des demandes client*/
		/*--On ferme la socket d'écoute inutile dans le processus fils*/
                    close(id_socket_ecoute);
                /*--Traitements du serveur*/
                    fct_serveur(tmp_services);
		/*--On tue le service*/
                close(tmp_services);
		exit(0);
		default:
		/*--On ferme le service inutile dans le père*/
		close(tmp_services);
		}
	}

        close(id_socket_ecoute);
	exit(0);
}

void fct_serveur(int p_socket){
    char requete[512];
    read(p_socket,requete,512);
    puts(requete);
}