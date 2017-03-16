#include "ofApp.h"
#include "Poco/RegularExpression.h"
using Poco::RegularExpression;

// ------- Created by Erica Jewell
// v.2 March 2017
// Visualization of Facebook's Ad Preferences keywords visible at:
// https://www.facebook.com/ads/preferences/edit

// In order to add new keywords, the computer needs to be connected to the internet.
// If the DuckDuckGo API crashes, find and crop images yourself relevant to keywords from www.facebook.com/ads/preferences/edit, and store these in a folder with your name as the title in /bin/data/.

// Using DuckDuckGo's Instant Answers API:
// https://duckduckgo.com/api
// Regular Expressions to find XML tags altered from:
// https://github.com/MadSciLabs/ofxWeatherBug/blob/master/src/ofxWeatherBug.cpp

// When v.1 of self/image was created, Facebook did not yet have images associated with keywords.

//--------------------------------------------------------------

void ofApp::setup(){
    
    // Opening alert for all users.
    ofSystemAlertDialog("Welcome to SELF/IMAGE.\n\nPlease enter your name in the next text box, followed by ad preference keywords gathered from:\n\nfacebook.com/ads/preferences/edit\n\nThis application will then display Facebook's aggregated image of you based on things it thinks you like.\n\nMove around by dragging the display and use the top keys on your keyboard to switch between viewing modes [ q-w-e-r-t-y-u-i ].\n\nThis version of the program gathers images from the search engine DuckDuckGo.");
    
    // - - - - - Setting up context - - - - - //
    ofDisableArbTex(); // Use GL_TEXTURE_2D textures. If removed, the images will be replaced by single colors.
    
    ofSetSmoothLighting(true);
    
    ofBackground(0);
    
    boxSize = 200;
    posX = - ofGetWidth();
    posY = - 1.2*ofGetHeight(); // Bottom box starting position.
    posZ = 400;
    
    counter = 0;
    
    // - - - - - Entry system - - - - - //
    // Ask the user to enter their name.
    person = ofSystemTextBoxDialog("Enter name", person);
    
    // NEW USER - If there is not already a folder with that name, proceed to gather preference keywords.
    if (dir.doesDirectoryExist(person) == 0){
        cout << "new person" << endl;
        // Create new folder with person's name
        workingDir = dir.getAbsolutePath();
        newDir = workingDir + "/../../../data/" + person;
        ofDirectory newfolder(newDir);
        newfolder.create();
        while (keyword != "x"  || keyword != "X"){
        keyword = ofSystemTextBoxDialog("Enter keyword or type X to stop", keyword);
            if (keyword == "x" || keyword == "X"){
                break;
            }
        cout << person << endl;
        cout << keyword << endl;
        
        // Input keywords into image search api URLs.
        string URL = "http://api.duckduckgo.com/?q=" + keyword + "&format=xml&pretty=1";
        
        // Load information retrieved from URL (XML file) into openFrameworks.
        ofHttpResponse resp = ofLoadURL(URL);
        
        // Turn the response into text.
        string rawData = resp.data.getText();
        
        // Parse the retrieved data to find the first image URL by matching the <Icon> tag with a regular expression.
        RegularExpression search("<Icon>(.+?)</Icon>");
        RegularExpression::Match matchimgURL;
        
        // Determine whether a match was found.
        int imgURLfound = search.match(rawData, matchimgURL);
        
        // If a match was found, get the image file URL from the <Icon> tag.
        if (imgURLfound == 2){
            string imgURL = rawData.substr(matchimgURL.offset, matchimgURL.length);
            string contentFirst = ofSplitString(imgURL, ">") [1];
            contentURL = ofSplitString(contentFirst, "<") [0];
            cout << contentURL << endl; // YES!!!
            // get the image type - .png / .jpg / .jpeg
            vector <string> split = ofSplitString(contentURL, "."); // Search for the front part of the URL.
            cout << split.size() << endl;
            fileType = split[2];
            cout << fileType << endl;
            // Load the image from the URL into the image variable.
            img.load(contentURL);
            // Check the height & width of the image and use whichever is smaller to crop a square.
            width = img.getWidth();
            height = img.getHeight();
            if (width >= height){
                croppedDim = height;
            }
            if (height >= width){
                croppedDim = width;
            }
            if (croppedDim > 60){
                croppedDim = croppedDim - 50;
            }
            img.crop(0, 0, croppedDim, croppedDim);
            cout << croppedDim << endl;

            // Save the image in the /bin folder and use the keyword as the filename.
            img.save(newDir + "/" + keyword + "." + fileType, OF_IMAGE_QUALITY_BEST);
        }
        
        // If no match was found, write "not found" to the console.
        if (imgURLfound == 0){
            cout << "not found" << endl;
            ofSystemAlertDialog("The keyword was not found on DuckDuckGo.");
        }
        }
    }
    
    
    // OLD USER - If a folder with the name already exists, ask whether new keywords should be added.
    else if (dir.doesDirectoryExist(person) == 1){
        cout << "already exists" << endl;
        workingDir = dir.getAbsolutePath();
        newDir = workingDir + "/../../../data/" + person;
        while (keyword != "x" || keyword != "X"){
            keyword = ofSystemTextBoxDialog("Enter more keywords or type X to stop", keyword);
            if (keyword == "x" || keyword == "X"){
                break;
            }
        // Input keywords into image search api URLs.
        string URL = "http://api.duckduckgo.com/?q=" + keyword + "&format=xml&pretty=1";
        
        // Load information retrieved from URL (XML file) into openFrameworks.
        ofHttpResponse resp = ofLoadURL(URL);
        
        // Turn the response into text.
        string rawData = resp.data.getText();
        
        // Parse the retrieved data to find the first image URL by matching the <Icon> tag with a regular expression.
        RegularExpression search("<Icon>(.+?)</Icon>");
        RegularExpression::Match matchimgURL;
        
        // Determine whether a match was found.
        int imgURLfound = search.match(rawData, matchimgURL);
            cout << imgURLfound << endl;
        // If a match was found, get the image file URL from the <Icon> tag.
        if (imgURLfound == 2){
            // separate the URL from the <Icon> tag
            string imgURL = rawData.substr(matchimgURL.offset, matchimgURL.length);
            string contentFirst = ofSplitString(imgURL, ">") [1];
            contentURL = ofSplitString(contentFirst, "<") [0];
            cout << contentURL << endl; // YES!!!
            // get the image type - .png / .jpg / .jpeg
            vector <string> split = ofSplitString(contentURL, "."); // Search for the front part of the URL.
            cout << split.size() << endl;
            fileType = split[2];
            cout << fileType << endl;
            // Load the image from the URL into the image variable.
            img.load(contentURL);
            // Check the height & width of the image and use whichever is smaller to crop a square.
            width = img.getWidth();
            height = img.getHeight();
            if (width >= height){
                croppedDim = height;
            }
            if (height >= width){
                croppedDim = width;
            }
            if (croppedDim > 60){
            croppedDim = croppedDim - 50;
            }
            img.crop(0, 0, croppedDim, croppedDim);
            cout << croppedDim << endl;
            
            // Save the image in the /bin folder and use the keyword as the filename.
            img.save(newDir + "/" + keyword + "." + fileType, OF_IMAGE_QUALITY_BEST); // Change to relevant filetype.
            
        }
        
        // If no match was found, write "not found" to the console.
        if (imgURLfound == 0){
            cout << "not found" << endl;
            ofSystemAlertDialog("The keyword was not found on DuckDuckGo.");
        }
        }
    }
    
// Prepare the same number of boxes as there are keywords in the name folder.
    nImages = dir.listDir(person);
    nBoxes = nImages;
    images.resize(nImages);
    boxes.resize(nBoxes);
    filenames.resize(nBoxes);
    
    for (int i = 0; i < nImages; i ++){
        string fileName = dir.getPath( i ); // Get file name for each image.
        images[i].load(fileName);
        filenames[i] = fileName;
    }
    
    for (int j = 0; j < nBoxes; j++){
        noiseSeeds.push_back(ofRandom(0, 10000));
    }
}

