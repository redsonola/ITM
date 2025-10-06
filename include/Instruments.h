//
//  Instruments.h
//  InteractiveTangoReadFromAndroid
//
//  Created by courtney on 7/28/16.
//
//

#ifndef InteractiveTangoReadFromAndroid_Instruments_h
#define InteractiveTangoReadFromAndroid_Instruments_h

#endif

#include <memory>
#include <cmath>

namespace InteractiveTango
{
    
    //files for instruments
#define INSTRUMENTS_FILE "/Users/courtney/Programs/cinder_0.9.2_mac/InteractiveTangoMilongaNewDB/ITM_Database/NDB/Instruments.csv"


class Instrument
{
protected:
    int instrumentID;
    std::string nameStr;
    std::vector<float> profile; //set of attributes about the instrument. currently we have pointy/smooth
    int melTrackID; //which track in ableton is this instrument rounted through?
    int melMidiCh; //which midi channel is the melody routed through
    int accompMidiCh; //which midi channel the accomp chaneel is routed through
    int midiNoteLow; //lowest note this instrument can play
    int midiNoteHigh; //highest note this instrument can play
    
public:
    
    Instrument(int instrID, std::string _name)
    {
        instrumentID = instrID;
        nameStr = _name;
    };
    
    void setName(std::string _name)
    {
        nameStr = _name;
    }
    
    void addProfile(float profile_)
    {
        profile.push_back( profile_ );
    };
    
    void setMelTrackID(int id_)
    {
        melTrackID = id_;
    };
    
    void setLowNote(int note)
    {
        midiNoteLow = note;
    }
    
    void setHighNote(int note)
    {
        midiNoteHigh = note;
    }
    
    int getHighNote()
    {
        return midiNoteHigh;
    }
    
    int getLowNote()
    {
        return midiNoteLow;
    }
    
    //put the note in range if out, shifts an 8va
    int putInRange(int note)
    {
        while(note > midiNoteHigh){
            note-=12;
        }
        while(note < midiNoteLow )
            note+=12;
        return note; 
    }
    
    void setAccompMidiCh(int ch)
    {
        accompMidiCh = ch;
    };
    
    void setMelMidiCh(int mel)
    {
        melMidiCh = mel;
    }
    
    int getTrackID()
    {
        return melTrackID;
    };
    
    int getMelodyMidiCh()
    {
        return melMidiCh;
    };
    
    int getAccompMidiCh()
    {
        return accompMidiCh;
    };
    
    int getInstrumentID()
    {
        return instrumentID;
    };
    
    bool fitsProfile( std::vector<float> _profile )
    {
        assert( _profile.size() == profile.size());
        bool fits = true;
        int index = 0;
        while ( fits && index < profile.size() )
        {
            fits = profile[index] == _profile[index];
            index++;
        }
        
        return fits;
    };
    
    bool fitsProfile( int p)
    {
        assert( _profile.size() == 1);

        return profile[0] == p;
    }
    
    int getProfile(int i)
    {
        assert( i < profile.size() && i >= 0 );
        return profile[i];
    };
    
    int getProfileSize(){ return profile.size(); };
    
    std::string getName(){ return nameStr; };
};
    
class Orchestra
{
protected:
    std::vector<int> profile;
    std::vector<Instrument *> instruments;
public:
    Orchestra(){};
    void addInstrument( Instrument *instr  )
    {
        instruments.push_back(instr);
        updateProfile();
        
        // Check that all instruments have consistent profile sizes after updating
        for (int i = 0; i < instruments.size(); i++) {
            assert(profile.size() == instruments[i]->getProfileSize());
        }
    };
    
    void removeInstrument(int index)
    {
        if( index < instruments.size() )
            instruments.erase( instruments.begin() + index );
        else
            std::cerr << "Warning! Tried to erase an instrument at " << index << " which does not exist!\n";
     }
        
