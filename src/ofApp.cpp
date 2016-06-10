#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    //ofSetFrameRate(60);
    
    ////////////////////////// SERIAL ////////////////////////
    ofSetVerticalSync(true);
    int baud = 115200;

    serial.listDevices();
    
    ofSetFrameRate(48);
    
    vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
#if TARGET_RASPBERRY_PI
    serial.setup(0, baud);
#else
    serial.setup("/dev/cu.usbmodem1421",baud);
#endif

    ////////////////////////// USERS ////////////////////////
    
//    for (int i = 0; i < numUsers; i++){
//        User temUser("GreuEssencia_C.wav","GreuEssencia_D.wav",i);
//        users.push_back(temUser);
//    }
    
    User temUser("GreuEssencia_C.wav","GreuEssencia_D.wav",0);
    users.push_back(temUser);
    //User temUser1("GreuEssencia2_C.wav","GreuEssencia2_D.wav",1);
    User temUser1("GreuEssencia_C.wav","GreuEssencia_D.wav",1);
    users.push_back(temUser1);
    
    ////////////////////////// LIGHT ////////////////////////
    Light::getInstance().initialize(numUsers); //numUsers
    
    
    ////////////////////////// SOUND ////////////////////////
    background.load("humans_OK.mp3",true);
    background.setVolume(0.85);
    background.setLoop(true);
    background.play();
    

}

//--------------------------------------------------------------
void ofApp::update(){
    
    unsigned  char lectura[3];
    serial.readBytes(lectura, 2);  //int readBytes(unsigned char * buffer, int length);
    
    int sensor=(int)lectura[0]-48; //ASCII comena per 48
    switch (sensor) {
        case 0:
            users[sensor].setHeartBeat(lectura[1]);
            cout<<lectura<<endl;
            break;
        case 1:
            users[sensor].setHeartBeat(lectura[1]);
            cout<<lectura<<endl;

            break;
//        case 2:
//            users[sensor].setHeartBeat(lectura[1]);
//            cout<<lectura<<endl;
//            break;
        default:
            break;
    }
    
    
    
    
    //////////////////////////////////// CURRENT USERS COMPUTATION ///////////////////////////////////////
    
    
    int numOpenedUsers = 0;
    int numSIusers = 0;
    
    for (int i = 0; i < numUsers; i++){
        if (users[i].getUserState()!= STATE_INITIAL) {
            numSIusers ++;
            if (users[i].getUserState()!= STATE_STOP) {
                currentUsers[i] = true;
                numOpenedUsers ++;
            }
        }
        else
            currentUsers[i] = false;
    }
    
    
    

    for (int i = 0; i < numUsers; i++){
        users[i].update();

        if(!currentUsers[i] && (numOpenedUsers != 0)){
            users[i].setAnotherUser(true);
            users[i].update();
        }
    }


    
    /////////////////// ASSIGNACIO DEL COMPORTAMENT SEGONS EL NOMBRE D'USUARIS ////////////////////////////
                                                                                                        ///
    
    if ((numOpenedUsers == numUsers) && (ofGetElapsedTimeMillis()-changeUser) > (timeXuser-timeTransition)){
        ///
        users[0].setTransition(true);
        users[1].setTransition(true);
        timeXuser = 5000;
    }
    
    
    if ((numOpenedUsers == numUsers) && (ofGetElapsedTimeMillis()-changeUser) > timeXuser){             ///
                                                                                                        ///
        users[0].setTransition(false);
        users[1].setTransition(false);
        //timeXuser = 5000;
        
        
        if (firstTime2) {
        // just do once!!!
            if (users[0].getTimeSinceUser() < users[1].getTimeSinceUser()) {     // Zero first
                users[0].setTimeSinceUser(users[1].getTimeSinceUser());
                users[0].setActive(false); // ho giren
                users[1].setActive(true);  // ho giren
                
            }                                                                                           ///
            else    {                                                                                   ///
                users[1].setTimeSinceUser(users[0].getTimeSinceUser());                                 ///
                users[1].setActive(false); // ho giren                                                  ///
                users[0].setActive(true);  // ho giren                                                  ///
                                                                                                        ///
            }                                                                                           ///
            timeXuser = 2500;                                                                           ///
            firstTime2 = false;                                                                         ///
                                                                                                        ///
        }                                                                                               ///
                                                                                                        ///
        users[0].changeActivity(); // canvia nomes un                                                   ///
        users[1].changeActivity();                                                                      ///
//        users[0].update();                                                                            ///
//                                                                                                      ///
//        if (users[0].isActive)                                                                        ///
//            users[1].setActive(false);                                                                ///
//        else                                                                                          ///
//            users[1].setActive(true);                                                                 ///
                                                                                                        ///
        changeUser = ofGetElapsedTimeMillis();                                                          ///
    }                                                                                                   ///
                                                                                                        ///
    // LA INTERACCIO DELS DOS HA D'ACABAR AL MATEIX MOMENT                                              ///
                                                                                                        ///
                                                                                                        ///
                                                                                                        ///
    if (numOpenedUsers == 1) {                                                                          ///
                                                                                                        ///
        for (int i = 0; i < numUsers; i++){                                                             ///
            users[i].setActive(true);                                                                   ///
            users[0].setAnotherUser(false);
            firstTime2 = true;                                                                          ///
        }                                                                                               ///
                                                                                                        ///
    }                                                                                                   ///
                                                                                                        ///
                                                                                                        ///
    if (numOpenedUsers == 0){
        
        if (numSIusers == 0) {                                                                          ///
            Light::getInstance().randomPlay(true);                                                      ///
            background.setVolume(0.8);
        }

        firstTime2 = true;                                                                              ///
                                                                                                        ///
        for (int i = 0; i < numUsers; i++){                                                             ///
            users[i].setActive(true);                                                                   ///
            users[i].setAnotherUser(false);                                                             ///
                                                                                                        ///
        }                                                                                               ///
    }                                                                                                   ///
    else {                                                                                              ///
        Light::getInstance().randomPlay(false);                                                         ///
        background.setVolume(0.1);                                                                      ///
    }                                                                                                   ///
                                                                                                        ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
        
    
    
    Light::getInstance().getInfo();
}



