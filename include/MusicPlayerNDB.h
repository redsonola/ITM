//
//  MusicPlayerNDB.h
//  InteractiveTangoMilonga2
//
//  Created by courtney on 3/21/18.
//

#ifndef MusicPlayerNDB_h
#define MusicPlayerNDB_h

#include <memory>

//DO THIS STUFF AFTER ACCOMPANIMENT ADDED AND BEFORE ORNAMENTS (tho ornaments should be easy I think...)
//TODO: perhaps will change onset of a note in the buffer rather than simply adding things in
//add the end of phrase delimiter (I think it will be a velocity)
//should make note to onset slow to one per step 2-3 notes before end...


//instruments... //DONE added instruments

namespace InteractiveTango {

//    //this is an interface for setting the midi channels for the send signals, since they need to agree with whatever channels the
//    //melody player is sending from .
//class SetMidiChannelsForSendSignals
//{
//protected:
//    std::vector<SendSignal *> sendSignals; //need to update the midi channel for these...
//public:
//    virtual void setSendSignals(std::vector<SendSignal *> sig)
//    {
//        sendSignals = sig;
//    };
//
//    virtual void setMidiChannels(std::vector<int> ch)
//    {
//        for(int i=0; i<sendSignals.size(); i++)
//            sendSignals[i]->changeChannel(ch);
//    }
//};

//this is an interface that provides functionality of controlling staccato/legato via C++ (instead of Max)
//***currently will only work with melody
class PointySmoothControllingNoteOffs
{
protected:
    PointyVsSmoothContinuous *pvc;
    PlayBufferedNotes *buffer;
    MidiMessage::NoteDuration noteDuration;
    std::vector<MidiNote> notesToAddOffs;
    float duration;
    
    bool channelChange;
    int channel;
    int prevChannel;

public:
    PointySmoothControllingNoteOffs(PlayBufferedNotes *buffer_ = NULL)
    {
        pvc = NULL;
        buffer = buffer_;
        channelChange = false;
        
        prevChannel = -1;
        
    };
    
    std::vector<MidiNote> getNoteOffs()
    {
        return notesToAddOffs;
    }
    
    void setPVC(PointyVsSmoothContinuous *pvc_)
    {
        pvc = pvc_;
    }
    
    void setPlayBufferedNotes( PlayBufferedNotes *buffer_)
    {
        buffer = buffer_;
    }
    
    void setChannel(int ch)
    {
        prevChannel= channel;
        channel =ch;
        channelChange = true;
    }
    
    void findNoteDuration()
    {
        //mirroring the max patch exactly
        int curMood = std::round((pvc->getCurMood() * 5.0) + 1.0);
        
        if( curMood < 3)
        {
            noteDuration = MidiNote::NoteDuration::ALL_LEGATO;
        }
        else if ( curMood == 3 )
        {
            noteDuration = MidiNote::NoteDuration::N4;
            duration = 1;
        }
        else if ( curMood == 4 )
        {
            noteDuration = MidiNote::NoteDuration::N8;
            duration = 0.5f;

        }
        else if ( curMood == 5 )
        {
            noteDuration = MidiNote::NoteDuration::N16;
            duration = 0.25f;

        }
        else
        {
            noteDuration = MidiNote::NoteDuration::N32;
            duration = 0.125f;

        }
    }
    
