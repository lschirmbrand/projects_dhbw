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

typedef struct{
    int n;
    double **data;
} Matrix;

typedef struct{
    int n;
    double *data;
} Vector;

typedef enum{
    JACOBI = 0, GAUSS_SEIDEL = 1
} Method;

int countEintraege(const char filename[]){
    FILE *fp;
    fp = fopen(filename, "r");
    int i = 0;
    char c;
    bool prevcharbreak = false;
        //Dateizeiger auf Anfang setzen
    fseek(fp, 0, SEEK_SET);
        //solange Dateiende nicht erreicht
    while((c = fgetc(fp)) != EOF) {
       if ((c == '\n' && prevcharbreak == false)|| c == ',') i++;   //if c = Zeilenumbruch oder Komma
       if (c =='\r') {//if c = Zeilenumbruch oder Komma
           i++;
           prevcharbreak = true; //daduch wird im Falle von Windows und co. \r\n umgangen, sodass die Einträge nicht doppelt gezählt werden
       }
    }
    fclose(fp);
    return i;
}

int getZeilen(int eintraege){//zählt anzahl der Zeilen
    int zeilen = 0;
    if (eintraege > 0){
        for (int i = 0; i<1000; i++)
            if(eintraege == zeilen * (zeilen + 1))
                break;
            else zeilen++;
        return zeilen;
    }
    else return 0;
}

int load (const char *filename, Matrix *A, Vector *b, Vector *x){
    //Datei öffnen
    FILE *fp;
    fp = fopen(filename, "r");
    //wenn datei ungültig ist kann gleich aufgehört werden
    if (fp == NULL) return false;

//zählen der enträge
    int eintraege = countEintraege(filename);
    printf("Eintraege:\t%d\n", eintraege);
//ermitteln wie viele zeilen und spalten
    int zeilen = getZeilen(eintraege), spalten = zeilen+1;
    printf("Zeilen:\t\t%d\nSpalten:\t%d\n", zeilen, spalten);

    // Matrix und Vektoren bekommen anzahl ihrer Daten und Zeilen
    A -> data = (double**)malloc(sizeof(double*) * zeilen);
    for(int x = 0; x < zeilen; x++)
        A -> data[x] = (double*)calloc(zeilen, sizeof(double));
    b -> data = (double*)malloc(sizeof(double*) * zeilen);
    x -> data = (double*)malloc(sizeof(double*) * zeilen);
    A -> n = zeilen;
    b -> n = zeilen;
    x -> n = zeilen;


    char numberstring[30]; // numberstring, CharArray dem "temp" immer wieder hinzugefügt wird, gibt Zahl einer Zelle im Stringformat aus, Array = (gebrauchte Größe + 1), weil terminierende 0
    long double number = 0; // number, hat Wert der einzelnen Zellen der Matrix
    char temp = 0; //temp: Je nachdem was "char c" einliest, wird temp dessen Wert zugewiesen
    numberstring[0] = '\0'; //Das CharArray wird leer initialisiert, die terminierende 0 am steht am Ende
    long double tempMatrix[zeilen][spalten]; //Temporäre Matrix, enthält noch nullzeilen und den vektor b
    int iSpalte = 0, iZeile = 0, aNullZeilen = 0;
    fseek(fp, 0, SEEK_SET); //Dateizeiger auf Anfang setzen
    char c;
    while((c=fgetc(fp)) != EOF) { //bis Dateiende erreicht wird
        temp = c;
        //falls Zeilenumsprung gelesen wird
        if (c == '\n'||c=='\r') {
            temp = '\0';
            strncat(numberstring, &temp, 1); //CharArray wird verkettet
            number = strtold(numberstring,'\0'); //Numbertostring wird zu double konvertiert und in Number gespeichert
            tempMatrix[iZeile][iSpalte] = number;
            iZeile++; //nächste Zeile
            iSpalte = 0; //erste Spalte
            numberstring[0] = '\0'; //CharArray wieder auf Anfang
        }
        //Falls Komma gelesen wird
        else if (c == ',') {
            temp = '\0';
            strncat(numberstring, &temp, 1); //CharArray wird verkettet
            number = strtold(numberstring,'\0'); //Numbertostring wird zu double konvertiert und in Number gespeichert
            tempMatrix[iZeile][iSpalte] = number;
            iSpalte++; //nächste Spalte
            numberstring[0] = '\0'; //CharArray wieder auf Anfang
        }
        //Wenn kein Zeilenumbruch oder Komma gelesen wird -> Zahl wird als Char eingelesen
        else strncat(numberstring, &temp, 1); //Verkettung zu CharArray/String
    }
//nullzeilen löschen
    for (int z = 0; z < zeilen; z++)
        for (int i = 0; i < spalten; i++)
            if(tempMatrix[z][i] != 0) break; //sobald ein Zeichen !=0 gelesen wird , wird Suche der Zeile abgebrochen, kann keine Nullzeile sein
            else if (i == spalten - 1 && tempMatrix[z][i] == 0){ //Wenn in letzter Spalte der Zeile 0 und noch kein Break erfolgt -> Alle Einträge der Zeile gleich 0
                aNullZeilen++;
                //Elemente werden verschoben
                for(int zv = z; zv < zeilen - 1; zv++)
                    for(int sv = 0; sv < spalten; sv++)
                        //Elemte aus Zusammensetzung [Zeile][Spalte] überschreiben Nullzeile
                        tempMatrix[zv][sv] = tempMatrix[zv+1][sv];
                for(int nz = 0; nz < spalten;nz++)
                    //Überschriebene Nullzeilen werden an Matrix unten wieder angehängt
                    tempMatrix[zeilen-1][nz] = 0;
            }
//tatsächliche matrix + vektor
    aNullZeilen /= 2;
    for(int vz = 0; vz < zeilen - aNullZeilen; vz++)
        //Vektor besteht aus allen Werten in der letzten Spalte
        b -> data[vz] = tempMatrix[vz][spalten-1];
    for(int mz = 0; mz < zeilen - aNullZeilen; mz++)
        //Matrix besteht aus allen Werten ohne die letzte Spalte
        for(int ms = 0; ms < spalten -1; ms++)
            A -> data[mz][ms] = tempMatrix[mz][ms];
    printf("Nullzeilen:\t%d\n\n\n", aNullZeilen);
    fclose(fp);
    return true;
}