//--------------------------------------------------------------
void ofApp::draw(){
    
    
    ofSetColor(0);
    
    
    ofDrawBitmapString((int)Light::getInstance().packDMX[0], 50, 50);
    ofDrawBitmapString((int)Light::getInstance().packDMX[1], 50, 70);
    ofDrawBitmapString((int)Light::getInstance().packDMX[2], 50, 90);
    
    ofDrawBitmapString((int)Light::getInstance().packDMX[3], 200, 50);
    ofDrawBitmapString((int)Light::getInstance().packDMX[4], 200, 70);
    ofDrawBitmapString((int)Light::getInstance().packDMX[5], 200, 90);
    
    ofDrawBitmapString((int)Light::getInstance().packDMX[6], 350, 50);
    ofDrawBitmapString((int)Light::getInstance().packDMX[7], 350, 70);
    ofDrawBitmapString((int)Light::getInstance().packDMX[8], 350, 90);
    
    ofDrawBitmapString((int)Light::getInstance().packDMX[9], 500, 50);
    ofDrawBitmapString((int)Light::getInstance().packDMX[10], 500, 70);
    ofDrawBitmapString((int)Light::getInstance().packDMX[11], 500, 90);
    
    
    
    ofDrawBitmapString((int)Light::getInstance().packDMX[12], 50, 150);
    ofDrawBitmapString((int)Light::getInstance().packDMX[13], 50, 170);
    ofDrawBitmapString((int)Light::getInstance().packDMX[14], 50, 190);
    
    ofDrawBitmapString((int)Light::getInstance().packDMX[15], 200, 150);
    ofDrawBitmapString((int)Light::getInstance().packDMX[16], 200, 170);
    ofDrawBitmapString((int)Light::getInstance().packDMX[17], 200, 190);
    
    ofDrawBitmapString((int)Light::getInstance().packDMX[18], 350, 150);
    ofDrawBitmapString((int)Light::getInstance().packDMX[19], 350, 170);
    ofDrawBitmapString((int)Light::getInstance().packDMX[20], 350, 190);
    
    ofDrawBitmapString((int)Light::getInstance().packDMX[21], 500, 150);
    ofDrawBitmapString((int)Light::getInstance().packDMX[22], 500, 170);
    ofDrawBitmapString((int)Light::getInstance().packDMX[23], 500, 190);
    
    
    
    ofDrawBitmapString((int)Light::getInstance().packDMX[24], 50, 250);
    ofDrawBitmapString((int)Light::getInstance().packDMX[25], 50, 270);
    ofDrawBitmapString((int)Light::getInstance().packDMX[26], 50, 290);
    
    ofDrawBitmapString((int)Light::getInstance().packDMX[27], 200, 250);
    ofDrawBitmapString((int)Light::getInstance().packDMX[28],200, 270);
    ofDrawBitmapString((int)Light::getInstance().packDMX[29], 200, 290);
    
    ofDrawBitmapString((int)Light::getInstance().packDMX[30], 350, 250);
    ofDrawBitmapString((int)Light::getInstance().packDMX[31], 350, 270);
    ofDrawBitmapString((int)Light::getInstance().packDMX[32], 350, 290);
    
    ofDrawBitmapString((int)Light::getInstance().packDMX[33], 500, 250);
    ofDrawBitmapString((int)Light::getInstance().packDMX[34], 500, 270);
    ofDrawBitmapString((int)Light::getInstance().packDMX[35], 500, 290);
    
    
    
    ofDrawBitmapString((int)Light::getInstance().packDMX[36], 350, 350);
    ofDrawBitmapString((int)Light::getInstance().packDMX[37], 350, 370);
    ofDrawBitmapString((int)Light::getInstance().packDMX[38], 350, 390);
    
    ofDrawBitmapString((int)Light::getInstance().packDMX[39], 500, 350);
    ofDrawBitmapString((int)Light::getInstance().packDMX[40], 500, 370);
    ofDrawBitmapString((int)Light::getInstance().packDMX[41], 500, 390);

}





void ofApp::keyPressed(int key)
{
    switch(key)
    {
        case 'f':
            users[1].setHeartBeat('H');
            break;
        case 'j':
            users[1].setHeartBeat('L');
            break;
    }
    
}

void ofApp::keyReleased(int key)
{
//    switch(key)
//    {
//        case 'f':
//            buttonON = false;
//            break;
//        case 'j':
//            buttonOFF = false;
//            break;
//    }
    
}
