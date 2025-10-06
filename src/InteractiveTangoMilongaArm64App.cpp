// /Users/courtney/Documents/Dissertation Work - Interactive Tango/wekinator/9-30-2016


/**
 6/20/19
Bug List
Missing accompaniment / leader reactions -- looks like the leader velocity is not being sent... -- FIXED BUT need to look at velocity pattern ugen

 Instruments heard - only accordion + guitar... ok, now some piano ? missing instruments? - just heard cello it sounds like shit -- note: get Chris Hein Cello.
 OK heard all melody instruments now I think - but melody is sporadic. Gah.
__Fix cadences__ -- OK, I think cadences are ok for fragments -- added cadence notes to por una cabeza and la cumparsita
 Need to fix how fast it changes busy / sparse.
 
 Add a lot of continuous control
 
 Need to check busy/sparse for both leader & follower as it is totes out of control
 
 7/2/2019
 ++No obvious bugs anymore. Need to check different songs and dance and then see.
 Ideas / Priorities:
 1. Continuous control (use puente?)
 2. Update to Live 10
 3. Busy/Sparse for melody -- 1-3 for which melody line, but could do something like 1-10 for how many notes... maybe with a different curve (see puente).
 
**/


//adding comments...

/*
 Feature List NEED:
 ++Circling -- Energy -- Musical Mapping **DONE
 ++Kicks -- set off ornaments **DONE
 ++Instruments mapped to busy/sparse , smooth/pointy **DONE -- some minor bugs -- need to fix
 ++Add orchestration (violin, guitar) -- full version of kontakt (?) **DONE -- want to add bass...
 ++spatial measure -- reverb/room_size tied to how similar the movements of dancers are to one another **DONE
 ++Volume mapped to busy/sparse + foot onset peaks (overall vol. & micro vol?) **DONE
 ++accent patterns -- different gestures, accented patterns
 
 Fixes
 ++different song structures (por una cabeza!) *** SOLVED
 ++everything stops when everyone stops moving (not just at the end of phrases) ( DONE )
 ++fix busy/sparse for leader in por una cabeza *** SOLVED
 ++accompaniment -- keep the same instrument throughout a phrase!  *** SOLVED
 ++filter crosscovar
 
 Features if time
 ++counter-melodies?
 
 the work that I did IMPROVED x, y & z
 what do we think that is?
 --->direct people's attention towareds that in experiments
 --->create that bridge
 next semester -- tying up these loose ends -- B123
 
 
 
 ----------------------
 ----------------------
 
 Brent -->
 Could be faster -- busy vs. sparse -- smaller time window
 Could be busier a bit more --
 staccato / legato pretty good
 
 Max Follower Step Num = 4.98
 couple windowed var = 0.38
 
 How to fix por una cabeza -- make the length in seconds longer / correspond with song

 
 -- made max seconds 1.5 beats, 0.75 for por una cabeza
 -- need to create separate Wekinator projects for leader and follower... and every dancer... :/
 -- with separate senders, etc. etc.
 
 -------------------
 -------------------
 --didn't go into busy fast enough
 --need to change up the repeating chord pattern.... make accomp. better
 
nature of the change:
 ---> try vector for machine learning -- nature of the change
 -->stickiness -- windowed variation ------------------> springs -- damping into the system
 --
 --2 seconds
 -->curves ---- maybe not come back
         -->designing the transistion of change, softening -- feel an accent
         -->attack volume -- accents, sfz,
         --> change quantize dynamically ------------ what are the transitions........ in perceptual states
 
        --rich, contradictory
 
 ------------
 3-26-2016
 Wekinator so far works best with:
 raw input into signal vector (not smoothed at all)
 either defaults OR
 downsample by constant rate: 10
 minimum length of 5
 match width 5
 match hop size 1
 using only one foot
 
--------------------------> try next time -- windowed variance difference, but with multipliers to make it signficant even in floats...
 3-29-2016 - test with Brent.
 
 ++current parameters for Fragments seem about right... right?
 ++perhaps more stickiness in S/L (?) --> I think it goes to staccato, Brent thinks it stay in legato too much
 ++busy/sparse seems about right -- still don't have that implemented for leader so unknown
 
 Por Una Cabeza
 Basically sounds good
 Goes into busy too fast
 
 +++ bugs found: TODO: need to make sure accompaniment keeps the same instrument the whole way through... this was implemented before but now somehow changes f'd this up
 +++ TODO: different song structure for Por Una Cabeza!!! EEEEK!KK!!!!K!K!
 +++ in the middle of wekinator
 
 +++ features are not good enough -- check windowed variation x, y, z
    +++ also see if can create signal vectors with higher variation - - HPF?
 
 
-----------------------------
-----------------------------
 4-1-2016
 
 ++Idea -- dynamic windowing for wekinator... ? -- when circling, larger window until get out... smaller window for steps and kicks

 -----------------------------
 -----------------------------
 4-5-2016
++need to train examples when dancing together
    self-awareness
 
 ++different examples -- open vs. closed
++need to accomodate nothing being chosen as output (instead of just keeping prev.)
++need to add step detection to distinguish from circling -- in code
 ++need to stop recording boleos near the top of the motion (not the end, but the middle)
 ++when recording a step -- used 2 steps started when left was on the floor, one collection, then 2nd collection when foot was on the floor
 ++don't do weird transitions when walking
 
 ++why it is better--> have direct control over gestures -- and not externally overlaid -- from within social dance
 intuitive...
 
 part of the paradigm --> individual vs.
 
 /;
 
 ++check WHERE (x,y,z) the peaks happen in boleos vs. stepping ---> could also be a clue
 ++scale values sent to wekinator......... DTW is like k-nearest neighbor
 ++make circling WERK first
 
 5-25-2016
 ++scale values then multiply by large constant -- 1-100 (best?) or 1-200
 ++change window size for vectors
 ++short pieces of time for gestures -- boleo vs. steps... have circling be 1/2 ?
 
 --------------------------------
 --------------------------------

6-10-2016
++include raw accel to send to wekinator (15Hz low-pass filter) -- accidentally only sent this + snapPeaks data & worked better (w/both feet)
++see if adding the params I thought I sending works... if not --> just send raw accel + snapPeaks since less CPU
 
 --------------------------------
 --------------------------------
 6-13-2016
 ++tried higher frame-rate... not sure if helps....
 ++still have problems distinguishing step from circle -- maybe just send a few signals of just peaks
 
 --------------------------------
 --------------------------------

 7-18-2016
 ++fixed por una cabeza structure
 
 7-19-2016
 ++boleo works
 
 7-20-2016
 ++boleo uses random note in key (depending on section & shiz)
 
 7-21-2016
ornaments are in the same instrumentation as the melody
 
 7-27-2016
 ++fixed CPU usage in Ableton
 ++added cello
 ++added guitar
 ++adding more to accompaniment
 ++taking orchestral thickness out of accompaniment profile -- now, more orchestration is busy/spare, etc. -- fixed fragments accompaniment database files to reflect that
 
Note that more notes in the accompaniment -- dancers had hard time hearing that so trying tying orchestral thickness to this -- maybe have a separate schema which uses more cross-correlation??
 
 8-2-2016
 ++change instrument pointy/rounded from 6 sec. to 3 sec. -- 6 sec. seemed like too big a window after init. trials... don't think of whole phrase movement when instrument changes... think of the time immediately before
 
 Questions to ask:
 ++do I need to put more / less notes? how to navigate?
 ++do the perceptual categories still hold??
 ++can you hear the reverb effect? -- sometimes... it is subtle
 
 8-7-2016
 ++NEW TODO: fix peak measurements -- should be size of peaks for ea. axis are compared against ea. other not just peaks whole-sale... or just try to normalize that first?
 fixed.
 
 
 8-23-2016
 added violin
 
 8-24-2016
 
 
 
 --------------------
 --------------------
 
 structural similarity???
 
----------
 --------
 Testing:
 To fix:
 ++reverb goes wonky? -- fixed, it was red-lining
 ++cello instrument a disaster (? fix --  ?)
 ++also look at the diff. var. for stepping leg? not just peak size? (fixed -- works better)
 
 changed orchestral window back to 5s.....
 fixed vel. (hopefully)
 
 -------------------- --------------------
 -------------------- --------------------
 
 ++busy/sparse took too long (too big of a window) (for volume-stepping) -- 8/12/2016 --> just changed from 2.5 to 1.5... maybe make sure that is the right window
 ++cello too classical ugh ugh cello --fixing...
 ++change midi velocity so that accents are more common...
 
++wrong guitar lick in places... section B beginning... whoops?
 
 -------------------- --------------------
 -------------------- --------------------
9/1/2016
++in max, fixed boleo 2 bangs
++sending accel signal to max, need to write plug-in
++records OSC when use the wii to record different wekinator dtw gestures
 
 -------------------- --------------------
 -------------------- --------------------
 9/3
++sends signals to max for mapping to audio processing for long steps (follower - melody, leader - accompaniment)
++if no steps for 4 bars turns off -- CHECK -- maybe need to check for leader, since only follower...
 
 9/8
 ++only continuous instruments for circling
 ++pitchbend fx

9/9
++quarternions added to send to wek
 
 
 
 -------------------- --------------------
 -------------------- --------------------
12/16/2016
++melody busy/sparse 0.75s window
++roomSize fixed
++numberOfBarsAfterStepBeforePausing = 2; //instead of 4
++added scale -- scaling busy sparse only
 
 -------------------- --------------------
 -------------------- --------------------

 1/20/2018
 TODO:
 Ableton -- fix pointy/smooth continous in ableton for two couples
 
 --fix send back for two couples
 
--------------
--------------
 change accompaniment:
 const float EVENT_MAPPING_WINDOW = 0.75;
 const float ORCHESTRAL_MAPPING_WINDOW = 2;
 
 ++so, there does not seem to be agreement among tango dancers about busy/sparse
 
 
Instructions:
 
 If running test:
 enable procedure:
    void setupSensorsForTest();
 in setup()
 Space bar to start. If running the test file, skip to 9000s (using 't').
 Add the video, if needed load via 'u' command, 'm' to show video as needed
 This is for one recording I have been using. If another, you must write another test program or use manual key commands to load
 everything individually. -- note to change back, change back in both creating orchestration player sections (MusicPlayer.h) & in loading the sound files (Soundfile.h)

 
 If running song or recording motion capture:
 enable  void addLiveDancer(SensorData *sensor, int sid);
 disable the setupSensorsForTest();
 this does all the sensor setup for 1 couple
 
 To record motion capture:
 Setup recording device with that software, and run (not on this computer)
 's' - to save all the sensors, press after everything is loaded
 
 
Keyboard commands:
's' - Save the sensor data
'i' - Init webcam (for recording motion capture)
'o' - Record a foot onset (to compare with onsets detected via analysis)
'm' - Show or hide a video of a recorded motion capture. It must be loaded.
'u' - Load a video, then open the sync file.Open picture file name\n";
'r' - Reset playback time to 0 + any FF part you added (hit after entering a number)
'l' - Load data from a previously recorded sensor
' ' - Play the loaded previously recorded sensor data
't' - Add seconds to playback time 0. Reset will go back to THIS time. Erase this by replacing with 0 or do another #
'd' - Output and set speed of current (loaded not live) sensor (?)
'c' - Change sensor color of selected shimmer (as displayed on screen)
'b' - Add a dancer - combines 3 sensors as dancer & via order specifies which sensor is on which body part. A dancer is not added until this is called 3 times
            1) left foot added 2) right foot added 3) back sensor added
            dancers can be referenced via the order 0-N that they were added
'p' - Add a pareja (couple) -- after 2nd add couple is created. First dancer added is leader.
'x' - Send OSC or not (starts with sending)
'z' - restart the song
 
 'q' - load first tango song by Courtney
 'a' - load 'por una cabeza'
 
 //for testing ornaments
 ',' - stillness
 ';' - circling
 '/' - boleo
 
*/

//#include "cinder/app/AppNative.h"
//#include "cinder/gl/gl.h"
//#include "cinder/gl/Texture.h"
//#include "cinder/Capture.h"
#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "cinder/app/RendererGl.h"
//#include "cinder/Capture.h" //needed for capture
#include "cinder/Log.h" //needed to log errors
#include "cinder/ip/Resize.h"
#include "cinder/Rand.h"
#include <fstream>
#include <sstream>
#include <stdlib.h>
//#include "statistics.h"
#include "fasper.h"
//#include <boost/thread.hpp>
//#include <boost/date_time/posix_time/posix_time.hpp>

//TODO add a define for if mac
#include <sys/resource.h>

#include "InteractiveTangoOSCMessages.h"

#define _COUT_DEBUG_ 0
#define SAVE_ALL_OSC 1 //define if you want to save all the incoming OSC
//#define WEKINATOR_ENABLED //if we are sending to wekinator
//#define PHOTO_SYNC_ENABLED  //if we are sending to another application to sync with a photo


//#include "OscSender.h"
//#include "OscListener.h"

#include "Osc.h"

#include "MidiUtility.h"


#include "MotionAnalysisParameterRanges.h"
#include "Containers.h"
#include "ShimmerData.h"
#include "VisualizeShimmer.h"
#include "ShimmerWrapper.h"
//#include "MocapCam.h"
#include "SensorData.h"
#include "MotionAnalysisOuput.h"
#include "InteractiveTangoSignalAnalysis.h"
#include "BeatTiming.h"
#include "InteractiveTangoStepDetection.h"
#include "PerceptualMappingScheme.h"
#include "PerceptualSchemas.h"
#include "SendOSCUGENs.h"
#include "GestureRecognizerUGENs.h"
#include "Instruments.h"
#include "SoundFile.h"
#include "SoundFileNDB.h"
#include "MusicPlayer.h"
#include "MappingSchemaEventInContinuousOut.h"
#include "MusicPlayerNDB.h"
#include "TangoFragmentsSectionALoader.h"
#include "Dancers.h"
#include "DancersNDB.h"

#include <sys/socket.h>
#include <netinet/in.h>

#include "SaveOSC.h"


using namespace ci;
using namespace ci::app;
using namespace std;
using protocol = asio::ip::udp;


class InteractiveTangoMilongaArm64App : public App {

public:
    void setup();
    void mouseDown( MouseEvent event );
    void keyDown( KeyEvent event );
    void update();
    void draw();
    SensorData * getSensor(std::string whichShimmer);
    InteractiveTangoMilongaArm64App();
    ~InteractiveTangoMilongaArm64App();
    
private:
    osc::ReceiverUdp mListener;
    osc::ReceiverUdp mBrentListener;
    SensorData * getSensor(string deviceID, int which);
    void drawGrid(float size=100.0f, float step=2.0f);
    void sendPhotoSync(float seconds);
    
