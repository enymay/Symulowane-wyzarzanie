
//  Algorytm wyznaczania harmonogramu dla problemu gniazdowego
//
//	Mateusz Krasowski, Krzysztof Mierzwa


#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cmath>
using namespace std;

class gniazdowy
{
public:
    int A[226]; // Nastepnik maszynowy
    int T[226];
    int P[226];
    int pi[226];
    int LP[226]; // Ilosc nastepnikow i poprzednikow
    int Si[226]; // Czas rozpoczecia
    int Ci[226]; // Czas zakonczenia
    int PT[226];
    int PA[226];
    int PH[226];    // pomocnicza - indeksy poprzednikow ,czy Ci PT jest wieksze od Ci PA
    int SK[226];    // sciezka krytyczna
    double Temp;
    double lambda;
    
    void get_data(); 	// Pobieranie danych
    void display_data(); 	// Wyswietlanie
    int sprawdz(int rozmiar, int i);	 // Sprawdza ddzie znajduje sie zadanie w bloku
    void swap(int a, int b);		// Zamiana zadan w bloku
    void calc_SKryt();		// Obliczanie sciezki kryt
    int Cmax();		
    void calc_para();
    int length();
    int losowanie_zamiany();
    int wyzarzanie( gniazdowy current);
    int n;
    int m;
};

void gniazdowy::get_data()
{
    fstream plik;
    
    plik.open("dane.txt");
    if( plik.good())
    {
        plik >> n;
        for(int i = 1; i <= n; i++)
        {
            plik >> T[i];
        }
        cout << endl;
        for(int i = 1; i <= n; i++)
        {
            plik >> A[i];
        }
        cout << endl;
        for(int i = 1; i <= n; i++)
        {
            plik >> P[i];
        }
        cout << endl;
        plik >> m;
        for(int i = 1; i <= (n+m); i++)
        {
            plik >> pi[i];
        }
        cout << endl;
    }
    else {
        cout << "There is no data file" << endl;
        exit(123);
    }
    plik.close();
    return;
    
}

void gniazdowy::display_data(){
    
    cout<<"data"<<endl;
    cout<<endl;
    cout<< "procesess number "<< n <<endl;
    cout<< "machinery number "<< m <<endl;
    cout<<endl;
    
    cout<< "T   ";
    for(int i=1; i<=n; i++){
        cout<<T[i]<<" ";
    }
    cout<<endl;
    cout<<endl;
    
    cout<< "A   ";
    for(int i=1; i<=n; i++){
        cout<<A[i]<<" ";
    }
    cout<<endl;
    cout<<endl;
    
    cout<< "P   ";
    for(int i=1; i<=n; i++){
        cout<<P[i]<<" ";
    }
    cout<<endl;
    cout<<endl;
    
    cout<< "pi   ";
    for(int i=1; i<=n+m; i++){
        cout<<pi[i]<<" ";
    }
    cout<<endl;
    cout<<endl;
    
    /*/
     cout<< "LP   ";
     for(int i=1; i<=n; i++){
     cout<<LP[i]<<" ";
     }
     cout<<endl;
     cout<<endl;
     */
    /*
     cout<< "Si   ";
     for(int i=1; i<=n; i++){
     cout<<Si[i]<<" ";
     }
     cout<<endl;
     cout<<endl;
     */
    cout<< "Ci   ";
    for(int i=1; i<=n; i++){
        cout<<Ci[i]<<" ";
    }
    cout<<endl;
    cout<<endl;
    
    
    cout<< "PT   ";
    for(int i=1; i<=n; i++){
        cout<<PT[i]<<" ";
    }
    cout<<endl;
    cout<<endl;
    
    
    cout<< "PA   ";
    for(int i=1; i<=n; i++){
        cout<<PA[i]<<" ";
    }
    cout<<endl;
    cout<<endl;
    
    /*
     cout<< "PH   ";
     for(int i=1; i<=n; i++){
     cout<<PH[i]<<" ";
     }
     cout<<endl;
     cout<<endl;
     */
    
    cout<< "SK   ";
    for(int i=1; i<=n; i++){
        cout<<SK[i]<<" ";
    }
    cout<<endl;
    cout<<endl;
    
    
    
    
    return;
}

int gniazdowy::sprawdz(int rozmiar, int i){
    // rozmiar - rozmiar sciezki krytycznej
    // sprawdza, w ktĂ›rej czĂŤĂşci bloku znajduje siĂŤ zadanie
    // 0 w bloku, 1 na lewym skraju, 2 na prawym skraju, 3 pojedynczy
    
    if (i==1){
        if(PA[SK[i]]==SK[i+1]) return 2;
        else return 3;
    }
    if (i==rozmiar){
        if(A[SK[i]]==SK[i-1]) return 1;
        else return 3;
    }
    
    
    if(PA[SK[i]]==SK[i+1]){
        if(A[SK[i]]==SK[i-1]){
            return 0;
        }
        else{
            return 2;
        }
    }
    else{
        if(A[SK[i]]==SK[i-1]){
            return 1;
        }
        else{
            return 3;
        }
    }
}

void gniazdowy::swap(int a, int b){
    int c, d;
    c=PA[a];
    d=A[b];
    A[c]=b;
    A[a]=d;
    A[b]=a;
    PA[a]=b;
    PA[b]=c;
    PA[d]=a;
    return;
}