    void update(float seconds)
    {
        if(!pvc) return;
        if(!buffer) return;
        
        findNoteDuration();
        
        std::vector<MidiNote> offs;
        std::vector<MidiNote *> notesToBePlayed = buffer->notesAboutToBeSent(seconds, channel);
        
        //end all the hanging notes -- note this only works for the melody -- not yet working for accomp.
        //actually accomp just needs to be 1 measure long, and not endless like melody. no accomp notes will last longer than a measure
        //still, this is not implemented for accomp -- that still has other stuff.
        if(notesToBePlayed.size()>0)
        {
            buffer->addNotesNoAdjustmentToTick(notesToAddOffs, seconds);
            notesToAddOffs.clear();
            std::cout << "Legato note-offs\n";
        }
        
        if(noteDuration != MidiNote::NoteDuration::ALL_LEGATO)
        {
            
            for(int i=0; i<notesToBePlayed.size(); i++)
            {
                //add the note offs to the buffer to be played
                //need to change to ticks.
                //then deal with legato notes & the hanging note problem.
                
                MidiNote note = MidiNote(notesToBePlayed[i]);
                note.velocity = 0;
                note.tick += std::round(duration * (float)note.tpb) ;
                offs.push_back(note);
                std::cout << "Sending note off here: " << note.toStr() <<"\n";
                buffer->addNotesNoAdjustmentToTick(offs, seconds);
                
                //ok here we need to negotiate ending SMOOTH instruments at the end of every next note & not bother with pointy
                
                //notesToAddOffs.push_back(note);
            }
        }
        else
        {
            //this is legato mode.
            
//            prevChannel= channel;
//            channel =ch;
//            channelChange = true;
              
            for(int i=0; i<notesToBePlayed.size(); i++)
            {

                //then deal with legato notes & the hanging note problem.
                
                MidiNote note = MidiNote(notesToBePlayed[i]);
                note.velocity = 0;
                note.tick = 0; ;
                notesToAddOffs.push_back(note);
            }
        }
        
    };
    
};

    
//new classes that use the new input method for songs and databases for songs
class MainMelodySectionNDB : public MainMelodySection, public PointySmoothControllingNoteOffs
{
protected:
    MelodyFile *curMelodyFile;
    std::vector<MidiNote> noteBuffer;
    Song *song;
    
    int curSection;
    int curPart;
    int midiCh;
    bool phraseStart;
    Instrument *curInstrument;
    PlayBufferedNotes *playNotes; //need to reference to see what is already in the buffer. fastest way to make this mod.

//TODO: why am I looping through the mapping objects like an idiot? should just name them in an enum. Its been 5 years and they
//haven't changed. FIX WHEN TIME. That was some overengineered bs.
    
public:
                                                                                //yes hacky should fix
    MainMelodySectionNDB(BeatTiming *timer, FootOnset *onset, Instruments *ins, Song *s, float perWindowSize=1) :
        MainMelodySection(timer, onset, ins, perWindowSize), PointySmoothControllingNoteOffs()
    {
        song = s;
        where_in_song_structure = -1;
        timesToRepeatSection();
        instrumentsAvailable = ins;
        curInstrument = NULL;
        playNotes = NULL;
        
        if(! instrumentsAvailable)
            return;
//        //update midi channels in send signal
//        for(int i=0; i<instrumentsAvailable->size(); i++)
//        {
//            allMidiChannels.push_back(instrumentsAvailable->getInstrumentViaIndex(i)->getMelodyMidiCh());
//        }
//            setMidiChannels(allMidiChannels);
    };
    
    //ok this does have access to this object via the noteoffs but I don't think that ever worked.
    void setPlayNotes(PlayBufferedNotes *pNotes)
    {
        playNotes = pNotes;
    }
    
    int getBVSMax()
    {
        return curMelodyFile->getBVSMax();
    }
    
    virtual  void update(float seconds = 0)
    {
        std::shared_ptr<std::vector<int>> hsprofile;
        update(hsprofile, seconds);
    }
    
    int getCurSection()
    {
        return curSection;
    }
    
    int getCurPart()
    {
        return curPart;
    }
    
    //OK should be in the constructor but will change sooon
    void initSection()
    {
        changeSection(0);
    }
    
    MelodyFile *getCurMelodyFile()
    {
        return curMelodyFile;
    }
    
    //get the current midiChannel
    virtual  int getMidiChannel()
    {
        return midiCh;
    }
    
