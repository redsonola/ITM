//
//  SoundFileNDB.h
//  InteractiveTangoMilonga2
//
//  Created by courtney on 3/20/18.
//

#ifndef SoundFileNDB_h
#define SoundFileNDB_h

#define MIDI_MAX 127


namespace InteractiveTango{
    
//class MoodMidiVelocityMap
//{
//protected:
//    double mood;
//    std::vector<int> midiVelocity;
//public:
//    MoodMidiVelocityMap(double m)
//    {
//        mood  = m;
//    }
//
//    double getMood(){return mood;}
//    void addVelocity(int vel)
//    {
//        midiVelocity.push_back(mood);
//    }
//    bool velocityFound(int vel)
//    {
//        bool found = false;
//        int i = 0;
//        while (!found && i < midiVelocity.size())
//        {
//            found = midiVelocity[i] == vel;
//            i++;
//        }
//        return found;
//    }
//
//};
////-------------------------------
//class MoodMidiVelocities
//{
//protected:
//    std::vector<MoodMidiVelocityMap> vmap;
//
//    MoodMidiVelocityMap *findMood(double mood)
//    {
//        MoodMidiVelocityMap *map = NULL;
//        bool found = false;
//        int i = 0;
//        while(!found && i<vmap.size())
//        {
//            found = vmap[i].getMood() == mood;
//            i++;
//        }
//        if(found) map  = &vmap[i-1];
//        return map;
//    }
//public:
//    MoodMidiVelocities(){};
//
//    void add(double mood, int velocity)
//    {
//        MoodMidiVelocityMap *map =  findMood(mood);
//        if(map == NULL)
//        {
//            MoodMidiVelocityMap m(mood);
//            vmap.push_back(m);
//            map = &vmap[vmap.size()-1];
//        }
//
//        map->addVelocity(velocity);
//    };
//
//    bool moodVelocityMappingExist(double mood, int velocity)
//    {
//        MoodMidiVelocityMap *map =  findMood(mood);
//        if(map==NULL) return false;
//        else
//        {
//            return map->velocityFound(velocity);
//        }
//    };
//
//};

//-------------------------------
class SectionStructureFromMidiFile
{
protected:
//    std::shared_ptr<std::vector<int>> moodProfile; //what kind of mood perception does this have?
//    std::shared_ptr<std::vector<int>> harmonySectionProfile; //what kind of harmony section does this have?  TBD - fix
    std::string midiFileName;
    int sectionID;
    int sectionOrder;
    int track;
    
    float bpm;
    float tpb;
    
    MidiFileUtility midiUtility;
    double curTicks; //where we are the file
    
//    std::vector<MidiNote> notes; //all the notes in the melody
    
    std::vector<MidiNote> curNoteBuffer; //the notes to be played soon
    
    float bvsMax; //min is 1, always. that is how it goes with this file database.
    float  curNotesLen;
    int whereTicks; //where we are in the melody, in ticks
    std::vector<int> where; //where we are in ea. track, index-wise
    bool attheEnd;
    int endOfPhraseDelimiter;
    bool duringCadence;
    
public:
    SectionStructureFromMidiFile(std::string midiFile, int section, int bvs, int t)
    {
        init(midiFile, bvs, section, t);
        duringCadence = false;
        
//        notes = midiUtility.getMelody(track);
    };
    
    int getSection(){ return sectionOrder; };
    
    virtual void init(std::string midiFile, int bvs, int section, int t)
    {
        midiFileName = midiFile;
        midiUtility.readMidiFile(midiFile);
        bpm = (float) midiUtility.getBPM();
        tpb = midiUtility.getTicksPerBeat();
        track = t;
        curTicks = 0;
        bvsMax=bvs;
        sectionOrder = section;
        curTicks = 0;
        duringCadence = false;
    };
    
    std::vector<MidiNote> getCurNoteBuffer()
    {
        return curNoteBuffer;
    };
    
