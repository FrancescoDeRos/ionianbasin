/* programma  MISFIT1.C
 * legge un file con due colonne di nomi file sac stessa stazione e stessa componente
 * e calcola il misfit tra le due forme d'onda 
 * Autore: Mario La Rocca [mario.larocca@unical.it]; ultime modifiche: Luglio 2025
 * compilare: gcc misfit1.c  sacio.a -lm -o misfit1
 */

#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <math.h> 
#include <ctype.h>
#include <unistd.h>
#include </usr/local/sac/include/sacio.h>

# define maxndat 50000  // numero massimo di dati da leggere
# define MAX 50000      // numero massimo dati sac da leggere  
# define ncmax 3000    // numero massimo di canali da elaborare
# define nsmax 3000   // numero massimo di stazioni 
# define lwmax 50000  // numero massimo di campioni finestra mobile

float fsam;        // frequenza di campionamento 

typedef struct
{ 	int flg_c, flg_p, pol, cmp;
 	float x, y;
 	char nome[50];
        char nomesta[10];
} 	LISTA; 

LISTA lista[ncmax+1];


int oops(char *s)
{   fprintf(stderr,"ERRORE FATALE: %s\n",s);
    exit(0);
}


void base(float temp[], int ndat) 
{  float media, sum = 0.0;         int i; 
   for (i=0; i<ndat; ++i)   sum = sum + temp[i]; 
   media = sum/ndat; 
   printf ("In BASE: ndat = %d, media = %f\n", ndat, media); 
   for (i=0; i<ndat; ++i)   temp[i] = temp[i] - media; 
}

void fb2(float fc, int lowpass, float temp[], int n) 
{  int j;  
   float y[maxndat]; 
   float a, b, c, d, a0, a1, a2, b1, b2; 
   float amort, pi; 
//   amort = (float)0.47;
   amort = (float)0.65;
   pi = (float)3.14159; 
      if (fc == 0) oops ("Frequenza zero per il filtro");
      a = (float)(tan((double)(pi*fc/fsam))); 
      b = 2 * a * a - 2; 
      c = 1 - 2*amort*a + a*a;
      d = 1 + 2*amort*a + a*a;      
   if (lowpass == 1) 
     {   a0 = a * a / d; 
         a1 = 2 * a0; 
     } 
   else if (lowpass == 0) 
     {  a0 = (float)1 / d; 
     	a1 = -2 * a0; 
     } 
     a2 = a0;    b1 = -b / d;   b2 = -c /d;  
     y[0] = temp[0];     y[1] = temp[1]; 
for (j=2; j<n; ++j) 
    y[j] = a0*temp[j] + a1*temp[j-1] + a2*temp[j-2] + b1*y[j-1] + b2*y[j-2]; 
for (j=0; j<n; ++j) 
    temp[j] = y[j]; 
}



void invert(float input[], int ndat)
{ int i; 
  float y[maxndat];  
for (i=0; i<ndat; ++i) 
    y[i] = input[ndat-i-1];  
for (i=0; i<ndat; ++i) 
    input[i] = y[i]; 
} 



void filtra(float yarray[], int nd, float fl, float fh)
{   int i, lowpass, lp, hp;
float fn;
if(fh < fl)
        oops("Correggere le frequenze per il filtro");
if(fl <= 0)
    hp = 0;
else
    hp = 1;

fn = fsam/2;
if(fh >= fn)
    lp = 0;
else
    lp = 1;

if(lp == 1)
{   printf("Filtro Passa Basso %f Hz\n", fh);
    lowpass = 1;
    fb2(fh, lowpass, yarray, nd);
    invert(yarray, nd);
    fb2(fh, lowpass, yarray, nd);
    invert(yarray, nd);
}

if(hp == 1)
{   printf("Filtro Passa Alto %f Hz\n", fl);
    base(yarray, nd);
    lowpass = 0;
    fb2(fl, lowpass, yarray, nd);
    invert(yarray, nd);
    fb2(fl, lowpass, yarray, nd);
    invert(yarray, nd);
}
}



float rspk(float dati[], int n) //rimuove spikes dal segnale
{   int i=0, j, n1=500, n2, ns=0, k;
float rms, d, med;

while(i<n)
{   nw:  med=n2=0;
    for(j=0;  j<n1;  j++)
    {   k=i+j;
        if(k<n)
        {   med = med + dati[k];
            ++n2;
        }
    }
    med=med/n2;
    rms=0;
    for(j=0;  j<n2;  j++)
    {   k=i+j;
        rms = rms + (dati[k]-med)*(dati[k]-med);
    }
    rms=(float)sqrt(rms/n2);
if(n2!=n1)
    printf("i = %d, n1 = %d, n2 = %d\n", i, n1, n2);

    for(j=0;  j<n2;  j++)
    {   k=i+j;
        if(k<n)
        {   d=dati[k+1]-dati[k];
            if(fabs(d) > 10*rms)
            {   dati[k+1] = dati[k];
                ++ns;
            }
        }
    }
    i=i+n2;
    if(i<n)
        goto nw;
}
printf("i = %d,  j = %d\n", i, j);
printf("Individuati e corretti %d spikes \n", ns);
}