    virtual void changeInstrument(float seconds)
    {
        std::shared_ptr<std::vector<float>> instrumentProfile(new std::vector<float>);
        OrchestralEvent *event;
        int moodNum;

        //find the correct scheme to get mood
        for(int i=0; i<mMappingSchemas.size(); i++)
        {
            moodNum =  ((PerceptualEvent * )mMappingSchemas[i])->getCurMood(mappingPerceptionWindowForMelody, seconds) ;
            //            std::cout << "in Curfile :"<< ((PerceptualEvent * )mMappingSchemas[i])->getName() << ":  " << moodNum << "     " << seconds << "  \n";
            
            if(( mMappingSchemas[i]->getMappingType() == MappingSchema::MappingSchemaType::ORCHESTRAL )  )
            {
                instrumentProfile->push_back( moodNum );
                event = (OrchestralEvent *)mMappingSchemas[i];
            }
        }
        
        //orchestrate melody with appropriate instrument
        std::vector<int> ids;
        float midMood = ((int)(event->getMaxMood())/2) + event->getMinMood();
        if(midMood == moodNum)
        {
            ids = instrumentsAvailable->instrumentsWithProfile(event->getMinMood());
            std::vector<int> ids2 = instrumentsAvailable->instrumentsWithProfile(event->getMaxMood());
            ids.insert(ids.end(), ids2.begin(), ids2.end());
        }
        else ids =  instrumentsAvailable->instrumentsWithProfile(instrumentProfile);
        int instrid = ids[ rand() % ids.size() ];
        midiCh = instrumentsAvailable->getInstrument(instrid)->getMelodyMidiCh();
        curInstrument = instrumentsAvailable->getInstrument(instrid);
        PointySmoothControllingNoteOffs::setChannel(midiCh); 
        

    }
    
    virtual void changeSection(float seconds)
    {
        where_in_song_structure++;
        if( where_in_song_structure >= song_structure.size() )
        {
            where_in_song_structure = 0;
        }
        
        curSection = song_structure[where_in_song_structure];
        SongSection *section = song->sections[curSection-1];
        
        //right now give equal probability to each part
        if (section->parts.size() < 2) curPart = 0;
        else
        {
            //randomly choose a part
            curPart = (int) std::round((((double) std::rand()) / ((double) RAND_MAX)) * (section->parts.size()-1));
        }
        SongSectionParts *part = section->parts[curPart];
        curMelodyFile = (MelodyFile *) part->melody;
        curMelodyFile->reset(); 
//        std::cout << "WHICH PART? " << curPart << std::endl;
        
        //set the current instrument
        changeInstrument(seconds);
    };
    
    //this needs to be called AFTER the playNoteBuffer gets the melody notes in the MusicPlayer class.
    virtual void updatePvCNoteOffs(float seconds)
    {
        PointySmoothControllingNoteOffs::update(seconds);
    }
    