    std::vector<SensorData *> mSensorData;
    std::vector<VisualizeShimmer *> visualizers;
    
    std::vector<InteractiveTango::UGEN *> mUGENs;
    int mHoldShimmerIndex;
    int mHoldShimmerIndex2;
    
    ci::ColorA generateRandomColor();
    void screenshot();
    void loadSensor(std::string input);
    void parseFilenameForInfo(string fname, string *deviceID, int *whichID);
    void addStepDetection(int id1, int id2, std::string port1, std::string port2);
    virtual void prepareSettings(Settings *settings);
    void handleOSC(osc::Message message, float seconds);
    void handleBrentOSC(osc::Message message, float seconds);
    //void handleDataOSCIPhone(osc::Message message, float seconds);
    void saveAllSensors();
    void updateSignalTree(float seconds, float pastSeconds);
    std::vector<ci::osc::Message> collectMessagesforDifferentPorts(std::string addr, std::vector<ci::osc::Message> *msgs, std::vector<ci::osc::Message> nmsgs);
    std::vector<ci::osc::Message> collectMessagesforDifferentPortsBack(std::vector<ci::osc::Message> *msgs);
    
    void loadChosenFiles();
    void loadFragmentsStaccatoSavedFiles();
    void loadPorUnaCabezaNormSavedFiles();
    void loadPorUnaCabezaStaccatoSavedFiles();
    void playPorUnaCabeza();
    void playElChoclo();
    void playFragments();
    void playLaCumparsita();
    void addPareja();
    void addDancer();
    void stopOSCToAbleton();
    void startOSCToAbleton();
    void stopStartOSCToAbleton();
    void restartSong();
    InteractiveTango::Pareja::SongIDs whichSongIsPlaying;
    void cycleThroughPlaylist();
    void changeSensorColor();
    void addMarkerToSensorFile(std::string marker);
    
    void setupSensorsForTest();
    void setupSensorsForTestTwoDancers();
    
    void printMenu();
    
    //for handling wii
    void handleWiiOSC(osc::Message message); // turn into commands in application
    void wiiToWekinator(osc::Message message); // forward commands to wekinator
    void printNormalWiiMenu();
    bool isWiiButtonDown(std::string addr, osc::Message message);
    
    //for machine learning in Wekinator, etc.
    bool isSendingToWekinator; //whether wii is sending messages to wekinator
    std::vector<osc::SenderUdp *> wekinatorSenders;
    std::vector<osc::SenderUdp *> wiiWekinatorSenders;
    std::vector<InteractiveTango::SendOSCToWek *> sendOSCToWekinators;
    int whichDTWWekinatorOutput;
    int selectedWekinatorSenderIndex;
    InteractiveTango::MidiOutUtility midiOut;
    
    void printWekModeMenu();
    void printChangingParamsMenu();
    int addSendtoWekinator();
    void sendOSCInputsToWekinator(std::vector<ci::osc::Message> msgs, float seconds, int index );
    void handleOutputWekOSC( osc::Message message, float seconds );


    //use wii to change movement min/max params....
    bool isChangingMovementParams;
    void changeMovementParamsWithWii(osc::Message message);
    
    
    //if there are any OSC messages here, clear before proceeding.
//    void clearResidualData();
    void clearListeningUDPSocket();
    
    void addTestDancersNoSensors();

    
    int beatIndex; //for debug

    std::string mSaveFilePath;
    
    ci::CameraPersp     mCamera;
//    MoCapCamera         moCam;
    Timer               mTimer;
    long                mIndex;
    bool                mShowMovie;
    bool                mPlayPrevious;
//    ReadMocapCameraFile    *mMocapFilePlayer;
    
    bool                playing_done;
    
    std::string         whichShimmer;
    osc::SenderUdp         syncSender;
    int                 selectedPareja; //for changing motion params, etc.
    
    //different senders for different messages
    //why? bc m4l is CRAZY
    osc::SenderUdp         sender;
    osc::SenderUdp         pVSsSender;
    osc::SenderUdp         ornamentSender; //this sends the ornaments... OSC is getting dropped, so testing if this is why (too much sending at once);
    osc::SenderUdp         circlingSender; //send circling steps
    osc::SenderUdp         longStepSender; //send long step info (not yet mapped)
    osc::SenderUdp         reverbSender;
    osc::SenderUdp         backSender;
//    osc::SenderUdp         remoteLaptopSender;
    void setupSenderUdp( osc::SenderUdp *sender );
//    osc::SenderUdp         backSenderFollower;
    int localPortCount;
    int getLocalPort()
    {
        localPortCount++;
        return localPortCount + LOCALPORT_START;
    };
    
    //saving OSC that the program receives
    InteractiveTango::SaveOSC *mSaveOSC;
    InteractiveTango::PlayOSC *mPlayOSC;
    
    void sendOSCMessages( std::vector<ci::osc::Message> msgs, float seconds  );
    void sendMidiMessages( std::vector<InteractiveTango::MidiMessage *> msgs);

    
    //to send OSC messages, etc. differing rates, keep Hz down
    float pastseconds;
    float lastSentOSC;

    
    void addCrossCoVar(int id1, int id2, std::string p1, std::string p2);
    
    std::vector<InteractiveTango::Dancer *> mDancers;
    std::vector<InteractiveTango::Pareja *> mParejas;
    InteractiveTango::DanceFloorNDB *danceFloor;

    InteractiveTango::BeatTiming mBeatTimer;
    bool    receivedBeat;
    
    //add dancers automatically
    bool addLiveDancers; //note -- this can set to false for testing I assume.
    void addLivePareja(std::map<int, int> &l, std::map<int, int> &f, int lID, int fID);
    void addLiveDancer(SensorData *sensor, int sid);
    std::map<int, int> leader, follower, leader2, follower2;
     
    bool shouldSendOSC;
    
    //steps controlled by a keyboard instead of a dancer
    bool fakeBSLeaderMode;
    bool fakeBSFollowerMode;
    bool fakeBSAccompMode;
    bool midiMappingMode;
    
    //error for osc sender
    void onSendError( asio::error_code error );
    
    
//    Engine *ep; //matlab engine
};

InteractiveTangoMilongaArm64App::InteractiveTangoMilongaArm64App() :
//setup the OSC senders
//mSender(LOCALPORT, DESTHOST, DESTPORT)
sender( getLocalPort(), SELF_IPADDR, SC_LANG_PORT ),
reverbSender( getLocalPort(), SELF_IPADDR, REVERB_PORT ),
pVSsSender( getLocalPort(), SELF_IPADDR, POINTY_VS_SMOOTH_PORT ),
syncSender( getLocalPort(), PC_IPADDR, PHOTO_TIMING_PORT ),
ornamentSender(getLocalPort(), SELF_IPADDR, ORNAMENT_PORT ),
circlingSender(getLocalPort(), SELF_IPADDR, CIRCLING_PORT ), //send circling steps
longStepSender(getLocalPort(), SELF_IPADDR, LONGSTEP_PORT ), //send long step info (not yet mapped)
backSender(getLocalPort(), SELF_IPADDR, BACK_PORT),
mListener(OSC_LISTENING_PORT),
mBrentListener(BRENT_LISTENING_PORT)
//remoteLaptopSender(getLocalPort(), REMOTELAPTOP_ADDRESS, REMOTELAPTOP_PORT)
{
    localPortCount=0;
}

InteractiveTangoMilongaArm64App::~InteractiveTangoMilongaArm64App()
{
    sender.close();
    reverbSender.close();
    pVSsSender.close();
    syncSender.close();
    ornamentSender.close();
    circlingSender.close();
    longStepSender.close();
    backSender.close();
    mListener.close();
    mBrentListener.close();
//    remoteLaptopSender.close();
}


// Unified error handler. Easiest to have a bound function in this situation,
// since we're sending from many different places.
void InteractiveTangoMilongaArm64App::onSendError( asio::error_code error )
{
    if( error ) {
        CI_LOG_E( "Error sending: " << error.message() << " val: " << error.value() );
        
        // If you determine that this error is fatal, make sure to flip mIsConnected. It's
        // possible that the error isn't fatal.
//        try {
//#if ! USE_UDP
//            // If this is Tcp, it's recommended that you shutdown before closing. This
//            // function could throw. The exception will contain asio::error_code
//            // information.
//            mSender.shutdown();
//#endif
//            // Close the socket on exit. This function could throw. The exception will
//            // contain asio::error_code information.
//            mSender.close();
//        }
//        catch( const osc::Exception &ex ) {
//            CI_LOG_EXCEPTION( "Cleaning up socket: val -" << ex.value(), ex );
//        }
//        quit();
    }
}

void InteractiveTangoMilongaArm64App::sendOSCInputsToWekinator(std::vector<ci::osc::Message> msgs, float seconds, int index )
{
    if( msgs.empty() ) return;
    
    ci::osc::SenderUdp *send = wekinatorSenders[index];
    
    for(int i=0; i<msgs.size(); i++)
    {
        send->send(msgs[i], std::bind( &InteractiveTangoMilongaArm64App::onSendError,
                                      this, std::placeholders::_1 ));
        
        
//        std::cout << "Sending.. " << msgs[i].getAddress() << "   ";
//        for( int j = 0; j<msgs[i].getNumArgs(); j++ )
//        {
//            std::cout << msgs[i].getArgFloat(j) << ",";
//        }
//        std::cout << std::endl;
    }
}

std::vector<ci::osc::Message> InteractiveTangoMilongaArm64App::collectMessagesforDifferentPorts(std::string addr, std::vector<ci::osc::Message> *msgs, std::vector<ci::osc::Message> nmsgs)
{
    
    int i = 0;
    while ( i < msgs->size() )
    {
        std::vector<int> toDelete;
        //boleo messages (last ones) are getting lost -- trying to see if sending through another port helps.
        if( !(msgs->at(i).getAddress().compare(addr) ) )
        {
            nmsgs.push_back(msgs->at(i));
            msgs->erase(msgs->begin()+i);
        }
        else
        {
            i++;
        }
    }
    return nmsgs;
};

std::vector<ci::osc::Message> InteractiveTangoMilongaArm64App::collectMessagesforDifferentPortsBack(std::vector<ci::osc::Message> *msgs)
{
    std::vector<ci::osc::Message> nmsgs;
    int i = 0;
    std::vector<int> toDelete;

    while ( i < msgs->size() )
    {
        //boleo messages (last ones) are getting lost -- trying to see if sending through another port helps.
        if( !(msgs->at(i).getAddress().compare(SEND_BACK)) ||
              !(msgs->at(i).getAddress().compare(SEND_LEFTFOOT)) ||
              !(msgs->at(i).getAddress().compare(SEND_RIGHTFOOT)))
        {
            nmsgs.push_back(msgs->at(i));
            toDelete.push_back(i);
        }
        i++;
    }
    
    //delete from msgs -- since previous had an error, only with send-back... why? WHY? I DO NOT KNOW.
    int j = 0;
    int minus =0;
    while( j < toDelete.size() )
    {
        msgs->erase(msgs->begin() + (toDelete[j]-minus));
        minus++;
        j++;
    }
    
    return nmsgs;
};


void InteractiveTangoMilongaArm64App::sendOSCMessages( std::vector<ci::osc::Message> msgs, float seconds )
{
//TODO: this should be refactored into something that makes sense
//But now it sends signals to different ports and sends continuous data at a set rate, now 15Hz
    
    if( msgs.empty() ) return;
    
//    for(int i=0; i<msgs.size(); i++)
//        std::cout << msgs[i].getAddress() << "\n" ;
    
    lastSentOSC += ( seconds - pastseconds );
    
    ci::osc::SenderUdp *send;
    bool shouldSend = true; //um ya. always true now.
    std::string firstAddress = msgs[0].getAddress();
    
    std::vector<ci::osc::Message> ornament_msgs;
    ornament_msgs = collectMessagesforDifferentPorts(CREATE_BOLEO_ORNAMENT, &msgs, ornament_msgs);

    std::vector<ci::osc::Message> circling_msgs;
    circling_msgs = collectMessagesforDifferentPorts(CIRCLING_VOLUME, &msgs, circling_msgs);
    circling_msgs = collectMessagesforDifferentPorts(CIRCLING_GYRO_VECTOR_LENGTH, &msgs, circling_msgs);
    circling_msgs = collectMessagesforDifferentPorts(CIRCLING_ACCEL, &msgs, circling_msgs);
    circling_msgs = collectMessagesforDifferentPorts(CIRCLING_GYR, &msgs, circling_msgs);
    circling_msgs = collectMessagesforDifferentPorts(PLAY_CIRCLING_CLIP, &msgs, circling_msgs);
    
    std::vector<ci::osc::Message> longstep_msgs;
    longstep_msgs = collectMessagesforDifferentPorts(LONGSTEP_ACCEL, &msgs, longstep_msgs);
    longstep_msgs = collectMessagesforDifferentPorts(LONGSTEP_START, &msgs, longstep_msgs);
    longstep_msgs = collectMessagesforDifferentPorts(LONGSTEP_END, &msgs, longstep_msgs);
    
//    std::vector<ci::osc::Message>  back_msgs = collectMessagesforDifferentPortsBack(&msgs);
    
    std::vector<ci::osc::Message> reverb_msgs;
    reverb_msgs = collectMessagesforDifferentPorts(ROOM_SIZE_GROUP_SIMILARITY, &msgs, reverb_msgs);
    
    //solution FOR NOW -- refactor later
    
    //init
    send = &sender;
    
//    if( !firstAddress.compare( PLAY_CLIP ) || !firstAddress.compare( PLAY_MELODY_CLIP ) )
//    {
//        send = &sender;
//        shouldSend = true;
//    }
//    else
//    {
        if(lastSentOSC > OSC_SEND_AVG_SPEED )
        {
            if( !firstAddress.compare( STEPTIME_REVERBTAIL ) )
            {
                send = &reverbSender;
            }
            else if( !firstAddress.compare( POINTY_VS_SMOOTH ) )
            {
                send = &pVSsSender;
            }
            shouldSend = true;
            lastSentOSC = 0;
        }
        else
        {
            shouldSend = true;
            send = &sender;
        
        }
//    }

    if( shouldSend )
    {
        for(int i=0; i<msgs.size(); i++)
        {
            if(send != NULL) send->send(msgs[i], std::bind( &InteractiveTangoMilongaArm64App::onSendError,
                                                           this, std::placeholders::_1 ));
            
            //send the remote laptop as well.
//            remoteLaptopSender.send(msgs[i], std::bind( &InteractiveTangoMilongaArm64App::onSendError, this, std::placeholders::_1 ));
            
//            if(msgs[i].getNumArgs() >= 1)
//            {
//                if(msgs[i].getArgType(0) == osc::ArgType::FLOAT)
//                {
//                    std::cout << msgs[i].getArgFloat(0);
//                }
//                if(msgs[i].getArgType(0) == osc::ArgType::DOUBLE)
//                {
//                    std::cout << msgs[i].getArgDouble(0);
//                }
//                std::cout << std::endl;
//            }
//
//            if(msgs[i].getNumArgs() >= 2)
//            {
//                if(msgs[i].getArgType(1) == osc::ArgType::FLOAT)
//                {
//                    std::cout << msgs[i].getArgFloat(1);
//                }
//                if(msgs[i].getArgType(1) == osc::ArgType::DOUBLE)
//                {
//                    std::cout << msgs[i].getArgDouble(1);
//                }
//                std::cout << std::endl;
//            }
            
        }
        for(int i=0; i<ornament_msgs.size(); i++)
        {
            ornamentSender.send(ornament_msgs[i], std::bind( &InteractiveTangoMilongaArm64App::onSendError,
                                                            this, std::placeholders::_1 ));
        }
        for(int i=0; i<circling_msgs.size(); i++)
        {
            circlingSender.send(circling_msgs[i], std::bind( &InteractiveTangoMilongaArm64App::onSendError,
                                                            this, std::placeholders::_1 ));
        }
//        for(int i=0; i<longstep_msgs.size(); i++)
//        {
//            longStepSender.send(longstep_msgs[i]);
//        }
//        for(int i=0; i<back_msgs.size(); i++)
//        {
//            backSender.send(back_msgs[i], std::bind( &InteractiveTangoMilongaArm64App::onSendError,
//                                                    this, std::placeholders::_1 ));
//
//            std::cout << "sending back messages: " << back_msgs[i].getAddress() << std::endl;
//        }
        for(int i=0; i<reverb_msgs.size(); i++)
        {
            reverbSender.send(reverb_msgs[i], std::bind( &InteractiveTangoMilongaArm64App::onSendError,
                                                        this, std::placeholders::_1 ));
        }

    }
}

