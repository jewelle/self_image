#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    vector<ofBoxPrimitive> boxes; // boxes to render
    vector<ofImage> images; // images to wrap on boxes
    vector<string> filenames; // names of image files (for labels)
    int nImages, nBoxes, boxSize;
    int posX, posY, posZ;
    int stepSize, YstepSize;
    int counter; // for timing scrolling boxes
    string person; // entry name
    ofDirectory dir; // object for reading folder contents
    float spinX, spinY;
    vector <float> noiseSeeds; // noise for box movement
    bool normal, words, camReset, labels, bigBox, boxStack, boxPop; // modes
    ofEasyCam easycam;
    ofLight pointLight, pointLight2, pointLight3;
    ofImage img;
    string contentURL;
    int croppedDim, height, width;
    string keyword;
    string workingDir, newDir, finalDir, fileType;
    
};