    virtual void update(std::shared_ptr<std::vector<int>> hsprofile, float seconds = 0)
    {
        if(!playNotes)
        {
            std::cout << "PlayBufferedFile not defined! Must set file\n";
            return;
        }
        
        secondsPlayed = beatTimer->getTimeInSeconds() - secondsStarted; //how long in seconds has the file been playing?
        curSeconds = seconds;
        noteBuffer.clear();
    
        if( beatTimer->isOnBeat(0.0, seconds) ) //exactly on beat
        {
            beatsPlayed++;
        }
    
        lastStepped = fo->isStepping() ;
    
        //shouldPlayNewFile() &&
        if(fo->isStepping() ) //wait for a foot onset to start
        {
            if( playNotes->readyToPlay(seconds) )
            {
                return; //nothing to do, note(s) already qeued up
            }
            else if(curMelodyFile->atEnd())
            {
//                std::cout << "changing section..\n";
                changeSection(seconds);
                phraseStart = true;

            }
            else phraseStart=false;
            
            shouldStartFile = true;
            beatsPlayed = 0;
            secondsStarted = beatTimer->getTimeInSeconds();
            curMelodyFile->update(findBusySparse());
            
            
            noteBuffer = curMelodyFile->getCurNoteBuffer();
            
            //adjust step detection wait time based on how close we are to a cadence.
            if(curMelodyFile->atEnd())
            {
                fo->setMinDurationBetweenSteps(beatTimer->getPulse8()*8.0); //wait after the cadence
            }
            if( curMelodyFile->cadenceNotes() )
            {
                fo->setMinDurationBetweenSteps(beatTimer->getPulse8()*3.0); //wait (slightly) longer during cadences..
            }
            else
            {
                fo->setMinDurationBetweenSteps(beatTimer->getPulse8()*2.0); //normal wait-times... (seems too high but let's see)
            }
            
//            std::cout << "Melody Busy / Sparse: " << findBusySparse() << " noteBuffer size: " << noteBuffer.size() << std::endl;

            
//            std::cout << "Busy Sparse Melody: " << findBusySparse() << " notes in buffer: " << noteBuffer.size() <<  std::endl;

            
            //put notes in the current instrument's range -- not this could have fixed some shit... 
            if( curInstrument ){
//                std::cout << curInstrument->getName() << ","  << noteBuffer.size() <<  std::endl;
                for(int i=0; i<noteBuffer.size(); i++)
                {
                    noteBuffer[i].pitch = curInstrument->putInRange(noteBuffer[i].pitch);
//                    std::cout << "pitch: " << noteBuffer[i].pitch << std::endl;
                }
            }
            
            if(noteBuffer.empty()) update(hsprofile, seconds); //get notes if there aren't notes -- probably need to change the section
            else
            {
                //make sure notes are sent via the correct channel
                for(int i=0; i<noteBuffer.size(); i++)
                {
                    noteBuffer[i].channel = midiCh;
                }
                
            }
                
                //TODO -- set/switch section  curMelodyFie  -- do in constructor as well
        } else shouldStartFile = false;
        
        
        //update the note durations for staccato/legato
            //TODO: implement
        
            
//            if(curSoundFile != NULL)
//            {
//                curMelodyFile->changeHarmonyIfHalfway(secondsPlayed);
//            }
    };
    
    virtual void timesToRepeatSection()
    {
        //TODO: put in database
        song_structure.clear();
        song_structure.push_back(1);
        song_structure.push_back(1);
        song_structure.push_back(1);
        song_structure.push_back(2);
        song_structure.push_back(2);
        
    }
    
    virtual void timesToRepeatSectionPorUnaCabeza()
    {
        //TODO: put in database
        song_structure.clear();
        song_structure.push_back(1);
        song_structure.push_back(2);
    }
    
    //OMFG this needs refactoring. Should be in a database.
    virtual void timesToRepeatSectionElChoclo()
    {
        //TODO: put in database
        song_structure.clear();
        song_structure.push_back(1);
        song_structure.push_back(2);
        song_structure.push_back(3);
        song_structure.push_back(4);

    }
    
    virtual std::vector<MidiNote> getNoteBuffer()
    {
        return noteBuffer;
    }
    
    virtual void printNoteBuffer()
    {
        if( noteBuffer.size() <= 0 ) return;
        
        std::cout << "-- Notes in Melody Buffer --" << std::endl;
        for(int i = 0; i<noteBuffer.size(); i++)
        {
            std::cout << noteBuffer[i].toStr() ;
        }
        std::cout << "-- END Melody Buffer --" << std::endl;

    }

    virtual bool shouldPlayNewFile() //new notes,  now, really -- TODO: change this name
    {
        float beatsPlay =  secondsPlayed / (60.0f/ (float)beatTimer->getBPM()); //how many beats have been played
        return beatsPlay > curMelodyFile->getNotesLen();
    };
    
    virtual std::vector<ci::osc::Message> getOSC()
    {
//        return MainMelodySection::getOSC();
        
        std::vector<ci::osc::Message> msgs;
        
       if(phraseStart)
       {
           ci::osc::Message msg;
           msg.setAddress(ITM_PHRASESTART);
           msgs.push_back(msg);
       }
        return msgs;
    };
    
    virtual int getWhereInSong()
    {
        return where_in_song_structure;
    }

}; //end of new melody section
    
