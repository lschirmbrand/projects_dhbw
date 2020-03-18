/*
Programm öffnet die CSV-Datei, im Falle eines Fehlschlags wird ein NULL-Pointer zurück gegeben, in diesem Fall wird eine Fehlermeldung ausgegeben.
Im Falle eines erfolgreichen Öffnens wird der Filestream aktiviert: Zuerst wird der Zeiger in der Datei auf den Anfang gesetzt, sodass er immer am Anfang ist.
Danach wird ein char c genutzt, um Char für Char den Filestream zu überprüfen.
Falls der Char ein Komma oder 13/10/1310 einliest, wird ein Eintrag hinzugefügt, beziehungsweise die Einträge um 1 erhöht.
Im nachfolgenden Schritt werden die Ausmaße der Gesamtmatrix erfasst.
Die Gesamtmatrix setzt sich aus einer quadratischen Matrix und einem Vektor b, der neben der quadratischen Matrix steht, zusammen.
Um das Verhältnis der Matrix auszurechnen, machen wir uns genau diese Information zu Nutzen, es resultiert:
Einträge = Zeilen*(Zeilen+1) = Zeilen * Spalten,



Folgend wird ein CharArray initialisiert, das einen String speichern soll.
Zusätzlich wird ein tempchar initialisiert der das gelesene Element temporär speichert.
Bei jeder Zeigerbewegung wird der String um temp erweitert, temp variiert und ist mal das Zeichen, mal die Terminierung des Strings, also das Stringende.
Wenn der numberstring voll ist, bzw endet, weil er terminiert wird, wird der string zu double konvertiert und number zugeschrieben.

// kurz gesagt, die csv datei wird geparst


Direkt darauf wird der Wert einer Zelle der tempMatrix zugewiesen, danach wird der Index des Arrays/der Matrix verschoben, in den dann wieder die neue number abgespeichert wird.
Im Falle eines Zeilenumbruchs wird der Index des Array in der Zeile um 1 erhöht und in der Spalte auf 0 gesetzt.
Dadurch wird das gesamte Array richtig abgespeichert, mit allen aus der csv-Datei übergeben Werten.
Im nächsten Schritt sollen alle Nullzeilen ignoriert werden, das heißt wir schieben sie an das Zeilenende des Arrays und ignorieren sie später bei der Übergabe an die
richtige Matrix und den Vektor, beziehungsweise passen die Arraygröße auf Zeilen-Nullzeilen an.
Danach wird die (neue&richtige) Matrix mit allen Werten der tempMatrix von 0<=x<Spalten-1 besetzt für alle Zeilen abzüglich der Nullzeilen.
Der Vektor wird aus allen Werten der Spalte in Reihenfolge der Zeilen, wieder ohne Nullzeilen, deklariert.
*/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    int n;
    double **data;
} Matrix;
typedef struct
{
    int n;
    double *data;
} Vector;
typedef enum
{
    JACOBI = 0, GAUSS_SEIDEL = 1
} Method;

int main() {
    Matrix A;
    Vector b;
    Vector x;
    double e = 0.000000000001;
    Method m = JACOBI;
    bool l = load(&A, &b, &x);

    if(l){
        printf("Datei erfolgreich geladen.\n\nWelches Verfahren?\nSchreibe '0' für Jacobi oder '1' für Gauß-Seidel: ");
        int auswahl;
        scanf("%d",&auswahl);
        if(auswahl == 0) m = JACOBI;
        else if(auswahl == 1) m = GAUSS_SEIDEL;
        solve(m, &A, &b, &x, e);
    }
    else printf("Die Datei konnte nicht geladen werden.\n\n\n");

    return 0;
}

