"""PyAudio Example: Play a wave file."""

import pyaudio
import wave
import sys
import yaml

CHUNK = 1024


if len(sys.argv) < 2:
    print("Provide a Sound ID")
    sys.exit(-1)

sound_dictionary = yaml.load(open('config.yaml'))

sound_file = 'willow-sound/'+ sound_dictionary[sys.argv[1]]['folder']+'/'+sound_dictionary[sys.argv[1]]['file_name'] 

print sound_file

wf = wave.open(sound_file, 'rb')

# instantiate PyAudio (1)
p = pyaudio.PyAudio()

# open stream (2)
stream = p.open(format=p.get_format_from_width(wf.getsampwidth()),
                channels=wf.getnchannels(),
                rate=wf.getframerate(),
                output=True)

# read data
data = wf.readframes(CHUNK)

# play stream (3)
while len(data) > 0:
    stream.write(data)
    data = wf.readframes(CHUNK)

# stop stream (4)
stream.stop_stream()
stream.close()

# close PyAudio (5)
p.terminate()