    //------------------------------------
    //------------------------------------
    //------------------------------------
    //------------------------------------
    class AccompanimentSectionNDB : public AccompanimentSection //, public SetMidiChannelsForSendSignals
    {
    protected:
        AccompFile *curAccompFile;
        std::vector<MidiNote> noteBuffer;
        Song *song;
        
        int curSection;
        int curPart;
//        std::vector<int> midiCh;
        MainMelodySectionNDB *mainMelody;
        Orchestra curOrch;
        float lastOrchPercept;
        int whereInSong;
        
        int bvsMax;
        int lastBVS;
        int currentBVS;
        
    public:
        //yes hacky should fix
        AccompanimentSectionNDB(BeatTiming *timer, Instruments *ins, Song *s, MainMelodySectionNDB *melody  ) :
        AccompanimentSection(timer, ins)
        {
            song = s;
            instrumentsAvailable = ins;
            mainMelody = melody;
            whereInSong = -1;
            lastBVS =0;
            currentBVS = 0;
        };
        
        virtual  void update(float seconds = 0)
        {
            std::shared_ptr<std::vector<int>> hsprofile;
            update(hsprofile, seconds);
        }
        
        Orchestra *getOrch() { return &curOrch; };
        
        //OK should be in the constructor but will change sooon
        void initSection()
        {
            changeSection(0, 0);
        }
        
//        //get the current midiChannel
//        virtual  int getMidiChannel()
//        {
//            return midiCh;
//        }
        
        virtual std::shared_ptr<std::vector<float>> getCurOrchestralProfile(OrchestralEvent *event, float seconds)
        {
            
            std::shared_ptr<std::vector<float>> instrumentProfile(new std::vector<float>);
            const float ORCHESTRAL_MAPPING_WINDOW = 1.5; //changed from 2 sec. we'll see
            
            //find the correct scheme to get mood
            for(int i=0; i<mMappingSchemas.size(); i++)
            {
                float moodNum =  ((PerceptualEvent * )mMappingSchemas[i])->getCurMood(ORCHESTRAL_MAPPING_WINDOW, seconds) ;
                

                if(( mMappingSchemas[i]->getMappingType() == MappingSchema::MappingSchemaType::ORCHESTRAL )  )
                {
                    instrumentProfile->push_back( moodNum );
                    event = (OrchestralEvent *) mMappingSchemas[i];
                                    }
            }
            assert(event != NULL) ;
            return instrumentProfile;

        };
        
        virtual void changeInstrument(float seconds, float curBVS)
        {
            std::shared_ptr<std::vector<float>> instrumentProfile(new std::vector<float>);
            const float ORCHESTRAL_MAPPING_WINDOW = 1.5; //changed from 2 sec. we'll see
            OrchestralEvent *event = NULL;
            
            //find the correct scheme to get mood
            for(int i=0; i<mMappingSchemas.size(); i++)
            {
                float moodNum =  ((PerceptualEvent * )mMappingSchemas[i])->getCurMood(ORCHESTRAL_MAPPING_WINDOW, seconds) ;
                

                if(( mMappingSchemas[i]->getMappingType() == MappingSchema::MappingSchemaType::ORCHESTRAL )  )
                {
                    instrumentProfile->push_back( moodNum );
                    event = (OrchestralEvent *) mMappingSchemas[i];
                                    }
            }
            assert(event != NULL) ;

            //orchestrate accomp with appropriate instruments
              curOrch  =  instrumentsAvailable->orchestraWithProfile(instrumentProfile, curBVS, event);
            ((AccompFile *)curAccompFile)->setOrchestration(curOrch);
            lastOrchPercept = event->getCurMood();
            
            //OK, now I need to make the option of having 0 instruments... bvs 1 should be 0, bvs max should be + 1 from the lines

        };
        