    void updateProfile()
    {
        int profileSize = profile.size();
        if( profileSize == 0 )
        {
            for( int i=0; i<instruments[0]->getProfileSize(); i++ )
            {
                profile.push_back( instruments[0]->getProfile(i) );
            }
        }
        else
        {
            
            profile.clear();
            for( int i=0; i<profileSize; i++ )
            {
                profile.push_back(0);
                for( int j=0; j<instruments.size(); j++ )
                {
                    profile[i] +=  instruments[j]->getProfile(i);
                }
                profile[i] = std::round(double(profile[i] / double(instruments.size())));
            }
        }
    }
    
    //this fudges for a middle value bc instruments are either min or max, so one instrument responding to a middle value can choose any one solo instrument
    //FOR NOW!
    bool fitsProfile( std::shared_ptr<std::vector<float>> profile_, bool fudgeMiddleIfOnlyOneInstrument = true, int minP = 1, int maxP = 3 )
    {
        assert( profile_->size() == profile.size() );
        int index = 0;
        bool fits = true;
        if( instruments.size() > 1 || (!fudgeMiddleIfOnlyOneInstrument)  )
        {
            while ( fits && index < profile.size() )
            {
                fits = profile[index] == profile_->at(index);
                index++;
            }
        }
        else
        {
            int middleVal = std::round(double(( minP + maxP ))/ 2.0);
            while ( fits && index < profile.size() )
            {
                fits = ( profile[index] == profile_->at(index) ) || ( profile_->at(index) == middleVal );
                index++;
            }
        }
        
        return fits;
    };
    
    bool fitsProfileExcludingTHESEInstruments(Orchestra *orch, std::shared_ptr<std::vector<float>> profile_, bool fudgeMiddleIfOnlyOneInstrument = true, int minP = 1, int maxP = 3  )
    {
        assert( profile_->size() == profile.size() );
        bool fits = true;
        
        //if the only instruments are the ones excluded, say that it fits, defacto. Of course, this shouldn't happen at this point
        if( size() == orch->size() )
        {
            std::cout << "Warning! Tried to for profile fit in orchestration but only have excluded instruments!\n";
            return fits;
        }
        
        //find the profile of instruments w/o the ones excluded
        std::vector<int> profileForCheck;
        double instrSize=0;
        for(int j=0; j<profile_->size(); j++)
        {
            profileForCheck.push_back(0);
            for(int i=0; i< size(); i++)
            {
                if( !orch->includesInstrument(instruments[i]) )
                {
                    profileForCheck[j] += instruments[i]->getProfile(j);
                    instrSize++;
                }
            }
            profileForCheck[j] = std::round(double(profileForCheck[j]) / instrSize);
        }
        
        //compare that profile with profile given
        int index = 0;
        if( instrSize > 1 || !fudgeMiddleIfOnlyOneInstrument  )
        {
            while ( fits && index < profileForCheck.size() )
            {
                fits = profileForCheck[index] == profile_->at(index);
                index++;
            }
        }
        else
        {
            int middleVal = std::round(double(( minP + maxP ))/ 2.0);
            while ( fits && index < profileForCheck.size() )
            {
                fits = ( profileForCheck[index] == profile_->at(index) ) || ( profile_->at(index) == middleVal );
                index++;
            }
        }
        
        return fits;
    };
    
    float percentThisInstrument( int instrumentID )
    {
        float count = 0;
        for(int i=0; i<instruments.size(); i++)
        {
            if( instruments[i]->getInstrumentID() == instrumentID )
            {
                count++;
            }
        }
        return count / (float) instruments.size();
    }
    
    float percentThisInstrumentPlusOneMore( int instrumentID )
    {
        float count = 1;
        for(int i=0; i<instruments.size(); i++)
        {
            if( instruments[i]->getInstrumentID() == instrumentID )
            {
                count++;
            }
        }
        return count / (float) instruments.size();
    }

    
    bool includesInstrument( int instrumentID )
    {
        return getInstrument(instrumentID) != NULL;
    };
    
    bool includesInstrument(Instrument *ins)
    {
        return includesInstrument(ins->getInstrumentID());
    };
        
    virtual Instrument *getInstrument(int id_)
    {
        Instrument *instr =  NULL;
        int index = 0;
        
        while( index < instruments.size() && instr == NULL )
        {
            if( instruments[index]->getInstrumentID() == id_ ) instr = instruments[index];
            index++;
        }
        
        return instr;
    }
        