void solve (Method method, Matrix *A, Vector *b, Vector *x, double e){
    int schritt = 0, maxSchritte = 100;
    Vector xAlt;
    xAlt.n = x -> n;
    xAlt.data = (double*)malloc(sizeof(double*) * x -> n);
    double diff = 0;

    if(method == JACOBI){
        do{// solange bis zum letzten wert kleiner oder gleich Fehlerschranke ist
            //Algorithmus - (Quelle: Pseudocode auf Wikipedia)
            for(int i = 0; i < x -> n; i++){
                x -> data[i] = b -> data[i];
                for(int j = 0; j < x -> n; j++)
                    if(i != j)
                         x -> data[i] = x -> data[i] - A -> data[i][j] * xAlt.data[j];
                x -> data[i] = x -> data[i] / A -> data[i][i];
            }

            //Überprüfung ob Differenz kleiner als die Fehlerschranke ist
            for(int i = 0; i < x -> n; i++){
                //Berechnung der kleinsten Differenz zwischen x und xAlt
                diff = xAlt.data[i] - x -> data[i];
                if(diff < 0) diff = diff * (-1); //Betrag der Differenz
                //Wenn die Differenz von einem der Werte des Vektors x kleiner oder gleich zur Fehlerschrankeist, kann aufgehört werden
                if(diff <= e) break;
                xAlt.data[i] = x -> data[i];
            }
            schritt++;
        }while(diff > e && schritt < maxSchritte);
    }
    else if(method == GAUSS_SEIDEL){
        //Normwerte := 0 damit Differenz bei jedem neuen Schritt neu berechnet werden kann:
        double normNeu, normAlt;
        do{
            normNeu = 0;
            normAlt = 0;
                //Zeilenschleife:
                for (int i = 0; i < x -> n; i++){
                    x -> data[i] = 0;
                    //Spaltenschleifen:
                    for (int j = 0; j < i; j++)
                        x -> data[i] = x -> data[i] + (A->data[i][j] * x -> data[j]);
                    for (int j = i + 1; j < x -> n; j++)
                        x -> data[i] = x -> data[i] + (A->data[i][j] * xAlt.data[j]);
                    x -> data[i] = (b->data[i] - x -> data[i]) / A->data[i][i];
                }
            //Berechnung NormNeu&NormAlt:
            for (int i = 0; i < x -> n; i++){
                normNeu += abs(x -> data[i] * x -> data[i]);
                normAlt += abs(xAlt.data[i] * xAlt.data[i]);
            }
            //Überprüfung ob Differenz kleiner als die Fehlerschranke ist
            for(int i = 0; i < x -> n; i++){
                //Berechnung der kleinsten Differenz zwischen x und xAlt
                diff = xAlt.data[i] - x -> data[i];
                if(diff < 0) diff = diff * (-1); //Betrag der Differenz
                //Wenn die Differenz von einem der Werte des Vektors x kleiner oder gleich zur Fehlerschrankeist, kann aufgehört werden
                if(diff <= e) break;
                xAlt.data[i] = x -> data[i];
            }
            schritt++;
            }while(diff > e && schritt < maxSchritte);
    }
    printf("Nach %d Schritten:\n\n",schritt);
}

int main() {
    Matrix A;
    Vector b;
    Vector x;
    double e;
    Method m;
    //Titel
    printf("\t ----------------------------\n\t|                            |\n\t|   LGS - Lösungsverfahren   |\n\t|    Jacobi / Gauß-Seidel    |\n\t|                            |\n\t ----------------------------\n\n");
    bool gueltig;
    do{
        //Fragt Anwender nach .csv-Datei
        char filename[100];
        printf("Dateipfad der .csv-Datei: ");
        scanf("%s",filename);
        //wenn Datei ungültig, wird do-while-Schleife wieder wiederholt
        gueltig = load(filename, &A, &b, &x);
        if(gueltig){
            printf("Datei erfolgreich geladen.\n\n");
            char auswahl;
            do{
                //Fragt Anwender nach Verfahren
                printf("Welches Verfahren?\nSchreibe '0' für Jacobi oder '1' für Gauß-Seidel: ");
                scanf("%s",&auswahl);
                if(auswahl == '0') m = JACOBI;
                else if(auswahl == '1') m = GAUSS_SEIDEL;
                else printf(" - Ungültige Eingabe -\n\n");
            // Wenn Anwender etwas anderes als '0' oder '1' eingibt, wird er erneut nach Verfahren gefragt
            } while(auswahl != '0' && auswahl != '1');
            //Frage nach Fehlerschranke
            printf("\nWie klein soll die Fehlerschranke sein? (Bsp. 10⁻¹⁰ = 1e-10): ");
            scanf("%lf",&e);
            printf("\n");
            solve(m, &A, &b, &x, e);
            //Ausgabe des Vektors x
            for (int i = 0; i < x.n;i++)
                printf("\tx%d = %f\n", i+1, x.data[i]);
            break;
        }
        else printf("Die Datei konnte nicht geladen werden.\n\n\n");
    } while(!gueltig);
    return 0;
}




