#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>

#define PORT_CONECTARE 2908               // portul folosit pentru listening

extern int errno;                         // codul de eroare utilizat pentru afisarea mesajelor de eroare 

struct StariDevice{                       // structura utilizata pentru monitorizarea starii dispozitivului asociat
    char volum[100];
}Stare;


typedef struct DateThread{
	int ID;                          //id thread
	int DescriptorClient;                                //descriptorul intors de accept
}threadDates;

static void *trateazaClientCuThread(void *);           // functia executata de fiecare thread ce realizeaza comunicarea cu clientii
void trimiteRaspuns(void *, char*);

int main()
{
  system("clear");
  int fisier_config = open("_resources/_Configurare/_config_Speaker.txt", O_RDONLY);  // citire stare

  char numar[255];
  bzero(numar, 255);
  
  read(fisier_config, numar, 255);
  close(fisier_config);

  strcpy(Stare.volum, numar);
  system("clear");
  printf("Volum curent: %s\n", Stare.volum);

  struct sockaddr_in server;	           // structura folosita de server
  struct sockaddr_in from;	
  
  int socketDescriptor;		                    //descriptorul de socket 
  int pid;
  pthread_t threads[100];                     //vector utilizat pentru a retine threadurile curente
	int i=0;
  
  socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);              // cream socket descriptorul pentru conexiune

  if(socketDescriptor == -1)                                       // daca am esuat la crearea socket descriptorului                   
  {
    perror("[server]Eroare la socket().\n");                        // afisam un mesaj de eroare si inchidem programul
    return errno;
  }
 
  int on=1;
  setsockopt(socketDescriptor, SOL_SOCKET,SO_REUSEADDR, &on, sizeof(on));        // setam optiunile socketului la nivelul de socket cu posibilitatea de a reutiliza adrese locale la bind
  
  bzero(&server, sizeof(server));                                 // curatam structurile pentru a le putea descrie
  bzero(&from, sizeof(from));

  server.sin_family = AF_INET;	                                   // descriem serverul pentru a accepta orice adresa IP la conectare prin portul specificat
  server.sin_addr.s_addr = htonl(INADDR_ANY);
  server.sin_port = htons(PORT_CONECTARE);
  
  int eAtasat = bind(socketDescriptor, (struct sockaddr *) &server, sizeof(struct sockaddr));           // atasam socketul la port

  if(eAtasat == -1)                                                 // daca am esuat
  {
    perror("[server]Eroare la bind().\n");                          // afisam un mesaj de eroare si inchidem programul
    return errno;
  }

  int eAscultat = listen(socketDescriptor, 10);                     // punem socketul sa asculte in port
  if(eAscultat == -1)                                               // daca nu se poate asculta
  {                                                                 
    perror ("[server]Eroare la listen().\n");                       // afisam un mesaj de eroare si inchidem programul  
    return errno;
  }

  while(1)
  {
    int client;
    threadDates* threadNou;                                                   
    int length = sizeof(from);

    printf("[server]Asteptam la portul %d...\n", PORT_CONECTARE);
    fflush(stdout);

    client = accept(socketDescriptor, (struct sockaddr *) &from, &length);                           //  acceptam un client nou

    if(client < 0)                                                                                  // daca am esuat
	  {
	    perror("[server]Eroare la accept().\n");                                                     // afisam o eroare si continuam  
	    continue;
	  }  
    write(client, numar, 255);

	  threadNou = (struct DateThread*)malloc(sizeof(struct DateThread));	              // am acceptat un client nou; pregatim un thread nou: ii alocam memorie                      
	  threadNou->ID = i++;                                                         // asociem ID-ul threadului nou    
	  threadNou->DescriptorClient = client;                                        // salvam in thread descriptorul clientului               
  
    pthread_create(&threads[i], NULL, &trateazaClientCuThread, threadNou);	                                // lansam in executie noul thread    
	}
  system("clear");                                                             // la inchiderea serverului, afisam starea finala, o salvam in fisierul de configurare si inchidem
  printf("STARE: %s\n", Stare.volum);

  int fileConfig = open("_resources/_Configurare/_config_Speaker.txt", O_WRONLY | O_TRUNC);
  write(fileConfig, Stare.volum, sizeof(Stare.volum));
  close(fileConfig);

  return 0;
};			

static void *trateazaClientCuThread(void * arg)
{		
    char mesaj[255];

    while(1){
      //system("clear");
      printf("Volum curent: %s\n", Stare.volum);

      struct DateThread threadDeProcesat; 
		  threadDeProcesat = *((struct DateThread*)arg);	
		  printf("[thread]- %d - Asteptam mesajul...\n", threadDeProcesat.ID);
		  fflush(stdout);		 

		  trimiteRaspuns((struct DateThread*)arg, mesaj);                  // trimitem raspunsul clientului
    
      printf("THREAD: %s\n", mesaj);
      if(!strcmp(mesaj, "exit")){                          
          break;
      }
    }

    pthread_detach(pthread_self());                

    int fileConfig = open("_resources/_Configurare/_config_Speaker.txt", O_WRONLY | O_TRUNC);          // la terminarea threadului, il inchidem si salvam starea rezultata de modificarile aduse de client
    write(fileConfig, Stare.volum, sizeof(Stare.volum));
    close(fileConfig);

    close((intptr_t)arg);
    return(NULL); 
};


void trimiteRaspuns(void *arg, char* raspunsSalvat)
{
  char sir[255], nr, i=0;
	struct DateThread threadDeProcesat; 
	threadDeProcesat = *((struct DateThread*)arg);

	if(read(threadDeProcesat.DescriptorClient, sir, sizeof(sir)) <= 0){       // daca am esuat cand am citit mesajul de la client in thread 
		 printf("[Thread %d]\n",threadDeProcesat.ID);
		                                                                         
     int idThreadCurent = pthread_self();                                   // identificam threadul curent
                                      
     pthread_exit(&idThreadCurent);		                                      // si ii oprim executia	
	}
	
	printf("[Thread %d]Mesajul a fost receptionat...%s\n",threadDeProcesat.ID, sir);
	
  if(strcmp(sir, "exit")){
    strcpy(Stare.volum, sir);
  }

  bzero(raspunsSalvat, 255);                                                  // procesam mesajul primit de la client
  strcpy(raspunsSalvat, sir);
  
  printf("[Thread %d]Trimitem mesajul inapoi...%s\n",threadDeProcesat.ID, sir);    
		      
	if(write(threadDeProcesat.DescriptorClient, sir, sizeof(sir)) <= 0)
	{
		printf("[Thread %d] ",threadDeProcesat.ID);
		perror("[Thread]Eroare la write() catre client.\n");
	}
	else{
    system("clear");
    printf("Volum curent: %s\n", Stare.volum);
  }
}
