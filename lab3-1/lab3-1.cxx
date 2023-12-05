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

void calibrazione() {
    Double_t calibroMultiV[8] {98.0 , 196.0 , 295.0 , 396.0 , 497.0 , 595.0 , 683.0, 784.0};
    // l'errore sul multimetro è trascurabile rispetto a quello sull'oscilloscopio
    Double_t erroreCalibroMultiV[8] {0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01};
    Double_t calibroOscilloV[8] {100.0, 200.0, 300.0, 400.0, 500.0, 600.0, 700.0, 800.0};
    // l'errore effettivo sull'oscilloscopio segue una formula che non ricordo
    Double_t erroreCalibroOscilloV[8] {2, 5, 5, 10, 10, 10, 20, 20};
    TGraphErrors * voltaggioVoltaggio = new TGraphErrors();
    for(int i=0; i<8; i++) {
        voltaggioVoltaggio->SetPoint(i, calibroMultiV[i], calibroOscilloV[i]);
        voltaggioVoltaggio->SetPointError(i, erroreCalibroMultiV[i], erroreCalibroOscilloV[i]);
    }
    voltaggioVoltaggio->SetTitle("Calibrazione Multimetro Oscilloscopio; Voltaggio Multimetro (mV); Voltaggio Oscilloscopio (mV)");
    TF1 * correlazione = new TF1("correlazione", "[0]+[1]*x", 98, 800);
    correlazione->SetParameters(0,1);
    voltaggioVoltaggio->Fit("correlazione", "R"); // R option limits the fit in the range of the function
    TCanvas * ccalibrazione = new TCanvas();
    voltaggioVoltaggio->Draw();
    ccalibrazione->Print("calibrazioneVV.pdf");
}

// ! Gli errori sono sbagliati, vanno controllate le formule per ottenere gli errori
void silicio() {
    // array di valori I
    Double_t correnteMultiS[17] {0.01, 0.02, 0.05, 0.07, 0.11, 0.14, 0.16, 0.25, 0.36, 0.53, 0.76, 0.89, 1.11, 1.65, 1.92, 3.97, 7.99};
    Double_t erroreCorrenteMultiS[17] {0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01};
    Double_t voltaggioOscilloS[17] {400.0, 450.0, 500.0, 520.0, 540.0, 550.0, 560.0, 580.0, 600.0, 620.0, 640.0, 650.0, 660.0, 680.0, 700.0, 750.0, 800.0};
    Double_t erroreVoltaggioOscilloS[17] {10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 20, 20, 20};
    TGraphErrors * silicioIV = new TGraphErrors(17);
    for(int i=0; i<17; i++) {
        silicioIV->SetPoint(i, voltaggioOscilloS[i], correnteMultiS[i]);
        silicioIV->SetPointError(i, erroreVoltaggioOscilloS[i], erroreCorrenteMultiS[i]);
    }
    silicioIV->SetTitle("Voltaggio-Corrente diodo Silicio; Voltaggio Oscilloscopio (mV); Log Corrente Multimetro (mA)");
    TF1 * silicioFunction = new TF1("silicioFunction", "[0]+([1]*exp(x*[2]))");
    silicioFunction->SetRange(400.0, 0.1, 650.0, 1.0);
    // TODO inserire i parametri attesi per fare il fit senza che root dia problemi
    //silicioFunction->SetParameters(0, 0.5);
    silicioIV->Fit("silicioFunction", "R"); // R option limits the fit in the range of the function
    TCanvas * cSilicio = new TCanvas(); 
    gPad->SetLogy();
    silicioIV->Draw();
    cSilicio->Print("silicioIV.pdf");
}

// ! Gli errori sono sbagliati, vanno controllate le formule per ottenere gli errori
void germanio() {
    // array di valori I
    Double_t correnteMultiG[16] {0.01, 0.02, 0.03, 0.04, 0.07, 0.11, 0.18, 0.26, 0.40, 0.58, 0.84, 1.21, 1.70, 2.34, 3.08, 5.17};
    Double_t erroreCorrenteMultiG[16] {0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01};
    Double_t voltaggioOscilloG[16] {70, 90, 110, 130, 150, 170, 190, 210, 230, 250, 270, 290, 310, 330, 350, 400};
    Double_t erroreVoltaggioOscilloG[16] {5, 5, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10};
    TGraphErrors * germanioIV = new TGraphErrors(16);
    for(int i=0; i<16; i++) {
        germanioIV->SetPoint(i, voltaggioOscilloG[i], correnteMultiG[i]);
        germanioIV->SetPointError(i, erroreVoltaggioOscilloG[i], erroreCorrenteMultiG[i]);
    }
    germanioIV->SetTitle("Voltaggio-Corrente diodio Germanio; Voltaggio Oscilliscopio (mV); Log Corrente Multimetro (mA)");
    TF1 * germanioFunction = new TF1("germanioFunction", "[0]+([1]*exp(x*[2]))");
    germanioFunction->SetRange(100.0, 0.01, 300.0, 1.75);
    // TODO inserire i parametri attesi per fare il fit senza che root dia problemi
    //germanioFunction->SetParameters(0, 0.5);
    germanioIV->Fit("germanioFunction", "R"); // R option limits the fit in the range of the function
    TCanvas * cGermanio = new TCanvas(); 
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
