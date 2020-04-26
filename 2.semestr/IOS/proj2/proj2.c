/*---------------*/
/*-----proj2-----*/
/*---------------*/
/*
  *Autor: Vaclav Dolecek (xdolec03)
  *Predmet: IOS

    ->To Do List<-
  -Nezapomenout smazat kontrolni vypisy
  -Dostat dost bodu
  -Dodělat semafory

  ->informace<-
  -bohuzel na cely ten semafor mi nevysel cas, funguje to pouze pro 4lidi a 5mist na mole
  -opravdu me to bavilo a doufam ze budou i dalsi takoveto projekty

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/mman.h>


/*return: dobrý = 0, chybový = 1*/
enum{dobry, chybovy};

/*Deklarace funkce*/
int RNG();

/*inicializace funkce
 * Funkce vraci nahodne cislo z intervalu <1,2>
 */
int RNG()
{
  /*RNG - generator nahodnych cisel*/
      int nahodne_cislo = 0; //navratova hodnota
      nahodne_cislo = rand()%2 + 1; //generator nahodnych cisel 1 nebo 2
      //printf("Nahodne cislo: %d\n", nahodne_cislo);
      return nahodne_cislo;
}
/*Definice maker pro sdilenou pameť(něco k file Descriptorovy)
 *toto by měli byt pristuova hesla*/
#define shmKEYradkovani "/radkovani"
#define shmKEYmolo "/molo"
#define shmKEYnh "/nh"
#define shmKEYns "/ns"
#define shmKEYfronta "/fronta"
#define shmKEYvylodeni "/vylodeni"
#define shmKEYkonec "/konec"

