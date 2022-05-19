#ifndef LoadResourceUtil_h
#define LoadResourceUtil_h

using namespace std;

class LoadResourceUtil
{
public:
    static GLuint initTexture(const string& name,bool repeat);
    static string loadShaderScript(const string& name);
    static string loadObjScript(const string& name);
    static void loadMusic(const string& name,const string& type,const int loop);
    static void playMusic();
    static void pauseMusic();
    static void playSound(const string &name,const string& type);
    
};

#endif