void InteractiveTangoMilongaArm64App::loadPorUnaCabezaStaccatoSavedFiles()
{
    //load sensors
    loadSensor("/Users/courtney/Documents/Dissertation Work - Interactive Tango/trials_recordings_3_1_2016/puc_staccato.porUnaCabeza_more_staccatoitm_file2106_2_7___Courtney__5__0__2__2.csv");
    
    std::cout << "Loaded Courtney's back sensor\n";
    
    loadSensor("/Users/courtney/Documents/Dissertation Work - Interactive Tango/trials_recordings_3_1_2016/puc_staccato.porUnaCabeza_more_staccatoitm_file2106_2_7___Courtney__0__0__2__1.csv");
    
    std::cout << "Loaded Courtney's left foot sensor\n";
    
    
    loadSensor("/Users/courtney/Documents/Dissertation Work - Interactive Tango/trials_recordings_3_1_2016/puc_staccato.porUnaCabeza_more_staccatoitm_file2106_2_7___Courtney__1__0__2__0.csv");
    
    std::cout << "Loaded Courtney's right foot sensor\n";
    
    
    loadSensor("/Users/courtney/Documents/Dissertation Work - Interactive Tango/trials_recordings_3_1_2016/puc_staccato.porUnaCabeza_more_staccatoitm_file2106_2_7___Brent__5__0__1__2.csv");
    
    std::cout << "Loaded Brent's back sensor\n";
    
    loadSensor("/Users/courtney/Documents/Dissertation Work - Interactive Tango/trials_recordings_3_1_2016/puc_staccato.porUnaCabeza_more_staccatoitm_file2106_2_7___Brent__0__0__1__1.csv");
    
    std::cout << "Loaded Brent's left foot sensor\n";
    
    loadSensor("/Users/courtney/Documents/Dissertation Work - Interactive Tango/trials_recordings_3_1_2016/puc_staccato.Brent__1__0__1__0.csv");
    
    std::cout << "Loaded all sensors\n";
}


void InteractiveTangoMilongaArm64App::loadFragmentsStaccatoSavedFiles()
{
    //load sensors
    loadSensor("/Users/courtney/Documents/Dissertation Work - Interactive Tango/trials_recordings_3_1_2016/courtneyBrentTestStaccato.diSarliCourtney_BrentTake2_staccatoitm_file2106_2_7___Courtney__5__0__2__2.csv");
    
    std::cout << "Loaded Courtney's back sensor\n";
    
    loadSensor("/Users/courtney/Documents/Dissertation Work - Interactive Tango/trials_recordings_3_1_2016/courtneyBrentTestStaccato.diSarliCourtney_BrentTake2_staccatoitm_file2106_2_7___Courtney__0__0__2__1.csv");
    
    std::cout << "Loaded Courtney's left foot sensor\n";
    
    
    loadSensor("/Users/courtney/Documents/Dissertation Work - Interactive Tango/trials_recordings_3_1_2016/courtneyBrentTestStaccato.Courtney__1__0__2__0.csv");
    
    std::cout << "Loaded Courtney's right foot sensor\n";
    
    
    loadSensor("/Users/courtney/Documents/Dissertation Work - Interactive Tango/trials_recordings_3_1_2016/courtneyBrentTestStaccato.diSarliCourtney_BrentTake2_staccatoitm_file2106_2_7___Brent__5__0__1__2.csv");
    
    std::cout << "Loaded Brent's back sensor\n";
    
    loadSensor("/Users/courtney/Documents/Dissertation Work - Interactive Tango/trials_recordings_3_1_2016/courtneyBrentTestStaccato.diSarliCourtney_BrentTake2_staccatoitm_file2106_2_7___Brent__0__0__1__1.csv");
    
    std::cout << "Loaded Brent's left foot sensor\n";
    
    loadSensor("/Users/courtney/Documents/Dissertation Work - Interactive Tango/trials_recordings_3_1_2016/courtneyBrentTestStaccato.diSarliCourtney_BrentTake2_staccatoitm_file2106_2_7___Brent__1__0__1__0.csv");
    
    std::cout << "Loaded all sensors for Fragments\n";
}

void InteractiveTangoMilongaArm64App::loadChosenFiles()
{
    //load sensors

    for(int i=0; i<6; i++)
    {
        std::cout << "Load sensor\n";
        loadSensor(App::getOpenFilePath().c_str());
    }
    
    std::cout << "Loaded all sensors\n";
    
}


void InteractiveTangoMilongaArm64App::loadPorUnaCabezaNormSavedFiles()
{
    //load sensors
    loadSensor("/Users/courtney/Documents/Dissertation Work - Interactive Tango/trials_recordings_3_1_2016/porUnaCabeza_norm.porUnaCabeza_trial_1itm_file2106_2_7___Courtney__5__0__2__2.csv");
    
    std::cout << "Loaded Courtney's back sensor\n";
    
    loadSensor("/Users/courtney/Documents/Dissertation Work - Interactive Tango/trials_recordings_3_1_2016/porUnaCabeza_norm.porUnaCabeza_trial_1itm_file2106_2_7___Courtney__0__0__2__1.csv");
    
    std::cout << "Loaded Courtney's left foot sensor\n";
    
    
    loadSensor("/Users/courtney/Documents/Dissertation Work - Interactive Tango/trials_recordings_3_1_2016/porUnaCabeza_norm.porUnaCabeza_trial_1itm_file2106_2_7___Courtney__1__0__2__0.csv");
    
    std::cout << "Loaded Courtney's right foot sensor\n";
    
    
    loadSensor("/Users/courtney/Documents/Dissertation Work - Interactive Tango/trials_recordings_3_1_2016/porUnaCabeza_norm.porUnaCabeza_trial_1itm_file2106_2_7___Brent__5__0__1__2.csv");
    
    std::cout << "Loaded Brent's back sensor\n";
    
    loadSensor("/Users/courtney/Documents/Dissertation Work - Interactive Tango/trials_recordings_3_1_2016/porUnaCabeza_norm.porUnaCabeza_trial_1itm_file2106_2_7___Brent__0__0__1__1.csv");
    
    std::cout << "Loaded Brent's left foot sensor\n";
    
    loadSensor("/Users/courtney/Documents/Dissertation Work - Interactive Tango/trials_recordings_3_1_2016/porUnaCabeza_norm.Brent__1__0__1__0.csv");
    
    std::cout << "Loaded all sensors\n";
    
    std::cout << "Loaded Por Una Cabeza song\n";
}

//std::vector<osc::Sender *> wekinatorSenders;
//std::vector<InteractiveTango::SendOSCToWek *> sendOSCToWekinators;
//int whichDTWWekinatorOutput;
//int selectedWekinatorSenderIndex;
int InteractiveTangoMilongaArm64App::addSendtoWekinator()
{
    
//SWITCH!!!!!!!!!!!!!!!!
//    osc::Sender *s = new osc::Sender();
//    s->setup(WEK_ADDR, WEKINATOR_PORT + sendOSCToWekinators.size());
//    wekinatorSenders.push_back(s);
//
//    osc::Sender *s2 = new osc::Sender();
//    s2->setup(WEK_ADDR, WEKINATOR_PORT_DTW - sendOSCToWekinators.size());
//    wiiWekinatorSenders.push_back(s2);
    
    osc::SenderUdp *s = new osc::SenderUdp(getLocalPort(), WEK_ADDR, WEKINATOR_PORT + sendOSCToWekinators.size());
    setupSenderUdp(s);
    wekinatorSenders.push_back(s);
    
    osc::SenderUdp *s2 = new osc::SenderUdp(getLocalPort(), WEK_ADDR, WEKINATOR_PORT_DTW - sendOSCToWekinators.size());
    setupSenderUdp(s2);
    wiiWekinatorSenders.push_back(s2);
    
    int index = sendOSCToWekinators.size();
    std::stringstream ss;
    ss << WEKINATOR_INPUTS << "/" << index;
    InteractiveTango::SendOSCToWek  *so = new InteractiveTango::SendOSCToWek( ss.str() );
    sendOSCToWekinators.push_back(so);
    
    return index;
}

void InteractiveTangoMilongaArm64App::setupSensorsForTest()
{
    //switch which function to change which files are loaded
//    loadPorUnaCabezaStaccatoSavedFiles();
//    loadPorUnaCabezaNormSavedFiles();
 //   loadFragmentsStaccatoSavedFiles();
    loadChosenFiles();
    
#ifdef WEKINATOR_ENABLED
    //right legs, then left
    for(int i=0; i<4; i++)
        addSendtoWekinator();
#endif
    

    
    //add the dancer 1
    InteractiveTango::Dancer *dancer = new InteractiveTango::Dancer(&mBeatTimer, sendOSCToWekinators[0], sendOSCToWekinators[2]);
    dancer->addSensorBodyPart(3, mSensorData[3], &mUGENs, InteractiveTango::Dancer::LeftFoot);
    dancer->addSensorBodyPart(4, mSensorData[4], &mUGENs, InteractiveTango::Dancer::RightFoot);
    dancer->addSensorBodyPart(5, mSensorData[5], &mUGENs, InteractiveTango::Dancer::Back);
    dancer->setDancerID( 0, &mUGENs );
    
    
    //add dancer/limb info to sensor data, so that it is saved in the file
    
    mSensorData[3]->setDancerLimb(mDancers.size(), InteractiveTango::Dancer::LeftFoot);
    mSensorData[4]->setDancerLimb(mDancers.size(), InteractiveTango::Dancer::RightFoot);
    mSensorData[5]->setDancerLimb(mDancers.size(), InteractiveTango::Dancer::Back);
    
    mDancers.push_back(dancer);
    std::cout << "Added Brent as a dancer\n";

    
    //add the dancer 2
    InteractiveTango::Dancer *dancer2 = new InteractiveTango::Dancer(&mBeatTimer, sendOSCToWekinators[1], sendOSCToWekinators[3]);
    dancer2->addSensorBodyPart(0, mSensorData[0], &mUGENs, InteractiveTango::Dancer::LeftFoot);
    dancer2->addSensorBodyPart(1, mSensorData[1], &mUGENs, InteractiveTango::Dancer::RightFoot);
    dancer2->addSensorBodyPart(2, mSensorData[2], &mUGENs, InteractiveTango::Dancer::Back);
    dancer2->setDancerID( 1, &mUGENs );
    
    //add dancer/limb info to sensor data, so that it is saved in the file
    mSensorData[0]->setDancerLimb(mDancers.size(), InteractiveTango::Dancer::LeftFoot);
    mSensorData[1]->setDancerLimb(mDancers.size(), InteractiveTango::Dancer::RightFoot);
    mSensorData[2]->setDancerLimb(mDancers.size(), InteractiveTango::Dancer::Back);
    
    mDancers.push_back(dancer2);
    std::cout << "Added Courtney as a dancer\n";

    
    //add the dancer
    InteractiveTango::Pareja *pareja = new InteractiveTango::Pareja(mDancers[1], mDancers[0], &mUGENs, &mBeatTimer );
    mParejas.push_back(pareja);
    danceFloor->addPareja(pareja);
    danceFloor->loadCourtneyTangoSongNumberOneIntoPlayer();

    std::cout << "Added a pareja\n";

    beatIndex=0;
    
    playing_done = false;
    
    
    //load the song
//    playPorUnaCabeza();
    
    
}

void InteractiveTangoMilongaArm64App::setupSensorsForTestTwoDancers()
{
    setupSensorsForTest();
    std::cout << "Added first couple\n";

    setupSensorsForTest();
    std::cout << "Added second couple\n";
}


void InteractiveTangoMilongaArm64App::clearListeningUDPSocket()
{
    //get any waiting messages now.
    int fd;
    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("cannot create socket");
        return;
    }
    struct sockaddr_in myaddr;
    memset((char *)&myaddr, 0, sizeof(myaddr));
    myaddr.sin_family = AF_INET;
    myaddr.sin_addr.s_addr = htonl(0x7f000001);
    myaddr.sin_port = htons(OSC_LISTENING_PORT);
    
    if ( ::bind(fd, (struct sockaddr *) &myaddr, sizeof(myaddr)) < 0 ) {
        perror("bind failed");
        return ;
    }
    
    ::close(fd);
};

