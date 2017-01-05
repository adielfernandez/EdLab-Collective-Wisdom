#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"


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

    
    ofxAssimpModelLoader model;
    
    
    bool bAnimate;
    bool bAnimateMouse;
    float animationPosition;

    //Scaling of model through
    //AssImp methods
    float modelScale;

    float normFactor;
    
    int meshNum, numMeshes;
    ofVec3f meshCenter;
    float maxX;
    float minX;
    float maxY;
    float minY;
    float maxZ;
    float minZ;
    

    float realMaxX;
    float realMinX;
    float realMaxY;
    float realMinY;
    float realMaxZ;
    float realMinZ;
    
    bool drawOnTex;
    
    ofTexture origTex;
    
    ofFbo newSkin;
    
    ofLight	frontLight;
    ofLight	backLight;
    ofLight	spineLight;
    
    ofMaterial material;
    
    ofEasyCam cam;
    

    
};
