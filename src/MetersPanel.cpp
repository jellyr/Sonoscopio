
#include "MetersPanel.h"
#include "ofApp.h"


ofApp* meMainAppPtr;

//----------------------------------------------
#pragma mark - Core Funcs
//----------------------------------------------
void MetersPanel::setup(int x, int y, int width, int height, ofBaseApp* appPtr, vector<ofxAudioAnalyzerUnit*>& chanAnalyzerPtrs){
    
    _x = x;
    _y = y;
    _w = width;
    _h = height;
    
    meMainAppPtr = dynamic_cast<ofApp*>(appPtr);
    channelAnalyzers = chanAnalyzerPtrs;
    
    //-------------------------
    _guiCompHeight = MT_GUI_COMP_HEIGHT;
    bordCol = ofColor::grey;
    bordWidth = 1;
    setupGui();
    //------------------------
    
    setBackgroundColor(ofColor::darkBlue);
    
    panelColor1 = COLOR_MAIN_A;
    panelColor2 = COLOR_MAIN_B;
    
    panelsNum = channelAnalyzers.size();
    
    int panelHeight = (_h - _guiCompHeight) / panelsNum;
    
    for (int i=0; i<panelsNum; i++){
        
        int y_pos = _y + panelHeight*i;
        ofxAAChannelMetersPanel * p = new ofxAAChannelMetersPanel(_x, y_pos, _w, panelHeight, channelAnalyzers[i]);
       
        if(i%2) p->setMainColor(panelColor2);
        else p->setMainColor(panelColor1);
        
        channelPanels.push_back(p);
    }
   
    _panelDir = METERS_SETTINGS_DIR;
    _bDrawFullDisplay = TRUE;
    
}
//----------------------------------------------
void MetersPanel::update(){
    //set meters values from ofxAudioAnalyzerUnit
    for(ofxAAChannelMetersPanel* p : channelPanels){
        p->update();
    }
    
    //update gui components
    for(int i=0; i<components.size(); i++){
        components[i]->update();
    }
}


//----------------------------------------------
void MetersPanel::draw(){
    
    //for testing
    //drawBackground();
    
    for(ofxAAChannelMetersPanel* p : channelPanels){
        p->draw();
    }
    
    for(int i=0; i<components.size(); i++){
        components[i]->draw();
    }

}
//----------------------------------------------
void MetersPanel::exit(){
    for (ofxAAChannelMetersPanel* p : channelPanels){
        p->exit();
    }
    channelPanels.clear();

}
//----------------------------------------------
#pragma mark - Settings
//----------------------------------------------