void InteractiveTangoMilongaArm64App::setupSenderUdp(osc::SenderUdp *sender)
{
    try {
        // Bind the sender to the endpoint. This function may throw. The exception will
        // contain asio::error_code information.
        sender->bind();
    }
    catch ( const osc::Exception &ex ) {
        CI_LOG_E( "Error binding: " << ex.what() << " val: " << ex.value() );
        quit();
    }
}

void InteractiveTangoMilongaArm64App::setup()
{
//setup to save OSC
#ifdef SAVE_ALL_OSC
    fs::path savePath = getSaveFilePath();
    mSaveOSC = new InteractiveTango::SaveOSC(savePath.c_str());
#else
    mSaveOSC = NULL;
#endif
    
    //always starts null -- need to press 'p' to open an file to play the saved OSC
    mPlayOSC = NULL;
    
    //just in case
    clearListeningUDPSocket();
    
    //listening to sensor data
//    mListener.setup( OSC_LISTENING_PORT );
    std::string oscMessages[] = {ABLETON_SYNC, ABLETON_BPM, ABLETON_BUSYSPARSE_SCALE, WII_PREFIX, WEKINATOR_OUTPUT_PREFIX, OSC_SHIMMERDATA,
        OSC_ANDROID_SENSOR_DATA, ANDROID_ORIENTATION, ANDROID_STEPDETECTION, ABLETON_STACCATO_LEGATO_INSTR_SCALE, OSC_M5STICK_DATA,
        DATA_OSC_IPHONE_DATA_ACCELX,
        DATA_OSC_IPHONE_DATA_ACCELY,
        DATA_OSC_IPHONE_DATA_ACCELZ,
        DATA_OSC_IPHONE_DATA_GYRX,
        DATA_OSC_IPHONE_DATA_GYRY,
        DATA_OSC_IPHONE_DATA_GYRZ
    };
    int OSC_MSG_NUMBER = 18;
    for (int i=0; i<OSC_MSG_NUMBER; i++)
    {
        std::stringstream addr;
        mListener.setListener( oscMessages[i], [&]( const osc::Message &msg ){
//            saveOSC->add(msg, getElapsedSeconds()); //add this line to save the OSC //this is handleOSC
            setpriority(PRIO_PROCESS, 0, 1);
            handleOSC(msg, getElapsedSeconds());
        });
    }
    
    std::string oscMessagesBrent[] = {DATA_OSC_IPHONE_DATA_ACCELX,
        DATA_OSC_IPHONE_DATA_ACCELY,
        DATA_OSC_IPHONE_DATA_ACCELZ,
        DATA_OSC_IPHONE_DATA_GYRX,
        DATA_OSC_IPHONE_DATA_GYRY,
        DATA_OSC_IPHONE_DATA_GYRZ};
    
    int OSC_BRENT_MSG_NUMBER = 6;
    for (int i=0; i<OSC_BRENT_MSG_NUMBER; i++)
    {
        std::stringstream addr;
        mBrentListener.setListener( oscMessagesBrent[i], [&]( const osc::Message &msg ){
//            saveOSC->add(msg, getElapsedSeconds()); //add this line to save the OSC //this is handleOSC
            setpriority(PRIO_PROCESS, 0, 1);
            handleBrentOSC(msg, getElapsedSeconds());
        });
    }
    
    
    
    try {
        // Bind the receiver to the endpoint. This function may throw.
        mListener.bind();
    }
    catch( const osc::Exception &ex ) {
        CI_LOG_E( "Error binding: " << ex.what() << " val: " << ex.value() );
        quit();
    }
    
    try {
        // Bind the receiver to the endpoint. This function may throw.
        mBrentListener.bind();
    }
    catch( const osc::Exception &ex ) {
        CI_LOG_E( "Error binding: " << ex.what() << " val: " << ex.value() );
        quit();
    }
    
    // UDP opens the socket and "listens" accepting any message from any endpoint. The listen
    // function takes an error handler for the underlying socket. Any errors that would
    // call this function are because of problems with the socket or with the remote message.
    // UDP opens the socket and "listens" accepting any message from any endpoint. The listen
    // function takes an error handler for the underlying socket. Any errors that would
    // call this function are because of problems with the socket or with the remote message.
    mListener.listen(
                     []( asio::error_code error, protocol::endpoint endpoint ) -> bool {
                         if( error ) {
                             CI_LOG_E( "Error Listening: " << error.message() << " val: " << error.value() << " endpoint: " << endpoint );
                             return false;
                         }
                         else
                             return true;
                     });
    
    mBrentListener.listen(
                     []( asio::error_code error, protocol::endpoint endpoint ) -> bool {
                         if( error ) {
                             CI_LOG_E( "Error Listening: " << error.message() << " val: " << error.value() << " endpoint: " << endpoint );
                             return false;
                         }
                         else
                             return true;
                     });
    
    setupSenderUdp(&sender);
    setupSenderUdp(&reverbSender);
    setupSenderUdp(&pVSsSender);
    setupSenderUdp(&syncSender);
    setupSenderUdp(&ornamentSender);
    setupSenderUdp(&circlingSender);//send circling steps
    setupSenderUdp(&longStepSender); //send long step info (not yet mapped)
    setupSenderUdp(&backSender);
//    setupSenderUdp(&remoteLaptopSender);

    
//    clearResidualData(); //even more just in case
    
    //init to false
    fakeBSLeaderMode = false;
    fakeBSFollowerMode = false;
    fakeBSAccompMode = false;
    midiMappingMode = false;
    
    mSaveFilePath = "";
    
    // set up the camera for screen display
    mCamera.setEyePoint(vec3(5.0f, 10.0f, 10.0f));
    mCamera.lookAt(vec3(0.0f, 0.0f, 0.0f));
    mCamera.setPerspective(60.0f, getWindowAspectRatio(), 1.0f, 1000.0f);
    
    // start timer and init variables for mocap replay
    mPlayPrevious = false;
//    mMocapFilePlayer = NULL;
    mShowMovie = false;
    mIndex = 0;
    
    //init indices, which hold user-selected sensor indices (yes, includes Androids)
    mHoldShimmerIndex = -1;
    mHoldShimmerIndex2 = -1;

     //initialize tempo / timing information
    mBeatTimer.setBPM(100);
    mBeatTimer.setError(2.1);
    receivedBeat = false;
    
    danceFloor =  new InteractiveTango::DanceFloorNDB(&mBeatTimer, &midiOut);
    
    //setupSensorsForTest();
    //setupSensorsForTestTwoDancers();
    addLiveDancers = true;
    shouldSendOSC = true;
    playing_done = false;

    lastSentOSC = 0;
    pastseconds = 0;
    
    //wekinator initialization & setting wii state
    isSendingToWekinator = false;
    isChangingMovementParams=false;
    selectedPareja = 0; //start w 0;
    printNormalWiiMenu();
    whichDTWWekinatorOutput = 1;
    selectedWekinatorSenderIndex = 0; //none created or connected
//    outputWekListener.setup(WEKINATOR_OUTPUT_PORT);
    
    //init for use of rand() throughout
    std::srand(time(NULL));
    
    //start timer AFTER initialization...
    mTimer.start(); //TODO: may need to use 'getElapsedSeconds' instead...
    
}

void InteractiveTangoMilongaArm64App::prepareSettings(Settings *settings)
{
    // settings->setWindowSize(1000,800);
    settings->setTitle("Interactive Tango Motion Capture and Analysis");
    settings->setFrameRate(FRAMERATE); //set fastest framerate
}

void InteractiveTangoMilongaArm64App::mouseDown( MouseEvent event )
{
    
}

void InteractiveTangoMilongaArm64App::sendPhotoSync(float seconds)
{
//send sync message to a machine recording mocap video, so that sensor & video are sync'd
    
    osc::Message msg;
    msg.setAddress(PHOTO_SYNC);
    msg.append(seconds);
    syncSender.send(msg, std::bind( &InteractiveTangoMilongaArm64App::onSendError,
                                   this, std::placeholders::_1 ));
}

void InteractiveTangoMilongaArm64App::handleOutputWekOSC(osc::Message message, float seconds)
{
    
    //remove '/wek' prefix + next '/'
    std::string addr = message.getAddress();
    std::string prefix = WEKINATOR_OUTPUT_PREFIX;
    addr = addr.substr( prefix.size()+1 );
    
    std::string sDancer = addr.substr(0, addr.find('/'));
    int whichDancer = std::atoi( sDancer.c_str() );
    InteractiveTango::Dancer::BodyPart whichBodyPart = InteractiveTango::Dancer::BodyPart::RightFoot; //init -- TODO: FIX WITH NONE
    
    if( addr.find( WEKINATOR_OUTPUTS ) != -1 ) return; //leave

    
    if(_COUT_DEBUG_)
    {
        if(whichDancer == 0)
        {
            std::cout << " Leader,";
        }
        else
        {
            std::cout << " Follower,";
        }
    }

    
    //ok, now find if right foot -- umm, so only doing right foot now
    if( addr.find(WEKINATOR_RIGHTFOOT) != -1 )
    {
        whichBodyPart = InteractiveTango::Dancer::BodyPart::RightFoot;
        if(_COUT_DEBUG_) std::cout << " right foot: ";
    }
    else if( addr.find(WEKINATOR_LEFTFOOT) != -1 )
    {
        whichBodyPart = InteractiveTango::Dancer::BodyPart::LeftFoot;
        if(_COUT_DEBUG_) std::cout << " left foot: ";
    }
    
    if( addr.find( WEKINATOR_OUTPUTS ) != -1 )
    {
        //don't anything yet......
    }
    else
    {
        InteractiveTango::TangoGestures gesture = InteractiveTango::TangoGestures::STILLNESS;
        
        if( addr.find( WEKINATOR_STILLNESS ) != -1 )
        {
            gesture = InteractiveTango::TangoGestures::STILLNESS;
            if(_COUT_DEBUG_) std::cout << "stillness\n";
        }
        else if( addr.find( WEKINATOR_CIRCLING ) != -1 )
        {
            gesture = InteractiveTango::TangoGestures::CIRCLING;
            if(_COUT_DEBUG_) std::cout << "circling\n";

        }
        else if( addr.find( WEKINATOR_BOLEO ) != -1 )
        {
            gesture = InteractiveTango::TangoGestures::BOLEO;
            if(_COUT_DEBUG_) std::cout << "boleo\n";

        }
        else if( addr.find( WEKINATOR_STEPS ) != -1 )
        {
            gesture = InteractiveTango::TangoGestures::STEPS;
            if(_COUT_DEBUG_) std::cout << "steps\n";

        }
        else if( addr.find( WEKINATOR_OCHOS ) != -1 )
        {
            gesture = InteractiveTango::TangoGestures::OCHOS;
            if(_COUT_DEBUG_) std::cout << "ochos\n";
        }
        
        if( whichBodyPart == InteractiveTango::Dancer::BodyPart::RightFoot  )
        {
            mDancers[whichDancer]->getRootFootGestureRecognition()->addRecognizedGesture(gesture, seconds);
        }
        else if( whichBodyPart == InteractiveTango::Dancer::BodyPart::LeftFoot )
        {
            mDancers[whichDancer]->getLeftFootGestureRecognition()->addRecognizedGesture(gesture, seconds);
        }
        
    }
};

////an attempt to clear the udpsocket of past OSC data --> since this APPEARS to be happening & f'in shiz up?
//void InteractiveTangoMilongaArm64App::clearResidualData()
//{
//    while( mListener.hasWaitingMessages() ) {
//        osc::Message message;
//        mListener.getNextMessage( &message );
//    }
//};

void InteractiveTangoMilongaArm64App::handleBrentOSC(osc::Message message, float seconds)
{
    
    setpriority(PRIO_PROCESS, 0, 1);
    
    //save all received messages here
    if(mSaveOSC!=NULL)
        mSaveOSC->add(message, seconds); //add this line to save the OSC
        
    std::string addr = message.getAddress();
    
    if( !addLiveDancers ) return;
    
    ShimmerData *shimmer = new ShimmerData;

    std::string dID = "Brent";
    int whichShimmer = 5;
    int whichSignal = 2;
    shimmer->setData( 19, seconds); //set timestamp from this program
    int whichIndex = 2;

    
    SensorData *sensor = getSensor(dID, whichShimmer);
    if( addr.find( DATA_OSC_IPHONE_DATA ) != std::string::npos )
    {
        if( !addr.compare( DATA_OSC_IPHONE_DATA_ACCELX ) )
        {
            whichIndex = 2;
            shimmer->setData( whichIndex, message.getArgFloat(0) );
            whichSignal = TYPE_ACCEL;

        }
        else if( !addr.compare( DATA_OSC_IPHONE_DATA_ACCELY ) )
        {
            whichIndex = 3;
            shimmer->setData( 3, message.getArgFloat(0) );
            whichSignal = TYPE_ACCEL;

        }
        else if( !addr.compare( DATA_OSC_IPHONE_DATA_ACCELZ ) )
        {
            whichIndex = 4;
            shimmer->setData( 4, message.getArgFloat(0) );
            whichSignal = TYPE_ACCEL;

        }
        else if( !addr.compare( DATA_OSC_IPHONE_DATA_GYRX ) )
        {
            whichIndex = 11;
            shimmer->setData( 11, message.getArgFloat(0) );
            whichSignal = TYPE_GYR;

        }
        else if( !addr.compare( DATA_OSC_IPHONE_DATA_GYRY ) )
        {
            whichIndex = 12;
            shimmer->setData( 12, message.getArgFloat(0) );
            whichSignal = TYPE_GYR;
        }
        else if( !addr.compare( DATA_OSC_IPHONE_DATA_GYRZ ) )
        {
            whichIndex = 13;
            shimmer->setData( 13, message.getArgFloat(0) );
            whichSignal = TYPE_GYR;

        }
    }

    if( !addr.compare( OSC_SHIMMERDATA )  || !addr.compare( OSC_ANDROID_SENSOR_DATA )  || !addr.compare( OSC_M5STICK_DATA )
       || ( addr.find( DATA_OSC_IPHONE_DATA) != std::string::npos ) )
        sensor->addShimmer( shimmer, whichIndex, whichSignal );

}
//void handleDataOSCIPhone(osc::Message message, float seconds);

