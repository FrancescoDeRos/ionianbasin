/* programma  CORRNORM.C
 * calcola la cross-correlazione normalizzata su finetra mobile di durata prestabilita 
 * tra N segnali filtrati e scrive il massimo in una matrice. 
 * Autore: Mario La Rocca [mario.larocca@unical.it]; ultime modifiche: Aprile 2020
 * compilare: corrnorm.c sacio.a -lm -o corrnorm
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
int lwin, kk, ii, jj, m, md, nskip, n;
int nlen, min_nlen, max = MAX, jdat;
float pmax, pinc, dp, distan, pxmax, pymax, px, py, px0, py0, pd;
float fl, fh, advance, amp, norm1, norm2, cmax, ans, skips, wins, sum, datum;
static float xc[ncmax], yc[ncmax], t[ncmax], temp[maxndat], fildat[maxndat];
static float yarray[maxndat], rms[ncmax], data[ncmax][maxndat], y1[lwmax], y2[lwmax];
static float cc[lwmax], n1[lwmax], n2[lwmax];
float scal_pkp, scal_pks, mod_kp, mod_ks;
float ksx, ksy, ksz, kpx, kpy, kpz, vp_ave, vs_ave, azirad;
float skip_s, lwin_s, beg, del, del0, beg0, dt, dd, dm, tm;
static char comp, linestring[200], KSTNM[10], metd[10];  
static char filename[80], fileinp[80], fileout[80], fileout2[80];
static char kname[80], out_ascii[80], *cvalue = NULL;
char *p1, *p2;
FILE *in, *out1, *out2;

iflag = wflag = oflag = lflag = hflag = sflag = 0;

if (argc < 5)
{   printf("\ncorrnorm -i inputfile -o outputfile [-w -l -h]\n");
    printf("-i [stringa] Nome file input (necessario) \n");
    printf("-o [stringa] Nome file output (necessario) \n");
    printf("-w [numero] lunghezza finestra mobile (s)\n");
    printf("-l [numero] frequenza di taglio inferiore \n");
    printf("-h [numero] frequenza di taglio superiore \n");
    printf("-s [numero] secondi da saltare all'inizio \n");
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
printf("-----   corrnorm, versione Aprile 2020 [mario.larocca@unical.it]  -----\n");
printf("iflag = %d, oflag = %d, lflag = %d, hflag = %d, wflag = %d, ", iflag, oflag, lflag, hflag, wflag);
printf("sflag = %d \n", sflag); 

if(wflag == 0)
    wins = 5;
printf("wins = %f,\n", wins);
if(lflag == 1)
    printf("fl = %f Hz\n", fl);
if(hflag == 1)
    printf("fh = %f Hz\n", fh);

in = fopen(fileinp, "r");
ns = 0;
while(feof(in) == 0)
{   printf("ns = %d,  ", ns);
    leggi: ;
    fgets (linestring, 200, in);
    if(feof(in) != 0)  	break; 
    sscanf(linestring, "%s", &kname);
    if(ns >= ncmax) 	break; 
    printf("lettura del file %s\n", kname);
    rsac1(kname, yarray, &nlen, &beg, &del, &max, &nerr, strlen(kname));
    if (nerr > 0)
        { 	printf("In rsac: nerr = %d\n", nerr);
        	printf("Errore nella lettura dei dati.  ns = %d\n", ns);
 		goto leggi;
        }
    printf("letto il file sac, nlen = %d, del = %f\n", nlen, del);
    if(ns == 0) 
    {   fsam = 1/del;
 	    nskip = skips*fsam +0.01;
    }
    lwin = fsam*wins +0.01; 
    printf("ns = %d, fsam = %f \n", ns, fsam); 
    if(fabs(fsam -1/del) < 0.0001)
    {   strcpy(lista[ns].nome, kname);
        base(yarray, nlen);   //----- Rimozione del valore medio dal segnale -----
        if(lflag == 1 || hflag == 1)
  	    filtra(yarray, nlen, fl, fh);  //----- Filtro del segnale -----
// ----- memorizzazione dei dati filtrati da analizzare in data[][] -----
 	printf("nlen = %d, nskip = %d \n", nlen, nskip); 
 	jdat = nskip + lwin;
// 	if(jdat > min_nlen)     jdat = min_nlen;
  	amp = 0;
 	for(jm = nskip;  jm < jdat;  jm++)
 	{   data[ns][jm-nskip] = yarray[jm];
 	    amp = amp + yarray[jm]*yarray[jm];
 	}
 	rms[ns] = sqrt(amp/(jdat-nskip));
    printf("jdat = %d,  nskip = %d, nlen = %d, lwin = %d\n", jdat, nskip, nlen, lwin);
// ++k;
    ns++;
    }
    else
    printf("ATTENZIONE: file %s non utilizzato, delta diverso dal primo \n", kname); 
}  // while(feof(in))
fclose(in);
printf("--- Terminata la lettura e il filtraggio dei dati --- \n\n");
// ------------- terminata la lettura dei dati --------------
printf("letto il file input, trovati %d segnali, ns = %d\n", ns, ns);
if(ns > ncmax)  
{   ns = ncmax;
    printf("attenzione, troppi file. verranno elaborati solo i primi %d ", ns); 
}
printf("Frequenza di campionamento: %f campioni/secondo\n", fsam);
printf("Durata finestra mobile: %f secondi = %d campioni\n", lwin/fsam, lwin);

// calcolo della cross-correlazione normalizzata per ogni coppia di segnali
out1 = fopen(fileout, "w");
printf("aperto il file %s\n", fileout);
if(ns == 2)
    out2 = fopen("cross-corr-norm.dat", "w");
for(i=0;  i<ns;  ++i)
    fprintf(out1, "%d  %s\n", i+1, lista[i].nome);
for(ki=0;  ki<ns;  ++ki) 
{   for(kj=ki;  kj<ns;  kj++)
    {   for(i=0;  i<3*lwin+1;  i++)
 	    cc[i] = n1[i] = n2[i] = y1[i] = y2[i] = 0;
 	for(i=0;  i<lwin;  ++i)
 	{   y1[i+lwin] = data[ki][i];
 	    y2[i+lwin] = data[kj][i];
 	}
 	for(i=0;  i<2*lwin-1;  i++) 
 	    for(j=0;  j<2*lwin;  j++)
 	    {   k = j+lwin-i;
 		if(k >= 0)
 	     	    cc[i] = cc[i] + y2[j]*y1[k];
//if(i < 10 && j < 10)  printf("%f  %f\n", y1[k], y2[j]); 
 	    }
 	norm1 = norm2 = cmax = 0;
 	for(j=lwin;  j<2*lwin;  j++)
        {   norm1 = norm1 + y1[j]*y1[j];
 	    norm2 = norm2 + y2[j]*y2[j];
 	} 
// 	printf("norm1 = %f,  norm2 = %f \n", norm1, norm2);
 	for(i=0;  i<2*lwin-1;  i++)
 	{   if((norm1 != 0) && (norm2 != 0))
 		cc[i] = cc[i]/sqrt(norm1*norm2);
 	    else
 		cc[i] = 0;
 	    if(cc[i] > cmax) 
 		cmax = cc[i];
 	} 
 	fprintf(out1, "%d  %d  %4.3f  %f  %f  %s  %s\n", ki+1, kj+1, cmax, rms[ki], rms[kj], lista[ki].nome, lista[kj].nome);
    if(ns == 2)
 	for(i=0;  i<2*lwin-1;  i++)
	    fprintf(out2, "%f\n", cc[i]);
    }
}
fclose(out1); 
if(ns == 2)
    fclose(out2); 
printf(" - - - -  F I N E  - - - - \n");
exit(0); 
} 