//--------------------------------------------------------------
void ofApp::update(){

    counter++; // Adding a value every update for scrolling label box function.
    
// - - - - - Set lighting points - - - - - //
    pointLight.setPosition(ofGetWidth()/2, ofGetHeight()/2, 1500);
    pointLight2.setPosition(ofGetWidth(), ofGetHeight(), 1500);
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofSetColor(255);
    ofDrawBitmapString(person, 6*(ofGetWidth()/7), ofGetHeight()/8, 0); // User's name in top right corner.
    
    ofEnableDepthTest(); // Renders graphics objects with respect to their z-coordinates.
    
    ofEnableLighting();
    pointLight.enable();
    pointLight2.enable();
    
    spinX = sin(ofGetElapsedTimef()*.35f); // Spinning speed.
    spinY = cos(ofGetElapsedTimef()*.075f);
    
    easycam.begin(); // Camera for exploring the scene with the mouse.

    ofSetColor(255, 255, 255);

    stepSize = boxSize*1.5; // How far apart each box is from its neighbor.
    YstepSize = boxSize*0.5; // Distance between rows.

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
            boxes[i].setPosition(ofGetWidth()/2, -(ofGetHeight() + (1.5*nBoxes*boxSize*0.7)) + (i*2*boxSize*0.7) + (counter*1.7), 0);
            boxes[i].rotate(spinX, 1.0, 0.0, 0.0);
            boxes[i].rotate(spinY, 0, 1.0, 0.0);
            images[i].getTexture().bind();
            boxes[i].draw();
            images[i].unbind();
            vector<string> separation = ofSplitString(filenames[i], "/");
            vector<string> label = ofSplitString(separation[1], ".");
            if (ofIsStringInString(label[0], "_") == true ){ // Replaces underscores in filename with spaces.
                label = ofSplitString(label[0], "_");
                string newlabel = ofJoinString(label, " ");
                ofDrawBitmapString(newlabel, (ofGetWidth()/2) + (boxSize*0.7), -(ofGetHeight() + (1.5*nBoxes*boxSize*0.7)) + (i*2*boxSize*0.7) + (counter*1.7), 0);
            }
            else{
                ofDrawBitmapString(label[0], (ofGetWidth()/2) + (boxSize*0.7), -(ofGetHeight() + (1.5*nBoxes*boxSize*0.7)) + (i*2*boxSize*0.7) + (counter*1.7), 0);
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
        
        if (dir.listDir(person) == 0){ // If there's no folder with that name, ask again.
            person = ofSystemTextBoxDialog("Enter name", person);
        }
        nImages = dir.listDir(person); // Number of files in the folder bin/data/___.
        nBoxes = nImages;
        images.resize(nImages); // Resize the vectors according to how many photos there are in the folder.
        boxes.resize(nBoxes);
        filenames.resize(nBoxes);
        
        for (int i = 0; i < nImages; i ++){
            string fileName = dir.getPath( i ); // Get file name for each image.
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