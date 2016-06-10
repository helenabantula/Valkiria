//
//  par.h
//  Corasonsitu
//
//  Created by Helena Bantulà i Fonts on 27/11/15.
//
//

#ifndef __Corasonsitu__par__
#define __Corasonsitu__par__

#include <iostream>
#include "ofxArtnet.h"
#include "ofMain.h"

#endif /* defined(__Corasonsitu__par__) */

class Par {

public:
    
    //unsigned char color[3];
    ofColor color;
    ofColor colorI;            //color inicial, sempre el mateix
    int id;
    bool isFadeIn = false;
    bool isFadeOut = false;
    bool isRandom = false;
    
    int time;
    
    int i;
    int stepFI = 200; //in milliseconds
    int stepFO = 200;
    float topFade = 0.3;
    int type=1;

    
    Par (ofColor,int);
    void fadeIn();
    void triggerFadeIn(int tipus, int step);
    void triggerFadeOut(int step);
    void fadeOut();
    void update();
    void randomMove();
    //void initialize(int time);
    ofColor getColor(){return color;};
    
    void setColor(int k){color = colorI*k;};

    
};
