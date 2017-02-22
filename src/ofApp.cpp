#include "ofApp.h"

//--------------------------------------------------------------

void ofApp::setup(){
    
// - - - - - Setting up context - - - - - //
    ofDisableArbTex(); // use GL_TEXTURE_2D textures. remove and the images will be replaced by single colors

    ofSetSmoothLighting(true);
    
    ofBackground(0);
    
    boxSize = 200;
    posX = - ofGetWidth();
    posY = - 1.2*ofGetHeight(); // bottom box starting position
    posZ = 400;
    
    counter = 0;
    
// - - - - - Entry system - - - - - //
    person = ofSystemTextBoxDialog("Enter name", person);

    if (dir.listDir(person) == 0){ // if there's no folder with that name, ask again
        person = ofSystemTextBoxDialog("Enter name", person);
    }
    
    nImages = dir.listDir(person); // number of files in the folder data/pictures/___
    nBoxes = nImages;
    images.resize(nImages); // resize the vectors according to how many photos there are in the folder
    boxes.resize(nBoxes);
    filenames.resize(nBoxes);
    
    for (int i = 0; i < nImages; i ++){
        string fileName = dir.getPath( i ); // get file name for each image
        images[i].load(fileName);
        filenames[i] = fileName;
    }

    for (int j = 0; j < nBoxes; j++){
        noiseSeeds.push_back(ofRandom(0, 10000));
    }
}