int load (Matrix *A, Vector *b, Vector *x){
    //Datei öffnen
    FILE *fp;
    fp = fopen("konv3.csv", "r");
    //wenn datei ungültig ist kann gleich aufgehört werden
    if (fp==NULL) return false;

    const int length = 30;
    char numberstring[length];              //Einführung numberstring, CharArray dem "temp" immer wieder hinzugefügt wird, gibt Zahl einer Zelle im Stringformat aus, Array = (gebrauchte Größe + 1), weil terminierende 0
    long double number = 0;                 //Einführung number, hat Wert der einzelnen Zellen der Matrix
    char temp = 0;                          //Einführung temp: Je nachdem was "char c" einliest, wird temp dessen Wert zugewiesen
    numberstring[0] = '\0';                 //Das CharArray wird leer initialisiert, die terminierende 0 am steht am Ende

//zählen der enträge
    int eintraege = countEintraege(&fp);
    printf("Eintraege:\t%d\n", eintraege);
//ermitteln wie viele zeilen und spalten
    int zeilen = getZeilen(eintraege), spalten = zeilen+1;
    printf("Zeilen:\t\t%d\nSpalten:\t%d\n", zeilen, spalten);


    A -> data = (double**)malloc(sizeof(double*) * zeilen);
    for(int x = 0; x < zeilen; x++){
        A -> data[x] = (double*)calloc(zeilen, sizeof(double));
    }
    b -> data = (double*)malloc(sizeof(double*) * zeilen);
    A -> n = zeilen;
    b -> n = zeilen;

    long double tempMatrix[zeilen][spalten];
    tempMatrix[0][0] = '/0';

    int iSpalte = 0, iZeile = 0, aNullZeilen = 0;
    fseek(fp, 0, SEEK_SET);         //Dateizeiger auf Anfang setzen
    char c;
    while((c=fgetc(fp)) != EOF) {                   //Schleife, s.o
        temp = c;
        if (c == '\n'||c=='\r') {                   //falls Zeilenumsprung gelesen wird
            temp = '\0';
            strncat(numberstring, &temp, 1);        //CharArray wird verkettet
            number = strtold(numberstring,'\0');    //Numbertostring wird zu double konvertiert und in Number gespeichert
            tempMatrix[iZeile][iSpalte] = number;
            iZeile++;                                   //Sprung in nächste Zeile
            iSpalte = 0;                                 //Spalte wird wieder auf 0 gesetzt
            numberstring[0] = '\0';                 //CharArray wird gecleared
        }
        if (c == ',') {                             //Falls Komma gelesen wird
            temp = '\0';
            strncat(numberstring, &temp, 1);        //CharArray wird verkettet
            number = strtold(numberstring,'\0');    //Numbertostring wird zu double konvertiert und in Number gespeichert
            tempMatrix[iZeile][iSpalte] = number;
            iSpalte++;                                   //Spalte wird um eins erhöht, also nach rechts geschoben
            numberstring[0] = '\0';                 //CharArray wird gecleared
        }
        if (c!='\n' && c != ',' && c != '\r'){      //Wenn kein Zeilenumbruch oder Komma gelesen wird -> Zahl wird als Char eingelesen
            strncat(numberstring, &temp, 1);        //Verkettung zu CharArray/String
        }
    }

//nullzeilen löschen
    for (int z = 0; z<zeilen; z++){
        for (int i = 0; i<spalten; i++){
            if(tempMatrix[z][i]!=0) break;                //sobald ein Zeichen !=0 gelesen wird , wird Suche der Zeile abgebrochen, kann keine Nullzeile sein
            else{
                if (i==spalten-1 && tempMatrix[z][i] == 0){      //Wenn in letzter Spalte der Zeile 0 und noch kein Break erfolgt -> Alle Einträge der Zeile gleich 0
                    aNullZeilen++;                             //Protokolierung der Nullzeilen
                    for(int zv=z; zv<zeilen-1; zv++){        //Schleife zur Verschiebung der Elemente, hier Veränderung der Zeile
                        for(int sv = 0; sv<spalten;sv++){    //Schleife zur Verschiebung der Elementem hier Veränderung der Spalte
                            tempMatrix[zv][sv]=tempMatrix[zv+1][sv]; //Elemte aus Zusammensetzung [Zeile][Spalte] überschreiben Nullzeile
                        }
                    }
                    for(int nz = 0; nz<spalten;nz++){
                        tempMatrix[zeilen-1][nz] = 0;           //Überschriebene Nullzeilen werden an Matrix unten wieder angehängt
                    }
                }
            }
        }
    }

//tatsächliche matrix + vektor
    aNullZeilen /= 2;
    const int  maxsteps = 10000;

    for(int vz = 0; vz<zeilen-aNullZeilen; vz++)
        b -> data[vz] = tempMatrix[vz][spalten-1];    //Vektor = tempMatrix-Matrix
    for(int mz=0; mz<zeilen-aNullZeilen; mz++)
        for(int ms=0; ms<spalten-1; ms++)
            A -> data[mz][ms]=tempMatrix[mz][ms];     //Quadratische Matrix = tempMatrix -Vektor

    printf("Nullzeilen:\t%d\n\n\n", aNullZeilen);
    fclose(fp);
    return true;
}