//NOTE: 2025 -- This is where all the data messages are handled!!
void InteractiveTangoMilongaArm64App::handleOSC(osc::Message message, float seconds)
{
//handles all the incoming OSC messages
//this includes: sensor data from Shimmer and Android, beat/tempo messages from Ableton,
    
    setpriority(PRIO_PROCESS, 0, 1);
    
    //boolean firstTime = true;
        
    //save all received messages here
    if(mSaveOSC!=NULL)
        mSaveOSC->add(message, seconds); //add this line to save the OSC
        
    std::string addr = message.getAddress();
    
        if( !addr.compare( ABLETON_SYNC ) )
        {
            mBeatTimer.setLastBeat(seconds, message.getArgInt32(0), message.getArgInt32(1), message.getArgFloat(2), message.getArgFloat(2) );
            receivedBeat = true;
        }
        else if( !addr.compare( ABLETON_BPM ) )
        {
            mBeatTimer.setBPM(message.getArgFloat(0));
        }
        else if( !addr.compare( ABLETON_BUSYSPARSE_SCALE ) )
        {
            danceFloor->changeBusySparseScale(InteractiveTango::TangoEntity::WhichDancerType(message.getArgInt32(0)), message.getArgFloat(1));
        }
        else if(!addr.compare( ABLETON_STACCATO_LEGATO_INSTR_SCALE) )
        {
            danceFloor->changeStaccatoLegatoInstrumentScale(InteractiveTango::TangoEntity::WhichDancerType(message.getArgInt32(0)), message.getArgFloat(1));
        }
        else if( ! addr.find(WII_PREFIX) )
        {
//            std::cout << "handling Wii OSC...\n";
            if( isSendingToWekinator )
                wiiToWekinator(message);
            else if ( isChangingMovementParams )
                changeMovementParamsWithWii(message);
            else
                handleWiiOSC(message);
        }
        else if( ! addr.find(WEKINATOR_OUTPUT_PREFIX) )
        {
            handleOutputWekOSC( message, seconds );
        }
        else //if( ! addr.find( OSC_SHIMMERDATA ) )
        {
            
            //sometimes this gets phantom data -- this MUST be enabled.
            if( !addLiveDancers ) return;
            
            ShimmerData *shimmer = new ShimmerData;
            std::string dID;
            int whichShimmer;
            SensorData::SensorType sensorType = SensorData::SensorType::M5STACK;
            if( addr.find( DATA_OSC_IPHONE_DATA ) != std::string::npos )
            {
                dID = "Courtney";
                whichShimmer = 5;
            }
            else
            {
                dID = message.getArgString(0) ;
                whichShimmer = message.getArgInt32(1);
            }
            //shimmer->setData( 1, message.getArgFloat(2) ); //set timestamp from device
            shimmer->setData( 19, seconds); //set timestamp from this program
            
            SensorData *sensor = getSensor(dID, whichShimmer);
            
            // Update OSC rate monitoring for this sensor
            sensor->updateOSCRate(seconds);
            
            int whichSignal = TYPE_ACCEL;
            int whichIndexForIndividualAdd = -1;
            if( addr.find( DATA_OSC_IPHONE_DATA ) != std::string::npos )
            {
                if( !addr.compare( DATA_OSC_IPHONE_DATA_ACCELX ) )
                {
                    shimmer->setData( 2, message.getArgFloat(0) );
                    whichIndexForIndividualAdd = 2;
                    whichSignal = TYPE_ACCEL;
                    sensorType = SensorData::SensorType::DATAOSC_IPHONE;
                   // std::cout << "x receive here: " << message.getArgFloat(0) << std::endl;
                }
                else
                    if( !addr.compare( DATA_OSC_IPHONE_DATA_ACCELY ) )
                {
                    shimmer->setData( 3, message.getArgFloat(0) );
                    whichIndexForIndividualAdd = 3;
                    whichSignal = TYPE_ACCEL;
                    sensorType = SensorData::SensorType::DATAOSC_IPHONE;
//                    std::cout << "y receive here: " << message.getArgFloat(0) << std::endl;

                }
            else if( !addr.compare( DATA_OSC_IPHONE_DATA_ACCELZ ) )
            {
                shimmer->setData( 4, message.getArgFloat(0) );
                whichIndexForIndividualAdd = 4;
                whichSignal = TYPE_ACCEL;
                // std::cout << "z receive here: " << message.getArgFloat(0) << std::endl;
                sensorType = SensorData::SensorType::DATAOSC_IPHONE;

            }
            else if( !addr.compare( DATA_OSC_IPHONE_DATA_GYRX ) )
            {
                shimmer->setData( 11, message.getArgFloat(0) );
                whichIndexForIndividualAdd = 11;
                whichSignal = TYPE_GYR;
                sensorType = SensorData::SensorType::DATAOSC_IPHONE;
                
            }
            else if( !addr.compare( DATA_OSC_IPHONE_DATA_GYRY ) )
            {
                shimmer->setData( 12, message.getArgFloat(0) );
                whichIndexForIndividualAdd = 12;
                whichSignal = TYPE_GYR;
                sensorType = SensorData::SensorType::DATAOSC_IPHONE;
            }
            else if( !addr.compare( DATA_OSC_IPHONE_DATA_GYRZ ) )
                {
                    shimmer->setData( 13, message.getArgFloat(0) );
                    whichIndexForIndividualAdd = 13;
                    whichSignal = TYPE_GYR;
                    sensorType = SensorData::SensorType::DATAOSC_IPHONE;
                }
            }
            if( !addr.compare( OSC_M5STICK_DATA ) )
            {
                for( int i=2; i<5; i++ ) //set accel values
                {
                    shimmer->setData( i, message.getArgFloat(i) );
                }
                for( int i=11; i<14; i++ ) //set gyro values
                {
                    shimmer->setData( i, message.getArgFloat(i-6) );
                }
            }
            if( !addr.compare( OSC_SHIMMERDATA ) )
            {
                ci::vec4 quat;
                sensorType = SensorData::SensorType::SHIMMER;
                
                for( int i=3; i<6; i++ )
                {
                    shimmer->setData( i-1, message.getArgFloat(i) );
                }
                for( int i=7; i<10; i++ )
                {
                    shimmer->setData( i+4, message.getArgFloat(i) );
                }
                for( int i=10; i<13; i++ )
                {
                    shimmer->setData(i+4,  message.getArgFloat(i) );
                }
                for( int i=13; i<17; i++ )
                {
                    quat[i-13] = message.getArgFloat(i);
                }
//                std::cout << message.getAddress();
//                std::cout << ": " <<  message.getArgAsString(0) << " , " << whichShimmer << " , " ;
//                for(int i=3; i<message.getNumArgs(); i++)
//                {
//                    std::cout << message.getArgFloat(i) << " , ";
//                }
//                std::cout << std::endl;
                
                
                shimmer->setQuarternion( quat[0], quat[1], quat[2], quat[3]  );
//                std::cout << dID << "," << whichShimmer << " quat: " << quat[0] << " , " << quat[1] << " , " << quat[2]  << " , " <<  quat[3] << std::endl ;
            }
            else if( !addr.compare( OSC_ANDROID_SENSOR_DATA ) )
            {
                sensorType = SensorData::SensorType::ANDROID;
                whichSignal = message.getArgInt32(3);
                int indexStart = 0;
                if( whichSignal == TYPE_ACCEL )
                {
                    indexStart = 2;
                }
                else if( whichSignal == TYPE_GYR  )
                {
                    indexStart = 11;
                }
                else if( whichSignal == TYPE_MAG )
                {
                    indexStart = 14;
                }
                
                for(int i=0; i<3; i++)
                {
                    shimmer->setData(i+indexStart,  message.getArgFloat(i+4) ); // TODO: check if right
                }
            }
            else if(!addr.compare( ANDROID_ORIENTATION  ) )
            {
                sensorType = SensorData::SensorType::ANDROID;
                float oVals[3];
                for(int i=0; i<3; i++)
                {
                    oVals[i] = message.getArgFloat(i+3) ;
                }
                shimmer->setOrientationMatrix(oVals[0], oVals[1], oVals[2]);
            }
            else if( !addr.compare( ANDROID_STEPDETECTION ) )
            {
                std::cout << "OMFG Detected a step!!!!!!!\n";
            }
            
            
            if( !addr.compare( OSC_SHIMMERDATA )  || !addr.compare( OSC_ANDROID_SENSOR_DATA )  || !addr.compare( OSC_M5STICK_DATA )
               || ( addr.find( DATA_OSC_IPHONE_DATA) != std::string::npos ) )
            {
                sensor->addShimmer( shimmer, whichIndexForIndividualAdd, whichSignal );
                sensor->setSensorType(sensorType); //TODO -- don't call this everytime -- only when created
            }
            
        }
//    }


}

void InteractiveTangoMilongaArm64App::printWekModeMenu()
{
    std::cout <<" CURRENT WII MENU \n";
    std::cout << "Wii Button 1 -- Wekinator start recording DTW\n";
    std::cout << "Wii Button 2 -- Wekinator stop recording DTW\n";
    std::cout << "Wii Button A -- Wekinator train\n";
    std::cout << "Wii Button B -- Wekinator Canceled train\n";
    std::cout << "Wii Button Up -- Wekinator start running\n";
    std::cout << "Wii Button Down -- Wekinator stop running\n";
    std::cout << "Wii Button Home -- Stop sending to Wekinator, wii back in Application mode\n";
}
void InteractiveTangoMilongaArm64App::printNormalWiiMenu()
{
    std::cout <<" CURRENT WII MENU \n";
    std::cout <<" Wii Button 1 -- Start/Stop sending OSC to Ableton\n";
    std::cout <<" Wii Button 2 -- Restart song\n";
    std::cout <<" Wii Button A -- Cycle through songs (change current song)\n";
    std::cout <<" Wii Button Home -- Sending to Wekinator Mode\n";
    std::cout <<" Wii Button B -- Changing Motion Params Mode\n";
    std::cout <<" Wii Button Up -- Marker in file denoting dance start\n";
    std::cout <<" Wii Button Down -- Marker in file denoting dance end\n";
    std::cout <<" Wii Button Right -- Marker in file denoting move start\n";
    std::cout <<" Wii Button Left -- Marker in file denoting move end\n";

    
}


void InteractiveTangoMilongaArm64App::printChangingParamsMenu()
{
    std::cout <<" CURRENT WII MENU \n";
    std::cout << "Wii Button B -- return to Normal Application mode\n";
    std::cout << "Wii Button A -- cycle through the motion params\n";
    std::cout << "Wii Button Home -- cycle through parejas\n";
    std::cout << "Wii Button UP -- increase the MAX parameter by the selected inc. value \n";
    std::cout << "Wii Button DOWN -- decrease the MAX parameter by the selected inc. value \n";
    std::cout << "Wii Button RIGHT -- increase the MIN parameter by the selected inc. value \n";
    std::cout << "Wii Button LEFT -- decrease the MIN parameter by the selected inc. value \n";
    std::cout << "Wii Button 1 -- Set the increment to change by 0.1 \n";
    std::cout << "Wii Button 2 -- Set the increment to change by 0.01 \n";
    std::cout << "Wii Button PLUS -- Save current motion params in text file\n";

}

bool InteractiveTangoMilongaArm64App::isWiiButtonDown(std::string addr, osc::Message message)
{
//    std::cout << addr << "-" << message.getAddress() << std::endl;

    if(!message.getAddress().compare(addr))
    {
        return ( message.getArgFloat(0) == 1.0);
    }
    else return false;
}

void InteractiveTangoMilongaArm64App::changeMovementParamsWithWii(osc::Message message)
{
    
    if(isWiiButtonDown(WII_B, message) )
    {
        isChangingMovementParams = false;
        std::cout << "Stopped changing movement params. In normal application mode\n";

        printNormalWiiMenu();
    }
    if(isWiiButtonDown(WII_A, message))
    {
        mParejas[selectedPareja]->incSelectedMotionParam();
        std::cout << "New selected motion param: " << mParejas[selectedPareja]->getSelectedMotionParamString() << std::endl;
    }
    else if(isWiiButtonDown(WII_HOME, message))
    {
        selectedPareja++;
        if(selectedPareja >= mParejas.size()) selectedPareja = 0;
        
        std::cout << "New selected pareja: " << selectedPareja << std::endl;
    }
    else if(isWiiButtonDown(WII_UP, message))
    {
        mParejas[selectedPareja]->increaseCurMotionParamMax();
        std::cout << mParejas[selectedPareja]->getSelectedMotionParamString() << " max value is now:" << mParejas[selectedPareja]->getMotionDataCoefficients().at(1) << std::endl;
    }
    else if(isWiiButtonDown(WII_DOWN, message))
    {
        mParejas[selectedPareja]->decreaseCurMotionParamMax();
        std::cout << mParejas[selectedPareja]->getSelectedMotionParamString() << " max value is now:" << mParejas[selectedPareja]->getMotionDataCoefficients().at(1) << std::endl;
    }
    else if(isWiiButtonDown(WII_RIGHT, message))
    {
        mParejas[selectedPareja]->increaseCurMotionParamMin();
        std::cout << mParejas[selectedPareja]->getSelectedMotionParamString() << " min value is now:" << mParejas[selectedPareja]->getMotionDataCoefficients().at(0) << std::endl;
    }
    else if(isWiiButtonDown(WII_LEFT, message))
    {
        mParejas[selectedPareja]->decreaseCurMotionParamMin();
        std::cout << mParejas[selectedPareja]->getSelectedMotionParamString() << " min value is now:" << mParejas[selectedPareja]->getMotionDataCoefficients().at(0) << std::endl;

    }
    else if(isWiiButtonDown(WII_1, message))
    {
        mParejas[selectedPareja]->motionParamIncValTenth();
        std::cout << "Set the increment to change by 0.1 \n";
    }
    else if(isWiiButtonDown(WII_2, message))
    {
        mParejas[selectedPareja]->motionParamIncValHundredth();
        std::cout << "Set the increment to change by 0.01 \n";
    }
    else if(isWiiButtonDown(WII_PLUS, message))
    {
        mParejas[selectedPareja]->saveCurrentMotionParams();
        std::cout << "Saved current motion params in text file\n";
    }
}