void gniazdowy::calc_para(){
    
    int head=0, bottom=0, temp, ns;
    int *Q =new int [n+1];    // queue
    int *test= new int [n+1]; // auxiliary table
    for(int i=0; i<=n+1; i++){
        Q[i]=0;
        test[i]=0;
    };
    
    //LP
    for(int i=1; i<=n; i++){
        for(int j=1; j<=n; j++){
            if (A[j]==i){
                LP[i]++;
            }
            if (T[j]==i){
                LP[i]++;
            }
            
        }
    }
    for(int i=1;i<=n;i++){
        PT[T[i]]=i;
        PA[A[i]]=i;
        
    }
    //queue operations
    for(int i=1;i<=n;i++){
        if(LP[i]==0 && test[i]==0){
            Q[++bottom]=i;
            test[i]=1;
        }
        while(head!=bottom){
            temp=Q[++head];
            ns=T[temp];
            if(ns!=0){
                LP[ns]--;
            }
            if(LP[ns]==0 && test[ns]==0){
                Q[++bottom]=ns;
                test[ns]=1;
            }
            
            ns=A[temp];
            if(ns!=0){
                LP[ns]--;
            }
            if(LP[ns]==0 && test[ns]==0){
                Q[++bottom]=ns;
                test[ns]=1;
            } 
            
            for(int i=1;i<=n;i++){
                Ci[i]=max(Ci[PA[i]],Ci[PT[i]])+P[i];
            }
            for(int i=1;i<=n;i++){
                if (Ci[PA[i]]>=Ci[PT[i]]){
                    PH[i]=PA[i];
                }
                else PH[i]=PT[i];
            }
            
        }
        
    }
    return;
}


int gniazdowy::length(){
    int k=1;
    while(SK[k]!=0){
        k++;
    }
    return k-1;
}


void gniazdowy::calc_SKryt(){
    
    int indeks=0;
    int Cimax=0;
    int k=0;
    
    for(int i=1; i<=length(); i++){
        SK[i]=0;
    }
    // szukamy najwiekszego Ci w tablicy
    for(int i=1;i<=n;i++){
        if(Cimax<Ci[i]){
            indeks=i;
            Cimax=Ci[i];
        };
    }
    
    while(indeks!=0){
        k++;
        SK[k]=indeks;
        indeks=PH[indeks];
    }
    return;
}

int gniazdowy::Cmax(){
    int max=0;
    for(int i=1;i<=n;i++){
        if(Ci[i]>max){
            max=Ci[i];
        }
    }
    return max;
}

int gniazdowy::losowanie_zamiany(){
    int dlugosc = length(); //dlugosc sciezli krytycznej
    int temp=0; 
    int licz=0; 
    int licz2=0; 
    int los=0; 
    int cmax = Cmax(); //wyznacz_Cmax
    int delta=0;
    double p,l;
    int i=0;
    
    while(i!=dlugosc){
        i++;
        temp = sprawdz(dlugosc,i); // sprawdzamy, w ktorym miejscu w bloku jest nasze i
        if(temp==1 || temp==2) licz++;		
    }
    
    los=rand()%licz+1;
    
    // po wylosowaniu szukamy wylosowanego indeksu
    i=0;
    while(licz2 != los){
        i++;
        temp = sprawdz(dlugosc,i);
        if(temp == 1 || temp == 2) licz2++;		
    }
    
    if(temp == 1){
        swap(SK[i],SK[i-1]); //zamiana
    }
    if(temp == 2){
        swap(SK[i+1],SK[i]); //zamiana
    }
    
    // sprawdzamy czy po zamiania cmax jest lepsze
    calc_para();
    
    delta = Cmax()-cmax;
    if(delta<0){
        calc_SKryt();
        return 1;
    }
    else{
        p=exp(-delta/Temp);
        l=(double)rand() / RAND_MAX;
        if(l<=p){ //minimum lokalne
            calc_SKryt();
            return 1;
        }
        else{
            if(temp==1){
                swap(SK[i-1],SK[i]); //zamiana
            }
            if(temp==2){
                swap(SK[i],SK[i+1]); // zamiana		
            }
            calc_para(); //wyliczenie cmax
            calc_SKryt(); //sciezka krytyczna
            return 0;
        }
    }
}


int gniazdowy::wyzarzanie(gniazdowy current){
    
    
    int max = Cmax();
    int step = 0;
    cout<<"Cmax "<<max<<endl;
    while(Temp>(0.1)){
        step++;
        Temp=Temp*lambda;
        losowanie_zamiany();
        if (max>Cmax()){
            max=Cmax();
            //display_data();
            cout<<"Cmax "<<max<<endl;
            cout << "Krok iteracji: " << step << endl;
        }
        
    }
    return max;
}


int main(void){
    
    srand(time(NULL));
    gniazdowy nowy;
    int max;
    for(int i = 0; i <= 225; i++)
    { 
        nowy.T[i]=0;
        nowy.A[i]=0;
        nowy.P[i]=0;
        nowy.pi[i]=0;
        nowy.LP[i]=0;
        nowy.Si[i]=0;
        nowy.Ci[i]=0;
        nowy.PT[i]=0;
        nowy.PA[i]=0;
        nowy.PH[i]=0;
        nowy.SK[i]=0;
    }
    
    nowy.Temp=1000.0; 
    nowy.lambda=0.9995;
    
    nowy.get_data();
    nowy.calc_para();
    nowy.calc_SKryt();
    nowy.display_data();
    max=nowy.wyzarzanie(nowy);
    system("PAUSE");
    return 0;
}