void MetersPanel::loadSettings(string rootDir){
    
    //string fileName = workingDir+"/"+"metersSettings.xml";
    
    //-:Load panel settings (gui)---------------------
    ofxXmlSettings xml;
    string filenamePanel = rootDir + _panelDir + "/meters_settings_gui.xml";
    
    if( xml.loadFile(filenamePanel) ){
        ofLogVerbose()<<"MainPanel: "<< filenamePanel <<" loaded.";
    }else{
        ofLogError()<< "MainPanel: unable to load " << filenamePanel ;
        return;
    }
    //-----------
    string text = xml.getValue("PANEL:MAX-FREQ", "");
    gMaxFreq->setText(text);
    setAnalyzerMaxEstimatedValue(PITCH_FREQ, std::stof (text));
    //-----------------
    text = xml.getValue("PANEL:MAX-HFC", "");
    gMaxHfc->setText(text);
    setAnalyzerMaxEstimatedValue(HFC, std::stof (text));
    //-----------------
    text = xml.getValue("PANEL:MAX-CENTROID", "");
    gMaxCentroid->setText(text);
    setAnalyzerMaxEstimatedValue(CENTROID, std::stof (text));
    //-----------------
    text = xml.getValue("PANEL:MAX-SPEC-COMP", "");
    gMaxSpecComp->setText(text);
    setAnalyzerMaxEstimatedValue(SPECTRAL_COMPLEXITY, std::stof (text));
    //-----------------
    text = xml.getValue("PANEL:MAX-ROLLOFF", "");
    gMaxRollOff->setText(text);
    setAnalyzerMaxEstimatedValue(ROLL_OFF, std::stof (text));
    //-----------------
    text = xml.getValue("PANEL:MAX-ODD-EVEN", "");
    gMaxOddEven->setText(text);
    setAnalyzerMaxEstimatedValue(ODD_TO_EVEN, std::stof (text));
    //-----------------
    
    
    
    //-:Load channelPannels settings---------------------
    for(int i=0; i<channelPanels.size(); i++){
        string filenameChannelPanel = rootDir + _panelDir + "/meters_settings" + ofToString(i) + ".xml";
        channelPanels[i]->loadSettingsFromFile(filenameChannelPanel);
    }
}
//----------------------------------------------
void MetersPanel::saveSettings(string rootDir){
    
    //-:Save panel settings (gui) ------------------
    string filenamePanel = rootDir + _panelDir + "/meters_settings_gui.xml";
    ofxXmlSettings savedSettings;
    savedSettings.addTag("PANEL");
    savedSettings.pushTag("PANEL");
    savedSettings.addValue("MAX-FREQ",      gMaxFreq->getText());
    savedSettings.addValue("MAX-HFC",       gMaxHfc->getText());
    savedSettings.addValue("MAX-CENTROID",  gMaxCentroid->getText());
    savedSettings.addValue("MAX-SPEC-COMP", gMaxSpecComp->getText());
    savedSettings.addValue("MAX-ROLLOFF",   gMaxRollOff->getText());
    savedSettings.addValue("MAX-ODD-EVEN",  gMaxOddEven->getText());
    
    savedSettings.saveFile(filenamePanel);
    
    
    //-:Save channelPannels settings---------------
    for(int i=0; i<channelPanels.size(); i++){
        string filenameChannelPanel = rootDir + _panelDir + "/meters_settings" + ofToString(i) + ".xml";
        channelPanels[i]->saveSettingsToFile(filenameChannelPanel);
    }
}
//----------------------------------------------
#pragma mark - Other funcs
//----------------------------------------------
void MetersPanel::reset(vector<ofxAudioAnalyzerUnit*>& chanAnalyzerPtrs){
    
    for (ofxAAChannelMetersPanel* p : channelPanels){
        p->exit();
    }
    channelPanels.clear();
    
    //--------------------------
    
    channelAnalyzers = chanAnalyzerPtrs;
    panelsNum = channelAnalyzers.size();
    
    int panelHeight = (_h - _guiCompHeight) / panelsNum;
    
    for (int i=0; i<panelsNum; i++){
        int y_pos = _y + panelHeight*i;
        ofxAAChannelMetersPanel * p = new ofxAAChannelMetersPanel(_x, y_pos, _w, panelHeight, channelAnalyzers[i]);
        if(i%2) p->setMainColor(panelColor2);
        else p->setMainColor(panelColor1);
        
        channelPanels.push_back(p);
    }
    //------------------------------
 
}

