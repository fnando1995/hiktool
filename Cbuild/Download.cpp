/*
Compile with:
g++ -o Cbuild/Download Cbuild/Download.cpp  -lhcnetsdk -lHCCore -lhpr -lHCCoreDevCfg  -L ./Cbuild/lib -L ./Cbuild/lib/HCNetSDKCom
*/

#include <iostream>
#include "HCNetSDK.h"
#include <stdio.h>
#include <cstring>
#include <unistd.h>
#include <stdlib.h>
#include <ctime>
#include <time.h>
using namespace std;


char *substr(char *s, int a, int b) {
    char *r = (char*)malloc(b);
    strcpy(r, "");
    int m=0, n=0;
    while(s[n]!='\0')
    {
        if ( n>=a && m<b ){
            r[m] = s[n];
            m++;
        }
        n++;
    }
    r[m]='\0';
    return r;
}

int main(int argc, char *argv[])
{
  char *host                                            = argv[1];
  int port                                              = atoi(argv[2]);
  char *user                                            = argv[3];
  char *password                                        = argv[4];
  DWORD channel                                         = atoi(argv[5]);
  char *path                                            = argv[6];
  NET_DVR_PLAYCOND struDownloadCond                     = {0};
  struDownloadCond.dwChannel                            = channel;
  struDownloadCond.struStartTime.dwYear                 = atoi(substr(argv[7],1,4));
  struDownloadCond.struStartTime.dwMonth                = atoi(substr(argv[8],0,2));
  struDownloadCond.struStartTime.dwDay                  = atoi(substr(argv[9],0,2));
  struDownloadCond.struStartTime.dwHour                 = atoi(substr(argv[10],0,2));
  struDownloadCond.struStartTime.dwMinute               = atoi(substr(argv[11],0,2));
  struDownloadCond.struStartTime.dwSecond               = atoi(substr(argv[12],0,2));
  struDownloadCond.struStopTime.dwYear                  = atoi(substr(argv[13],1,4));
  struDownloadCond.struStopTime.dwMonth                 = atoi(substr(argv[14],0,2));
  struDownloadCond.struStopTime.dwDay                   = atoi(substr(argv[15],0,2));
  struDownloadCond.struStopTime.dwHour                  = atoi(substr(argv[16],0,2));
  struDownloadCond.struStopTime.dwMinute                = atoi(substr(argv[17],0,2));
  struDownloadCond.struStopTime.dwSecond                = atoi(substr(argv[18],0,2));
  NET_DVR_Init();
  NET_DVR_SetConnectTime(2000, 1);
  NET_DVR_SetReconnect(10000, true);
  NET_DVR_USER_LOGIN_INFO struLoginInfo = {0};
  struLoginInfo.bUseAsynLogin = 0;
  strcpy(struLoginInfo.sDeviceAddress, host);
  struLoginInfo.wPort = port;
  strcpy(struLoginInfo.sUserName, user);
  strcpy(struLoginInfo.sPassword, password);
  NET_DVR_DEVICEINFO_V40 struDeviceInfoV40;
  LONG lUserID = NET_DVR_Login_V40(&struLoginInfo, &struDeviceInfoV40);
  if (lUserID < 0)
  {
    printf("Login error, %d ", NET_DVR_GetLastError());
    NET_DVR_Cleanup();
    return -1;
  }
  NET_DVR_DEVICEINFO_V30 struDeviceInfo = struDeviceInfoV40.struDeviceV30;
  if (struDeviceInfo.byIPChanNum <= 0)
  {
    printf("NVR's IP has no IP chanNum ");
    NET_DVR_Cleanup();
    return -1;
  }
  DWORD dwReturned = 0;
  DWORD iGroupNO = 0;
  NET_DVR_IPPARACFG_V40 IPAccessCfgV40;
  memset(&IPAccessCfgV40, 0, sizeof(NET_DVR_IPPARACFG));
  if (!NET_DVR_GetDVRConfig(lUserID, NET_DVR_GET_IPPARACFG_V40, iGroupNO, &IPAccessCfgV40, sizeof(NET_DVR_IPPARACFG_V40), &dwReturned))
  {
    printf("NET_DVR_GET_IPPARACFG_V40 error, %d ", NET_DVR_GetLastError());
    NET_DVR_Logout(lUserID);
    NET_DVR_Cleanup();
    return -1;
  }
  for (int i = 0; i < IPAccessCfgV40.dwDChanNum; i++)
  {
    int ch = IPAccessCfgV40.dwStartDChan + i;
  }
  int hPlayback;
  hPlayback = NET_DVR_GetFileByTime_V40(lUserID, path, &struDownloadCond);
  if (hPlayback < 0)
  {
    printf("NET_DVR_GetFileByTime_V40 fail,last error %d ", NET_DVR_GetLastError());
    NET_DVR_Logout(lUserID);
    NET_DVR_Cleanup();
    return -1;
  }
  if (TRUE != NET_DVR_PlayBackControl_V40(hPlayback, NET_DVR_PLAYSTART, NULL, 0, NULL, NULL))
  {
    printf("Play back control failed [%d] ", NET_DVR_GetLastError());
    NET_DVR_Logout(lUserID);
    NET_DVR_Cleanup();
    return -1;
  }
  int nPos = 0;
  int counter = 0;
  for (nPos = 0; nPos < 100 && nPos >= 0; nPos = NET_DVR_GetDownloadPos(hPlayback))
  {
    usleep(1000000);
  }
  if (TRUE != NET_DVR_StopGetFile(hPlayback))
  {
    printf("failed to stop get file [%d] ", NET_DVR_GetLastError());
    NET_DVR_Logout(lUserID);
    NET_DVR_Cleanup();
    return -1;
  }
  printf("SAVED:  %s\n", path);
  NET_DVR_Logout(lUserID);
  NET_DVR_Cleanup();
  return 0;
}
