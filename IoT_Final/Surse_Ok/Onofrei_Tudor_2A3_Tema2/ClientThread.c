#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>

extern int errno; // codul de eroare returnat de anumite apeluri 
int portConectare; // portul de conectare la server*

int main (int argc, char *argv[])
{
  int socketDescriptor;			  // descriptorul de socket pentru server
  struct sockaddr_in server;	// structura folosita pentru conectare  

  char bufferMesaj[255];      

  if (argc != 3)
    {
      printf ("Sintaxa: %s adresa_server _ port\n", argv[0]);
      return -1;
    }

  portConectare = atoi(argv[2]);
  socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);      // cream socketul pentru conectare

  if(socketDescriptor == -1)                               // daca a esuat crearea socketului
  {
    perror ("Eroare la socket().\n");                      // afisam un mesaj de eroare si inchidem programul
    return errno;
  }

  server.sin_family = AF_INET;                             // descriem structura utilizata pentru conectarea la server   
  server.sin_addr.s_addr = inet_addr(argv[1]);
  server.sin_port = htons(portConectare);
  
  if(connect(socketDescriptor,(struct sockaddr *)& server, sizeof(struct sockaddr)) == -1) // daca a esuat conectarea la server
  {
    perror ("[client]Eroare la connect().\n");
    return errno;
  }

  int okConectat = 1;
  while(okConectat){                                                      // cat timp clientul decide sa trimita cereri
    printf("[client]Introduceti un numar pentru volum: ");                
    
    scanf("%s", bufferMesaj);                                             // se citeste cererea in buffer            
    
    if(write(socketDescriptor, bufferMesaj, 255) <= 0)                    // daca nu s-a reusit scrierea                       
    {
      perror("[client]Eroare la write() spre server.\n");                 // afisam un mesaj de eroare si inchidem programul 
      return errno;
    }

    if(read(socketDescriptor, bufferMesaj, 255) < 0)                       // daca avem erori la citirea mesajului trimis de server
    {   
      perror("[client]Eroare la read() de la server.\n");                  // afisam un mesaj de eroare si inchidem programul
      return errno;
    }
    
    printf("[client]Mesajul primit este: %s\n", bufferMesaj);              // afisam mesajul primit de la server  
    if(!strcmp(bufferMesaj, "exit")){                                             // daca clientul a decis sa se deconecteze
        okConectat = 0;                                                            // iesim din bucla principala   
    }
  }
  close(socketDescriptor);                                                // inchidem conexiunea  

  return 0;
}