//----------------------------------------------
void MetersPanel::resize(int y, int w, int h){
    
    _w = w;
    
    
    for(int i=0; i<channelPanels.size(); i++){
        channelPanels[i]->setWidth(_w);
    }
    
    adjustPosAndHeight(y, h);
    
    adjustGuiSize(_y, _w, _h);
}
//----------------------------------------------
void MetersPanel::adjustPosAndHeight(int y, int h){
    
    _y = y;
    _h = h;
    
    int panelHeight = (_h - _guiCompHeight) / panelsNum;
    
    for(int i=0; i<channelPanels.size(); i++){
        int y_pos = _y + panelHeight*i;
        channelPanels[i]->adjustPosAndHeight(y_pos, panelHeight);
    }
    
    
    
}
//----------------------------------------------
void MetersPanel::setFullDisplay(bool b){
    
    _bDrawFullDisplay = b;
    for (auto chPan : channelPanels){
        chPan->setFullDisplay(_bDrawFullDisplay);
    }
    
}
//----------------------------------------------
void MetersPanel::setupGui(){
    
    ofxDatGuiComponent* component;
    
    component = new ofxDatGuiButton("FULL DISPLAY");
    component->setLabelAlignment(ofxDatGuiAlignment::CENTER);
    component->onButtonEvent(this, &MetersPanel::onButtonEvent);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    components.push_back(component);
    
    gMaxFreq = new ofxDatGuiTextInput("MAX FREQ", "#freq#");
    component = gMaxFreq;
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->onTextInputEvent(this, &MetersPanel::onTextInputEvent);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    component->setLabelMargin(10.0);
    components.push_back(component);
    
    gMaxHfc = new ofxDatGuiTextInput("MAX HFC", "#hfc#");
    component = gMaxHfc;
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->onTextInputEvent(this, &MetersPanel::onTextInputEvent);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    component->setLabelMargin(10.0);
    components.push_back(component);
    
    gMaxCentroid = new ofxDatGuiTextInput("MAX CENTROID", "#centroid#");
    component = gMaxCentroid;
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->onTextInputEvent(this, &MetersPanel::onTextInputEvent);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    component->setLabelMargin(10.0);
    components.push_back(component);
    
    gMaxSpecComp = new ofxDatGuiTextInput("MAX SPEC-COMP", "#sp-cmp#");
    component = gMaxSpecComp;
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->onTextInputEvent(this, &MetersPanel::onTextInputEvent);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    component->setLabelMargin(10.0);
    components.push_back(component);
    
    gMaxRollOff = new ofxDatGuiTextInput("MAX ROLL-OFF", "#roll-off#");
    component = gMaxRollOff;
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->onTextInputEvent(this, &MetersPanel::onTextInputEvent);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    component->setLabelMargin(10.0);
    components.push_back(component);
    
    gMaxOddEven = new ofxDatGuiTextInput("MAX ODD-EVEN", "#oddEven#");
    component = gMaxOddEven;
    component->setLabelAlignment(ofxDatGuiAlignment::LEFT);
    component->onTextInputEvent(this, &MetersPanel::onTextInputEvent);
    component->setBorder(bordCol, bordWidth);
    component->setBorderVisible(TRUE);
    component->setStripeVisible(false);
    component->setLabelMargin(10.0);
    components.push_back(component);

    adjustGuiSize(_y, _w, _h);
    
}
//----------------------------------------------
void MetersPanel::adjustGuiSize(int y, int w, int h){
    
    int guiCompWidth = _w / 7;
    
    int gui_y = _y + _h - _guiCompHeight;
    int gui_x = _x;
    
    ofxDatGuiComponent* component;
    //FULL DISPLAY
    component = components[0];
    component->setPosition(gui_x, gui_y);
    component->setWidth(guiCompWidth, 0.35);//width x2
    
    //MAX PITCH FREQ
    gui_x += guiCompWidth;
    component = components[1];
    component->setPosition(gui_x, gui_y);
    component->setWidth(guiCompWidth, 0.6);
    
    //MAX HFC
    gui_x += guiCompWidth;
    component = components[2];
    component->setPosition(gui_x, gui_y);
    component->setWidth(guiCompWidth, 0.6);
    
    //MAX CENTROID
    gui_x += guiCompWidth;
    component = components[3];
    component->setPosition(gui_x, gui_y);
    component->setWidth(guiCompWidth, 0.6);
    
    //MAX SPEC COMP
    gui_x += guiCompWidth;
    component = components[4];
    component->setPosition(gui_x, gui_y);
    component->setWidth(guiCompWidth, 0.6);
    
    //MAX ROLL-OFF
    gui_x += guiCompWidth;
    component = components[5];
    component->setPosition(gui_x, gui_y);
    component->setWidth(guiCompWidth, 0.6);
    
    //MAX ODD TO EVEN
    gui_x += guiCompWidth;
    component = components[6];
    component->setPosition(gui_x, gui_y);
    component->setWidth(guiCompWidth, 0.6);

    
}
//----------------------------------------------
vector<std::map<string, float>>& MetersPanel::getMetersValues(){
    
    //??? valuesForOsc declariation here or in header?
    valuesForOsc.clear();
    
    for (int i=0; i<channelPanels.size(); i++){
        
        std::map<string, float> channelMap;
        
        for(ofxAAMeter* m : channelPanels[i]->meters){
           
            if( m->getName()==MTR_NAME_MFCC || m->getName()==MTR_NAME_SPECTRUM ||
               m->getName()==MTR_NAME_HPCP || m->getName()==MTR_NAME_MEL_BANDS){
                
                //???send vector<floats> ??? send binMeters values to OSC?
                
            }else if (m->getName()==MTR_NAME_ONSETS){
                string key =  m->getName();
                ofxAAOnsetMeter* om = dynamic_cast<ofxAAOnsetMeter*>(m);
                channelMap[key] = om->getValue();
                
            }else{
                string key = m->getName();
                channelMap[key]= m->getValue();
            }
            
        }
        valuesForOsc.push_back(channelMap);
    }
    return valuesForOsc;
}
//--------------------------------------------------------------
#pragma mark - Gui listeners
//--------------------------------------------------------------

