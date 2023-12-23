// C++ libraries
#include <algorithm>
#include <iostream>
#include <cmath>

// ROOT libraries
#include "TMath.h"
#include "TGraphErrors.h"
#include "TF1.h"
#include "TCanvas.h"

void setstyle() {
    gROOT->SetStyle("Modern");
    gStyle->SetOptFit(1111);
}

// VALORI ERRORI //
/*
    OSCILLOSCOPIO
    (fondoscala/5)*(1 se si legge a fascia || 0.5 se si distingue la mezza riga)

    MULTIMETRO
    0.01 mA
*/

void calibrazione() {
    Double_t calibroMultiV[8] {98.0 , 196.0 , 295.0 , 396.0 , 497.0 , 595.0 , 683.0, 784.0};
    // creazione vettore degli errori sui voltaggi presi col multimetro
    Double_t erroreCalibroMultiV[8];
    // calcolo errore multimetro
    for(int i=0;  i<8; i++){
        if(i<=2){
            // fondo scala 320.0 mV
            erroreCalibroMultiV[i] = (calibroMultiV[i]*0.3*0.01)+0.01;
        } else {
            // fondo scala 3200 mV
            erroreCalibroMultiV[i] = (calibroMultiV[i]*0.3*0.01)+3;
        }
    }
    // dati raccolti con l'oscilloscopio
    Double_t calibroOscilloV[8] {100.0, 200.0, 300.0, 400.0, 500.0, 600.0, 700.0, 800.0};
    // l'errore effettivo sull'oscilloscopio segue una formula che non ricordo
    Double_t scalaCalibroOscilloV[8] {20, 50, 50, 100, 100, 100, 200, 200};
    // calcolo errore oscilloscopio
    Double_t erroreCalibroOscilloV[8];
    for (int i=0; i<8; i++) {
        // sigmaL=sigmaZ-->2sigmaL^2=sigmaL^2+sigmaZ^2
        //                                       sigmaL                          sigmaL                            sigmaC                 sigmaC
        erroreCalibroOscilloV[i] = TMath::Sqrt(2*(scalaCalibroOscilloV[i]*0.2*1)*(scalaCalibroOscilloV[i]*0.2*1) + (0.03*calibroOscilloV)*(0.03*calibroOscilloV));
    }
    // creazione grafico X-Y con errori
    TGraphErrors * voltaggioVoltaggio = new TGraphErrors();
    for(int i=0; i<8; i++) {
        voltaggioVoltaggio->SetPoint(i, calibroMultiV[i], calibroOscilloV[i]);
        voltaggioVoltaggio->SetPointError(i, erroreCalibroMultiV[i], erroreCalibroOscilloV[i]);
    }
    // Setup titolo e label assi
    voltaggioVoltaggio->SetTitle("Calibrazione Multimetro Oscilloscopio; Voltaggio Multimetro (mV); Voltaggio Oscilloscopio (mV)");
    // definizione funzione per il fit
    TF1 * correlazione = new TF1("correlazione", "[0]+[1]*x", 98, 800);
    // imboccare root che se no fa i capricci
    correlazione->SetParameters(0,1);
    // fit della funzione sul thgrapherror
    voltaggioVoltaggio->Fit("correlazione", "R"); // R option limits the fit in the range of the function
    // inizializzazione canvas e stampa del grafico con fit
    TCanvas * ccalibrazione = new TCanvas();
    voltaggioVoltaggio->Draw();
    ccalibrazione->Print("calibrazioneVV.pdf");
}