int solve (Method method, Matrix *A, Vector *b, Vector *x, double e){

    int zeilen = A -> n, spalten = A -> n + 1, aNullZeilen=0;

//--------------------------------------------------------//--------------------------------------------------------//--------------------------------------------------------//--------------------------------------------------------

    //Berechnungansatz Jacobi-Verhalten
    double vorher[zeilen], nachher[zeilen], minDiff = 0, diff = 0;
    long double xneu[zeilen-aNullZeilen], xstart[zeilen-aNullZeilen];
    int schritt = 0, maxSchritte = 100;

    if(method == JACOBI){
        do{
            for(int i = 0; i < zeilen-aNullZeilen; i++)
                xneu[i] = b-> data[i];
            for(int i = 0;i<zeilen-aNullZeilen; i++)
            {
                for(int j = 0;j<zeilen-aNullZeilen; j++)
                    if(i!=j)
                         xneu[i] = xneu[i]-A->data[i][j]*xstart[j];
                xneu[i] = xneu[i]/A->data[i][i];
            }
            for(int i = 0; i < zeilen-aNullZeilen; i++)
            {
                xstart[i] = xneu[i];
                vorher[i] = nachher[i];
                nachher[i] = xstart[i];
                double diff = vorher[i] - nachher[i];
                if(diff < 0) diff = diff * (-1);        // differenz zwischen vorher nachher
                if(diff < minDiff || schritt == 0) minDiff = diff;
            }
            schritt++;
        }while(minDiff > e || schritt > maxSchritte);

        for (int i = 0; i<zeilen-aNullZeilen; i++)
            printf("x%d = %.10Lf\n", i, nachher[i]); //Ausgabe
        printf("Schritte: %d\n\n",schritt);
    }




    else if(method == GAUSS_SEIDEL){


//--------------------------------------------------------//--------------------------------------------------------//--------------------------------------------------------
        //Berechnungsansatz Gauß Seidel Verfahren

       //Iterationsschleife bis Konvergenz erreicht:
       double normNeu;
       double normAlt;

       do{
                /* Normwerte := 0 damit Differenz bei jedem
                 * neuen Schritt neu berechnet werden kann:*/
                normNeu = 0;
                normAlt = 0;
                    //Zeilenschleife:
                    for (int i = 0; i < zeilen - aNullZeilen; i++)
                    {
                        xneu[i] = 0;
                        //Spaltenschleifen:
                        for (int j = 0; j < i; j++)
                            xneu[i] = xneu[i] + (A->data[i][j] * xneu[j]);
                        for (int j = i + 1; j < zeilen-aNullZeilen; j++)
                            xneu[i] = xneu[i] + (A->data[i][j] * xstart[j]);
                        xneu[i] = (b->data[i] - xneu[i]) / A->data[i][i];
                    }
                //Berechnung NormNeu&NormAlt:
                for (int k = 0; k < zeilen - aNullZeilen; k++)
                {
                    normNeu += abs(xneu[k] * xneu[k]);
                    normAlt += abs(xstart[k] * xstart[k]);
                }

    //fehlt noch implementierung von fehlerschranke

                for (int copy = 0; copy < zeilen-aNullZeilen; copy++)
                    xstart[copy] = xneu[copy];

                schritt++;

            }while(schritt < maxSchritte);
        for(int p = 0; p < zeilen - aNullZeilen; p++)
            printf("x%d = %.10Lf\n",p, nachher[p]);
        printf("schritte: %d",schritt);
    }



    return 0;
}


int countEintraege(){
    FILE *fp;
    fp = fopen("testdateien/konv3.csv", "r");

    int i = 0;
    char c;
    bool prevcharbreak = false;
    fseek(fp, 0, SEEK_SET);                 //Dateizeiger auf Anfang setzen

    while((c=fgetc(fp)) != EOF) {             //while Dateiende nicht erreicht
       char temp = c;                               //temp = momentanes Zeichen
       if ((c == '\n' && prevcharbreak==false)|| c == ',') {    //if c = Zeilenumbruch oder Komma
           i++;
       }
       if (c=='\r') {                          //if c = Zeilenumbruch oder Komma
           i++;
           prevcharbreak = true;               //daduch wird im Falle von Windows und co. \r\n umgangen, sodass die Einträge nicht doppelt gezählt werden
       }
     }

     fclose(fp);
     return i;

}
int getZeilen(int eintraege){
    int zeilen = 0;
    if (eintraege > 0){
        for (int i = 0; i<1000; i++)
            if(eintraege == zeilen*(zeilen+1))
                break;
            else zeilen++;
        return zeilen;
    }
    else return 0;
}



