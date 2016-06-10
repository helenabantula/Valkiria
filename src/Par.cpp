//
//  par.cpp
//  Corasonsitu
//
//  Created by Helena Bantulà i Fonts on 27/11/15.
//
//

#include "Par.h"

Par::Par(ofColor colorC, int idC){
    
    color = colorC;
    colorI = colorC;
    id = idC;
}


void Par::update(){
    
    if (isFadeIn)
        fadeIn();
    
    if (isFadeOut)
        fadeOut();
    
    if (isRandom)
        randomMove();
    
}



void Par::triggerFadeIn(int tipus, int step){ //trigger: ep, estic fent un fadein
    type = tipus;         //1: fadeIn+fadeOut,   2: fadeIn
    isFadeIn = true;
    stepFI = step;
    time = ofGetElapsedTimeMillis();
}


void Par::fadeIn(){
    
    color[0] = (ofGetElapsedTimeMillis()-time)*((float)colorI[0]*topFade/(float)stepFI);
    color[1] = (ofGetElapsedTimeMillis()-time)*((float)colorI[1]*topFade/(float)stepFI);;
    color[2] = (ofGetElapsedTimeMillis()-time)*((float)colorI[2]*topFade/(float)stepFI);;
    
    if ((ofGetElapsedTimeMillis()-time)>stepFI){

        color = colorI*topFade;
        
        
        isFadeIn=false;
        
        if (type==1){
            stepFO = stepFI;
            triggerFadeOut(stepFO);
        }
    
    }
}


void Par::triggerFadeOut(int step){ //trigger: ep, estic fent un fadeOut
    if (color[0]!=0 || color[1]!=0 || color[2]!=0){
        isFadeOut = true;
        time = ofGetElapsedTimeMillis();
        stepFO = step;
    }
}


void Par::fadeOut(){
    color[0] = colorI[0]*topFade-(ofGetElapsedTimeMillis()-time)*((float)colorI[0]*topFade/(float)stepFO);
    color[1] = colorI[1]*topFade-(ofGetElapsedTimeMillis()-time)*((float)colorI[1]*topFade/(float)stepFO);
    color[2] = colorI[2]*topFade-(ofGetElapsedTimeMillis()-time)*((float)colorI[2]*topFade/(float)stepFO);
    

    if ((ofGetElapsedTimeMillis()-time)>stepFO) {         //sha esgotat el temps
        isFadeOut=false;
        color=0;
    }

}


void Par::randomMove(){
 
    topFade = ofNoise(ofGetElapsedTimef()/1.5+(300*id));             //300*id per a crear offset entre els pars
    topFade = ofMap(topFade, 0.2, 0.8, 0.3, 1, true);       //mapping dels valors del soroll a 0 i 1, valors mes grans que accepta el par
    color = colorI*topFade;
    //color.r = colorI.r*topFade;
    //color.b = colorI.b*topFade;
    


}