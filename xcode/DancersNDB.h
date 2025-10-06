//
//  DancersNDB.h
//  InteractiveTangoMilonga2
//
//  Created by courtney on 3/24/18.
//

#ifndef DancersNDB_h
#define DancersNDB_h

namespace InteractiveTango
{
    class DanceFloorNDB : public DanceFloor {
    protected:
        SongLoader loader;
        
        //ok this is hacky but I am tired
        int followerLeftFootControlStart;
        int followerRightFootControlStart;
        int leaderLeftFootControlStart;
        int leaderRightFootControlStart;

    public:
        DanceFloorNDB(BeatTiming *timer, MidiOutUtility *midiOut): DanceFloor(timer) {
            if(player != NULL) delete player;
            
            loader.load(timer);
            
            player = new MusicPlayerNDB(timer, midiOut);
        }
        
        virtual void mappingForOneCouple()
        {
            //everything in parent class 1st
            DanceFloor::mappingForOneCouple();
            
            //now set the midi control numbers for leader and follower
            int controlNum = couples[0]->getSendSignalsFollower()[0]->getNumberOfMidiControlsNeeded();
            int controlNumNext = couples[0]->getSendSignalsLeader()[0]->getNumberOfMidiControlsNeeded();

            int midiCC = 1;
            
            std::vector<int> channels;
            channels.push_back(20);
            
            followerLeftFootControlStart = midiCC;
            followerRightFootControlStart = midiCC+controlNum;
            leaderLeftFootControlStart = midiCC+controlNum*2;
            leaderRightFootControlStart = midiCC+controlNum*2+controlNumNext;
            
            couples[0]->getSendSignalsFollower()[0]->setStartControlNum(followerLeftFootControlStart);
            couples[0]->getSendSignalsFollower()[0]->changeChannel(channels);
            couples[0]->getSendSignalsFollower()[1]->setStartControlNum(followerRightFootControlStart);
            couples[0]->getSendSignalsFollower()[1]->changeChannel(channels);
            
            couples[0]->getSendSignalsLeader()[0]->setStartControlNum(leaderLeftFootControlStart);
            couples[0]->getSendSignalsLeader()[0]->changeChannel(channels);
            couples[0]->getSendSignalsLeader()[1]->setStartControlNum(leaderRightFootControlStart);
            couples[0]->getSendSignalsLeader()[1]->changeChannel(channels);

        }
        
        //TODO: implement all the send signals for 2 couples
        

        void setMidiMapping(int i)
        {
            const int TURN_OFF_MIDI = -2;
            const int TURN_OFF_MIDI_MAPPING = -1;

            
            if(i == TURN_OFF_MIDI_MAPPING )
            {
                couples[0]->getSendSignalsFollower()[0]->setMidiMappingMode(TURN_OFF_MIDI_MAPPING);
                couples[0]->getSendSignalsFollower()[1]->setMidiMappingMode(TURN_OFF_MIDI_MAPPING);
                couples[0]->getSendSignalsLeader()[0]->setMidiMappingMode(TURN_OFF_MIDI_MAPPING);
                couples[0]->getSendSignalsLeader()[1]->setMidiMappingMode(TURN_OFF_MIDI_MAPPING);
            }
            else
            {
                //                if( i < followerRightFootControlStart )
                //                {
                //                    couples[0]->getSendSignalsFollower()[0]->setMidiMappingMode(i);
                //                }
                //                else if( i < leaderLeftFootControlStart )
                //                {
                //                    couples[0]->getSendSignalsFollower()[1]->setMidiMappingMode(i);
                //                }
                //                else if( i < leaderRightFootControlStart )
                //                {
                //                    couples[0]->getSendSignalsLeader()[0]->setMidiMappingMode(i);
                //                }
                //                else
                //                {
                //                    couples[0]->getSendSignalsLeader()[1]->setMidiMappingMode(i);
                //                }
                
                couples[0]->getSendSignalsFollower()[0]->setMidiMappingMode(TURN_OFF_MIDI);
                couples[0]->getSendSignalsFollower()[1]->setMidiMappingMode(TURN_OFF_MIDI);
                couples[0]->getSendSignalsLeader()[0]->setMidiMappingMode(TURN_OFF_MIDI);
                couples[0]->getSendSignalsLeader()[1]->setMidiMappingMode(TURN_OFF_MIDI);
                
                const int FOLLOWER = 1;
                const int LEADER = 2;
                if( i == FOLLOWER ) //follower
                {
                    couples[0]->getSendSignalsFollower()[0]->setMidiMappingMode(1);
                    couples[0]->getSendSignalsFollower()[1]->setMidiMappingMode(2);
                }
                else if ( i == LEADER )
                {
                    couples[0]->getSendSignalsLeader()[0]->setMidiMappingMode(3);
                    couples[0]->getSendSignalsLeader()[1]->setMidiMappingMode(4);
                
                }
            }
        
        }