    virtual void getNextNotes(float howManyBeatsMax, int mood)
    {
        curNoteBuffer.clear();
        
        float curNoteCollectionDuration = 0;
        bool endOfPhrase = false;
        attheEnd = false;
        
        //endOfPhraseDelimiter
        
        int i = 0;
        MidiNote *nextNote = NULL;
        
        while (curNoteCollectionDuration < howManyBeatsMax && !endOfPhrase && !atEnd() )
        {
            MidiNote *note = midiUtility.getMelodyNoteAtAbsTick(int(mood), curTicks);
//            std::cout << "mood: " << mood << " curTicks: " <<  std::endl;
            
            
            if(note == NULL)
            {   endOfPhrase = true;
                curTicks = 0;
                std::cout << "end\n";
            }
            //TODO: check but I think I haven't implemented this... 
//            else{
//                endOfPhrase =  note->pitch == endOfPhraseDelimiter; //change to velocity rather that pitch maybe???
//            }
            
            if(!endOfPhrase)
            {
                //                    int keepVelocity = MIDI_MAX*(1.0/(float)bvsMax) * (float)mood;
                //                    if( note.velocity >= keepVelocity)
                
                curNoteBuffer.push_back(*note);
                
                //OK, only put one note in the buffer if it is a cadence note -- that is the velocity (as a sign/label) will be 127)
                const int CADENCE_VELOCITY = 127;
                nextNote = midiUtility.getMelodyNoteAtAbsTick(int(mood), note->absTick + 1);
                if(nextNote == NULL) endOfPhrase = true;
                else
                {
                    //if one of the last notes of the melody only output this one note
                    if(nextNote->oneOftheLastMelodyNotes || nextNote->velocity >= CADENCE_VELOCITY)
                    {
                        curNoteCollectionDuration = howManyBeatsMax;
                        duringCadence = true;
                    }
                    else
                    {
                        curNoteCollectionDuration += ( (float) nextNote->tick ) / ( (float)tpb );
                        duringCadence = false;

                    }
                    if(note->absTick < 0 || note->tick==0 ) note->absTick = 0;
                        curTicks = note->absTick + 1;
                    }
                }
            else
            {
                std::cout << "end phrase\n";
                //
                //                    if ( curNoteBuffer.empty() ) //skip over end of phrase delim and cont. if at start
                //                        endOfPhrase = false;
                
            }
            attheEnd = endOfPhrase;
            if(attheEnd) duringCadence = true; // just in case. should be true by then anyways. & may be true when attheend is false.

        //****  this was for debugging an infinite loop  ****
//            if(i>25)
//            {
//                std::cout << "Entering infinite loop. curNoteCollectionDuration: " << curNoteCollectionDuration << " cur note->absTick: " << note->absTick << " curTicks: " << curTicks <<  " note.tick: " << note->tick << " pitch: " << note->pitch << std::endl;
//
//                if(nextNote == NULL) std::cout << "next note is null\n";
//                else
//                {
//
//                    std::cout << "Next note: " << nextNote->absTick <<  " nextnote.tick: " << nextNote->tick << " pitch: " << nextNote->pitch << std::endl;
//
//                }
//            }
            i++;
        }
        

        curNotesLen = curNoteCollectionDuration;
        

    }
    
    bool cadenceNotes()
    {
        return duringCadence;
    }

    bool atEnd(){ return attheEnd; };
    
    float getBVSMax(){ return bvsMax; };

    
};
    
    class MelodyFile : public SectionStructureFromMidiFile
    {
    protected:

        //how many beats of notes to output per request -- 1 is one beat, 0.5 is a half-note
        float beatCountPerMin;
        float beatCountPerMax;
        int endOfPhraseDelimiter;
        
        long stepCount;
        
