//
//  SensorData.h
//  InteractiveTangoReadFromAndroid
//
//  Created by courtney on 9/11/14.
//
//

#ifndef InteractiveTangoReadFromAndroid_SensorData_h
#define InteractiveTangoReadFromAndroid_SensorData_h

#include "ReadCSV.h"
#include <map>
#include <chrono>
#include <iomanip>


#define SENSORDATA_BUFFER_SIZE 3100 //uhhh yal
#define REAL_TIME_SENSOR 0
#define LOADER_SENSOR 1

class OSCSaveToFile : public ShimmerSaveToFile
{
public:
    
    OSCSaveToFile(std::string f, std::string s ) : ShimmerSaveToFile(f, s)
    {
    };
    
	virtual void save(std::vector<ShimmerData *> data, float seconds, int curNumAdded)
	{
//        std::cout << "saving now: " << seconds << " how many: "  << curNumAdded << "data.size()" << data.size() << "\n";
		std::ofstream myfile(filename, std::ios::app);
        
		for (int i = 0; i < data.size(); i++)
		{

            data[i]->setData(19, seconds);
            
			myfile << dataIndex << ",";
			for (int j = 0; j < SHIMMER_NUM_ARGS+1; j++)
			{
				myfile << data[i]->getData(j);
				myfile << ",";
			}
            for(int j=0; j<4; j++)
            {
                myfile << data[i]->getQuarternion(j) << ",";
            }
            for(int j=0; j<3; j++)
            {
                myfile << data[i]->getOrientationMatrix(j);
                if(j<2) myfile << ",";
            }
			myfile << std::endl;
			dataIndex++;
		}
        
		myfile.close();
	};
    
    //add a marker in a file
    virtual void addMarker(std::string marker, float seconds)
    {
        //add a marker in a file
        std::ofstream myfile(filename, std::ios::app);

        myfile << seconds << "," << marker << std::endl;
        
        myfile.close();

    };
};

class OSCLoadFromFile : public ShimmerLoadFromFile
{
protected:
    float speed;
public:
    OSCLoadFromFile( std::string filename ) : ShimmerLoadFromFile()
    {
        speed = 1;
		index = -1;
        modifier_num = 0;
        looping = -1;
        
		//get the port number
		int findex, gindex = 0;
		findex = filename.find_first_of(".");
		gindex = filename.find(".", findex + 1);
		port = filename.substr( findex + 1, gindex - findex - 1 );
        
		//load from file now
//        ShimmerLoadFromFile::loadFromFile(filename);
//        for( int i=0; i<sData.size(); i++ )
//            sData[i]->setData(0, i);
        
        loadFromFile(filename);
        index = 0;
    };
    
    inline void setSpeed(float s)
    {
        speed = s;
    }
    
    virtual void loadFromFile(std::string filename)
	{
        
        ReadCSV csvReader(filename);
        int csvIndex = 0;
        
        while( !csvReader.eof() )
        {
            std::vector<std::string> tokens = csvReader.getTokensInLine();
            ShimmerData *shimmer = new ShimmerData;
            
            shimmer->setData(0, csvIndex);

            int i;
            for (i = 1; i <= SHIMMER_NUM_ARGS+1; i++) //plus one for cam.
            {
                shimmer->setData(i-1, std::atof(tokens[i].c_str()));
            }
            if( tokens.size() > 20 )
            {
                float quat[4], oVals[3];
                for(int j=0; j<4; j++)
                {
                    quat[j] = std::atof(tokens[i].c_str());
                    i++;
                }
                for(int j=0; j<3; j++)
                {
                    oVals[j] = std::atof(tokens[i].c_str());
                    i++;
                }
                shimmer->setOrientationMatrix(oVals[0], oVals[1], oVals[2]);
                shimmer->setQuarternion(quat[0], quat[1], quat[2], quat[3]);
            }
            sData.push_back(shimmer);
            csvIndex++;
            
        }
        
        //take out beginning chuck while loading...
        float cameraTime = sData[0]->getCameraTimeStamp();
        float cameraTime2 = sData[0]->getCameraTimeStamp();
        while ( sData.size() > 0 && cameraTime == cameraTime2 )
        {
            sData.erase(sData.begin());
            cameraTime2 = sData[0]->getCameraTimeStamp();
        }
        
        csvReader.close();
	};
    