    size_t size()
    {
        return instruments.size();
    };
    
    Instrument *getInstrViaIndex(int index)
    {
        assert( index < size() && index >= 0 );
        if(!( index < size() && index >= 0 ))
            std::cout << "error that the assert didn't stop: " << index << "is out of bounds. line 303 Instruments.h\n >_<\n";
        return instruments[index];
    };
    
    void clear()
    {
        instruments.clear();
        profile.clear();
    };
    
    void addfromOtherOrchestra(Orchestra *orch)
    {
        if( orch == NULL ) return;
        for(int i=0; i<orch->size(); i++)
        {
            if( !includesInstrument( orch->getInstrViaIndex(i)->getInstrumentID() ) )
                addInstrument( orch->getInstrViaIndex(i) ) ;
        }
    };
        
        
    bool includesOrchestration( Orchestra *orch )
    {
        bool hasOrch = false;
        int index = 0;
        while ( !hasOrch && ( index < orch->size() ) )
        {
            hasOrch = includesInstrument(  orch->getInstrViaIndex(index)->getInstrumentID() );
            index++;
        }
        return hasOrch;
    };
        
        
};
    
class InstrumentLoader
{
protected:
    std::vector<Instrument *> instruments;
public:
    virtual std::vector<Instrument *> load()=0;
};
    
    
class Instruments
{
protected:
    std::vector<Instrument *> instruments;
    InstrumentLoader *loader;
public:
    Instruments(InstrumentLoader * l)
    {
        loader = l;
        instruments = loader->load();
    };
    
    //TODO: write deconstructor
    
    void addInstrument(Instrument * instr)
    {
        instruments.push_back(instr);
    };
    
    size_t size()
    {
        return instruments.size(); 
    }
    
    virtual Instrument *getInstrument(int id_)
    {
        Instrument *instr =  NULL;
        int index = 0;
        
        while( index < instruments.size() && instr == NULL )
        {
            if( instruments[index]->getInstrumentID() == id_ ) instr = instruments[index];
            index++;
        }
        
        return instr;
    }
    
    virtual Instrument *getInstrumentViaIndex(int i)
    {
        return instruments[i];
    }
    
    std::vector<int> instrumentsWithProfile( std::shared_ptr<std::vector<float>> profile_ )
    {
        std::vector<int> instr;
        for( int i=0; i<instruments.size(); i++ )
        {
            if( instruments[i]->fitsProfile( *(profile_.get()) ) )
            {
                instr.push_back( instruments[i]->getInstrumentID() );
            }
        }
        return instr;
    };
    
    std::vector<int> instrumentsWithProfile( float profile_)
    {
        std::vector<int> instr;
        
        for( int i=0; i<instruments.size(); i++ )
        {
            if( instruments[i]->fitsProfile( profile_) )
            {
                instr.push_back( instruments[i]->getInstrumentID() );
            }
        }
        return instr;
    };
    
    Orchestra orchestraWithProfile( std::shared_ptr<std::vector<float>> profile_ )
    {
        Orchestra orch;
        std::vector<int> instr = instrumentsWithProfile(profile_);
        for( int i=0; i<instr.size(); i++ )
        {
            orch.addInstrument( getInstrument(instr[i]));
        }
        return orch;
    };
    
    //TODO: create an orchestra of a specific size.... using different instruments...
    //TODO: profile is also only 1
    //keeps the 1st instrument of the current orchestra
    //currently the profile just changes what it adds & doesn't take into account the entire "profile"
    Orchestra orchestraWithProfileKeepOne( std::shared_ptr<std::vector<float>> profile_, int instrSize,
                                   OrchestralEvent *orchPerceptualEvent,
                                   Orchestra *startOrch=NULL) //for adding to an existing orchestra
    {
        Orchestra orch;

        if( startOrch != NULL )
            orch = *startOrch;
        
        //remove all but first instrument
        for( int i=orch.size()-1; i>0; i-- )
        {
            orch.removeInstrument( i );
        }
        return orchestraWithProfile(profile_, instrSize, orchPerceptualEvent, &orch);
    }
    