    public:
        //note that track is 1 since it is always 1 for the melody
        MelodyFile(std::string midiFile, int section, int bvs, float bcpmin=0.75, float bcpmax=1, int enddelim=NO_DATA) : SectionStructureFromMidiFile(midiFile, section, bvs, 1)
        {
            //TODO: do in loader when write it
            for(int i=0; i<bvs; i++)
                where.push_back(0);
            
            reset();
            beatCountPerMin = bcpmin;
            beatCountPerMax = bcpmax;
            endOfPhraseDelimiter = enddelim;
            attheEnd = false;
            stepCount = 0;
            
//            std::vector<MidiNote> tracks1 = midiUtility.getMelody(1);
//            for(int i=0; i<tracks1.size(); i++)
//            {
//                std::cout << "midinote pitch: " << tracks1[i].pitch << " tick: " << tracks1[i].tick << " absTick: " << tracks1[i].absTick << std::endl;
//            }
//
//            tracks1 = midiUtility.getMelody(2);
//            for(int i=0; i<tracks1.size(); i++)
//            {
//                std::cout << "midinote pitch: " << tracks1[i].pitch << " tick: " << tracks1[i].tick << " absTick: " << tracks1[i].absTick << std::endl;
//            }
//
//            tracks1 = midiUtility.getMelody(3);
//            for(int i=0; i<tracks1.size(); i++)
//            {
//                std::cout << "midinote pitch: " << tracks1[i].pitch << " tick: " << tracks1[i].tick << " absTick: " << tracks1[i].absTick << std::endl;
//            }

        };
        
        virtual void init(std::string midiFile, int section, int bvs, float bcpmin=1, float bcpmax=2, int enddelim=10)
        {
            SectionStructureFromMidiFile::init(midiFile, bvs, section, 1);
            
            for(int i=0; i<bvs; i++)
                where.push_back(0);
            
            reset();
            beatCountPerMin = bcpmin;
            beatCountPerMax = bcpmax;
            endOfPhraseDelimiter = enddelim;
            
        };
        
        void reset()
        {
            whereTicks = 0;
            curNoteBuffer.clear();
            curTicks = 0;
            for(int i=0; i<where.size(); i++)
            {
                where[i]=0;
            }

        }
        
        //currently accepts busy/sparse
        void update(int bvs)
        {
            
            //determine how long the beat will be randomly
//            float howManyBeatsMax = ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) *
//                       (beatCountPerMax-beatCountPerMin)) + beatCountPerMin;
            
//            std::cout << "howManyBeatsMax:" << howManyBeatsMax <<" bvs:" << bvs << " count:" << stepCount << std::endl;
            
            float howManyBeatsMax = 0.5; //half a beat max. testing. 6/30/2021
            getNextNotes(howManyBeatsMax, bvs);
            
            stepCount++;
            
        }
        

        
        float getNotesLen(){ return curNotesLen; }; //in beats not seconds
        float curTick(){ return curTicks; };
        
    };
    
    //----------------------------------------------
    
    
    class AccompFile: public SectionStructureFromMidiFile
    {
        //note that track is 1 since it is always 1 for the melody
        
    protected:
        int lines; //hmmm
        MelodyFile *melody; //this will tell us where we are
        int whereStartInFile; //which line the accomp starts in the file
        float howMuchToPlay; //in beats
        bool playing;
        float secondsStart;
        BeatTiming *beatTimer;
        Orchestra orchestration;
        bool sendingNotes;
        
        int beatsInAMeasure = 4; //can get this from MidiUtility if we actually have non-4/4 but for now... for simplicity sake...
    public:
        AccompFile(std::string midiFile, int section, int bvs, MelodyFile *mel, int whichLine, BeatTiming *timer, float playTime =4, float bcpmin=0.75, float bcpmax=1, int enddelim=10) : SectionStructureFromMidiFile(midiFile, section, bvs, 1)
        {
            beatTimer = timer;
            whereStartInFile = whichLine;
            reset();
            attheEnd = false;
            howMuchToPlay = playTime;  //for now, play in measure increments
            sendingNotes = false;
        };
        
        void setOrchestration(Orchestra orch)
        {
            orchestration = orch;
        }
        
        double getCurTicks(){ return curTicks; };
        
        void setMelodyFile(MelodyFile *mel)
        {
            melody = mel;
        }
        
