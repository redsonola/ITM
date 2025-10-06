#include "cinder/gl/Vbo.h"
#include "cinder/Camera.h"
#include "cinder/app/App.h"
#include "cinder/ImageIo.h"
#include "cinder/Text.h"
#include "cinder/Vector.h"

#define TRANS_INC 0.5f

class VisualizeShimmer
{
private: 
	std::vector<ci::vec3>	vData;
	ci::vec3				transVector;
	ci::ColorA				color;
	std::string				port;
	ci::TextLayout			txt; 
	ci::vec2				bounds; 

public: 
	VisualizeShimmer();
	virtual void update(std::vector<ShimmerData *> data);
    virtual float updateValue(float value, int which);

	virtual void draw();
	inline void setColor(ci::ColorA c){ color = c; }; 
	void setPort(std::string p)
	{ 
		port = p; 
		std::stringstream ss; ss << "COM " << p; 
		txt.setColor(color);
		txt.setFont(ci::Font("Arial", 10));
        txt.addLine(ss.str());
	};
	inline void setTranslationVector(ci::vec3 tr){transVector = tr;};
	void translateUp(){ transVector.y += TRANS_INC; };
	void translateDown(){ transVector.y -= TRANS_INC; };
	void translateLeft(){ transVector.x -= TRANS_INC; };
	void translateRight(){ transVector.x += TRANS_INC; };
	inline void setBounds(ci::vec2 b){ bounds = b; };
};
VisualizeShimmer::VisualizeShimmer()
{
	color = ci::ColorA(0, 1, 0, 1); //random initial color, not really random, green.
	transVector = ci::vec3(0,0,0);
}
float VisualizeShimmer::updateValue(float value, int which)
{
    if( value != NO_DATA )
    {
        value = value / G_FORCE;
        value = value * 6 - 3;
        value = value + transVector[which];
    }
    return value;
}
void VisualizeShimmer::update(std::vector<ShimmerData *> data)
{
    //init
    ci::vec3 lastPos;
    for(int i=0; i<3; i++)
        lastPos[i] = NO_DATA;
    if(vData.size() > 0)
        lastPos = vData[vData.size()-1];
    vData.clear();
	

    for (int i = 0; i < data.size(); i++)
    {
        ci::vec3 pos = data[i]->getAccelData();
//** handled iphone DataOSC sending separate messages this on the SensorData end as it is more important to be fixed there **
//**Therefore not using this code, but just using previous.
//        for( int j =0; j< 3;j++)
//        {
//            if( pos[j] != NO_DATA )
//                pos[j] = updateValue(pos[j], j);
//            else
//            {
//                if(i == 0)
//                {
//                    pos[j] = lastPos[j]; //it will equal the previous, even if NO_DATA
//                }
//                else if( i > 0 && vData.size() > 0 )
//                {
//                    pos[j] = vData[vData.size()-1][j];
//                }
//            }
//        }
//        vData.push_back(pos);

        if( pos.x != NO_DATA ) //for handling android data
        {
            pos.x = pos.x / G_FORCE;
            pos.y = pos.y / G_FORCE;
            pos.z = pos.z / G_FORCE;

            
            //TODO -- fix constants??
            pos.x = pos.x * 6 - 3;
            pos.y = pos.y * 6 - 3;
            pos.z = pos.z * 6 - 3;
            
            pos = pos + transVector;
            vData.push_back(pos);
        }
	}
}



void VisualizeShimmer::draw()
{
	ci::gl::color(color);
	ci::gl::drawSphere(transVector, 0.05f);
	ci::vec2 pos(transVector.x, transVector.y); pos.y -= 0.1;

	for (int i = 0; i < vData.size(); i++)
	{
		ci::gl::drawSphere(vData[i], 0.1f);
	}

	//ci::gl::setMatricesWindow(bounds);
	//ci::gl::enableAlphaBlending(false);
	//ci::gl::Texture tex = ci::gl::Texture( txt.render(true, false) );
	//pos.x = ((pos.x+1.0)/2.0) * bounds.x;
	//pos.y = ((pos.y + 1.0) / 2.0) * bounds.y;
	//ci::gl::draw(tex, pos);
	//ci::gl::disableAlphaBlending();

}
