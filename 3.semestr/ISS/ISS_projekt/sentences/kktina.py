#toto je muj první program v Pythonu



import numpy as np
import matplotlib.pyplot as plt
import soundfile as sf
import IPython
import soundfile
from scipy.signal import spectrogram, lfilter, freqz, tf2zpk

#Fs = 16000
#N = 512
#wlen = 25e-3*Fs
#wshift = 10e-3*Fs
#woverlap = wlen - wshift

data, fs = soundfile.read('sa1.wav')

data.min(), data.max()


#f, t, Sxx = signal.spectrogram(x, fs)
#plt.pcolormesh(t, f, Sxx)
#plt.ylabel('Frequency [Hz]')
#plt.xlabel('Time [sec]')
#plt.show()