        void reset()
        {
            whereTicks = 0;
            curNoteBuffer.clear();
            for(int i=0; i<where.size(); i++)
            {
                where[i]=0;
            }
            playing = false;
            secondsStart = 0;
            
            
        }
        
        void play()
        {
            playing = true;
        }
        
        void stop()
        {
            playing = false;
        }
        
        //currently accepts busy/sparse
        //currently  only implemented for written out accompanment
        void update(float seconds, int bvs)
        {
            curNoteBuffer.clear();
            

            if(playing)
            {
                if((seconds-secondsStart >= (beatTimer->beatsToSeconds(howMuchToPlay))) && beatTimer->isOnBeat(seconds, 0.0))
                {                    
                    secondsStart = seconds;
                    //fill the note buffer;
                    getNextNotes(bvs);
                    sendingNotes = curNoteBuffer.size() > 0;
                }
                else
                {
                    sendingNotes = false;
                }
            }
            
        }
        
        bool isSendingNotes(){return sendingNotes;};
        
        virtual void getNextNotes(int mood)
        {
            
            if(mood <= 0) return; //if it is so sparse, it is the minimium, don't output notes
            
            curTicks = melody->curTick(); //get the accomp. based on where the melody is
            
            //find the tick that is on a measure start
            int whereInBeats = floor(((double)curTicks) / ((double)tpb) );
            int whereInMeasure = floor(((double)whereInBeats) / ((double)beatsInAMeasure));
            curTicks = whereInMeasure * beatsInAMeasure * tpb;
            
            //add more lines of music with a busier dancers
            for(int i=whereStartInFile; i<whereStartInFile+mood; i++)
            {
                std::vector<MidiNote *> accompMidiNote = midiUtility.getAccompNotesAtAbsTick(i, curTicks, curTicks+(beatsInAMeasure*tpb));
                
                
                for(int j=0; j<accompMidiNote.size()-1; j++) //fixed so it dooesn't over 1.25.2018
                {
//                    std::cout << accompMidiNote[j]->pitch << " " << accompMidiNote[j]->tick << " " << accompMidiNote[j]->absTick << "\n";
                    
                    try {
                        int instrIndex = i-whereStartInFile;

                        accompMidiNote[j]->channel = orchestration.getInstrViaIndex(instrIndex)->getAccompMidiCh();
                        
                        //put notes in chosen instrument range
                        accompMidiNote[j]->pitch = orchestration.getInstrViaIndex(instrIndex)->putInRange(accompMidiNote[j]->pitch);

                        accompMidiNote[j]->voice = i;
                        
                        
                        curNoteBuffer.push_back(*accompMidiNote[j]);

                    }
                    catch(...)
                    {
                        std::cerr << "error is here\n";
                        exit(1);
                    }
                }
                
            }
        }
        
    };
    
    //----------------------------------------------
    class PlayBufferedNotes //performs midi sequencing of notes generated by the dancers
    {
    protected:
        std::vector<MidiNote> buffer;
        std::vector<float> secondsAdded;
        MidiOutUtility *midiOut;
        BeatTiming *beattimer;
    public:
        PlayBufferedNotes(BeatTiming *timer, MidiOutUtility *midi)
        {
            beattimer = timer;
            midiOut = midi;
        }
        
        //note: for accomp starts, the start tick may not be zero
        void addNotes(std::vector<MidiNote> newNotes, float seconds, double curTicks=-1)
        {
            double startTick = 0;
            if(newNotes.size()<=0) return;
            
            if(curTicks == -1) //then it will be the melody
            {
                for(int i=0; i<newNotes.size(); i++){
                        if(i==0) newNotes[0].tick = 0;
                        double tickToAdd =newNotes[i].tick;
                        newNotes[i].tick += startTick;
                        startTick += tickToAdd;
                        newNotes[i].setSection( MidiMessage::WhichSection::MELODY );
                    
                        buffer.push_back(newNotes[i]);
                        secondsAdded.push_back(seconds);
                }
            }
            else {
                addNotesAccomp(newNotes, seconds, curTicks);
             }
        }
        