    std::vector<ShimmerData *> getData(float seconds)
	{
		assert(index > -1 && index < sData.size());
        
		std::vector<ShimmerData *> shimmers;
        float stamp = sData[index]->getCameraTimeStamp();
        
        if( ((seconds*speed)+modifier_num) > looping && looping > -1)
        {
            reset();
        }
        
//        std::cout << "------------START---------------\n";
//        std::cout << " index: " << index <<  "stamp: " << stamp << " seconds:" << seconds << "datasize: " << sData.size() <<  std::endl;

        
        while (index < sData.size() && stamp <= ((seconds+modifier_num) ) )
        {
//            std::cout << "cur seconds" << seconds + modifier_num ;
            stamp = sData[index]->getCameraTimeStamp();
//            std::cout << " index: " << index <<  "stamp: " << stamp << std::endl;
            shimmers.push_back(sData[index]);
            index++;
        }
//        std::cout << "------------END---------------\n";

		return shimmers;
	};
    
    virtual int getCurSample()
	{
		return index;
	};
    
    virtual inline bool done()
    {
        return index >= sData.size();
    };

};

class SensorData
{
public:
    enum SensorType { SHIMMER, M5STACK, ANDROID, DATAOSC_IPHONE };
    
    
    SensorData(std::string deviceID, int shimmer, VisualizeShimmer *v = NULL, SensorType sensorType_ = M5STACK)
    {
        setDeviceID(deviceID);
        setWhichShimmer(shimmer);
        setSensorType(sensorType_); // Default to SHIMMER
        saveFile = NULL;
        setVisualizer(v);
        setDancerLimb(0, 0);
        setPareja(0);
        curNumAdded = 0;
            
        // Initialize OSC rate monitoring
        lastRatePrintTime = 0.0;
        messageCount = 0;
        rateUpdateInterval = 2.0; // Print rate every 2 seconds
            
        // Initialize sample rate detection
        actualSampleRate = 37; // Default fallback -- make it with the m5stack
        sampleRateCalculated = false;
        };
    
    inline void setPort(std::string p)
    {
        port = p;
    };
    
    inline std::string getPort()
    {
        return port; 
    };
    
    inline int getWhichSensor()
    {
        //this is returning which sensor it is according to android/shimmer setup
        return whichShimmer; 
    };
    
    inline std::string getDeviceID()
    {
        return mDeviceID;
    };
    
    inline void setDeviceID(std::string idz)
    {
        mDeviceID = idz;
    };
    
    inline void setSensorType(SensorType type)
    {
        sensorType = type;
    };
    
    inline SensorType getSensorType()
    {
        return sensorType;
    };
    
    
    
    bool same( std::string deviceID, int shimmer )
    {
        return ( !deviceID.compare( mDeviceID ) && shimmer == whichShimmer );
    };
    
    inline void setWhichShimmer(int shimmer)
    {
        whichShimmer = shimmer;
    };
    
    void addShimmer( ShimmerData *data )
    {
        mShimmerData.push_back(data);
    };
    
    //for updating messages that don't send all x, y, z values at once
    void addShimmer( ShimmerData *data, int which, int type )
    {
        if(which == -1)
            addShimmer(data);
        else
        {
            if( dataTobeAdded == NULL )
            {
                dataTobeAdded = data;
            }
            else
            {
                dataTobeAdded->setData(which, data->getData(which));
                    
                int startIndex = 2;
                if( type == TYPE_GYR )
                {
                    startIndex = 11;
                }
                else if( type == TYPE_MAG)
                {
                    startIndex = 14;
                }
                bool finished = true;
                int index = startIndex;
                while ( finished && (index < startIndex + 3) )
                {
                    finished = finished && (dataTobeAdded->getData(index) != NO_DATA);
                    index++;
                }
                if(finished)
                {
                    addShimmer(dataTobeAdded);
                    dataTobeAdded = NULL;
//Some prints for debugging
//                    if( type == TYPE_ACCEL )
//                    {
//                        ci::vec3 accel = mShimmerData[mShimmerData.size()-1]->getAccelData();
//                        /std::cout << accel.x << "," << accel.y << "," << accel.z << std::endl;
//                    }
                }
            }
        }
    }
    