        //just send max of each foot. -- don't want separate controls for different feet at the moment.
        MidiControlMessage *maxMIDIControl(MidiControlMessage *L, MidiControlMessage *R, int newChNumber)
        {
            MidiControlMessage *msg = L;
            int val = std::max( L->value, R->value );
            msg->value = val;
            msg->channel = newChNumber;
            return msg;
        }
        
        std::vector<MidiMessage *> maxMIDIControlVector(std::vector<MidiMessage *> L, std::vector<MidiMessage *> R, int newChNumber)
        {
            std::vector<MidiMessage *>  msgs;
            int j = R.size()-1;
            for( int i=L.size()-1; i>=0 && j>-1; i-- )
            {
                msgs.push_back( maxMIDIControl((MidiControlMessage *) L[i], (MidiControlMessage *) R[j], newChNumber ) );
                j--;
            }
            return msgs;
        }
        
        //ok just for one couple for now -- TODO: add more couples.
        //This just averages out the 2 feet.
        virtual std::vector<MidiMessage *> getMIDI(){
            
            std::vector<MidiMessage *>  msgs;
            //channel 1 is follower, channel 2 is leader
            
            if(couples.size() <= 0)
                return msgs;
                        
            std::vector<MidiMessage *>  fmsgs =  maxMIDIControlVector(couples[0]->getSendSignalsFollower()[0]->getMIDI(), couples[0]->getSendSignalsFollower()[1]->getMIDI(), 1 );
            std::vector<MidiMessage *>  lmsgs =   maxMIDIControlVector(couples[0]->getSendSignalsLeader()[0]->getMIDI(), couples[0]->getSendSignalsLeader()[1]->getMIDI(), 2 );
            
            for(int i=0; i<fmsgs.size(); i++)
            {
                msgs.push_back(fmsgs[i]);
            }
            
            for(int i=0; i<lmsgs.size(); i++)
            {
                msgs.push_back(lmsgs[i]);
            }
            
            return msgs;
        }
        
        void leaderFakeStep()
        {
            couples[0]->getLeader()->getOnsets()->createFakeStep();
        }
        
        void followerFakeStep()
        {
            couples[0]->getFollower()->getOnsets()->createFakeStep();
        }
        
        void followerFakeBusySparse(double m)
        {
            ((PerceptualEvent *) couples[0]->getMappingSchema(BUSY_SPARSE_FOLLOWER))->setFakeMood(m);
        }
        
        void leaderFakeBusySparse(double m)
        {
            ((PerceptualEvent *) couples[0]->getMappingSchema(BUSY_SPARSE_LEADER))->setFakeMood(m);
        }
        
        void accompFakeBusySparse(double m)
        {
            //convert to 1-20 -- hacky I know should fix -- numbers are from scaleBVSFrom20to5() in class ExperimentalAccompanimentSection
//            if(m==2) m = 8;
//            else if(m==3) m = 13;
//            else if(m==4) m= 16;
//            else m=20;
            
            //ok do nothing for now
            
        }
        //new database version -- which sends midi instead of OSC
        virtual void loadCourtneyTangoSongNumberOneIntoPlayer()
        {
            destroyCurSong();
            if( couples.size() <= 0 )
            {
                std::cout << "Warning! No couples on dance floor. Cannot load song! ";
                return;
            }
            
            Song *song = loader.getSong(TangoEntity::SongIDs::FRAGMENTS);
            melody = new MainMelodySectionNDB( mTimer, melodyOnsetDancer->getOnsets() , &instruments, song ); //so far only FOLLOWER... change?
            ((MainMelodySectionNDB *)melody)->timesToRepeatSection();
            accompaniment = new AccompanimentSectionNDB(mTimer, &instruments, song, (MainMelodySectionNDB *) melody  );  //add back when accompaniment ndb  is done
            
            for( int i=0; i<couples.size(); i++ )
                couples[i]->setMotionAnalysisParamsForSong(TangoEntity::SongIDs::FRAGMENTS);
            
            loadSong();
//            loadCourtneyTangoSongNumberOneOrnaments();
            
            selectedSong = TangoEntity::SongIDs::FRAGMENTS;
            std::cout << "Loaded Fragments\n";
        };
        
        //new database version -- which sends midi instead of OSC
        virtual void loadPorUnaCabezaIntoPlayer()
        {
            destroyCurSong();
            if( couples.size() <= 0 )
            {
                std::cout << "Warning! No couples on dance floor. Cannot load song! ";
                return;
            }
            
            Song *song = loader.getSong(TangoEntity::SongIDs::POR_UNA_CABEZA);
            melody = new MainMelodySectionNDB( mTimer, melodyOnsetDancer->getOnsets() , &instruments, song ); //so far only FOLLOWER... change?
            ((MainMelodySectionNDB *)melody)->timesToRepeatSectionPorUnaCabeza();
            accompaniment = new AccompanimentSectionNDB(mTimer, &instruments, song, (MainMelodySectionNDB *) melody  );  //add back when accompaniment ndb  is done
            
            //              ornaments
            for( int i=0; i<couples.size(); i++ )
                couples[i]->setMotionAnalysisParamsForSong(TangoEntity::SongIDs::FRAGMENTS);
            
            loadSong();
            //            loadCourtneyTangoSongNumberOneOrnaments();
            
            selectedSong = TangoEntity::SongIDs::POR_UNA_CABEZA;
            std::cout << "Loaded Por Una Cabeza\n";
        };
        