//control this ITM application via wiimote
void InteractiveTangoMilongaArm64App::handleWiiOSC(osc::Message message)
{
    
    if(isWiiButtonDown(WII_1, message)) //stop or start sending OSC
    {
        startOSCToAbleton();
    }
    else if(isWiiButtonDown(WII_2, message)) //restart this song
    {
        stopOSCToAbleton();
    }
    else if(isWiiButtonDown(WII_HOME, message)) //send to Wekinator
    {
        isSendingToWekinator = true;
        isChangingMovementParams = false;
        
        std::cout << "Sending to Wekinator now -- note wii state in this mode also\n";
        std::cout << "Press 'HOME' again to exit mode\n";
        printWekModeMenu();
    }
//    else if(isWiiButtonDown(WII_B, message))
//    {
//        if( mParejas.size() <= 0 ){
//            std::cout << "There are no parejas entered in... must enter a pareja to change motion params\n";
//
//        }
//        else
//        {
//            isChangingMovementParams = true;
//            std::cout << "Current Pareja is: " << selectedPareja << std::endl;
//            std::cout << "Changing movement params now -- note wii state in this mode also\n";
//            std::cout << "Press 'B' again to exit mode\n";
//
//            printChangingParamsMenu();
//        }
//    }
    else if(isWiiButtonDown(WII_A, message))
    {
        cycleThroughPlaylist();
    }
    else if(isWiiButtonDown(WII_UP, message))
    {
        addMarkerToSensorFile("START DANCE");
    }
    else if(isWiiButtonDown(WII_DOWN, message))
    {
        addMarkerToSensorFile("END DANCE");
    }
    else if(isWiiButtonDown(WII_RIGHT, message))
    {
        addMarkerToSensorFile("START MOVE");
    }
    else if(isWiiButtonDown(WII_LEFT, message))
    {
        addMarkerToSensorFile("END MOVE");
    }
}

void InteractiveTangoMilongaArm64App::addMarkerToSensorFile(std::string marker)
{
    if( mSensorData.size() <= 0 ) return;
    
    float seconds = mTimer.getSeconds() ;

    for(int i=0; i<mSensorData.size(); i++)
    {
        mSensorData[i]->markerInFile(marker, seconds);
    }
    std::cout << "Added marker " << marker << "in file at " << seconds << "seconds.\n";
}


/*
//Wekinator messages to send
#define WEKINATOR_RECORD "/wekinator/control/startRecording"
#define WEKINATOR_STOP_RECORD "/wekinator/control/stopRecording"
#define WEKINATOR_OUTPUTS "/wekinator/control/outputs" // (with list of floats): Send Wekinator current output values to populate the boxes/sliders on its GUI. Attach one float per output, in order.
#define WEKINATOR_START_DTW_RECORDING "/wekinator/control/startDtwRecording" // (with 1 int) : Start recording dynamic time warping examples for the gesture type given by the int (gesture types are indexed starting from 1)
#define WEKINATOR_STOP_DTW_RECORDING "/wekinator/control/stopDtwRecording // Stop recording dynamic time warping examples (no int required)
#define WEKINATOR_TRAIN "/wekinator/control/train" // Train on current examples
#define WEKINATOR_CANCELTRAIN "/wekinator/control/cancelTrain" // Cancel current training (if it’s in progress)
#define WEKINATOR_STARTRUNNING "/wekinator/control/startRunning" // Start running (if possible)
#define WEKINATOR_STOPRUNNING "/wekinator/control/stopRunning" // Stop running (if currently running)
//send sensor information to the wekinator for machine learning and gesture recognition
 */

void InteractiveTangoMilongaArm64App::wiiToWekinator(osc::Message message)
{
    osc::Message wek_msg;
    bool send = false;
    
    if(isWiiButtonDown(WII_1, message))
    {
        wek_msg.setAddress(WEKINATOR_START_DTW_RECORDING);
        wek_msg.append(whichDTWWekinatorOutput);
        std::cout << "Wekinator start recording DTW\n";
        std::cout << "Current DTW Wekinator output: " << whichDTWWekinatorOutput << std::endl;
        
        std::stringstream ss; ss << selectedWekinatorSenderIndex << "," << whichDTWWekinatorOutput << ",";
        sendOSCToWekinators[selectedWekinatorSenderIndex]->setRecord(true, ss.str());

        send = true;
    }
    else if(isWiiButtonDown(WII_2, message))
    {
        wek_msg.setAddress(WEKINATOR_STOP_DTW_RECORDING);
        std::cout << "Wekinator stop recording DTW\n";
        sendOSCToWekinators[selectedWekinatorSenderIndex]->setRecord(false);

        send = true;

    }
    else if(isWiiButtonDown(WII_A, message))
    {
        wek_msg.setAddress(WEKINATOR_TRAIN);
        std::cout << "Wekinator train\n";
        send = true;
    }
    else if(isWiiButtonDown(WII_B, message))
    {
        wek_msg.setAddress(WEKINATOR_CANCELTRAIN);
        std::cout << "Wekinator Canceled train\n";
        send = true;
    }
    else if(isWiiButtonDown(WII_UP, message))
    {
        wek_msg.setAddress(WEKINATOR_STARTRUNNING);
        std::cout << "Wekinator start running\n";
        send = true;
    }
    else if(isWiiButtonDown(WII_DOWN, message))
    {
        wek_msg.setAddress(WEKINATOR_STOPRUNNING);
        std::cout << "Wekinator stop running\n";

        send = true;
    }
    else if(isWiiButtonDown(WII_HOME, message)) //restart this song
    {
        isSendingToWekinator = false;
        std::cout << "Stopped sending to Wekinator, wii back in Application mode\n";
        printNormalWiiMenu();

    }
    else if(isWiiButtonDown(WII_PLUS, message)) //restart this song
    {
        whichDTWWekinatorOutput++;
        std::cout << "Incremented DTW wekinator output, it is now: " << whichDTWWekinatorOutput << std::endl;
    }
    else if(isWiiButtonDown(WII_MINUS, message)) //restart this song
    {
        whichDTWWekinatorOutput--;
        std::cout << "Decremented DTW wekinator output, it is now: " << whichDTWWekinatorOutput << std::endl;
    }
    else if(isWiiButtonDown(WII_RIGHT, message)) //restart this song
    {
        selectedWekinatorSenderIndex++;
        if(selectedWekinatorSenderIndex >= wekinatorSenders.size()) selectedWekinatorSenderIndex = 0;
        std::cout << "Incremented wekinator sender index, it is now: " << selectedWekinatorSenderIndex << std::endl;
    }
    else if(isWiiButtonDown(WII_LEFT, message)) //restart this song
    {
        selectedWekinatorSenderIndex--;
        if(selectedWekinatorSenderIndex < 0) selectedWekinatorSenderIndex = 0;
        std::cout << "Decremented wekinator sender index, it is now: " << selectedWekinatorSenderIndex << std::endl;
    }
    
#ifdef WEKINATOR_ENABLED
    if (send)
        wiiWekinatorSenders[selectedWekinatorSenderIndex]->send(wek_msg, std::bind( &InteractiveTangoMilongaArm64App::onSendError,
                                                                              this, std::placeholders::_1 ));
#endif
}
                                                                                   
void InteractiveTangoMilongaArm64App::sendMidiMessages( std::vector<InteractiveTango::MidiMessage *> msgs)
{
    for(int i=0; i<msgs.size(); i++)
    {
        if(msgs[i]->which==InteractiveTango::MidiMessage::MessageTypes::CONTROL)
        {
            midiOut.send( (InteractiveTango::MidiControlMessage *) msgs[i] );
//            std::cout << "Sending control message: " << msgs[i]->channel << ","<<
//            ((InteractiveTango::MidiControlMessage *)msgs[i])->control << "," <<
//            ((InteractiveTango::MidiControlMessage *)msgs[i])->value << "\n";
        }
        else
        {
            midiOut.send( (InteractiveTango::MidiNote *) msgs[i] );
        }
    }
    
    //delete all sent messages
    for(int i=0; i<msgs.size(); i++)
    {
        delete msgs[i];
    }
    msgs.clear();

}
void InteractiveTangoMilongaArm64App::updateSignalTree(float seconds, float pastSeconds)
{
    //update this first
    if( shouldSendOSC )
    {
        for(int i=0; i<mUGENs.size(); i++)
        {
            mUGENs[i]->update(seconds);
        }
    }
    //update dancer entities... TODO: move this to ugens, bc can now inherit from UGen?
    for( int i=0; i<mDancers.size(); i++ )
    {
        mDancers[i]->update();
    }
    for( int i=0; i<mParejas.size(); i++ )
    {
        mParejas[i]->update();
    }
    
    //send the rest of the OSC after the dance floor (music player) is sent so music player messages have priority
    if( shouldSendOSC )
    {
        if( danceFloor != NULL )
        {
            danceFloor->update(seconds); //must be last thing updated b4 wek is sent
            sendOSCMessages(danceFloor->getOSC(), seconds); //first one to send OSC
            sendMidiMessages(danceFloor->getMIDI());
        }
        else std::cout << "dance floor is null\n";

        //ok now send OSC messages for rest of signal tree
        for(int i=0; i<mUGENs.size(); i++){
            sendOSCMessages(mUGENs[i]->getOSC(), seconds);
//            sendMidiMessages(mUGENs[i]->getMIDI()); //don't send just for the sensors right now 7/13/2021

        }
        
        
    }
#ifdef WEKINATOR_ENABLED
    //update sendToWekinator -- must be at the end...
    for( int i=0; i<sendOSCToWekinators.size(); i++ )
    {
        sendOSCToWekinators[i]->update();
        std::vector<osc::Message> msgs = sendOSCToWekinators[i]->getOSC();
        sendOSCInputsToWekinator(msgs, seconds, i);
//        if(msgs.size()!=0)
//        std::cout << "i: " << i << " sigNum: " << sendOSCToWekinators[i]->getSignalCount() << "  sent: " << msgs.size() << " seconds  "  << seconds - pastSeconds << std::endl;
    }
#endif
};

void InteractiveTangoMilongaArm64App::update()
{
    setpriority(PRIO_PROCESS, 0, 1);
    
    float seconds = getElapsedSeconds();  //mTimer.getSeconds() ;
    
    if( playing_done){
        std::cout << "Playing current sensor file is done\n";
        return; //don't update after everything is done playing
    }
    
    //send photo sync message to application/machine running video camera
#ifdef PHOTO_SYNC_ENABLED
    sendPhotoSync(seconds);
#endif
    
    //handle incoming OSC messages
//    handleOSC(seconds);

    //update UGens
    for(int i=0; i<mSensorData.size() && !playing_done; i++)
    {
        mSensorData[i]->update(seconds);
        
        //check if done playing loaded sensor
        if( !addLiveDancers )
        {
            LoadedSensor *s = (LoadedSensor *) mSensorData[i];
            playing_done = s->done();
            if(playing_done) std::cout << "Done playing sensor file.\n";
        }

    }
    mBeatTimer.update(seconds);
    
    //update signal processing tree and send OSC
    updateSignalTree(seconds, pastseconds);
    
    //clear memory
    for(int i=0; i<mSensorData.size(); i++)
    {
        mSensorData[i]->cleanupBuffer();
    }
    
    //update the video player (if replaying a past motion capture)
//    if( moCam.isInit() ) moCam.update();
//    if (mPlayPrevious && mMocapFilePlayer != NULL)
//    {
//        mMocapFilePlayer->update(seconds);
//    }
    
    pastseconds = seconds;
    
    //clear current sensor data (more received via OSC)
    for(int i=0; i<mSensorData.size(); i++)
        mSensorData[i]->eraseData();
    
    //play saved OSC messages, if valid.
    if( mPlayOSC != NULL )
        mPlayOSC->update(getElapsedSeconds());
}

void InteractiveTangoMilongaArm64App::stopStartOSCToAbleton()
{
    shouldSendOSC = !shouldSendOSC;
    if (shouldSendOSC) std::cout << "Sending OSC\n";
    else
    {
        restartSong();
        std::cout << "Stopped sending OSC\n";
    }
    std::flush(std::cout);
}

void InteractiveTangoMilongaArm64App::startOSCToAbleton()
{
    shouldSendOSC = true;
    std::cout << "Sending OSC\n";
    std::flush(std::cout);
}

//this also restarts

void InteractiveTangoMilongaArm64App::stopOSCToAbleton()
{
    shouldSendOSC = false;
    std::cout << "Stopped sending OSC\n";
    restartSong();
    std::flush(std::cout);
}


void InteractiveTangoMilongaArm64App::restartSong()
{
    //restart the song!
    danceFloor->restartPlayer();
    std::cout << "Re-starting song\n";
    std::flush(std::cout);
}

