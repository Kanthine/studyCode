//
//  TextureObj.cpp
//  ParticlePlayground
//
//  Created by 苏莫离 on 2019/5/15.
//

#include "TextureObj.hpp"
#include <math.h>
#include <vector>

#include "ShaderTool.h"
#include "GLMatrixState.hpp"
#import "GLResourceManager.h"


TextureObj::TextureObj(int vCount, float *vertices,float *normals,float *texCoors) {
    this -> vCount = vCount / 3.0;
    this -> mVertexBuffer = vertices;
    this -> mTexCoorBuffer = texCoors;
}

void TextureObj::initShader() {
    mProgram = ShaderTool::createProgram("VertexShader_brazier", "FragmentShader_brazier");
    muMVPMatrixHandle = glGetUniformLocation(mProgram, "uMVPMatrix");
    muMMatrixHandle = glGetUniformLocation(mProgram, "uMMatrix");
    maLightLocationHandle = glGetUniformLocation(mProgram, "uLightLocation");
    maCameraHandle = glGetUniformLocation(mProgram, "uCamera");
    maPositionHandle = glGetAttribLocation(mProgram, "aPosition");
    maTexCoorHandle = glGetAttribLocation(mProgram, "aTexCoor");
}

void TextureObj::drawSelf(GLuint texId) {
    glUseProgram(mProgram);//指定使用某套着色器程序
    
    glUniformMatrix4fv(muMVPMatrixHandle, 1, GL_FALSE, GLMatrixState::getFinalMatrix());
    glUniformMatrix4fv(muMMatrixHandle, 1, GL_FALSE, GLMatrixState::getMMatrix());
    glUniform3fv(maLightLocationHandle, 1, GLMatrixState::lightPositionFB);
    glUniform3fv(maCameraHandle, 1, GLMatrixState::cameraFB);
    glVertexAttribPointer(maPositionHandle,3, GL_FLOAT, GL_FALSE,3 * sizeof(float), mVertexBuffer);
    glVertexAttribPointer(maTexCoorHandle,2, GL_FLOAT, GL_FALSE,2 * sizeof(float), mTexCoorBuffer);
    
    glEnableVertexAttribArray(maPositionHandle); //启用顶点位置数据数组
    glEnableVertexAttribArray(maTexCoorHandle);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texId);
    glDrawArrays(GL_TRIANGLES, 0, vCount);
}


//将字符串分解为字符数组
size_t TextureLoader::splitString(const string& strSrc, const string& strDelims, vector<string>& strDest) {
    string delims = strDelims;
    string STR;
    if(delims.empty()) delims = " **";
    
    string::size_type pos=0;
    string::size_type LEN = strSrc.size();
    while(pos < LEN ){
        STR="";
        while( (delims.find(strSrc[pos]) != std::string::npos) && (pos < LEN) )
        {
            ++pos;
        }
        if(pos==LEN) {
            return strDest.size();
        }
        while( (delims.find(strSrc[pos]) == std::string::npos) && (pos < LEN) )
        {
            STR += strSrc[pos++];
        }
        if( ! STR.empty() )
        {
            strDest.push_back(STR);
        }
    }
    return strDest.size();
}

bool TextureLoader::tryParseDouble(const char *s, const char *s_end, double *result) {
    if (s >= s_end)
    {
        return false;
    }
    
    double mantissa = 0.0;
    int exponent = 0;
    
    char sign = '+';
    char exp_sign = '+';
    char const *curr = s;
    
    int read = 0;
    bool end_not_reached = false;
    if (*curr == '+' || *curr == '-')
    {
        sign = *curr;
        curr++;
    }
    else if (isdigit(*curr)) { /* Pass through. */ }
    else
    {
        goto fail;
    }
    
    while ((end_not_reached = (curr != s_end)) && isdigit(*curr))
    {
        mantissa *= 10;
        mantissa += static_cast<int>(*curr - 0x30);
        curr++;    read++;
    }
    
    if (read == 0)
        goto fail;
    if (!end_not_reached)
        goto assemble;
    
    if (*curr == '.')
    {
        curr++;
        read = 1;
        while ((end_not_reached = (curr != s_end)) && isdigit(*curr))
        {
            mantissa += static_cast<int>(*curr - 0x30) * pow(10.0, -read);
            read++; curr++;
        }
    }
    else if (*curr == 'e' || *curr == 'E') {}
    else
    {
        goto assemble;
    }
    
    if (!end_not_reached)
        goto assemble;
    
    if (*curr == 'e' || *curr == 'E')
    {
        curr++;
        if ((end_not_reached = (curr != s_end)) && (*curr == '+' || *curr == '-'))
        {
            exp_sign = *curr;
            curr++;
        }
        else if (isdigit(*curr)) { /* Pass through. */ }
        else
        {
            goto fail;
        }
        
        read = 0;
        while ((end_not_reached = (curr != s_end)) && isdigit(*curr))
        {
            exponent *= 10;
            exponent += static_cast<int>(*curr - 0x30);
            curr++;    read++;
        }
        exponent *= (exp_sign == '+'? 1 : -1);
        if (read == 0)
            goto fail;
    }
    
    assemble:
        *result = (sign == '+'? 1 : -1) * ldexp(mantissa * pow(5.0, exponent), exponent);
        return true;
    fail:
        return false;
}