    //TODO: create an orchestra of a specific size.... using different instruments...
    //TODO: profile is also only 1
    Orchestra orchestraWithProfile( std::shared_ptr<std::vector<float>> profile_, int instrSize,
                                   OrchestralEvent *orchPerceptualEvent,
                                   Orchestra *startOrch=NULL) //for adding to an existing orchestra
    {
        assert(profile_->size() == 1); //this code will not work otherwise
        Orchestra orch;
        
        if( startOrch != NULL ){
            orch = *startOrch;
            instrSize = instrSize-orch.size();
//            std::cout << "curOrch.size(): " << orch.size() << std::endl;

        }
        
        if(instrSize<=0)
            return orch; 
        
        float orchPrecept = profile_->at(0);
        
//        std::cout << "orchPrecept: " << orchPrecept << std::endl;
//        std::cout << "orchPrecept: " << orchPrecept << std::endl;
//        std::cout << "orchPrecept: " << orchPrecept << std::endl;

        int numberPointyInstruments = std::round(orchPrecept * (float) instrSize);
        
        int numberOfSmoothInstruments = instrSize - numberPointyInstruments;
        
        //get pointy instruments
        std::vector<int> pointyInstruments = instrumentsWithProfile(orchPerceptualEvent->getMaxMood());

        int index = 0;
        if(pointyInstruments.size() > 0)
            index = rand() % pointyInstruments.size();  //choose randomly where to start
        for(int i=0; i<numberPointyInstruments; i++)
        {
            orch.addInstrument( getInstrument(pointyInstruments[index] ) );
            if( index < pointyInstruments.size()-1 )
                index++;
            else index=0;
        }
        
        //get smooth instruments
        std::vector<int> smoothInstruments = instrumentsWithProfile(orchPerceptualEvent->getMinMood());
        if(smoothInstruments.size() > 0)
            index = rand() % smoothInstruments.size();  //choose randomly where to start
        for(int i=0; i<numberOfSmoothInstruments; i++)
        {
            orch.addInstrument( getInstrument(smoothInstruments[index] ) );
            if( index < smoothInstruments.size()-1 )
                index++;
            else index=0;
        }
        
//        std::cout << "orch.size(): " << orch.size() ;
//        std::cout << "  instrSize: " << instrSize << std::endl;

        
        return orch;
    };
};
    
    
    
class TangoInstrumentsLoader : public InstrumentLoader
    {
    public:
        
        TangoInstrumentsLoader( std::string instrumentFile_ )
        {
            instrumentFile = instrumentFile_;
        };
        
        
        virtual std::vector<Instrument *> load()
        {
            bool first = true;
            readInstrumentFile.init(instrumentFile); 
            
            while( !readInstrumentFile.eof() )
            {
                std::vector<std::string> tokens = readInstrumentFile.getTokensInLine();
                
                if( first )
                {
                    first = false;
                }
                else if(tokens.size() != 0)
                {
                    int instrID = std::atoi(tokens[0].c_str());
                    std::string n = tokens[1];
                    Instrument *instr = new Instrument(instrID, n);
                    instr->setName(tokens[1]);
                    instr->setMelTrackID(std::atoi(tokens[2].c_str()));
                    instr->addProfile( std::atof(tokens[3].c_str()) );
                    instr->setMelMidiCh(std::atoi(tokens[4].c_str()) );
                    instr->setAccompMidiCh(std::atoi(tokens[5].c_str()) );
                    instr->setLowNote( std::atoi(tokens[6].c_str()) );
                    instr->setHighNote( std::atoi(tokens[7].c_str()) );

                    
//                    int midiNoteLow; //lowest note this instrument can play
//                    int midiNoteHigh; //highest note this instrument can play

                    
                    instruments.push_back(instr);
                }
            }
            
            return instruments;
            
        };
    protected:
        std::string instrumentFile;
        ReadCSV readInstrumentFile;
    };
    
    class TangoInstruments : public Instruments
    {
    public:
        TangoInstruments() : Instruments( new TangoInstrumentsLoader(INSTRUMENTS_FILE) )
        {
            
        };
        
        
    };
    
    


};
