#HERE NEED TO WRITE WHAT IS THIS DOWNLOADER!!


export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/tia/Documents/PROJECTS/COUNTER_SYSTEMS/TIA_videoanalyticssystem/TIA_videoanalyticssystem/modules/downloaders/downloader_hikvision/Cbuild/lib:/home/tia/Documents/PROJECTS/COUNTER_SYSTEMS/TIA_videoanalyticssystem//TIA_videoanalyticssystem/modules/downloaders/downloader_hikvision/Cbuild/lib/HCNetSDKCom

g++ -o HikvisionSDK_VideoRecording_downloader HikvisionSDK_VideoRecording_downloader.cpp  -lhcnetsdk -lHCCore -lhpr -lHCCoreDevCfg  -L ./lib -L ./lib/HCNetSDKCom