void MetersPanel::onButtonEvent(ofxDatGuiButtonEvent e)
{
    //ofLogVerbose() << "onButtonEvent: " << e.target->getLabel() << "::" << e.enabled ;
    

    if(e.target->getLabel()=="FULL DISPLAY"){
        
        toggleFullDisplay();
        
    }
    
}
//--------------------------------------------------------------
void MetersPanel::onDropdownEvent(ofxDatGuiDropdownEvent e)
{
//    //ofLogVerbose() << "onDropdownEvent: " << e.child ;
//    
//    switch (e.child) {
//            
//        case 0:
//            currentTrackType = CURVES;
//            break;
//        case 1:
//            currentTrackType = BANGS;
//            break;
//        case 2:
//            currentTrackType = SWITCHES;
//            break;
//            
//        default:
//            break;
//    }
}
//--------------------------------------------------------------
void MetersPanel::onTextInputEvent(ofxDatGuiTextInputEvent e)
{
    //cout << "onButtonEvent: " << e.text << endl;
    //cout << "onTextInput: " << e.text << endl;
    if (e.target->getLabel()=="MAX FREQ"){
        setAnalyzerMaxEstimatedValue(PITCH_FREQ, std::stof (e.text));
    }else if (e.target->getLabel()=="MAX HFC"){
        setAnalyzerMaxEstimatedValue(HFC, std::stof (e.text));
    }else if (e.target->getLabel()=="MAX CENTROID"){
        setAnalyzerMaxEstimatedValue(CENTROID, std::stof (e.text));
    }else if (e.target->getLabel()=="MAX SPEC-COMP"){
        setAnalyzerMaxEstimatedValue(SPECTRAL_COMPLEXITY, std::stof (e.text));
    }else if (e.target->getLabel()=="MAX ROLL-OFF"){
        setAnalyzerMaxEstimatedValue(ROLL_OFF, std::stof (e.text));
    }else if (e.target->getLabel()=="MAX ODD-EVEN"){
        setAnalyzerMaxEstimatedValue(ODD_TO_EVEN, std::stof (e.text));
    }
    
    ofLogVerbose()<<"Set: "<<e.target->getLabel()<<" = " << std::stof (e.text);
        
}
//--------------------------------------------------------------
void MetersPanel::setAnalyzerMaxEstimatedValue(ofxAAAlgorithm algorithm, float value){
    for (ofxAudioAnalyzerUnit* anUnit : channelAnalyzers){
        anUnit->setMaxEstimatedValue(algorithm, value);
    }
}