    void saveToFile( std::string f )
    {
        std::stringstream ss;
        //filename thus has: deviceID, shimmerID, pareja, dancer, limb information
        ss << mDeviceID << "__" << whichShimmer << "__" << whichPareja << "__" << whichDancer << "__" <<whichLimb;
        std::cout << ss.str() << std::endl;
        
        if(saveFile==NULL)
            delete saveFile; 
            
        saveFile = new OSCSaveToFile( f,  ss.str());
    };
    
    void saveToFile( std::string f, std::string date_header )
    {
        std::stringstream ss;
        //filename thus has: deviceID, shimmerID, pareja, dancer, limb information
        ss << date_header <<  "__" << mDeviceID << "__" << whichShimmer << "__" << whichPareja << "__" << whichDancer << "__" <<whichLimb;
        std::cout << ss.str() << std::endl;
        
        if(saveFile==NULL)
            delete saveFile;
        
        saveFile = new OSCSaveToFile( f,  ss.str());
    };
    
    void markerInFile(std::string marker, float seconds)
    {
        if(saveFile==NULL)
        {
            std::cout << "This sensor is not being saved. Cannot add marker.\n";
        }
        else
        {
            saveFile->addMarker(marker, seconds);
        }
    };
    
    void updateVisualizer()
    {
        if (visualizer != NULL)
            visualizer->update(mShimmerData);
    }
    
    void save(float seconds)
    {
        if(saveFile != NULL)
        {
            saveFile->save( mShimmerData, seconds, curNumAdded);
        }
        mShimmerData.clear();
    };
    
    void eraseData()
    {
        mShimmerData.clear();
    };
    
    inline void setVisualizer(VisualizeShimmer *v){ visualizer = v; };
    
    virtual void update(float seconds)
    {
        updateVisualizer();
        addToBuffer(mShimmerData);
        curNumAdded = mShimmerData.size();

        
//        saveWorkerThread = boost::thread(&SensorData::save, this, seconds);
        save(seconds);
    }; //maybe will use later
    
    virtual inline void start()
    { //dummy
    };
    
    virtual std::vector<ShimmerData *> getBuffer( int bufferSize = 25 )
    {
        if( bufferSize > mBuffer.size() )
        {
            return mBuffer;
        }
        std::vector<ShimmerData *>::iterator startIter = mBuffer.end() - bufferSize;
        std::vector<ShimmerData *>::iterator endIter = mBuffer.end();

        return std::vector<ShimmerData *>( startIter, endIter );
    };
    
    //this gets all the new samples from the buffer
    //then it puts all the samples that have the same time-stamp into one Shimmer data value
//    virtual std::vector<ShimmerData *> getNewIntegratedSamples()
//    {
//    
//    
//    };
    
    virtual inline int getNewSampleCount()
    {
        return curNumAdded;
    };
    
    virtual void resetPlaybackTimer() //TODO
    {
        
    };

    virtual int getType()
    {
        return REAL_TIME_SENSOR;
    };
    
    
    virtual void cleanupBuffer()
    {
        
        if( mBuffer.size() > SENSORDATA_BUFFER_SIZE )
        {
            int numErase = mBuffer.size() - SENSORDATA_BUFFER_SIZE;
            for(int i=0; i<numErase; i++ )
            {
                if( mBuffer[i] != NULL)
                    delete mBuffer[i];
                mBuffer[i] = NULL;
            }
            mBuffer.erase( mBuffer.begin(), mBuffer.begin()+numErase );
        }
    };
    
    void setDancerLimb(int dancer, int limb)
    {
        whichDancer = dancer;
        whichLimb = limb;
    };
    
    void setPareja(int pareja)
    {
        whichPareja = pareja;
    };
    
    void updateOSCRate(double currentTime)
    {
        // Add current timestamp
        oscTimestamps.push_back(currentTime);
        messageCount++;
        
        // Keep only last 100 timestamps to avoid memory issues
        if (oscTimestamps.size() > 100) {
            oscTimestamps.erase(oscTimestamps.begin());
        }
        
        // Calculate actual sample rate if we have enough data
        if (oscTimestamps.size() >= 10 && !sampleRateCalculated) {
            double timeSpan = oscTimestamps.back() - oscTimestamps.front();
            actualSampleRate = (oscTimestamps.size() - 1) / timeSpan;
            sampleRateCalculated = true;
            
            std::cout << "Calculated actual sample rate for " << mDeviceID << "_" << whichShimmer
                      << " (" << getSensorTypeString() << "): "
                      << std::fixed << std::setprecision(2) << actualSampleRate << " Hz" << std::endl;
        }
        
        // Calculate and print rate every rateUpdateInterval seconds
        if (currentTime - lastRatePrintTime >= rateUpdateInterval) {
            printOSCRate();
            lastRatePrintTime = currentTime;
            messageCount = 0;
        }
    }
    