        void printOutAccompValues(std::vector<MidiNote> addedNotes)
        {
            std::cout << "Accomp note buffer - after tick\n";
            for(int i=0; i<addedNotes.size(); i++){

                float curBeat = ( (float)addedNotes[i].tick ) / ( (float)addedNotes[i].tpb );
                std::cout << i << ": " << addedNotes[i].voice ;
                std::cout << "," << curBeat << std::endl;
            }
        }

        void addNotesAccomp(std::vector<MidiNote> newNotes, float seconds, double curTicks)
        {

            for(int i=0; i<newNotes.size(); i++){
                newNotes[i].tick = ( ((float)newNotes[i].absTick)-((float)curTicks) );
            
//                addedNotes.push_back(newNotes[i]);
                buffer.push_back(newNotes[i]);
                secondsAdded.push_back(seconds);
            }
//            printOutAccompValues(addedNotes);
        }
        
        //basically for note-offs
        void addNotesNoAdjustmentToTick(std::vector<MidiNote> newNotes, float seconds)
        {
            for(int i=0; i<newNotes.size(); i++)
            {
                buffer.push_back(newNotes[i]);
                secondsAdded.push_back(seconds);
            }
        }


    
        //returns the current size of the buffer
        size_t size()
        {
            return buffer.size(); 
        }
        
        std::vector<MidiNote *> notesAboutToBeSent(float seconds, int channel)
        {
            //return references to the notes about to be sent.
            std::vector<MidiNote *> buf;
            

            for(int i=0; i<buffer.size(); i++)
            {
                if( (((double)buffer[i].tick / (double)buffer[i].tpb)) * ((60.0)/(double)beattimer->getBPM()) <= seconds - secondsAdded[i] ) //TODO synch with beats
                {
                    if(buffer[i].channel == channel)
                        buf.push_back(&buffer[i]);
                }
            }
            return buf;
        }
        
        //ok looks at it's buffer. if there is an upcoming melody note, then it has a note there ready to play
        //for a step that has already been sent there.
        //this will be called by the melody section. so the melody section will have a pointer now to this object/class.
        //it is called where there is a new step. so that melodies don't get out of order & don't go by too quickly
        bool readyToPlay(float seconds)
        {
            bool ready = false;
            int i = 0;
            
            while( i<buffer.size() && !ready)
            {
                ready = buffer[i].getSection() == MidiMessage::WhichSection::MELODY;
                i++;
            }
            if( ready )
            {
                
                //update the rest of the note ticks
                i--;
                if( buffer[i].tick != 0 )
                {
                    double beatsPassed = (seconds - secondsAdded[i]) * ((double)beattimer->getBPM()/60.0) ;
                    double ticksPassed = beatsPassed * buffer[i].tpb;
                    double howManyTicksPassedToAdjust = buffer[i].tick - ticksPassed;
                    for(int j=i+1; j<buffer.size(); j++)
                    {
                        if( buffer[j].getSection() == MidiMessage::WhichSection::MELODY )
                        {
                            buffer[j].tick -= howManyTicksPassedToAdjust;
                        }
                    }
                    buffer[i].tick = 0; //ok, now this will play next update
                }
            }
            return ready;
        }
        
        void update(float seconds)
        {
            int i = 0;
            bool mel = false;
            bool first = true;
            
            while( i<buffer.size() )
            {
                if( (((double)buffer[i].tick / (double)buffer[i].tpb)) * ((60.0)/(double)beattimer->getBPM()) <= seconds - secondsAdded[i] ) //TODO synch with beats
                {
                    midiOut->send(buffer[i], buffer[i].channel); //TODO: which midi-channel?
                    if( buffer[i].getSection() == MidiMessage::WhichSection::MELODY ) //debug
                    {
                    
//                        if(first)
//                        {
//                            std::cout << "------ START PLAYED NOTES --------" << std::endl;
//                            mel = true;
//                        }
                        std::cout << buffer[i].toStr();

                    }
                    try {
                        secondsAdded.erase(secondsAdded.begin() + i);
                        buffer.erase(buffer.begin() + i);
                    }
                    catch(...)
                    {
                        std::cerr << "STD_LENGTH ERROR???" << std::endl;
                        exit(1); 
                    }
                }
                else
                {
                    i++;
                }
            }
            
//            if(mel)
//            {
//                std::cout << "------ END PLAYED NOTES --------" << std::endl;
//            }

            
        }

    };
    