float TextureLoader::parseFloat(const char* token) {
    token += strspn(token, " \t");
    const char *end = token + strcspn(token, " \t\r");
    double val = 0.0;
    tryParseDouble(token, end, &val);
    float f = static_cast<float>(val);
    return f;
}

int TextureLoader::parseInt(const char *token) {
    token += strspn(token, " \t");
    int i = atoi(token);
    return i;
}

TextureObj * TextureLoader::loadTextureFromObjFile(const std::string &vname) {
    TextureObj* lo;
    //原始顶点坐标列表--直接从obj文件中加载
    vector<float> alv;
    //结果顶点坐标列表--按面组织好
    vector<float> alvResult;
    //原始纹理坐标列表
    vector<float> alt;
    //结果纹理坐标列表
    vector<float> altResult;
    //原始法向量列表
    vector<float> aln;
    //法向量结果列表
    vector<float>alnResult;
    
    try{
        //获取obj的内容字符串
        string objSource = GLResourceManager::loadObjScript(vname);
        
        vector<string> vectorContents;//记录切分后的每一行的字符串
        string delimsContents="\r\n";//分隔符
        splitString(objSource, delimsContents,vectorContents); //将字符串按行切为字符串数组
        size_t length=vectorContents.size();//记录行数
    
        vector<string> splitStrs; //把一行数据分割后的字符串存在vector里面
        vector<string> splitStrsF; //把面数据分割后的字符串存在vector里面
        
        for(int countI=0;countI<length;countI++) {//遍历每一行
            string tempContents=vectorContents[countI];
            if(tempContents==""){
                continue;
            }
            string delims ="[ ]+";//分割字符
            splitStrs.clear();
            TextureLoader::splitString(tempContents,delims, splitStrs); //调用自定义的分割函数
            if(splitStrs[0]=="v") {//顶点坐标行
                //若为顶点坐标行则提取出此顶点的XYZ坐标添加到原始顶点坐标列表中
                alv.push_back(parseFloat(splitStrs[1].c_str()));
                alv.push_back(parseFloat(splitStrs[2].c_str()));
                alv.push_back(parseFloat(splitStrs[3].c_str()));
            } else if(splitStrs[0]=="vt") { //纹理坐标行
                alt.push_back(parseFloat(splitStrs[1].c_str()));
                alt.push_back(1-parseFloat(splitStrs[2].c_str()));
            } else if(splitStrs[0]=="vn") { //此行为法向量行
                //若为纹理坐标行则提取ST坐标并添加进原始纹理坐标列表中
                aln.push_back(parseFloat(splitStrs[1].c_str()));//放进aln列表中
                aln.push_back(parseFloat(splitStrs[2].c_str())); //放进aln列表中
                aln.push_back(parseFloat(splitStrs[3].c_str())); //放进aln列表中
            } else if(splitStrs[0]=="f") { //面数据行
                int index[3];//三个顶点索引值的数组
                //计算第0个顶点的索引，并获取此顶点的XYZ三个坐标
                string delimsF ="/";//分割字符
                splitStrsF.clear();
                splitString(splitStrs[1].c_str(),delimsF,splitStrsF);
                
                /// 顶点坐标
                index[0]=parseInt(splitStrsF[0].c_str())-1;
                float x0=alv[3*index[0]];
                float y0=alv[3*index[0]+1];
                float z0=alv[3*index[0]+2];
                
                alvResult.push_back(x0);
                alvResult.push_back(y0);
                alvResult.push_back(z0);
                
                //纹理坐标
                int indexTex=parseInt(splitStrsF[1].c_str())-1;
                //第0个顶点的纹理坐标
                altResult.push_back(alt[indexTex*2]);
                altResult.push_back(alt[indexTex*2+1]);
                
                //计算第0个顶点的法向量索引
                int indexN=parseInt(splitStrsF[2].c_str())-1;//获取法向量编号
                if (indexN >= 0) {
                    float nx0=aln[3*indexN];//获取法向量的x值
                    float ny0=aln[3*indexN+1];//获取法向量的y值
                    float nz0=aln[3*indexN+2];//获取法向量的z值
                    alnResult.push_back(nx0);//放入alnResult列表
                    alnResult.push_back(ny0);//放入alnResult列表
                    alnResult.push_back(nz0);    //放入alnResult列表
                }

                
                //计算第1个顶点的索引，并获取此顶点的XYZ三个坐标
                splitStrsF.clear();
                splitString(splitStrs[2].c_str(),delimsF,splitStrsF);//tempsa[2]
                index[1]=parseInt(splitStrsF[0].c_str())-1;
                float x1=alv[3*index[1]];
                float y1=alv[3*index[1]+1];
                float z1=alv[3*index[1]+2];
                
                alvResult.push_back(x1);
                alvResult.push_back(y1);
                alvResult.push_back(z1);
                
                
                //获取纹理坐标编号并计算第1个顶点的纹理坐标
                indexTex=parseInt(splitStrsF[1].c_str())-1;
                //第1个顶点的纹理坐标
                altResult.push_back(alt[indexTex*2]);
                altResult.push_back(alt[indexTex*2+1]);
                
                //计算第1个顶点的索引，并获取此顶点的XYZ三个坐标
                indexN=parseInt(splitStrsF[2].c_str())-1;
                if (indexN >= 0) {
                    float nx1=aln[3*indexN];
                    float ny1=aln[3*indexN+1];
                    float nz1=aln[3*indexN+2];
                    alnResult.push_back(nx1);
                    alnResult.push_back(ny1);
                    alnResult.push_back(nz1);
                }
   
                
                //计算第2个顶点的索引，并获取此顶点的XYZ三个坐标
                splitStrsF.clear();
                splitString(splitStrs[3].c_str(),delimsF,splitStrsF);//tempsa[3]
                index[2]=parseInt(splitStrsF[0].c_str())-1;
                float x2=alv[3*index[2]];
                float y2=alv[3*index[2]+1];
                float z2=alv[3*index[2]+2];
                alvResult.push_back(x2);
                alvResult.push_back(y2);
                alvResult.push_back(z2);
                
                //获取纹理坐标编号并计算第2个顶点的纹理坐标
                indexTex=parseInt(splitStrsF[1].c_str())-1;
                //第2个顶点的纹理坐标
                altResult.push_back(alt[indexTex*2]);
                altResult.push_back(alt[indexTex*2+1]);
                
                //计算第2个顶点的索引，并获取此顶点的XYZ三个坐标
                indexN=parseInt(splitStrsF[2].c_str())-1;
                if (indexN >= 0) {
                    float nx2=aln[3*indexN];
                    float ny2=aln[3*indexN+1];
                    float nz2=aln[3*indexN+2];
                    alnResult.push_back(nx2);
                    alnResult.push_back(ny2);
                    alnResult.push_back(nz2);
                }
 
            }
            
            splitStrs.clear();
            //读完一行
        }
        
        int count = 0;
        size_t size = alvResult.size(); // 生成顶点数组
        float* vXYZ = new float[size];
        for(int i = 0; i < size; i++){
            count++;
            vXYZ[i]=alvResult[i];
        }
        
        size = altResult.size(); //生成纹理数组
        float* tST = new float[size];//用于存放结果纹理坐标数据的数组
        for(int i=0;i<size;i++) { //将纹理坐标数据存入数组
            tST[i]=altResult[i];
        }
        
        //生成法向量数组
        size=alnResult.size();//获取法向量列表的大小
        float* nXYZ = new float[size];//创建存放法向量的数组
        for(int i=0;i<size;i++) {
            nXYZ[i] = alnResult[i];//将法向量值存入数组
        }
        //创建加载物体对象
        lo = new TextureObj(count, vXYZ, nXYZ, tST);
    
    } catch(exception e) {
        cout<<"抛出异常:"<<e.what()<<endl;
    }
    return lo;
}