//--------------------------------------------------------------
void ofApp::update(){

    counter++; // adding a value every update for scrolling label box function
    
// - - - - - Set lighting points - - - - - //
    pointLight.setPosition(ofGetWidth()/2, ofGetHeight()/2, 1500);
    pointLight2.setPosition(ofGetWidth(), ofGetHeight(), 1500);
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofSetColor(255);
    ofDrawBitmapString(person, 6*(ofGetWidth()/7), ofGetHeight()/8, 0); // user's name in top right corner
    
    ofEnableDepthTest(); // renders graphics objects with respect to their z-coordinates
    
    ofEnableLighting();
    pointLight.enable();
    pointLight2.enable();
    
    spinX = sin(ofGetElapsedTimef()*.35f); // spinning speed
    spinY = cos(ofGetElapsedTimef()*.075f);
    
    easycam.begin(); // camera for exploring the scene with the mouse

    ofSetColor(255, 255, 255);

    stepSize = boxSize*1.5; // how far apart each box is from its neighbor
    YstepSize = boxSize*0.5; // distance between rows

// - - - - - Reset camera view - - - - - //
    if (camReset == true){
        easycam.reset();
        camReset = false;
    }
// - - - - - Box stack popping - - - - - //
    if (boxStack == true){
        easycam.begin();
        for ( int i = 0; i < nBoxes; i++){
            boxes[i].set(boxSize);
            boxes[i].rotate(spinX, 1.0, 0.0, 0.0);
            boxes[i].rotate(spinY, 0, 1.0, 0.0);
            ofPoint pointLocation;
            pointLocation.x = 0;
            pointLocation.y = -ofGetHeight()/3 + (500*ofNoise(noiseSeeds[i]));
            pointLocation.z = 0;
            boxes[i].setPosition(pointLocation);
            images[i].getTexture().bind();
            boxes[i].draw();
            images[i].unbind();
            noiseSeeds[i] = noiseSeeds[i]+0.01;
        }
        easycam.end();
    }
// - - - - - Intense noisey popping - - - - - //
    else if (boxPop == true){
        easycam.begin();
        for ( int i = 0; i < nBoxes; i++){
            boxes[i].set(boxSize);
            boxes[i].rotate(spinX, 1.0, 0.0, 0.0);
            boxes[i].rotate(spinY, 0, 1.0, 0.0);
            ofPoint pointLocation;
            pointLocation.x = 0;
            pointLocation.y = -ofGetHeight()/3 + (500*ofNoise(noiseSeeds[i]*200));
            pointLocation.z = 0;
            boxes[i].setPosition(pointLocation);
            images[i].getTexture().bind();
            boxes[i].draw();
            images[i].unbind();
            noiseSeeds[i] = noiseSeeds[i]+0.01;
        }
        easycam.end();
    }
// - - - - - Flashing big box - - - - - //
    else if (bigBox == true){
        easycam.begin();
        for ( int i = 0; i < nBoxes; i++){
            boxes[i].set(boxSize*1.7);
            images[i].getTexture().bind();
            boxes[i].draw();
            images[i].unbind();
            boxes[i].rotate(spinX, 1.0, 0.0, 0.0);
            boxes[i].rotate(spinY, 0, 1.0, 0.0);
            float pointLocation = ofNoise(noiseSeeds[i]*stepSize);
            boxes[i].setPosition(pointLocation, pointLocation, pointLocation);
            noiseSeeds[i] = noiseSeeds[i]+0.01;
        }
        easycam.end();
    }
// - - - - - Label view - - - - - //
    else if (labels == true){
        easycam.end();
        for ( int i = 0; i < nBoxes; i++){
            boxes[i].set(boxSize*0.7);
            boxes[i].setPosition(ofGetWidth()/2, -(ofGetHeight() + (1.5*nBoxes*boxSize*0.7)) + (i*2*boxSize*0.7) + counter, 0);
            boxes[i].rotate(spinX, 1.0, 0.0, 0.0);
            boxes[i].rotate(spinY, 0, 1.0, 0.0);
            images[i].getTexture().bind();
            boxes[i].draw();
            images[i].unbind();
            vector<string> separation = ofSplitString(filenames[i], "/");
            vector<string> label = ofSplitString(separation[1], ".");
            if (ofIsStringInString(label[0], "_") == true ){ // replace underscores in filename with spaces
                label = ofSplitString(label[0], "_");
                string newlabel = ofJoinString(label, " ");
                ofDrawBitmapString(newlabel, (ofGetWidth()/2) + (boxSize*0.7), -(ofGetHeight() + (1.5*nBoxes*boxSize*0.7)) + (i*2*boxSize*0.7) + counter, 0);
            }
            else{
                ofDrawBitmapString(label[0], (ofGetWidth()/2) + (boxSize*0.7), -(ofGetHeight() + (1.5*nBoxes*boxSize*0.7)) + (i*2*boxSize*0.7) + counter, 0);
            }
        }
        easycam.begin();
    }
// - - - - - Original view - - - - - //
    else {
        easycam.begin();
        for ( int i = 0; i < nBoxes; i++){
            boxes[i].set(boxSize);
            if ( i >= (nBoxes/3) && i <=  2*(nBoxes/3)){
                boxes[i].setPosition((posX + ((i-(nBoxes/3))*stepSize)), (posY + ((i-(nBoxes/3))*YstepSize) + boxSize*4), (posZ + ((i-(nBoxes/3))*-200)));
            }
            else if ( i >= 2*(nBoxes/3) ){
                boxes[i].setPosition((posX + ((i-(2*(nBoxes/3)))*stepSize)), (posY + ((i-(2*(nBoxes/3)))*YstepSize) + boxSize*2), (posZ + ((i-(2*(nBoxes/3)))*-200)));
            }
            else {
                boxes[i].setPosition((posX + (i*stepSize)), (posY + (i*YstepSize)), (posZ + (i*-200)));
            }
            boxes[i].rotate(spinX, 1.0, 0.0, 0.0);
            boxes[i].rotate(spinY, 0, 1.0, 0.0);
            images[i].getTexture().bind();
            boxes[i].draw();
            images[i].unbind();
        }
        easycam.end();
    }
    easycam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
// - - - - - Press n to change person being visualised - - - - - //
    if (key == 'n'){
    
        person = ofSystemTextBoxDialog("Enter name", person);
        
        if (dir.listDir(person) == 0){ // if there's no folder with that name, ask again
            person = ofSystemTextBoxDialog("Enter name", person);
        }
        nImages = dir.listDir(person); // number of files in the folder data/pictures
        nBoxes = nImages;
        images.resize(nImages); // resize the vectors according to how many photos there are in the folder
        boxes.resize(nBoxes);
        filenames.resize(nBoxes);
        
        for (int i = 0; i < nImages; i ++){
            string fileName = dir.getPath( i ); // get file name for each image
            images[i].load(fileName);
            filenames[i] = fileName;
        }
        for (int j = 0; j < nBoxes; j++){
            noiseSeeds.push_back(ofRandom(0, 10000));
        }
    }

// - - - - - q = one box with flashing layers of other box on top, w = go back - - - - - - //
    if (key == 'q')
        bigBox = true;
    if (key == 'w')
        bigBox = false;
    
// - - - - - e = box popping, r = go back - - - - - - //
    if (key == 'e')
        boxStack = true;
    if (key == 'r')
        boxStack = false;

// - - - - - t = box popping, y = go back - - - - - - //
    if (key == 't')
        boxPop = true;
    if (key == 'y')
        boxPop = false;

// - - - - - u = names list, i = go back - - - - - - //
    if (key == 'u')
        labels = true;
        counter = 0;
    if (key == 'i')
        labels = false;

// - - - - - o = reset camera - - - - - //
    if (key == 'o')
        camReset = true;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}