void InteractiveTangoMilongaArm64App::keyDown( KeyEvent event )
{
    if(midiMappingMode)
    {
        if(event.getChar()=='m')
        {
            midiMappingMode = false;
            ((InteractiveTango::DanceFloorNDB *) danceFloor)->setMidiMapping(-1); //turns off the midi mapping
            return; //I know its so bad.
        }
            
        std::vector<char> chars = {'1','2','3','4','5','6','7','8','9','0','q','w','e','r','t','y','u','i','p','a','s','d','f','g','h','j','k','l'};
        int control =  std::find (chars.begin(), chars.end(), event.getChar()) - chars.begin() + 1;
        ((InteractiveTango::DanceFloorNDB *) danceFloor)->setMidiMapping(control);
        std::cout << "Only sending midi control signal: " << control << std::endl;
        midiMappingMode = false;
        return;
    }
    
    if(fakeBSLeaderMode || fakeBSFollowerMode || fakeBSAccompMode)
    {
        
        if((event.getChar() != '1' && event.getChar() != '2' && event.getChar() != '3') ||
           !(fakeBSAccompMode && (event.getChar() == '4' || event.getChar() == '5') ))
        {
            
            std::cout << "Not a valid value to change busy/sparse. Exiting mode...";
        }
        
        char c[1]; c[0] = event.getChar();
        std::string s(c);
        double res = std::atof(s.c_str());
        
        if(fakeBSLeaderMode)
        {
            danceFloor->leaderFakeBusySparse(res);
            std::cout << "Set leader busy/sparse to :" << res << std::endl;
            
        }
        else if(fakeBSFollowerMode)
        {
            danceFloor->followerFakeBusySparse(res);
            std::cout << "Set follower busy/sparse to :" << res << std::endl;
        }
        else{
            danceFloor->accompFakeBusySparse(res);
            std::cout << "Set accompaniment busy/sparse to :" << res << std::endl;
        }
        
        fakeBSLeaderMode = false;
        fakeBSFollowerMode = false;
        fakeBSAccompMode = false;
        
    }
    

    
    if( event.getChar() == 's' )
    {
        mSaveFilePath = App::getSaveFilePath().c_str();
        for( int i=0; i<mSensorData.size(); i++)
        {
            mSensorData[i]->saveToFile(mSaveFilePath);
        }
        mBeatTimer.save(mSaveFilePath);
    }
//    else if( event.getChar() == 'i' )
//    {
//        moCam.init(&mTimer);
//        std::cout << "Initialized webcam\n" << std::endl;
//    }
    else if (event.getChar() == 'o')
    {
//        int i = ((LoadedSensor *) mSensorData[0])->getCurSample();
//        std::cout << "ONSET, "<< i <<"\n";
    }
//    else if (event.getChar() == 'm')
//    {
//        mShowMovie = !mShowMovie;
//    }
    else if (event.getChar() == 'u')
    {
        cout << "Open picture file name\n";
        std::string picFileName = App::getOpenFilePath().string();
        std::string pfname = picFileName.substr(0, picFileName.find('.') - 1);
        
        cout << "Open file with time references\n";
        std::string refFileName = App::getOpenFilePath().string();
        
        if (!picFileName.empty() && !refFileName.empty())
        {
//            if (mMocapFilePlayer != NULL)
//            {
//                delete mMocapFilePlayer;
//            }
//            mMocapFilePlayer = new ReadMocapCameraFile(pfname, refFileName);
            
            mPlayPrevious = true;
        }
    }
    else if(event.getChar() == 'r')
    {
        std::cout << "Playback time reset\n";
        mTimer.stop();
        mTimer = ci::Timer();
        mTimer.start();
        
        for(int i=0; i<mSensorData.size(); i++)
        {
            mSensorData[i]->resetPlaybackTimer();
        }
    }
    else if( (event.getChar() >= '0' && event.getChar() <= '9') || event.getChar() == '.' )
    {
        std::stringstream ss; ss << event.getChar();
        whichShimmer.append( ss.str() );
    }
    else if(event.getChar() == 'c')
    {
        changeSensorColor();
    }
    else if (event.getChar()=='m')
    {
        midiMappingMode = !midiMappingMode;
        if(midiMappingMode)
            std::cout << "In Midi Mapping Mode\n";
        else{
            std::cout << "This is an indication that there is an error in the keyDown in midiMapping... \n";
        }
        
    }
    else if (event.getChar() == 'x')
    {
        stopStartOSCToAbleton();
    }
    else if( event.getChar() == 'z' )
    {
        restartSong();
    }
    else if( event.getChar() == 'q' )
    {
        playFragments();
    }
    else if( event.getChar() == 'a' )
    {
        playPorUnaCabeza();
    }
    else if( event.getChar() == 'w' )
    {
        playLaCumparsita();
    }
    else if( event.getChar() == 'y' )
    {
        playElChoclo();
    }
    //for testing ornaments
    else if( event.getChar() == ',' ) //stillness
    {
        if( mDancers.size() >= 2 )
        {
            mDancers[1]->getRootFootGestureRecognition()->addStillness( mTimer.getSeconds() );
//            mDancers[0]->getRootFootGestureRecognition()->addStillness( mTimer.getSeconds() );

//            std::cout << "added Stillness!\n";

        }
        else
        {
            std::cout << "Dancers must be defined!\n";
        }
    }
    else if( event.getChar() == ';' ) // circling
    {
        if( mDancers.size() >= 2 )
        {
            mDancers[1]->getRootFootGestureRecognition()->addCircling( mTimer.getSeconds() );
//            mDancers[0]->getRootFootGestureRecognition()->addCircling( mTimer.getSeconds() );

            //            std::cout << "added Circling!\n";

        }
        else
        {
            std::cout << "Dancers must be defined!\n";
        }
    }
    else if( event.getChar() == '/' ) // boleo
    {
        if( mDancers.size() >= 2 )
        {
            mDancers[1]->getRootFootGestureRecognition()->addBoleo( mTimer.getSeconds() );
//            mDancers[0]->getRootFootGestureRecognition()->addBoleo( mTimer.getSeconds() );

//            std::cout << "added Boleo!\n";
        }
        else
        {
            std::cout << "Dancers must be defined!\n";
        }
    }
    else if( event.getChar() == 't' )
    {
        addTestDancersNoSensors();
    }
    else if( event.getChar() == 'l')
    {
        if(danceFloor->parejaCount() < 1)
        {
            std::cout << "Cannot create a fake step because there are no dancers on the floor\n";
            return;
        }
        else{
            std::cout << "Creating a fake step for leader...";
            danceFloor->leaderFakeStep();
            std::cout << "done\n";
            
        }
    }
    else if( event.getChar() == 'f')
    {
        if(danceFloor->parejaCount() < 1)
        {
            std::cout << "Cannot create a fake step because there are no dancers on the floor\n";
            return;
        }
        else{
            std::cout << "Creating a fake step for follower...";
            danceFloor->followerFakeStep();
            std::cout << "done\n";
            
        }
    }   else if( event.getChar() == 'b')
    {
        if(danceFloor->parejaCount() < 1)
        {
            std::cout << "Cannot create a fake busy/sparse because there are no dancers on the floor\n";
            return;
        }
        else{
            std::cout << "In fake busy/sparse for follower. Please choose a value 1-3\n";
            fakeBSFollowerMode = true;
        }
    }
    else if( event.getChar() == 'n')
    {
        if(danceFloor->parejaCount() < 1)
        {
            std::cout << "Cannot create a fake busy/sparse because there are no dancers on the floor\n";
            return;
        }
        else{
            std::cout << "In fake busy/sparse for leader. Please choose a value 1-3\n";
            fakeBSLeaderMode = true;
        }
    }
    else if( event.getChar()=='p' )
    {
        fs::path openPath = getOpenFilePath();
        mPlayOSC = new InteractiveTango::PlayOSC(openPath.c_str(), SELF_IPADDR, OSC_LISTENING_PORT, LOCALPORT_START);
    }

//    else if( event.getChar() == 'm')
//    {
//        if(danceFloor->parejaCount() < 1)
//        {
//            std::cout << "Cannot create an accomp. fake busy/sparse because there are not enouh dancers on the floor\n";
//            return;
//        }
//        else{
//            std::cout << "In fake busy/sparse for leader. Please choose a value 1-3\n";
//            fakeBSAccompMode = true;
//        }
//    }
}

void InteractiveTangoMilongaArm64App::addTestDancersNoSensors()
{
    std::cout << "Adding a test couple to floor to be controlled via keyboard...\n";
    
    int wekIndexStart = sendOSCToWekinators.size();
    
#ifdef WEKINATOR_ENABLED
    //create wekinator senders -- right legs then left legs for ea. couple... legacy TODO: fix!!!
    for( int i=wekIndexStart; i<wekIndexStart+4; i++ )
        addSendtoWekinator();
#endif
    
    
    //create fake sensors
    SensorData * leaderback = getSensor("Brent", 5);
    SensorData * leaderright = getSensor("Brent", 2);
    SensorData * leaderleft = getSensor("Brent", 1);
    
    //add the leader
#ifdef WEKINATOR_ENABLED
    InteractiveTango::Dancer *dancer = new InteractiveTango::Dancer(&mBeatTimer, sendOSCToWekinators[wekIndexStart], sendOSCToWekinators[wekIndexStart+2]);
#else
        InteractiveTango::Dancer *dancer = new InteractiveTango::Dancer(&mBeatTimer, NULL, NULL);
#endif
    dancer->setDancerType(InteractiveTango::TangoEntity::WhichDancerType::LEADER1);
    dancer->addSensorBodyPart(0, leaderleft, &mUGENs, InteractiveTango::Dancer::LeftFoot);
    dancer->addSensorBodyPart(1, leaderright, &mUGENs, InteractiveTango::Dancer::RightFoot);
    dancer->addSensorBodyPart(2, leaderback, &mUGENs, InteractiveTango::Dancer::Back);
    dancer->setDancerID( 0, &mUGENs );
    mDancers.push_back(dancer);
    
    std::cout << "Added Brent as dancer\n";
    
    //add the follower
    
    //create fake sensors
    SensorData * followerback = getSensor("Courtney", 5);
    SensorData * followerright = getSensor("Courtney", 2);
    SensorData * followerleft = getSensor("Courtney", 1);
    
#ifdef WEKINATOR_ENABLED
    InteractiveTango::Dancer *dancer2 = new InteractiveTango::Dancer(&mBeatTimer, sendOSCToWekinators[wekIndexStart+1], sendOSCToWekinators[wekIndexStart+3]);
#else
        InteractiveTango::Dancer *dancer2 = new InteractiveTango::Dancer(&mBeatTimer, NULL, NULL);
#endif
    dancer->setDancerType(InteractiveTango::TangoEntity::WhichDancerType::FOLLOWER1);
    dancer2->addSensorBodyPart(3, followerleft, &mUGENs, InteractiveTango::Dancer::LeftFoot);
    dancer2->addSensorBodyPart(4, followerright, &mUGENs, InteractiveTango::Dancer::RightFoot);
    dancer2->addSensorBodyPart(5, followerback, &mUGENs, InteractiveTango::Dancer::Back);
    dancer2->setDancerID( 1, &mUGENs );
    mDancers.push_back(dancer2);
    
    std::cout << "Added Courtney as dancer\n";
    
    //add the dancer
    InteractiveTango::Pareja *pareja = new InteractiveTango::Pareja(mDancers[mDancers.size()-2], mDancers[mDancers.size()-1], &mUGENs, &mBeatTimer );
    mParejas.push_back(pareja);
    danceFloor->addPareja(pareja);
    std::cout << "Incluye la pareja\n";
    
    //saving all the sensor info
    //TODO: make this work again.
    //       saveAllSensors();
    
    
    danceFloor->loadCourtneyTangoSongNumberOneIntoPlayer();
    
}

void InteractiveTangoMilongaArm64App::changeSensorColor()
{
    if( mSensorData.size() <= 0) return;
    
    int cindex = atoi( whichShimmer.c_str() );
    if( cindex < mSensorData.size()  && cindex >= 0 )
    {
        std::cout << "Changing color on sensor: " << cindex << std::endl;
        visualizers[cindex]->setColor( generateRandomColor() );
    }
    whichShimmer = "";
}

void InteractiveTangoMilongaArm64App::addPareja()
{
    //add a pareja
    int sIndex = atoi( whichShimmer.c_str() );
    if ( ( sIndex < 0  ) || ( sIndex > mDancers.size() ) )
    {
        std::cout << "Dancers doesn't exist: " << sIndex << "\n";
    }
    else if (mHoldShimmerIndex == -1)
    {
        mHoldShimmerIndex = sIndex;
        std::cout << "Adding dancer #: " << mHoldShimmerIndex << " in a pareja\n";
    }
    else
    {
        //add the dancer
        InteractiveTango::Pareja *pareja = new InteractiveTango::Pareja(mDancers[mHoldShimmerIndex], mDancers[sIndex], &mUGENs, &mBeatTimer );
        mParejas.push_back(pareja);
        std::cout << "Added dancer #: " << mHoldShimmerIndex << " and dancer: " << sIndex << " in a pareja\n";
        
        danceFloor->addPareja(pareja);
        
        //reset hold shimmer ids
        mHoldShimmerIndex = -1;
        mHoldShimmerIndex2 = -1;
    }
    whichShimmer = "";
}


void InteractiveTangoMilongaArm64App::addDancer()
{
    //add a dancer (b for bailarine, d was taken)
    int sIndex = atoi( whichShimmer.c_str() );
    if ( ( sIndex < 0  ) || ( sIndex > mSensorData.size() ) )
    {
        std::cout << "Shimmer doesn't exist: " << sIndex << "\n";
    }
    else if (mHoldShimmerIndex == -1)
    {
        mHoldShimmerIndex = sIndex;
        std::cout << "Left leg of dancer is sensor #: " << mHoldShimmerIndex << "\n";
    }
    else if (mHoldShimmerIndex2 == -1)
    {
        mHoldShimmerIndex2 = sIndex;
        std::cout << "Right leg of dancer is sensor #: "  << mHoldShimmerIndex2 << "\n";
    }
    else
    {
        //add the dancer
        int wekIndex = addSendtoWekinator();
        int wekIndex2 = addSendtoWekinator();
        InteractiveTango::Dancer *dancer = new InteractiveTango::Dancer(&mBeatTimer, sendOSCToWekinators[wekIndex], sendOSCToWekinators[wekIndex2]);
        dancer->addSensorBodyPart(mHoldShimmerIndex, mSensorData[mHoldShimmerIndex], &mUGENs, InteractiveTango::Dancer::LeftFoot);
        dancer->addSensorBodyPart(mHoldShimmerIndex2, mSensorData[mHoldShimmerIndex2], &mUGENs, InteractiveTango::Dancer::RightFoot);
        dancer->addSensorBodyPart(sIndex, mSensorData[sIndex], &mUGENs, InteractiveTango::Dancer::Back);
        
        //add dancer/limb info to sensor data, so that it is saved in the file
        mSensorData[mHoldShimmerIndex]->setDancerLimb(mDancers.size(), InteractiveTango::Dancer::LeftFoot);
        mSensorData[mHoldShimmerIndex2]->setDancerLimb(mDancers.size(), InteractiveTango::Dancer::RightFoot);
        mSensorData[sIndex]->setDancerLimb(mDancers.size(), InteractiveTango::Dancer::Back);
        
        mDancers.push_back(dancer);
        
        std::cout << "Added dancer # " << mDancers.size() << " with right leg as sensor # " << mHoldShimmerIndex << " left leg as sensor # " << mHoldShimmerIndex2
        << " & back as sensor # " << sIndex << std::endl;
        
        //reset hold shimmer ids
        mHoldShimmerIndex = -1;
        mHoldShimmerIndex2 = -1;
    }
    whichShimmer = "";

}

//InteractiveTango::Pareja::SongIDs whichSongIsPlaying;
void InteractiveTangoMilongaArm64App::cycleThroughPlaylist()
{
//just 2 songs... TODO: fix for MORE songs
    
    std::cout << "Cyling through playlist...\n";
    if (whichSongIsPlaying == InteractiveTango::Pareja::SongIDs::POR_UNA_CABEZA)
    {
        playFragments();
    }
    else
    {
        playPorUnaCabeza();
    }

};


