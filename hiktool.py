import os
COMPILE_FILES_DIR       =   "./Cbuild/"
PORT                    =   8000

def download_video(user,password,IP,channel,init_time,end_time,filepath):
    def timeToList(valueTime):
        return valueTime.replace(" ", '-').replace(":", '-').split('-')
    """
    This function download a video ranged between the init and end time from the 
    camera [IP-channel]. The video will saved to disk at filepath.    
    
    :param user:            (str) Device user for login.
    :param password:        (str) Device password for login.
    :param IP:              (str) "192.168.0.100".
    :param channel:         (int) device channel (DVR starts at 1, NVR starts at 33).
    :param init_time:       (str) "2020-02-11 14:00:00".
    :param end_time:        (str) "2020-02-11 14:00:01".
    :param filepath         (str) Filepath to save video to disk.
    
    :return:                0 if everything "ok" else -1.
    """
    init_time   =   timeToList(init_time)
    end_time    =   timeToList(end_time)
    comando     =   "{} {} {} {} {} {} {} {} {}".format(
                            COMPILE_FILES_DIR+'Download',
                            IP,
                            PORT,
                            user,
                            password,
                            channel,
                            filepath,
                            init_time,
                            end_time
                            )
    result      =   os.system(comando)
    return result

def download_images_from_channels(user,password,IP,channels,path):
    """
    This functions saved to disk images in JPG format of all channels
    of the device. You must indicate which channels.

    :param user:            (str) Device user for login.
    :param password:        (str) Device password for login.
    :param IP:              (str) "192.168.0.100".
    :param channels:        (str) a string containing the channels separated by a blank "1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16"
    :param path:            (str) path to save the images downloaded

    :return: None
    """

    filename_prefix = path + IP.replace(".", "_") + '-'
    comando = "{} {} {} {} {} {} {}".format(
                COMPILE_FILES_DIR+'Download_jpg',
                IP,
                PORT,
                user,
                password,
                filename_prefix,
                channels
                )
    return os.system(comando)