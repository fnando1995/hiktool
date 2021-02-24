/*
Compile with:
g++ -std=c++0x -o Cbuild/Download_jpg Cbuild/Download_jpg.cpp  -lhcnetsdk -lHCCore -lhpr -lHCCoreDevCfg  -L ./Cbuild/lib -L ./Cbuild/lib/HCNetSDKCom
*/
#include <iostream>
#include "HCNetSDK.h"
#include <stdio.h>
#include <cstring>
#include <unistd.h>
#include <stdlib.h>
#include <ctime>
#include <time.h>
#include <string.h>
using namespace std;

int main(int argc, char *argv[]){
    char *host                                            = argv[1];
    int port                                              = atoi(argv[2]);
    char *user                                            = argv[3];
    char *password                                        = argv[4];
    string path                                           = argv[5];
    int numbers_of_channels                               = argc-6;
    int channels[numbers_of_channels];
    for (int i=0;i<numbers_of_channels;i++){
        channels[i]=atoi(argv[6+i]);
    }
    NET_DVR_Init();
    NET_DVR_SetConnectTime(5000, 2);                        //Maximo tiempo conexion 5 segundos 2 intentos
    NET_DVR_SetReconnect(1000, true);                       //Reconexion Habilitada, 1secgundo de intervalo
    NET_DVR_USER_LOGIN_INFO struLoginInfo         = {0};
    struLoginInfo.bUseAsynLogin                   = 0;
    struLoginInfo.wPort                           = port;
    strcpy(struLoginInfo.sDeviceAddress, host);
    strcpy(struLoginInfo.sUserName, user);
    strcpy(struLoginInfo.sPassword, password);
    NET_DVR_DEVICEINFO_V40 struDeviceInfoV40;
    LONG lUserID = NET_DVR_Login_V40(&struLoginInfo, &struDeviceInfoV40);
    if (lUserID < 0){
        printf("ERROR,X,%d,%s,%s\n",NET_DVR_GetLastError(),host,"all-login");  // "X" for time check not able to perform
        NET_DVR_Cleanup();
        return -1;
    }
    /*
    Iterando sobre los canales del DVR/NVR
    */
    for( int i=0;i<numbers_of_channels;i++){
      LPNET_DVR_JPEGPARA JpegPara = new NET_DVR_JPEGPARA;
      JpegPara->wPicQuality = 0;
      JpegPara->wPicSize = 0;
      string filename = path + std::to_string(channels[i]) +".jpg";
      char *cstr = new char[filename.length() + 1];
      strcpy(cstr, filename.c_str());
      bool Capture = NET_DVR_CaptureJPEGPicture(lUserID,channels[i],JpegPara,cstr);
      if(!Capture){
        printf("ERROR,%d,%s,%i\n",NET_DVR_GetLastError(),host,channels[i]);
      }else{
        printf("COMPLETE,%d,%s,%i\n",NET_DVR_GetLastError(),host,channels[i]);
      }
    }
    NET_DVR_Logout(lUserID);
    NET_DVR_Cleanup();
    return 0;

}
