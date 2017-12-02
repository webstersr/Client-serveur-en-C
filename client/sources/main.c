#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/types.h>        
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>

void erreur(char *p_fonction){
	perror(p_fonction);
	exit(errno);
}

void usage(char *p_nom_programme){
	fprintf(stderr,"%s prend 2 paramètres, <nomserveur> <numeroport>\n", p_nom_programme);
	exit(-1);
}

int main(int argc, char*argv[]){
    struct sockaddr_in adresse_serveur;
    struct hostent *ip_serveur;
    int id_socket_ecriture;
    /*récupération sur la ligne de cmd*/
    const char* NOM_SERV = argv[1];
    const int PORT_SOURCE = atoi(argv[2]);

    /*Création de la socket qui va nous permettre d'écrire*/
    id_socket_ecriture=socket(AF_INET, SOCK_STREAM, 0);
    if(id_socket_ecriture==-1) erreur("socket");
    
    /*configuration pour la liaison adresse du serveur et socket client*/
    adresse_serveur.sin_family = AF_INET;
    adresse_serveur.sin_port = htons(PORT_SOURCE);
    ip_serveur=gethostbyname(NOM_SERV);
    memcpy(&adresse_serveur.sin_addr.s_addr,ip_serveur->h_addr,ip_serveur->h_length);
    
    /*connexion au serveur*/
    if(connect(id_socket_ecriture, (struct sockaddr *)&adresse_serveur,sizeof(struct sockaddr_in))==-1) erreur("connect");
    /*nous sommes connectés, traitement*/
    write(id_socket_ecriture,"lol",3);

    /*on ferme la socket desormais inutile*/
    close(id_socket_ecriture);
    exit(0);
}