void silicio() {
    // array di valori I
    Double_t correnteMultiS[17] {0.01, 0.02, 0.05, 0.07, 0.11, 0.14, 0.16, 0.25, 0.36, 0.53, 0.76, 0.89, 1.11, 1.65, 1.92, 3.97, 7.99};
    Double_t erroreCorrenteMultiS[17] {0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01};
    Double_t voltaggioOscilloS[17] {400.0, 450.0, 500.0, 520.0, 540.0, 550.0, 560.0, 580.0, 600.0, 620.0, 640.0, 650.0, 660.0, 680.0, 700.0, 750.0, 800.0};
    Double_t scalaVoltaggioOscilloS[17] {10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 20, 20, 20};
    // calcolo errore oscilloscopio
    Double_t erroreVoltaggioOscilloS[17];
    for (int i=0; i<17; i++) {
        erroreVoltaggioOscilloS[i] = scalaVoltaggioOscilloS[i]*0.2*1;
    }
    // creazione grafico X-Y con errori
    TGraphErrors * silicioIV = new TGraphErrors(17);
    for(int i=0; i<17; i++) {
        silicioIV->SetPoint(i, voltaggioOscilloS[i], correnteMultiS[i]);
        silicioIV->SetPointError(i, erroreVoltaggioOscilloS[i], erroreCorrenteMultiS[i]);
    }
    // setup titolo e label
    silicioIV->SetTitle("Voltaggio-Corrente diodo Silicio; Voltaggio Oscilloscopio (mV); Log Corrente Multimetro (mA)");
    // inizializzazione della funzione per il fit
    TF1 * silicioFunction = new TF1("silicioFunction", "[0]+([1]*exp(x*[2]))");
    // impostazione range di validità della funzione nel range esponenziale del V-I
    silicioFunction->SetRange(400.0, 0.1, 650.0, 1.0);
    // TODO inserire i parametri attesi per fare il fit senza che root dia problemi
    // imboccare root che se no fa i capricci
    //silicioFunction->SetParameters(0, 0.5);
    // fit della funzione
    silicioIV->Fit("silicioFunction", "R"); // R option limits the fit in the range of the function
    // inizializzazione canvas
    TCanvas * cSilicio = new TCanvas(); 
    // impostazione scala logaritmica e stampa
    gPad->SetLogy();
    silicioIV->Draw();
    cSilicio->Print("silicioIV.pdf");
}

void germanio() {
    // array di valori I
    Double_t correnteMultiG[16] {0.01, 0.02, 0.03, 0.04, 0.07, 0.11, 0.18, 0.26, 0.40, 0.58, 0.84, 1.21, 1.70, 2.34, 3.08, 5.17};
    Double_t erroreCorrenteMultiG[16] {0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01};
    Double_t voltaggioOscilloG[16] {70, 90, 110, 130, 150, 170, 190, 210, 230, 250, 270, 290, 310, 330, 350, 400};
    Double_t scalaVoltaggioOscilloG[16] {5, 5, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10};
    // calcolo errore oscilloscopio
    Double_t erroreVoltaggioOscilloG[16];
    for (int i=0; i<16; i++) {
        erroreVoltaggioOscilloG[i] = scalaVoltaggioOscilloG[i]*0.2*1;
    }
    // creazione grafico X-Y con errori   
    TGraphErrors * germanioIV = new TGraphErrors(16);
    for(int i=0; i<16; i++) {
        germanioIV->SetPoint(i, voltaggioOscilloG[i], correnteMultiG[i]);
        germanioIV->SetPointError(i, erroreVoltaggioOscilloG[i], erroreCorrenteMultiG[i]);
    }
    // setup titolo e label
    germanioIV->SetTitle("Voltaggio-Corrente diodio Germanio; Voltaggio Oscilliscopio (mV); Log Corrente Multimetro (mA)");
    // inizializzazione della funzione per il fit
    TF1 * germanioFunction = new TF1("germanioFunction", "[0]+([1]*exp(x*[2]))");
    // impostazione range di validità della funzione nel range esponenziale del V-I
    germanioFunction->SetRange(100.0, 0.01, 300.0, 1.75);
    // TODO inserire i parametri attesi per fare il fit senza che root dia problemi
    // imboccare root che se no fa i capricci
    //germanioFunction->SetParameters(0, 0.5);
    // fit della funzione
    germanioIV->Fit("germanioFunction", "R"); // R option limits the fit in the range of the function
    // inizializzazione canvas
    TCanvas * cGermanio = new TCanvas(); 
    // impostazione scala logaritmica e stampa
    gPad->SetLogy();
    germanioIV->Draw();
    cGermanio->Print("germanioIV.pdf");
}

void analisi() {
    /* impostazione grafica, con stile "Modern" e opzioni di visualizzazione del fit
       1111 (non mi ricordo cosa voglia dire e non ho voglia di controllare)*/
    setstyle();
    /* fit V-V con sull'asse x il voltaggio ottenuto sul multimetro
       e sull'asse y il voltaggio ottunto sull'oscilloscopio     */
    calibrazione();
    /* fit V-I con sull'asse x il voltaggio ottenuto sull'oscilloscopio
       e sull'asse y la corrente ottunta sul multimetro          */
    silicio();
    /* fit V-I con sull'asse x il voltaggio ottenuto sull'oscilloscopio
       e sull'asse y la corrente ottunta sul multimetro          */
    germanio();
}
