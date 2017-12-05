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

/**
 * La fonction renvoie l'erreur passé en parametre
 * @param {char*} *pFonction : Erreur à renvoyer
 * return void
 */
void erreur(char *pFonction){
	perror(pFonction);
	exit(errno);
}

/**
 * La fonction ecrit un message d'erreur en cas de parametre incorrect
 * @param {char*} *pNomProgramme : Nom du programme
 * return void
 */
void usage(char *pNomProgramme){
	fprintf(stderr,"%s prend 2 paramètres, <nomserveur> <numeroport>\n le nom du serveur doit commencer par a-z ou A-Z.\n le numero de port doit etre entier\n", pNomProgramme);
	exit(-1);
}

void envoie_requete_serveur(int pSocket);

int main(int argc, char*argv[]){
    struct sockaddr_in tmpAdresseServeur;
    struct hostent *tmpIpServeur;
    int tmpIdSocketEcriture;
    /*récupération sur la ligne de cmd*/
if(argc!=3) usage(argv[0]);
    const char* NOM_SERV = argv[1];
    const int PORT_SOURCE = atoi(argv[2]);

    /*on regarde si les choses passé en argument ont un minimum de cohérence*/
    if((NOM_SERV[0]<'A' && NOM_SERV[0]>'Z') || (NOM_SERV[0]<'a' && NOM_SERV[0]>'z')|| PORT_SOURCE==0) usage(argv[0]);
    
    /*Création de la socket qui va nous permettre d'écrire*/
    tmpIdSocketEcriture=socket(AF_INET, SOCK_STREAM, 0);
    if(tmpIdSocketEcriture==-1) erreur("socket");
    
    /*configuration pour la liaison adresse du serveur et socket client*/
    tmpAdresseServeur.sin_family = AF_INET;
    tmpAdresseServeur.sin_port = htons(PORT_SOURCE);
    tmpIpServeur=gethostbyname(NOM_SERV);
    memcpy(&tmpAdresseServeur.sin_addr.s_addr,tmpIpServeur->h_addr,tmpIpServeur->h_length);
    
    /*connexion au serveur*/
    if(connect(tmpIdSocketEcriture, (struct sockaddr *)&tmpAdresseServeur,sizeof(struct sockaddr_in))==-1) erreur("connect");
    /*nous sommes connecté, traitement*/
    envoie_requete_serveur(tmpIdSocketEcriture);
    /*on ferme la socket desormais inutile*/
    close(tmpIdSocketEcriture);
    exit(0);
}

/**
 * La fonction ecrit des données pour le serveur
 * @param {int} pSocket : Socket d'ecoute du serveur
 * @return void
 */
void envoie_requete_serveur(int pSocket){
	//Ecrire un while pour le buffer pour qu'il transmete tout le message   
	char* tmpMessage = "Je me présente je m'appel Heny, je voudrais bien réussir ma vie... SALUT JIMMY"; 
	int* tmpTailleBuffer = (int*)malloc(sizeof(int));
        *tmpTailleBuffer = strlen(tmpMessage);
	//On envoie la taille du paquet
	write(pSocket,tmpTailleBuffer,sizeof(int));
	//On envoie le paquet	
	write(pSocket,tmpMessage,*tmpTailleBuffer);
	
	
}
  