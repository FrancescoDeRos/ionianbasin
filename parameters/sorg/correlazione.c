/* programma    CORRELAZIONE.C
 * calcola la cross-correlazione tra due sismogrammi su finestra mobile.
 * I due sismogrammi vengono filtrati nella banda richiesta.
 * Autore: Mario La Rocca;  Ultima revisione: Febbraio 2016.
 * compilare: gcc correlazione.c /usr/local/sac/lib/sacio.a -lm -o correlazione
 */

#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <math.h> 

# define maxndat 500000       // numero massimo di dati da memorizzare
# define MAX 500000          // numero massimo dati sac da leggere  

float fsam;      // frequenza di campionamento 


int oops(char *s)
{   fprintf(stderr,"ERRORE FATALE: %s\n",s);
    exit(0);
}

/* ----------  calcolo e sottrazione della media  --------- */
void base(float temp[], int ndat) 
{  float media, sum = 0;         int i; 
   for(i=0;  i<ndat;  ++i)   sum = sum + temp[i]; 
   media = sum/ndat; 
   printf("In BASE, ndat = %d, media = %f\n", ndat, media); 
   for(i=0;  i<ndat;  ++i)   temp[i] = temp[i] - media; 
   printf("tolta la media \n"); 
}


/* ----------- filtro di ordine 2 dei dati  ----------- */
void fb2(float fc, int lowpass, float *temp, int n) 
{  int j;  
   static float y[MAX]; 
   float a, b, c, d, a0, a1, a2, b1, b2; 
   float amort, pi; 
//printf("fc, lowpass, n = %f %i %ld\n", fc, lowpass, n); 
   amort = (float)0.65;   pi = (float)3.14159; 
      if (fc == 0) oops ("Frequenza zero per il filtro");
      a = (float)(tan((double)(pi*fc/fsam))); 
      b = 2 * a * a - 2; 
      c = 1 - 2*amort*a + a*a;
      d = 1 + 2*amort*a + a*a;      
   if (lowpass == 1) 
     {   a0 = a * a / d; 
     a1 = 2 * a0; } 
   else if (lowpass == 0) 
     {   a0 = (float)1 / d; 
     a1 = -2 * a0; } 
     a2 = a0;    b1 = -b / d;   b2 = -c /d;  
/*   printf("inside function fb2. fsam, n, fc, a: %i  %i  %f  %f \n",fsam,n,fc, a);
printf ("inside function fb2. lowpass, a0, a1, a2, b, b1, b2, c, d: \n %i %f %f %f %f %f %f %f %f\n", lowpass, a0, a1, a2, b, b1, b2, c, d); */
     y[0] = temp[0];     y[1] = temp[1]; 
for (j=2; j<n; ++j) 
    y[j] = a0*temp[j] + a1*temp[j-1] + a2*temp[j-2] + b1*y[j-1] + b2*y[j-2]; 
for (j=0; j<n; ++j) 
    temp[j] = y[j]; 
}


/* ---------- inversione del tempo per filtro backward ---------- */
void invert(float input[], int ndat)
{ int i; 
  static float y[MAX];  
/*printf (" function invert ndat = %i\nplot 'seism' with line", ndat); */
for (i=0; i<ndat; ++i) 
    y[i] = input[ndat-i-1];  
for (i=0; i<ndat; ++i) 
    input[i] = y[i]; 
 } 


/* --------  filtro passa banda forward e backward ------------- */
void filter(float *temp, int ndat, float fl, float fh, float *fildat) 
{ int lowpass, i; 
     lowpass = 1;
	 fb2(fh, lowpass, temp, ndat); 
     lowpass = 0; 
       fb2(fl, lowpass, temp, ndat);   
       invert(temp, ndat); 
	 lowpass = 1; 
       fb2(fh, lowpass, temp, ndat); 
     lowpass = 0; 
       fb2(fl, lowpass, temp, ndat); 
       invert(temp, ndat); 
 	for (i=0; i<ndat; ++i) 
 	{  fildat[i] = temp[i];  
 	/* printf ("fildat =  %f\n", fildat[i]); */ 
 	} 
} 