        //this will keep one instrument
        virtual void changeInstrumentForMeasure(float seconds, float curBVS)
        {
            std::shared_ptr<std::vector<float>> instrumentProfile(new std::vector<float>);
            const float ORCHESTRAL_MAPPING_WINDOW = 1.5; //changed from 2 sec. we'll see
            OrchestralEvent *event = NULL;
            float bvsMax = getBVSMax();
            
            //find the correct scheme to get mood
            for(int i=0; i<mMappingSchemas.size(); i++)
            {
                float moodNum =  ((PerceptualEvent * )mMappingSchemas[i])->getCurMood(ORCHESTRAL_MAPPING_WINDOW, seconds) ;
                

                if(( mMappingSchemas[i]->getMappingType() == MappingSchema::MappingSchemaType::ORCHESTRAL )  )
                {
                    instrumentProfile->push_back( moodNum );
                    event = (OrchestralEvent *) mMappingSchemas[i];
                                    }
            }
            assert(event != NULL) ;
            
            //using power of 2 to make everything not negative. this part just sees whether there
            //is enough change in the orchestration perceptual category to warrant a change in the middle
            //of a phrase (so -- at least 25% different).
            float percentToChangeOrchestration = 0.35 * 0.35;
            float curOrchPercept = event->getCurMood();
            
            
            float percentChangedBetweenOrchs = lastOrchPercept-curOrchPercept;
            percentChangedBetweenOrchs = percentChangedBetweenOrchs /( (float ) event->getMaxMood() - (float) event->getMinMood() );
            percentChangedBetweenOrchs = percentChangedBetweenOrchs * percentChangedBetweenOrchs;
            
            //orchestrate accomp with appropriate instruments
            if(percentChangedBetweenOrchs >= percentToChangeOrchestration || lastBVS < curBVS)
            {
                curOrch  =  instrumentsAvailable->orchestraWithProfileKeepOne(instrumentProfile, curBVS, event, &curOrch);
                ((AccompFile *)curAccompFile)->setOrchestration(curOrch);
            }
            
        };

        
        bool isSendingNotes()
        {
            if(curAccompFile)
                return curAccompFile->isSendingNotes();
            else return false; 
        }
        
        virtual bool changeSection(float seconds, float curBVS)
        {
            int lastWhereInSong = whereInSong;
            whereInSong = mainMelody->getWhereInSong();
            
            //then don't need to change
            if(lastWhereInSong == whereInSong || whereInSong==-1)
                return false;
            
            curSection = mainMelody->getCurSection();
            curPart = mainMelody->getCurPart();


            
            try {
                SongSection *section = song->sections[curSection-1];
                SongSectionParts *part = section->parts[curPart];
                curAccompFile = (AccompFile *) part->accomp;
                bvsMax = curAccompFile->getBVSMax();
                ((AccompFile *)curAccompFile)->setMelodyFile( (MelodyFile *) part->melody ) ; //hmmmm...
            }
            catch(...)
            {
                std::cerr << "Length error\n?";
                exit(1);
            }
            changeInstrument(seconds, curBVS);
            return true;
        };
        
        int getBVSMax()
        {
            return bvsMax;
        };
        
        int getCurBVS(float seconds)
        {
            return getBusySparse(seconds, 0.75); //for debugging
        }
        
        virtual void update(std::shared_ptr<std::vector<int>> hsprofile, float seconds = 0)
        {
            noteBuffer.clear();

            curAccompFile->play(); //always playing for now
            bvsMax = curAccompFile->getBVSMax();
            lastBVS = currentBVS;
            currentBVS = getBusySparse(seconds, 1.5);

            bool sectionChanged = changeSection(seconds, currentBVS);
//            std::cout << "Current Accomp/Leader BvS: " << curBVS << std::endl;
//            std::cout << "BvS Max: " << bvsMax << std::endl;
            
            //if the curAccompFile is sending notes, it is at the beginning of the measure. Therefore, we should
            //adjust the orchestration for that measure
            //TODO: test while dancing -- could change this every 2, 3, or 4 measures instead.
            //TODO: currently this changes every call to update, but only needs to on the measure... but needs to be adjusted b4
            //the call to curAccompFile. Should test whether its on the measure independently in the curAccompFile
            if( !sectionChanged && getBVSMax() > 0 )
                changeInstrumentForMeasure(seconds, currentBVS);
            
            //maybe keep the 1st instrument then add/subtract the rest -- unless orchestration of 0
            curAccompFile->update(seconds, currentBVS); //TODO: use profile
            noteBuffer = curAccompFile->getCurNoteBuffer();
            

            
//            if(noteBuffer.size() > 0)
//                std::cout << "Sending accomp notes... " << noteBuffer.size() << std::endl;
            

        };
        
