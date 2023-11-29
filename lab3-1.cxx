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
    Double_t erroreCalibroOscilloV[8] {0.02, 0.05, 0.05, 0.1, 0.1, 0.1, 0.2, 0.2};
    TGraphErrors * voltaggioVoltaggio = new TGraphErrors();
    for(int i=0; i<8; i++) {
        voltaggioVoltaggio->SetPoint(i, calibroMultiV[i], calibroOscilloV[i]);
        voltaggioVoltaggio->SetPointError(i, erroreCalibroMultiV[i], erroreCalibroOscilloV[i]);
    }
    TF1 * correlazione = new TF1("correlazione", "[0]+[1]*x", 98, 800);
    correlazione->SetParameters(0,1);
    voltaggioVoltaggio->Fit("correlazione");
    // ricordarsi di cambiare i nomi degli assi inserendo le appropriate unità di misura
    // devono essere mV e mV, perché se no si hanno un po di problemini
    TCanvas * ccalibrazione = new TCanvas();
    voltaggioVoltaggio->Draw();
}

void silicio() {
    gStyle->SetOptFit(0000);
    // array di valori I
    Double_t correnteMulti[17] {0.01, 0.02, 0.05, 0.07, 0.11, 0.14, 0.16, 0.25, 0.36, 0.53, 0.76, 0.89, 1.11, 1.65, 1.92, 3.97, 7.99};
    // array di logI          
    Double_t correnteMultiLog[17];            
    // riempimento array di valori logI
    for(int i=0; i<17; i++) {
        correnteMultiLog[i] = log(correnteMulti[i]);
    }
    Double_t erroreCorrenteMulti[17] {0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01};
    Double_t voltaggioOscillo[17] {400.0, 450.0, 500.0, 520.0, 540.0, 550.0, 560.0, 580.0, 600.0, 620.0, 640.0, 650.0, 660.0, 680.0, 700.0, 750.0, 800.0};
    Double_t erroreVoltaggioOscillo[17] {0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.2, 0.2, 0.2};
    TGraphErrors * silicioIV = new TGraphErrors(17);
    for(int i=0; i<17; i++) {
        silicioIV->SetPoint(i, voltaggioOscillo[i], correnteMultiLog[i]);
        silicioIV->SetPointError(i, erroreVoltaggioOscillo[i], erroreCorrenteMulti[i]);
    }
    TF1 * silicioFunction = new TF1("silicioFunction", "[0]+[1]*x");
    silicioFunction->SetRange(540.0, -1.0, 650.0, -0.06);
    silicioFunction->SetParameters(0, 0.5);
    silicioIV->Fit("silicioFunction");
    // ricordarsi di cambiare i nomi degli assi inserendo le appropriate unità di misura
    // devono essere mV e mA
    TCanvas * cSilicio = new TCanvas(); 
    silicioIV->Draw();
}

void germanio() {

}

void analisi() {
    setstyle();
    /* fit V-V con sull'asse x il voltaggio ottenuto sul multimetro
       e sull'asse y il voltaggio ottunto sull'oscilloscopio     */
    calibrazione();
    silicio();
}