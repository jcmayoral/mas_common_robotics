"""PyAudio Example: Play a wave file."""

import pyaudio
import wave
import sys
import yaml
import rospy

class SoundDiagnoser:
    def __init__(self, config_file):
        rospy.init_node("sound_diagnoser", anonymous = False)
        common_path = "/home/jose/ros/src/mas_common_robotics/mcr_tools/mcr_sound_diagnoser/ros/"
        self.sound_dictionary = yaml.load(open(common_path + "config/" + config_file+".yaml"))
        self.CHUNK = 1024 #TODO

        self.audio_manager = pyaudio.PyAudio()

        for sound in self.sound_dictionary:
            rospy.Subscriber(self.sound_dictionary[sound]['topic'], rospy.AnyMsg, self.mainCB)
            sound_file = common_path + 'willow-sound/'+ self.sound_dictionary[sound]['folder']+'/'+ self.sound_dictionary[sound]['file_name'] 
            self.playSound(sound_file)

        rospy.spin()

    def mainCB(self, msg):
        print("Inside CB")

    def playSound(self,sound_file):
        wf = wave.open(sound_file, 'rb')
        data = wf.readframes(self.CHUNK)
        stream = self.audio_manager.open(format=self.audio_manager.get_format_from_width(wf.getsampwidth()),
                      channels=wf.getnchannels(),
                      rate=wf.getframerate(),
                      output=True)

        while len(data) > 0:
            stream.write(data)
            data = wf.readframes(self.CHUNK)

            # stop stream (4)
        stream.stop_stream()
        stream.close()
 