// --------   MAIN PROGRAM CROSS-CORRELAZIONE NORMALIZZATA   --------
main (int argc, char *argv[]) 
{  int ncamp, nini, i, j, j2, jm, k, ki, kj, l, l2, ns, nerr;
int isign=1, index, co, iflag, oflag, wflag, lflag, hflag, aflag, sflag;
int lwin, kk, ii, jj, m, md, nskip, n, bestlag;
int npt, npt1, npt2, max = MAX, jdat;
float fl, fh, beg, del1, del2, wins, skips;
static float yarray[maxndat], data[3][maxndat], cc[maxndat];
double somma, quad, sln, mis_a, mis_an, norm1, norm2, cmax, R1, dot, misfit, maxcorr;
static char sta1[10], sta2[10], cmp1[10], cmp2[10], linestring[200], KSTNM[10];  
static char filename[80], fileinp[80], fileout[80], fileout2[80], KCMPNM[10];
static char kname1[100], kname2[100], *cvalue = NULL;
double misfit_global = 0.0;
int nstations = 0;
FILE *in, *out;

iflag = wflag = oflag = lflag = hflag = sflag = 0;

if (argc < 2)
{   printf("\nmisfit1 -i inputfile [-w -l -h]\n");
    printf("-i [stringa] Nome file input (necessario) \n");
    printf("-o [stringa] Nome file output \n");
//    printf("-w [numero] lunghezza finestra mobile (s)\n");
//    printf("-l [numero] frequenza di taglio inferiore \n");
//    printf("-h [numero] frequenza di taglio superiore \n");
//    printf("-s [numero] secondi da saltare all'inizio \n");
    printf("\n");
    exit(0);
}

opterr = 0;
while((co = getopt(argc, argv, "i:o:l:h:s:w:")) != -1)
  switch (co)
  { case 'i':
      iflag = 1;              cvalue = optarg;
      strcpy(fileinp, optarg);          break;
    case 'o':
      oflag = 1;              cvalue = optarg;
      strcpy(fileout, optarg);          break;
    case 'l':
      lflag = 1;              cvalue = optarg;
      fl = atof(cvalue);                break;
    case 'h':
      hflag = 1;              cvalue = optarg;
      fh = atof(cvalue);                break;
    case 'w':
      wflag = 1;              cvalue = optarg;
      wins = atof(cvalue);              break;
    case 's':
      sflag = 1;              cvalue = optarg;
      skips = atof(cvalue);             break;
    case '?':
    if (optopt == 'i' || optopt == 'o' || optopt == 'w' || optopt == 'l' || optopt == 'h')
        fprintf (stderr, "L'opzione -%c richiede un argomento.\n", optopt);
    else if (isprint (optopt))
        fprintf (stderr, "Opzione sconosciuta `-%c'.\n", optopt);
    else
        fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
        return 1;
    default:
        abort ();
  }

for(index = optind; index < argc; index++)
    printf ("Non-option argument %s\n", argv[index]);

if(iflag == 0)
    oops("L'opzione -i inputfile e' necessaria");
printf("-----   misfit1, versione Luglio 2025 [mario.larocca@unical.it]  -----\n");
printf("iflag = %d, oflag = %d, lflag = %d, hflag = %d, wflag = %d, ", iflag, oflag, lflag, hflag, wflag);
if(oflag == 0) 
    sprintf(fileout, "misfit.tempfile.txt"); 
//printf("sflag = %d \n", sflag); 

if(lflag == 1)
    printf("fl = %f Hz\n", fl);
if(hflag == 1)
    printf("fh = %f Hz\n", fh);

out = fopen(fileout, "w"); 
in = fopen(fileinp, "r");
while(feof(in) == 0)
{   leggi: ;
    fgets (linestring, 200, in);
    if(feof(in) != 0)  	break; 
    sscanf(linestring, "%s %s", &kname1, &kname2);
//  lettura primo file, dati reali  
    rsac1(kname1, yarray, &npt1, &beg, &del1, &max, &nerr, strlen(kname1));
    if(nerr > 0)
    { 	printf("In rsac: nerr = %d\n", nerr);
       	printf("Errore nella lettura dei dati.  file %d\n", kname1);
 	goto leggi;
    }
    getkhv("KSTNM", &KSTNM, &nerr, 5, 8);
    if(nerr > 0)  exit(nerr);
    sscanf(KSTNM, "%s", &sta1);
    getkhv("KCMPNM", &KCMPNM, &nerr, 6, 8);
    if(nerr > 0)  exit(nerr);
    sscanf(KCMPNM, "%s", &cmp1);

//    printf("%s  npt1 %d   del1 %f\n", kname1, npt1, del1);
    for(jm=0;  jm<npt1;  jm++)
        data[1][jm] = yarray[jm];

// lettura secondo file, dati sintetici  
    rsac1(kname2, yarray, &npt2, &beg, &del2, &max, &nerr, strlen(kname2));
    if (nerr > 0)
    {   printf("In rsac: nerr = %d\n", nerr);
        printf("Errore nella lettura dei dati.  file  %d\n", kname2);
        goto leggi;
    }
    getkhv("KSTNM", &KSTNM, &nerr, 5, 8);
    if(nerr > 0)  exit(nerr);
    sscanf(KSTNM, "%s", &sta2);
    getkhv("KCMPNM", &KCMPNM, &nerr, 6, 8);
    if(nerr > 0)  exit(nerr);
    sscanf(KCMPNM, "%s", &cmp2);

//    printf("%s  npt2 %d  del2 %f\n", kname2, npt2, del2);
    for(jm=0;  jm<npt2;  jm++)
 	data[2][jm] = yarray[jm];
    if(fabs(del1-del2) > 0.0001)
    {   printf("Attenzione: i file hanno diverso delta, non vengono elaborati \n");
 	goto leggi;
    }

    npt = npt1;
    if(npt2 < npt) 
	npt = npt2; 
    somma = quad = mis_a = 0;
    for(i=0;  i<npt;  i++)
    {   somma = somma + del1*(data[1][i]-data[2][i])*(data[1][i]-data[2][i]);
	quad = quad + del1*data[1][i]*data[1][i];
 	sln = log(fabs(data[1][i]/(data[2][i])));
 	mis_a = mis_a + sln*sln;
    }
    mis_an = somma/quad; 

// calcolo della cross-correlazione normalizzata
    for(i=0;  i<3*npt+1;  i++)
        cc[i] = 0;
    for(i=0;  i<2*npt-1;  i++)
    {   for(j=0;  j<2*npt;  j++)
        {   k = j+npt-i;
            if(k >= 0)
                cc[i] = cc[i] + data[2][j]*data[1][k];
//if(i < 10 && j < 10)  printf("%f  %f\n", y1[k], y2[j]); 
        }
    }
    norm1 = norm2 = cmax = 0;
    for(j=0;  j<npt;  j++)
    {   norm1 = norm1 + data[1][j]*data[1][j];
        norm2 = norm2 + data[2][j]*data[2][j];
    }
//      printf("norm1 = %f,  norm2 = %f \n", norm1, norm2);
    for(i=0;  i<2*npt-1;  i++)
    {   if((norm1 != 0) && (norm2 != 0))
            cc[i] = cc[i]/sqrt(norm1*norm2);
        else
           cc[i] = 0;
        if(cc[i] > cmax)
            cmax = cc[i];
    }
	//ritsema2002+tromp2005
    bestlag = 0;
    maxcorr = -1e30;
    for(i=0; i<3*npt+1; i++)
        cc[i] = 0;

    for(i=0; i<2*npt-1; i++) {
        for(j=0; j<2*npt; j++) {
            k = j+npt-i;
            if(k >= 0 && k < npt) {
                cc[i] += data[2][j] * data[1][k];
            }
        }
        if(cc[i] > maxcorr) {
            maxcorr = cc[i];
            bestlag = i - (npt-1);   // lag relative to center
        }
    }
    //phi_ds and phi_ss at tau_m
    double phi_ds = 0.0, phi_ss = 0.0;
    for(j=0; j<npt; j++) {
        int k = j + bestlag;
        if(k >= 0 && k < npt) {
            phi_ds += data[1][j] * data[2][k];  
        }
        phi_ss += data[2][j] * data[2][j];      
    }
    R1 = 0.0;
    if(phi_ss != 0.0) {
        R1 = phi_ds / phi_ss;
    }
	double diff = R1 - 1.0;
	misfit_global += diff * diff;
	nstations++;
	misfit = 0;
	for (j = 0; j < npt; j++) {
		double diff = data[1][j] - data[2][j];
		misfit += diff * diff;
	}
	misfit = sqrt(misfit / npt);
	dot = 0.0;
	for (j = 0; j < npt; j++) {
		dot += data[1][j] * data[2][j];
	}
	R1 = dot / sqrt(norm1 * norm2);   // already normalized
    fprintf(out, "%s  %s  %s  %s  %f  %f  %f %f  %f %f\n", sta1, cmp1, sta2, cmp2, mis_an, mis_a, cmax, bestlag, R1, misfit); 

}  // while(feof(in))
double chi = 0.5 * misfit_global;
printf("glob amplitude misfit chi(m) = %.6f (N=%d stations)\n", chi, nstations);
fclose(in);     fclose(out);
printf(" - - - -  F I N E  - - - - \n");
exit(0); 
} 