    //-------------------------------
    class SongSectionParts //for now just structs really,  no behavior
    {
    public:
        SectionStructureFromMidiFile *melody;
        SectionStructureFromMidiFile *accomp;
        int sectionID;
        float probability;
        
        SongSectionParts()
        {
            melody = NULL;
            accomp =  NULL;
            probability = 0;
        }
    };
    //-------------------------------
    class SongSection
    {
    public:
        std::vector<SongSectionParts *> parts;
        int sectionOrder;
    };
    //-------------------------------
    class Song
    {
    public:
        std::vector<SongSection *> sections; 
    };
    //-------------------------------
    class SongLoader
    {
    protected:
        std::string dbFile;
        std::vector<Song *> songs;
    public:
        SongLoader(std::string db ="/Users/courtney/Programs/cinder_0.9.2_mac/InteractiveTangoMilongaNewDB/ITM_Database/ITMSongs.csv")
        {
            dbFile = db;
        };
        
        std::vector<Song *> *getSongs()
        {
            return &songs;
        };
        
        Song *getSong(int index)
        {
            return songs[index];
        }
        
        virtual void load(BeatTiming *timer) //note -- this code expects all songs and section to be in order -- at this point I do not see a reason
                            //to make the database order invariant, but it could be an improvement in stability if at some point there are more devs, etc.
        {
            ReadCSV songData;
            songData.init(dbFile);
            bool first = true;
            
            int whichSong = -1;
            int whichSection = -1;
            
            SongSection *section = NULL;
            Song *song = NULL;
            
            while(  !songData.eof() )
            {
                std::vector<std::string> tokens = songData.getTokensInLine();
                
                if (first)
                {
                    first = false;
                }
                else if(tokens.size() > 0)
                {
                    //SongID    SectionID    SongSection    MidiFile    BSMax BSAccompMax   AccompGeneric    WrittenOutAccompStart    Key    Modality    EndPhraserDelimiter
                    
                    int songID = std::atoi(tokens[0].c_str());
                    int sectionID = std::atoi(tokens[1].c_str());
                    int songSection = std::atoi(tokens[2].c_str());
                    std::string mfile = tokens[3];
                    float bvsMax = std::atoi(tokens[4].c_str());
                    float bvsAccompMax = std::atoi(tokens[5].c_str());
                    int accompStart = std::atoi(tokens[7].c_str());
                    int endPhraseDelim = std::atoi(tokens[10].c_str());
                    float prob = std::atof(tokens[11].c_str());
                    
                    if(songID-1 > whichSong)
                    {
                        whichSong++;
                        song = new Song();
                        songs.push_back(song);
                        whichSection = -1;
                    }
                    
                    if(songSection-1 > whichSection)
                    {
                        whichSection++;
                        section = new SongSection();
                        song->sections.push_back(section);
                    }
                    
                    //(std::string midiFile, int bvs=3, float bcpmin=1, float bcpmax=2, int enddelim=10)
                    SongSectionParts *part = new SongSectionParts();
                    part->melody = new MelodyFile(mfile, songSection, bvsMax); //note -  enddelim not yet implemented
                    part->accomp = new AccompFile(mfile, songSection, bvsAccompMax, ((MelodyFile *) part->melody), accompStart, timer);
                    part->sectionID = sectionID;
                    part->probability = prob; 
                    section->parts.push_back(part);
                    section->sectionOrder = songSection;
                }
            }
            
            
        }
        
    };
    
};

#endif /* SoundFileNDB_h */