    double getActualSampleRate()
    {
        if (!sampleRateCalculated && oscTimestamps.size() >= 10) {
            double timeSpan = oscTimestamps.back() - oscTimestamps.front();
            actualSampleRate = (oscTimestamps.size() - 1) / timeSpan;
            sampleRateCalculated = true;
        }
        return actualSampleRate;
    }

    void printOSCRate()
    {
        if (oscTimestamps.size() >= 2) {
            double timeSpan = oscTimestamps.back() - oscTimestamps.front();
            double rate = (oscTimestamps.size() - 1) / timeSpan;
            
            std::cout << "OSC Rate for " << mDeviceID << "_" << whichShimmer
                      << " (" << getSensorTypeString() << "): "
                      << std::fixed << std::setprecision(2) << rate
                      << " Hz (Messages: " << messageCount << ")" << std::endl;
        }
    }

    std::string getSensorTypeString()
    {
        switch(sensorType) {
            case SensorData::SensorType::SHIMMER: return "SHIMMER";
            case SensorData::SensorType::M5STACK: return "M5STACK";
            case SensorData::SensorType::ANDROID: return "ANDROID";
            case SensorData::SensorType::DATAOSC_IPHONE: return "IPHONE";
            default: return "UNKNOWN";
        }
    }
    
protected:
    std::vector<ShimmerData *> mShimmerData;
    std::vector<ShimmerData *> mBuffer; //keep a buffer data
    std::string mDeviceID;
    std::string port;
    int whichShimmer;
    OSCSaveToFile *saveFile;
    VisualizeShimmer *visualizer;
    int curNumAdded;
    ShimmerData *dataTobeAdded = NULL;
    SensorType sensorType;
    
    // OSC rate monitoring
    std::vector<double> oscTimestamps;
    double lastRatePrintTime;
    int messageCount;
    double rateUpdateInterval;

    // Sample rate detection
    double actualSampleRate;
    bool sampleRateCalculated;
    
//    boost::thread saveWorkerThread;

    
    int whichDancer;
    int whichPareja;
    int whichLimb;
    
    // BUFFER_SIZE
    virtual void addToBuffer( std::vector<ShimmerData *> data )
    {
        
        mBuffer.reserve( data.size() + mBuffer.size() ); // preallocate memory
        mBuffer.insert( mBuffer.end(), data.begin(), data.end() );

    };

};

class LoadedSensor : public SensorData
{
public:
    
    LoadedSensor(std::string filename, std::string deviceID, int which,
                 VisualizeShimmer *v = NULL) : SensorData(deviceID, which, v)
    {
        oscLoader = new OSCLoadFromFile(filename);
    };
    
    virtual void update(float seconds)
    {
        assert(oscLoader != NULL);
        
		if (oscLoader->started() && !oscLoader->done())
		{
 			std::vector<ShimmerData *> shimmers = oscLoader->getData(seconds);
			for (int i = 0; i < shimmers.size(); i++)
			{
				mShimmerData.push_back(shimmers[i]);
			}
		}
	
        if (visualizer != NULL)
            visualizer->update(mShimmerData);
        
        updateVisualizer();
        addToBuffer(mShimmerData);
        curNumAdded = mShimmerData.size();

    }
    
    virtual void resetPlaybackTimer()
    {
        oscLoader->reset();
    };
    
    virtual void start()
    {
        oscLoader->start();
    }
    
    virtual void stop()
    {
        oscLoader->stop();
    }
    
    virtual void setSpeed(float s)
    {
        oscLoader->setSpeed(s);
    }
    
    virtual int getType()
    {
        return LOADER_SENSOR;
    };

    void setTimeModifierNum(float modTime)
    {
        assert(oscLoader != NULL);
        oscLoader->setTimeModifierNum(modTime);
    };
    
    int getCurSample()
    {
       return oscLoader->getCurSample();
    };
    
    bool done()
    {
        if( oscLoader != NULL )
            return oscLoader->done();
        else return false;
    };
    
protected:
    std::string filename;
    OSCLoadFromFile *oscLoader;
    
};

#endif
