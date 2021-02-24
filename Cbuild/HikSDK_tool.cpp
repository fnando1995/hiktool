/*
Compile with:
g++ -std=c++0x -o Cbuild/HikSDK_tool Cbuild/HikSDK_tool.cpp  -lhcnetsdk -lHCCore -lhpr -lHCCoreDevCfg  -L ./Cbuild/lib -L ./Cbuild/lib/HCNetSDKCom
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


int compare_time(int seconds_allowed,LONG lUserID)
{
    //variables needed
    time_t now;
    struct tm real_time;
    struct tm device_time;
    double seconds;
    // getting actual time
    time(&now);  /* get current time; same as: now = time(NULL)  */
    real_time   = *localtime(&now);
    //getting Device time
    NET_DVR_TIME timeParams = { 0 };
    DWORD dwReturnLen;
    int net_time = NET_DVR_GetDVRConfig(lUserID,NET_DVR_GET_TIMECFG,0,&timeParams, sizeof(NET_DVR_TIME), &dwReturnLen);
    // transform device time to compare
    device_time = *localtime(&now);
    device_time.tm_year = timeParams.dwYear-1900;
    device_time.tm_mon = timeParams.dwMonth-1;
    device_time.tm_mday = timeParams.dwDay;
    device_time.tm_hour = timeParams.dwHour;
    device_time.tm_min = timeParams.dwMinute;
    device_time.tm_sec = timeParams.dwSecond;
    // comparing times
    seconds = abs(difftime(mktime(&real_time),mktime(&device_time)));
//    printf("Real time  : %s", asctime(&real_time));
//    printf("Device time: %s", asctime(&device_time));
//    printf ("%.f seconds of difference.\n", seconds);
    if (seconds <= seconds_allowed)
    {
    return 0;
    }
    else
    {
    return seconds;
}
}


int main(int argc, char *argv[]){
    char *host                                            = argv[1];
    int port                                              = atoi(argv[2]);
    char *user                                            = argv[3];
    char *password                                        = argv[4];
    int seconds_allowed                                   = atoi(argv[5]);
    string path                                           = argv[6];
    int numbers_of_channels                               = argc-7;
    int channels[numbers_of_channels];
    for (int i=0;i<numbers_of_channels;i++){
        channels[i]=atoi(argv[7+i]);
    }
    /*
    logeo al DVR/NVR
    */
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
    int time_check = compare_time(seconds_allowed,lUserID);
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
        printf("ERROR,%d,%d,%s,%i\n",time_check,NET_DVR_GetLastError(),host,channels[i]);
      }else{
        printf("COMPLETE,%d,%d,%s,%i\n",time_check,NET_DVR_GetLastError(),host,channels[i]);
      }
    }
    /*
    Limpiando conexion al DVR/NVR
    */
    NET_DVR_Logout(lUserID);
    NET_DVR_Cleanup();
    return 0;

}