        virtual void loadLaCumparsitaIntoPlayer()
        {
            destroyCurSong();
            if( couples.size() <= 0 )
            {
                std::cout << "Warning! No couples on dance floor. Cannot load song! ";
                return;
            }
            
            Song *song = loader.getSong(TangoEntity::SongIDs::LA_CUMPARSITA);
            melody = new MainMelodySectionNDB( mTimer, melodyOnsetDancer->getOnsets() , &instruments, song ); //so far only FOLLOWER... change?
            ((MainMelodySectionNDB *)melody)->timesToRepeatSectionPorUnaCabeza(); //this has the same song structure... altho may add some alt. melodies
            accompaniment = new AccompanimentSectionNDB(mTimer, &instruments, song, (MainMelodySectionNDB *) melody  );  //add back when accompaniment ndb  is done
            
//            //              ornaments
//            for( int i=0; i<couples.size(); i++ )
//                couples[i]->setMotionAnalysisParamsForSong(TangoEntity::SongIDs::LA_CUMPARSITA);
            for( int i=0; i<couples.size(); i++ )
                couples[i]->setMotionAnalysisParamsForSong(TangoEntity::SongIDs::FRAGMENTS);
            
            loadSong();
            //            loadCourtneyTangoSongNumberOneOrnaments();
            
            selectedSong = TangoEntity::SongIDs::LA_CUMPARSITA;
            std::cout << "Loaded La Cumparsita\n";
        };
        
        virtual void loadElChocloIntoPlayer()
        {
            destroyCurSong();
            if( couples.size() <= 0 )
            {
                std::cout << "Warning! No couples on dance floor. Cannot load song! ";
                return;
            }
            
            Song *song = loader.getSong(TangoEntity::SongIDs::EL_CHOCLO);
            melody = new MainMelodySectionNDB( mTimer, melodyOnsetDancer->getOnsets() , &instruments, song ); //so far only FOLLOWER... change?
            ((MainMelodySectionNDB *)melody)->timesToRepeatSectionElChoclo(); //this has the same song structure... altho may add some alt. melodies
            accompaniment = new AccompanimentSectionNDB(mTimer, &instruments, song, (MainMelodySectionNDB *) melody  );  //add back when accompaniment ndb  is done
            
            for( int i=0; i<couples.size(); i++ )
                couples[i]->setMotionAnalysisParamsForSong(TangoEntity::SongIDs::FRAGMENTS);
            
            loadSong();
            //            loadCourtneyTangoSongNumberOneOrnaments();
            
            selectedSong = TangoEntity::SongIDs::EL_CHOCLO;
            std::cout << "Loaded El Choclo\n";
        };
        
        //this assumes only 1 couple -- or at the least that the 1st couple will have the responsibility of sending signals.
        virtual void loadSong()
        {
            melody->addSchema( mMappingSchemas[MELODY_BS] );  //not using to choose melodies at the MOMENT //choosing again 8/2014
            mMappingSchemas[MELODY_BS]->setName("Melody Busy Sparse");
            melody->addSchema( mMappingSchemas[MELODY_INSTRUMENT_PR] );
//            ((MainMelodySectionNDB *)melody)->setSendSignals(couples[0]->getSendSignalsFollower());
            ((MainMelodySectionNDB *) melody)->setPVC((PointyVsSmoothContinuous *) mMappingSchemas[CONT_MELODY_PC]); //set staccato/legato mapping from motion

            
            ((MainMelodySectionNDB *) melody)->initSection(); //uses a mapping schema to choose an instrument, so must init this after gets mapping params
            
            // accompaniment->addSchema( mMappingSchemas[THICK_THIN] ); //7-29 deleting for now... busy/spare more accurate... I think...

            //TODO: add back when accompaniment NDB is implemented
            accompaniment->addSchema( mMappingSchemas[ACCOMPANIMENT_BS] );
            mMappingSchemas[ACCOMPANIMENT_BS]->setName("Leader Busy Sparse");
            accompaniment->addSchema( mMappingSchemas[ACCOMPANIMENT_INSTRUMENT_PR] );
//            ((AccompanimentSectionNDB *)accompaniment)->setSendSignals(couples[0]->getSendSignalsLeader());

            
            ( ( AccompanimentSectionNDB * )accompaniment )->initSection();
            
            //set to whatever it says in the song database
            ( (PerceptualEvent *) mMappingSchemas[ACCOMPANIMENT_BS])->setMinMaxMood(0, ((AccompanimentSectionNDB *)accompaniment)->getBVSMax());
            ( (PerceptualEvent *) mMappingSchemas[MELODY_BS])->setMinMaxMood(1, ((MainMelodySectionNDB *)melody)->getBVSMax());


            player->setMainMelody( melody );
            player->addAccompaniment( accompaniment );
        };
        
    };
    
};

#endif /* DancersNDB_h */