int main (int argc, char *argv[]) 
{  int nwin, lwin, i, j, k, kk, jk, kt, N; 
int nerr, NZYEAR, NZJDAY, NZHOUR, NZMIN, NZSEC, NZMSEC;
int nlen1, nlen2, ndat, max = MAX;
float fl, fh, cmax, tshift, tempo, wins, norm, norm1, norm2; 
static float temp[MAX], fildat1[MAX], fildat2[MAX], ts[10000]; 
static float yarray1[MAX], yarray2[MAX], cc[10000], cm[10000][100], y1[10000], y2[10000]; 
float beg, beg1, beg2, del, del1, del2;
static char file1[60], file2[60], fileout[60], fileouts[60];
char cwins[10], cfl[10], cfh[10];
FILE *out1, *out2, *out3; 

if (argc < 7)  oops("Digitare: correlazione file1 file2 fileout wins(s) fl fh \n"); 
strcpy(file1, *++argv); 
strcpy(file2, *++argv); 
strcpy(fileout, *++argv); 
strcpy(cwins, *++argv); 
strcpy(cfl, *++argv); 
strcpy(cfh, *++argv); 

wins = atof(cwins);
fl=atof(cfl);
fh=atof(cfh);

printf("file1 = %s,  file2 = %s,  fileout = %s \n", file1, file2, fileout);
printf("wins = %f secondi,  fl = %f Hz,  fh = %f Hz \n", wins, fl, fh);

rsac1(file1, yarray1, &nlen1, &beg1, &del1, &max, &nerr, strlen(file1));

if (nerr > 0)
        {  printf("In rsac: nerr = %d\n", nerr);
	   oops("Errore nella lettura dei dati. ");
        }
//printf("letto il file %s, nlen1 = %d\n", file1, nlen1);
fsam = 1/del1;

// ------ lettura header del primo file sac  ------
getnhv("NZYEAR", &NZYEAR, &nerr, 6);
        if(nerr > 0)  exit(nerr);
getnhv("NZJDAY", &NZJDAY, &nerr, 6);
        if(nerr > 0)  exit(nerr);
getnhv("NZHOUR", &NZHOUR, &nerr, 6);
        if(nerr > 0)  exit(nerr);
getnhv("NZMIN", &NZMIN, &nerr, 5);
        if(nerr > 0)  exit(nerr);
getnhv("NZSEC", &NZSEC, &nerr, 5);
        if(nerr > 0)  exit(nerr);
getnhv("NZMSEC", &NZMSEC, &nerr, 6);
 	if(nerr > 0)  exit(nerr);

rsac1(file2, yarray2, &nlen2, &beg2, &del2, &max, &nerr, strlen(file2));

if (nerr > 0)
        {  printf("In rsac: nerr = %d\n", nerr);
	   oops("Errore nella lettura dei dati. ");
        }
//printf("letto il file %s, nlen2 = %d\n", file2, nlen2);
//  ------------  Filtro dati  --------------- 
    base(yarray1, nlen1);
    base(yarray2, nlen2);
    filter(yarray1, nlen1, fl, fh, fildat1);
    filter(yarray2, nlen2, fl, fh, fildat2);

ndat=nlen1;
if(nlen2<ndat)
    ndat=nlen2;
if(maxndat < ndat) 	ndat = maxndat; 

out2=fopen("datifil.dat", "w");
for(i=0;  i<ndat;  i++)
    fprintf(out2, "%f %f\n", fildat1[i], fildat2[i]);
fclose(out2);

lwin=wins*fsam+0.1;
printf("Frequenza di campionamento: %f,  lwin = %d\n", fsam, lwin);
printf("Durata finestra mobile: %d campioni = %f secondi \n", lwin, wins);

nwin=ndat/lwin;
printf("numero di finestre = %d\n", nwin);
for(kk=1;  kk<=nwin;  ++kk)
{ //  jk=(kk-1)*lwin/2;
    jk=(kk-1)*lwin;
    printf("Finestra %d di %d, jk = %d\n", kk, nwin, jk);
    for(i=0;  i<3*lwin+1;  i++)
    {   cc[i] = 0;
 	y1[i] = 0;
 	y2[i] = 0;
    }
    for(i=0;  i<lwin;  i++)
    {	y1[i+lwin] = fildat1[i+jk];
 	y2[i+lwin] = fildat2[i+jk];
    }

    for(i=0;  i<2*lwin-1;  i++)
        for(j=0;  j<2*lwin;  j++)
        {   k = j+lwin-i;
            if(k >= 0)
                cc[i] = cc[i] + y2[j]*y1[k];
        }
    norm1 = norm2 = cmax = 0;
    for(j=lwin;  j<2*lwin;  j++)
    {   norm1 = norm1 + y1[j]*y1[j];
        norm2 = norm2 + y2[j]*y2[j];
    }
//  printf("norm1 = %f,  norm2 = %f \n", norm1, norm2);
    for(i=0;  i<2*lwin-1;  i++)
    {   if((norm1 != 0) && (norm2 != 0))
            cc[i] = cc[i]/sqrt(norm1*norm2);
        else
            cc[i] = 0;
    }
    for(i=0;  i<2*lwin;  i++)
        cm[i][kk] = cc[i];
}

out1 = fopen(fileout, "w");
for(i=0;  i<2*lwin;  i++)
{   for(kk=1;  kk<=nwin;  kk++)
 	fprintf(out1, "%f ", cm[i][kk]);
    fprintf(out1, "\n");
} 
fclose(out1); 

/*
// header file out
beg=(lwin/2)/fsam;
del=beg;
printf("begin = %f,  delta = %f \n", beg, del);

setnhv("NPTS", &nwin, &nerr, 4);
        if(nerr > 0)    exit(nerr);
setfhv("B", &beg, &nerr, 1);
        if(nerr > 0)    exit(nerr);
setfhv("DELTA", &del, &nerr, 5);
        if(nerr > 0)    exit(nerr);
setnhv("NZYEAR", &NZYEAR, &nerr, 6);
        if(nerr > 0)    exit(nerr);
setnhv("NZJDAY", &NZJDAY, &nerr, 6);
        if(nerr > 0)    exit(nerr);
setnhv("NZHOUR", &NZHOUR, &nerr, 6);
        if(nerr > 0)    exit(nerr);
setnhv("NZMIN", &NZMIN, &nerr, 5);
        if(nerr > 0)    exit(nerr);
setnhv("NZSEC", &NZSEC, &nerr, 5);
        if(nerr > 0)    exit(nerr);
setnhv("NZMSEC", &NZMSEC, &nerr, 6);
        if(nerr > 0)    exit(nerr);

sprintf(fileouts, "%s.sac", fileout);
remove(fileouts);
wsac0(fileouts, ts, ts, &nerr, strlen(fileouts));
printf("scritto il file  %s \n", fileouts);
*/

//fclose(out3);
printf(" - - - -  F I N E  - - - - \n");
exit(0); 
} 