void InteractiveTangoMilongaArm64App::playPorUnaCabeza()
{
    std::cout << "'a' - load 'por una cabeza'\n";
    danceFloor->restartPlayer();
    danceFloor->loadPorUnaCabezaIntoPlayer();
    whichSongIsPlaying = InteractiveTango::Pareja::SongIDs::POR_UNA_CABEZA;

}

void InteractiveTangoMilongaArm64App::playElChoclo()
{
    std::cout << "'c' - load 'el choclo'\n";
    danceFloor->restartPlayer();
    danceFloor->loadElChocloIntoPlayer();
    whichSongIsPlaying = InteractiveTango::Pareja::SongIDs::EL_CHOCLO;

}
                                                                                   

void InteractiveTangoMilongaArm64App::playLaCumparsita()
{
    std::cout << "'w' - load 'la cumparsita'\n";
    danceFloor->restartPlayer();
    danceFloor->loadLaCumparsitaIntoPlayer();
    whichSongIsPlaying = InteractiveTango::Pareja::SongIDs::LA_CUMPARSITA;
    
}

void InteractiveTangoMilongaArm64App::playFragments()
{
    std::cout << "'q' - load first tango song by Courtney\n";
    danceFloor->restartPlayer();
    danceFloor->loadCourtneyTangoSongNumberOneIntoPlayer();
    whichSongIsPlaying = InteractiveTango::Pareja::SongIDs::FRAGMENTS;
}

//cout for now
void InteractiveTangoMilongaArm64App::printMenu()
{


}

ci::ColorA InteractiveTangoMilongaArm64App::generateRandomColor()
{
//gen random color, to ID sensors on screen
    
    float r, g, b;
    
    r = Rand::randFloat();
    g = Rand::randFloat();
    b = Rand::randFloat();
    
    return ColorA(r, g, b, 1);
}


void InteractiveTangoMilongaArm64App::drawGrid(float size, float step)
{
//draw 3d grid on the screen
    
    gl::color(Colorf(0.2f, 0.2f, 0.2f));
    for (float i = -size; i <= size; i += step)
    {
        gl::drawLine(vec3(i, 0.0f, -size), vec3(i, 0.0f, size));
        gl::drawLine(vec3(-size, 0.0f, i), vec3(size, 0.0f, i));
    }
}

void InteractiveTangoMilongaArm64App::screenshot()
{
//for motion capture during program run (now run another machine, so obsolete, unless CPUs get faster)
//
//    gl::TextureRef tex =  moCam.getTexture();
//    if (!tex) {
//        return;
//    }
    
    //save screenshot & file with frame # to time references
    std::stringstream ss;  ss << mIndex;
//    ci::writeImage(getAppPath() / fs::path("frame" + ss.str() + ".png"), *tex );
    
    fs::path refPath = getAppPath() / fs::path("frame_reference.csv");
    std::ofstream myfile(refPath.string(), std::ios::app);
    myfile << mIndex << "," << mTimer.getSeconds() << endl;
    myfile.close();
    
    mIndex++;
}

void InteractiveTangoMilongaArm64App::draw()
{
    //clear out the window with black
    if(!receivedBeat)
        gl::clear(Color(0, 0, 0));
    else
        gl::clear(Color(1, 0, 0));
    gl::color(1, 1, 0, 1);
    ci::gl::setMatrices(mCamera);
    
    // enable the depth buffer (after all, we are doing 3D)
    gl::enableDepthRead();
    gl::enableDepthWrite();
    
//    // draw the grid on the floor
    drawGrid();
    
    gl::color(1,0,1,1);
    ci::gl::drawSphere(ci::vec3(0, 0, 0), 0.05f);
    
    for (int i = 0; i < visualizers.size(); i++)
    {

        ci::gl::setMatrices(mCamera);
        visualizers[i]->draw();
    }
    
    gl::color(1, 1, 1, 1);
    gl::setMatricesWindow(getWindowWidth(), getWindowHeight());
    
    if (mShowMovie)
    {
//        moCam.draw();
        screenshot();
    }
    
//    if (mPlayPrevious && mMocapFilePlayer != NULL)
//    {
//        mMocapFilePlayer->draw();
//    }
    receivedBeat = false;
}

SensorData * InteractiveTangoMilongaArm64App::getSensor(string deviceID, int which)
{
    bool found = false;
    int index = 0;
    
    while( !found && index < mSensorData.size() )
    {
        found = mSensorData[index]->same( deviceID, which );
        index++;
    }
    
    if(!found)
    {
        std::cout << "deviceID:" << deviceID << "   which: " <<  which  <<  std::endl;
        
        VisualizeShimmer *visualizer = new VisualizeShimmer();
        ColorA c = generateRandomColor();
        visualizer->setColor( c );
        visualizer->setBounds(app::App::getWindowSize());
        visualizers.push_back(visualizer);
        
        SensorData *sensor = new SensorData( deviceID, which, visualizer );
        mSensorData.push_back(sensor);
        
        //add to dancer if relevant
        if( addLiveDancers )
        {
            addLiveDancer(sensor, mSensorData.size()-1);
        }
        

        if(!mSaveFilePath.empty()) sensor->saveToFile(mSaveFilePath);
        return sensor;
    }
    else return mSensorData[index-1];
}

SensorData *InteractiveTangoMilongaArm64App::getSensor(std::string whichShimmer)
{
//WARNING: will not create a new one
    
    int which = std::atoi(whichShimmer.c_str());
    
    if( which < 0 || which > mSensorData.size() )
    {
        return NULL;
    }
    else
    {
        return mSensorData[which];
    }
    
}

//adds just one couple
void InteractiveTangoMilongaArm64App::addLiveDancer(SensorData *sensor, int sid)
{
    std::string leaderName = "Brent";
    std::string followerName = "Courtney";
    std::string leader2Name = "Leader2";
    std::string follower2Name = "Follower2";
    
    std::cout << "adding live dancer: " + sensor->getDeviceID() + " dID:" << sensor->getWhichSensor() << std::endl;
    
    if (!leaderName.compare(sensor->getDeviceID()))
    {
        switch( sensor->getWhichSensor() )
        {
            case 0:
                leader.insert(std::pair<int, int>(0, sid));
                break;
            case 1:
                leader.insert(std::pair<int, int>(1, sid));
                break;
            case 5:
                leader.insert(std::pair<int, int>(2, sid));
                break;
            default:
                std::cout << "NOT ADDED\n";
                break;
        }
    }
    else if ( !followerName.compare( sensor->getDeviceID() ) )
    {
        switch( sensor->getWhichSensor() )
        {
            case 0:
                follower.insert(std::pair<int, int>(0, sid));
                break;
            case 1:
                follower.insert(std::pair<int, int>(1, sid));
                break;
            case 5:
                follower.insert(std::pair<int, int>(2, sid));
                break;
            default:
                std::cout << "NOT ADDED\n";
                break;
        }
    }
    else if (!leader2Name.compare(sensor->getDeviceID()))
    {
        switch( sensor->getWhichSensor() )
        {
            case 0:
                leader2.insert(std::pair<int, int>(0, sid));
                break;
            case 1:
                leader2.insert(std::pair<int, int>(1, sid));
                break;
            case 5:
                leader2.insert(std::pair<int, int>(2, sid));
                break;
            default:
                break;
        }
    }
    else if ( !follower2Name.compare( sensor->getDeviceID() ) )
    {
        switch( sensor->getWhichSensor() )
        {
            case 0:
                follower2.insert(std::pair<int, int>(0, sid));
                break;
            case 1:
                follower2.insert(std::pair<int, int>(1, sid));
                break;
            case 5:
                follower2.insert(std::pair<int, int>(2, sid));
                break;
            default:
                break;
        }
    }
    
    //have all the sensors and androids connected, create dancers and pareja -- TODO: make for any # of dancers!
    addLivePareja(leader, follower, 0, 1);
    addLivePareja(leader2, follower2, 2, 3);
    
    //switch couple order if needed so that mappings stay consistent between devices
    if(danceFloor->parejaCount() == 2)
    {
        if( danceFloor->getLeaderDancerID(0) != 0 )
        {
            danceFloor->clearCouples();
            danceFloor->addPareja(mParejas[1]);
            danceFloor->addPareja(mParejas[0]);
        }
    }
}

void InteractiveTangoMilongaArm64App::addLivePareja(std::map<int, int> &l, std::map<int, int> &f, int lID, int fID)
{

    if(f.size()==3 && l.size() ==3)
    {
        std::cout << "adding pareja\n";
        int wekIndexStart = sendOSCToWekinators.size();
        
        //create wekinator senders -- right legs then left legs for ea. couple... legacy TODO: fix!!!
#ifdef WEKINATOR_ENABLED
        for( int i=wekIndexStart; i<wekIndexStart+4; i++ )
            addSendtoWekinator();
        
        //add the leader
        InteractiveTango::Dancer *dancer = new InteractiveTango::Dancer(&mBeatTimer, sendOSCToWekinators[wekIndexStart], sendOSCToWekinators[wekIndexStart+2]);
#else
        //add the leader
        InteractiveTango::Dancer *dancer = new InteractiveTango::Dancer(&mBeatTimer, NULL, NULL);
        
#endif

        dancer->addSensorBodyPart(l[0], mSensorData[l[0]], &mUGENs, InteractiveTango::Dancer::LeftFoot);
        dancer->addSensorBodyPart(l[1], mSensorData[l[1]], &mUGENs, InteractiveTango::Dancer::RightFoot);
        dancer->addSensorBodyPart(l[2], mSensorData[l[2]], &mUGENs, InteractiveTango::Dancer::Back);
        dancer->setDancerID( lID, &mUGENs );
        mDancers.push_back(dancer);
        
        
        //add dancer/limb info to sensor data, so that it is saved in the file
        mSensorData[l[0]]->setDancerLimb(mDancers.size(), InteractiveTango::Dancer::LeftFoot);
        mSensorData[l[1]]->setDancerLimb(mDancers.size(), InteractiveTango::Dancer::RightFoot);
        mSensorData[l[2]]->setDancerLimb(mDancers.size(), InteractiveTango::Dancer::Back);
        
        std::cout << "Added Brent as dancer: " << l.size() << "\n";
        
        //add the follower
#ifdef WEKINATOR_ENABLED

        InteractiveTango::Dancer *dancer2 = new InteractiveTango::Dancer(&mBeatTimer, sendOSCToWekinators[wekIndexStart+1], sendOSCToWekinators[wekIndexStart+3]);
#else
         InteractiveTango::Dancer *dancer2 = new InteractiveTango::Dancer(&mBeatTimer, NULL, NULL);
#endif
        dancer2->addSensorBodyPart(f[0], mSensorData[f[0]], &mUGENs, InteractiveTango::Dancer::LeftFoot);
        dancer2->addSensorBodyPart(f[1], mSensorData[f[1]], &mUGENs, InteractiveTango::Dancer::RightFoot);
        dancer2->addSensorBodyPart(f[2], mSensorData[f[2]], &mUGENs, InteractiveTango::Dancer::Back);
        dancer2->setDancerID( fID, &mUGENs );
        mDancers.push_back(dancer2);
        
        //add dancer/limb info to sensor data, so that it is saved in the file
        mSensorData[f[0]]->setDancerLimb(mDancers.size(), InteractiveTango::Dancer::LeftFoot);
        mSensorData[f[1]]->setDancerLimb(mDancers.size(), InteractiveTango::Dancer::RightFoot);
        mSensorData[f[2]]->setDancerLimb(mDancers.size(), InteractiveTango::Dancer::Back);
        
        std::cout << "Added Courtney as dancer\n";
        std::cout << "Added Brent as dancer: " << f.size() << "\n";

        
        //add the dancer
        InteractiveTango::Pareja *pareja = new InteractiveTango::Pareja(mDancers[mDancers.size()-2], mDancers[mDancers.size()-1], &mUGENs, &mBeatTimer );
        mParejas.push_back(pareja);
        danceFloor->addPareja(pareja);
        std::cout << "Added a pareja\n";
        
        //saving all the sensor info
        //TODO: make this work again.
 //       saveAllSensors();
        
        
        danceFloor->loadCourtneyTangoSongNumberOneIntoPlayer();
        
        
        //add an extra value to show done with adding pair //TODO: FIX!!! REFACTOR
        f.insert(std::pair<int, int>(3, 0));
        l.insert(std::pair<int, int>(3, 0));
    }
    else
    {
        std::cout << "missing sensors: courtney - " << f.size() << " brent - " << l.size() << "\n";
    }

}

void InteractiveTangoMilongaArm64App::saveAllSensors()
{

    std::time_t  timev;
    struct tm * now = localtime( & timev );
    
    std::stringstream ss;
    mSaveFilePath = App::getSaveFilePath().c_str();
    
    
    ss << "itm_file" << (now->tm_year + 1900) << '_' << (now->tm_mon + 1) << '_' <<  now->tm_mday << "_";
    
    for( int i=0; i<mSensorData.size(); i++)
    {
        mSensorData[i]->saveToFile(mSaveFilePath, ss.str());
    }
    mBeatTimer.save(mSaveFilePath);

}

void InteractiveTangoMilongaArm64App::parseFilenameForInfo(string fname, string *deviceID, int *whichID)
{
    *deviceID = fname.substr(fname.find('.') + 1, fname.find('_') - 1);
    *whichID = std::atoi( fname.substr(fname.find('_') + 2, fname.find('.') - 1 ).c_str());
}

void InteractiveTangoMilongaArm64App::loadSensor(std::string input)
{
    VisualizeShimmer *visualizer = new VisualizeShimmer();
    SensorData *sensor;
    int whichShimmer;
    std::string deviceID;
    parseFilenameForInfo(input, &deviceID, &whichShimmer);

    sensor = new LoadedSensor(input, deviceID, whichShimmer, visualizer);
    
    ColorA c = generateRandomColor();
    visualizer->setColor( c );
    visualizer->setBounds(app::App::getWindowSize());
    mSensorData.push_back(sensor);
    visualizers.push_back(visualizer);
}
CINDER_APP( InteractiveTangoMilongaArm64App, RendererGl,
           []( InteractiveTangoMilongaArm64App::Settings *settings ) //note: this part is to fix the display after updating OS X 1/15/18
           {
               settings->setHighDensityDisplayEnabled( true );
               settings->setTitle("Interactive Tango Milonga");
               settings->setFrameRate(FRAMERATE); //set fastest framerate
           } )