/*main neni teřeba představovat*/
int main(int argc, char *argv[])
{
  /*Deklarace a inicializace promených*/
    int P = 0; //pocet lidi jedne skupiny (celkem lidi/procesu 2P)
    int H = 0; //maximalni doba generovani hackera (milisekundy)
    int S = 0; //maximalni doba generovani safera (milisekundy)
    int R = 0; //maximalni doba plavby(milisekundy)
    int W = 0; //maximalni doba navratu na molo (milisekundy)
    int C = 0; //kapacita mola

    int hack = 0; //pocet hackeru resp. kolikaty hacker to je
    int serf = 0;//poce serfru resp. kolikaty serfr to je
    int nahodne_cislo = 0;
    int i = 0;

  /*Deklarace PID pro jednotlivé procesy*/
    pid_t PID_hacker = 1;
    pid_t PID_serf = 1;

  /*otevirani souboru*/
    FILE *soubor;
    soubor = fopen("proj2.out", "w");

    /*konrola otecreni*/
    if( soubor == NULL)
      {
          fprintf(stderr, "Problem pri otevirani souboru soubor\n");
      }

  /*deklarace odkazu na sdilenou pamet*/
  /*ukazatele na FD*/
    int radkovani_fd;
    int molo_pocet_fd;
    int NH_fd;
    int NS_fd;
    int zavora1_sem_fd;
    int vylodeni_sem_fd;
    int konec_fd;

  /*ukazatele na sdilenou pamet*/
    int *radkovani = 0;// aktualni pocet radku
    int *molo_pocet = 0;// aktualni celkem pocet lidi na molu
    int *NH = 0;//aktualni pocet Hackeru na molu
    int *NS = 0;//aktualni pocet Serfru na molu
    int *zavora1_sem = 0;//pro spani
    int *vylodeni_sem = 0; //aktualni pocet procesu v rade
    int *konec = 0;

  /*kontrola zda nekdo nezavolal program jen tak nazdařbuh*/
    if( argc == 1 )
    {
      fprintf(stdout, "Pokud si nevíte rady dejte: ./proj2 help\n");
      return dobry;
    }

  /*kontrola zda nekdo v prvnim parametru nevola o pomoc*/
    if( strcmp(argv[1], "help") == 0 )
      {
        fprintf(stdout, "help pro proj2\n");
        fprintf(stdout, "-Argumenty:\n");
        fprintf(stdout, "\t P => pocet generovanych osob\n");
        fprintf(stdout, "\t     -musi byt veci nez 2 a musi byt sude\n");
        fprintf(stdout, "\t H => maximalni doba generovani Hacker\n");
        fprintf(stdout, "\t     -jednotky milisekundy, interval <0,2000>\n");
        fprintf(stdout, "\t S => maximalni doba generovani Serfra\n");
        fprintf(stdout, "\t     -jednotky milisekundy, interval <0,2000>\n");
        fprintf(stdout, "\t R => maximalni doba plavby\n");
        fprintf(stdout, "\t     -jednotky milisekundy, interval <0,2000>\n");
        fprintf(stdout, "\t W => maximalni doba k navratu na molo\n");
        fprintf(stdout, "\t     -jednotky milisekundy, interval <20,2000>\n");
        fprintf(stdout, "\t C => kapacita mola\n");
        fprintf(stdout, "\t     -cele čisla, C => 5\n");

        return dobry;
      }

  /*kontrola poctu argumentu
   *Pokud jich bude 7 muze se pokracovat
   *jinak ne
   */
    if ( argc == 7 )
    {

    /*Konverze stringu na intiger*/
    /*strol( ukazatel na string, zbytek stringu, soustava)*/
      P = strtol(argv[1], NULL, 10);
      H = strtol(argv[2], NULL, 10);
      S = strtol(argv[3], NULL, 10);
      R = strtol(argv[4], NULL, 10);
      W = strtol(argv[5], NULL, 10);
      C = strtol(argv[6], NULL, 10);

    /*kontrola argumentu*/
      if( P < 2 || (P % 2) != 0 )
        {
          fprintf(stderr, "Zle zadany argument P.\n Zadali jste %d \n Pro informace ./proj2 help\n", P);
          return 1;
        }
      if( H < 0 || H > 2000 )
        {
          fprintf(stderr, "Zle zadany argument H.\n Zadali jste %d \n Pro informace ./proj2 help\n", H);
          return 1;
        }
      if( S < 0 || S > 2000 )
        {
          fprintf(stderr, "Zle zadany argument S.\n Zadali jste %d \n Pro informace ./proj2 help\n",S);
          return 1;
        }
      if( R < 0 || R > 2000 )
        {
          fprintf(stderr, "Zle zadany argument R.\n Zadali jste %d \n Pro informace ./proj2 help\n", R);
          return 1;
        }
      if( W < 20 || W > 2000 )
        {
          fprintf(stderr, "Zle zadany argument W.\n Zadali jste %d \n Pro informace ./proj2 help\n", W);
          return 1;
        }
      if( C < 5 )
      {
        fprintf(stderr, "Zle zadany argument C.\n Zadali jste %d \n Pro informace ./proj2 help\n", C);
        return 1;
      }

  /*sdílená pamet*/
    /*Otevřeni File Descriptor
     *Prideleni prav pro File Descriptor*/
      radkovani_fd = shm_open(shmKEYradkovani, O_RDWR, S_IRUSR | S_IWUSR);
      molo_pocet_fd = shm_open(shmKEYmolo, O_RDWR, S_IRUSR | S_IWUSR);
      NH_fd = shm_open(shmKEYnh,  O_RDWR, S_IRUSR | S_IWUSR);
      NS_fd = shm_open(shmKEYns, O_RDWR, S_IRUSR | S_IWUSR);
      zavora1_sem_fd = shm_open(shmKEYfronta, O_RDWR, S_IRUSR | S_IWUSR);
      vylodeni_sem_fd = shm_open(shmKEYvylodeni, O_RDWR, S_IRUSR | S_IWUSR);
      konec_fd = shm_open(shmKEYkonec, O_RDWR, S_IRUSR | S_IWUSR);



    /*mapovani pameti
     *Přiděleni prav pro mapovaneho pametoveho prostoru pomoci File Descriptorem*/
      radkovani = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, radkovani_fd, 0);
      molo_pocet = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, molo_pocet_fd, 0);
      zavora1_sem = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, zavora1_sem_fd, 0);
      NH = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, NH_fd, 0);
      NS = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, NS_fd, 0);
      vylodeni_sem = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, vylodeni_sem_fd, 0);
      konec = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, konec_fd, 0);

    /*zavirani File Descriptor*/
      close(radkovani_fd);
      close(molo_pocet_fd);
      close(NH_fd);
      close(NS_fd);
      close(vylodeni_sem_fd);
      close(zavora1_sem_fd);


  /*Konstruktor*/
    /*For pro Konstruktor
     *For bude provedem P*3 pro jistotu
     *ze se udelaly vsechny procesy*/
      for (; i <= (P*3); i++)
        {
        /*RNG*/
          nahodne_cislo = RNG();
        /*Vyroba hackera*/
          if ( nahodne_cislo != 1 )
            {
            /*Podminka ktera zajisti ze se vyrobi pozadovany pocet hackeru*/
              if ( hack <= (P-1) )
              {
                hack++; //cislo procesu
              /*Funkce pro cekani vymezenou dobu*/
                struct timespec req = {0};
                req.tv_sec = 0;
                req.tv_nsec = H * 1000000L;//prepocet nanosekun na mikrosekundy
                nanosleep(&req, (struct timespec *)NULL);//samotne cekani
                PID_hacker = fork();//vyroba hackera
              }
            }
        /*Vyroba Safera*/
          else if( nahodne_cislo != 2 )
            {
            /*Podminka ktera zajisti ze se vyrobi pozadovny pocet serfru*/
              if ( serf <= (P-1) )
              {
                serf++; //cislo procesu
              /*Funkce pro cekani vymezenou dobu*/
                struct timespec req = {0};
                req.tv_sec = 0;
                req.tv_nsec = S * 1000000L; //prepocet nanosekun na mikrosekundy
                nanosleep(&req, (struct timespec *)NULL);//samotne cekani
                PID_serf = fork();//vyroba serfra
              }
            }
          /*Pokud nastane chyba v RNG*/
            else
              {
                fprintf(stderr, "Chyba ve Funkci RNG\n");
              }

        /*Podmínka pro vypusteni hackera z Konstruktoru*/
            if ( PID_hacker == 0 )
              {
              /*Pocitadlo radku*/
                *radkovani = *radkovani + 1;
              /*tisknuti vystupu*/
                fprintf(soubor, "%d\t: HACK %d\t: starts\n", *radkovani, hack);
              /*uvolneni noveho hackera z Konstruktoru*/
                break;
              }

        /*zde muze jen safer*/
              if ( PID_serf == 0 )
                {
                /*Pocitadlo radku*/
                  *radkovani = *radkovani + 1;
                /*tisknuti vystupu*/
                  fprintf(soubor, "%d\t: SERF %d\t: starts\n", *radkovani, serf);
                /*uvolneni noveho serfra z Konstruktoru*/
                  break;
                }
            /*pri posledni projeti FOR cyklu*/
              if ( i == (P*3))
              {
                /*zaslouzeny odpocinek pro hlavni proces
                 *dat cas na provedeni programu*/
                sleep(2);
              }
          }
  /*Semafor*/
    /*zavora pred semaforem*/
     for( ;*zavora1_sem == 1; )
        {
          /*Funkce pro cekani vymezenou dobu*/
            struct timespec req = {0};
            req.tv_sec = 0;
            req.tv_nsec = W * 1000000L; //prepocet nanosekun na mikrosekundy
            nanosleep(&req, (struct timespec *)NULL);//samotne cekani
        }


    /*pocitadlo lidi na mole*/
      *molo_pocet = *molo_pocet + 1;

    /*Semafor samotny
     *Semafor zaroven vytvori kapitana lode*/
      /*spoustec semaforu
       *prvni 3 procesy mohou v klidu projit na molo_pocet
       *pokud je prazdne*/
        if( *molo_pocet >= 4 )
            {
            /*zavreni zavory a uzamknuti semaforu*/
              *zavora1_sem = 1;

            /*Pokud pro odpluti je treba Hacker*/
              if( *NH == 3 && *NS == 0 )
                {
                  /*Pokud pride hacker, stane se z nej kapitan*/
                    if( PID_hacker == 0 )
                      {
                      /*povyseni hackera na kapitana*/
                        PID_hacker = 2;
                      /*pricteni osoby na mole*/
                        *NH+=1;
                      }
                    /*pokud pride serfr, bude zaslan na molo*/
                      else
                        {

                        }
                }
            /*Pokud pro odpluti je treba Serf*/
              else if( *NH == 2 && *NS == 1 )
                {
                  /*Pokud pride Serf, stane se z nej kapitan*/
                    if( PID_serf == 0 )
                      {
                      /*povyseni serfra na kapitana*/
                        PID_serf = 2;
                      /*pricteni osoby na mole*/
                        *NS+=1;
                      }
                    /*pokud pride hacker, bude zaslan na molo*/
                      else
                        {

                        }
                }
            /*Pokud pro odpluti je treba hacker*/
              else if( *NH == 1 && *NS == 2 )
                {
                  /*pokud pride hacker, stane se z nej kapitan*/
                    if( PID_hacker == 0 )
                      {
                      /*povyseni hackera na kapitana*/
                        PID_hacker = 2;
                      /*pricteni osoby na mole*/
                        *NH+=1;
                      }
                    /*pokud pride serfr, bude zaslan na molo*/
                      else
                        {

                        }
                }
            /*Pokud je treba pro odpluti serfr*/
              else if( *NH == 0 && *NS == 3 )
                {
                /*Pokud pride serfr, stane se z nej kapitan*/
                  if( PID_serf == 0 )
                    {
                    /*povyseni serfra na kapitana*/
                      PID_serf = 2;
                    /*pricteni osoby na mole*/
                      *NS+=1;
                    }
                  /*pokud pride hacker, bude zaslan na molo*/
                    else
                      {

                      }
                }

              }
          /*Kapitan odblokuje molo pro nastup na lod*/
            if( PID_serf == 2 || PID_hacker == 2 )
              {
                *molo_pocet = 0;//odblokovani mola
                if( PID_serf == 2 )
                  {
                  /*pocitadlo radku*/
                    *radkovani = *radkovani + 1;
                  /*nulovani pocitadla lidi na mole*/
                    *NH = 0;
                    *NS = 0;
                  /*vypis vystupu*/
                    fprintf(soubor,"%d\t: SERF %d\t: boards\t : %d\t: %d\n", *radkovani, serf, *NH, *NS);
                  /*uzamknuti lodi*/
                    *vylodeni_sem = 1;
                  }
                else if( PID_hacker == 2 )
                  {
                  /*pocitadlo radku*/
                    *radkovani = *radkovani + 1;
                  /*nulovani pocitadla lidi na mole*/
                    *NH = 0;
                    *NS = 0;
                  /*vypis vystupu*/
                    fprintf(soubor, "%d\t: HACK %d\t: boards \t : %d\t: %d\n", *radkovani, hack, *NH, *NS);
                  /*uzamknuti lodi*/
                    *vylodeni_sem = 1;
                  }
              }


      /*Molo*/
      /*Cekaji na kapitana pro hacker*/
        if ( PID_hacker == 0 )
          {
          /*pricteni pocitadla lidi na mole*/
            *NH = *NH + 1;
          /*pocitadlo radku*/
            *radkovani = *radkovani + 1;
          /*tisk na vystup*/
            fprintf(soubor, "%d\t: HACK %d\t: waits \t : %d\t: %d\n", *radkovani, hack, *NH, *NS);
          /*cekani na kapitana*/
            for( ;(*NH+*NS) > 4; )
                {
                /*hacker spi v kajute*/
                  sleep(3);
                }

          }
        /*Cekani na kapitana pro serfra*/
          if ( PID_serf == 0 )
            {
            /*pricteni pocitadla lidi na mole*/
              *NS = *NS + 1;
            /*pocitadlo radku*/
              *radkovani = *radkovani + 1;
            /*vypis na vystup*/
              fprintf(soubor, "%d\t: SERF %d\t: waits \t : %d\t: %d\n", *radkovani, serf, *NH, *NS);
            /*cekani na kapitan*/
              for(;(*NH+*NS) > 4;)
                {
                /*serfr spi v kajute*/
                  sleep(3);
                }
            }

      /*LOD*/
        /*na lod nastupuje kapitan jako posledni
         *pred odplutim otevre molo pro dalsi lidi*/
          if( PID_serf == 2 || PID_hacker == 2 )
            {
              *zavora1_sem = 0;//otevira zavoru pred semaforem u mola
            /*funkce pro cekani po vymezenou dobu (plavba)*/
              struct timespec req = {0};
              req.tv_sec = 0;
              req.tv_nsec = R * 1000000L; //prepocet nanosekund na milisekundy
              nanosleep(&req, (struct timespec *)NULL);//samotne cekani
            /*povoleni vystupovani z lodi*/
              *vylodeni_sem = 0;
            /*Pokud byl kapitan serfr*/
              if( PID_serf == 2)
                {
                /*pocitadlo radku*/
                  *radkovani = *radkovani + 1;
                /*tisk na vystup*/
                  fprintf(soubor, "%d\t: SERF %d\t: capitan exits\t : %d\t: %d\n", *radkovani, serf, *NH, *NS);
                /*ukonceni kapitana*/
                  exit(99);
                }
              /*pokud byl kapitan hacker*/
              else
                {
                /*pocitadlo radku*/
                  *radkovani = *radkovani + 1;
                /*tisk na vystup*/
                  fprintf(soubor, "%d\t: HACK %d\t: capitan exits\t : %d\t: %d\n", *radkovani, hack, *NH, *NS);
                /*ukonceni kapitana*/
                  exit(99);
                }

              }
          /*vystuovani pro posadku*/
            /*pro Hackera*/
              if( PID_hacker == 0 )
                {
                /*cekani na moznost vystoupeni*/
                  for( ;*vylodeni_sem == 0; )
                  {
                    /*stani u dveri*/
                  }
                }
            /*pro serfra*/
              if( PID_serf == 0 )
                {
                /*cekani na moznost vystoupeni*/
                  for( ;*vylodeni_sem == 0; )
                    {
                      /*stani u dveri*/
                    }
                }
    /*Destruktor*/
      /*konec pro hacker*/
        if ( PID_hacker == 0 )
          {
          /*pocitadlo radku*/
            *radkovani = *radkovani + 1;
          /*vypis na vystup*/
            fprintf(soubor,"%d\t: HACK %d\t: member exits\t : %d\t: %d\n", *radkovani, hack, *NH, *NS);
          /*ukonceni procesu*/
            exit(99);
          }

        /*konec pro safera*/
          if ( PID_serf == 0)
            {
            /*pocitadlo radku*/
              *radkovani = *radkovani + 1;
            /*tisk na vystup*/
              fprintf(soubor,"%d\t: SERF %d\t: member exits\t : %d\t: %d\n", *radkovani, serf, *NH, *NS);
            /*ukonceni procesu*/
              exit(99);
            }
    }
  /*pokud byl zada spatny pocet argumentu*/
    else
    {
      fprintf(stderr, "Zadali jste spatný pocet argumentu %d\n", argc);
      fprintf(stderr, "Pro pomoc zadejde : ./proj2 hepl\n");
    }
  /*pojistka pokud by kapitan neotevrel zavoru*/
      zavora1_sem = 0;
  /*zavirani souboru*/
    fclose(soubor);
  /*dealokace sdilené pameti*/
    shm_unlink(shmKEYradkovani);
    shm_unlink(shmKEYmolo);
    shm_unlink(shmKEYnh);
    shm_unlink(shmKEYns);
    shm_unlink(shmKEYkonec);

  /*demapovani sdileno pameti*/
    munmap(vylodeni_sem, sizeof(int));
    munmap(NS, sizeof(int));
    munmap(NH, sizeof(int));
    munmap(zavora1_sem, sizeof(int));
    munmap(molo_pocet, sizeof(int));
    munmap(radkovani, sizeof(int));
    munmap(konec, sizeof(int));
  /*navratova hodnota ze vsecko je v pohode*/
	return dobry;
}