        double getCurTicks(){return curAccompFile->getCurTicks(); }//this is current ticks in file...
        
        virtual std::vector<MidiNote> getNoteBuffer()
        {
            return noteBuffer;
        }
        
//        virtual bool shouldPlayNewFile() //new notes,  now, really -- TODO: change this name
//        {
//            float beatsPlay =  secondsPlayed / (60.0f/ (float)beatTimer->getBPM()); //how many beats have been played
//            return beatsPlay > curMelodyFile->getNotesLen();
//        };
        
        virtual std::vector<ci::osc::Message> getOSC()
        {
            //        return MainMelodySection::getOSC();
            
            std::vector<ci::osc::Message> msgs;
            return msgs;
        };
        
    }; //end of new melody section
    //------------
    //------------
    //------------
    //------------

    
    class MusicPlayerNDB : public MusicPlayer //should have a song structure, will see
    {
    protected:
//        MainMelodySection *main_melody; //determines structure (for now)
//        std::vector<MelodySection *> c_melodies;
//        std::vector<AccompanimentSection *> accompaniments;
//        std::vector<OrnamentationSection *> ornaments;
//
//        std::shared_ptr< std::vector<int> > curHarmonyProfile; //always have a harmony profile
        
        PlayBufferedNotes *playMidiNotes;
        //um, a bit of a spaghetti move... and yet... this is the fastest way to add that functionality
        
    public:
        MusicPlayerNDB(BeatTiming *timer, MidiOutUtility *midiOut) : MusicPlayer()
        {
            playMidiNotes = new PlayBufferedNotes(timer, midiOut);
        };
        
        ~MusicPlayerNDB()
        {
            delete playMidiNotes;
        };
        
        virtual void setMainMelody(MainMelodySection *melody)
        {
            main_melody = melody;
            if(main_melody != NULL)
            {
                ((MainMelodySectionNDB *)main_melody)->setPlayBufferedNotes(playMidiNotes);
                ((MainMelodySectionNDB *)main_melody)->setPlayNotes(playMidiNotes); //ok need to refactor obv. wtf mate. see above.
            }
        };
        
        virtual void update(float seconds = 0)
        {

            if(main_melody == NULL) return;
            std::shared_ptr<std::vector<int>> hsprofile(new std::vector<int> ); //empty one for main melody... should fix that setup
            Orchestra curMelodyOrchestration;
            
            main_melody->update(hsprofile, seconds);
            playMidiNotes->addNotes(((MainMelodySectionNDB *)main_melody)->getNoteBuffer(), seconds);
//            ((MainMelodySectionNDB *)main_melody)->printNoteBuffer();
//            ((MainMelodySectionNDB *)main_melody)->updatePvCNoteOffs(seconds); //currently disabled
            
            
//            int sizebefore = playMidiNotes->size() ;
            accompaniments[0]->update(hsprofile, seconds);
            playMidiNotes->addNotes(((AccompanimentSectionNDB *)accompaniments[0])->getNoteBuffer(), seconds,
                                    ((AccompanimentSectionNDB *)accompaniments[0])->getCurTicks());
            
//            if(((AccompanimentSectionNDB *)accompaniments[0])->isSendingNotes())
//            std::cout << "Accomp note buffer: " << ((AccompanimentSectionNDB *)accompaniments[0])->getNoteBuffer().size() << std::endl;
            
//            if(playMidiNotes->size()-sizebefore > 0)
//            {
//                 int bvs= ((AccompanimentSectionNDB *)accompaniments[0])->getCurBVS(seconds);
//                 std::cout << "Instrument size: " << bvs << std::endl;
//            }
            playMidiNotes->update(seconds);
            
        };
    };
};


#endif /* MusicPlayerNDB_h */
