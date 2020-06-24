###Feedback Delay Network  
  
clone a singel folder (git version>=2.25 required) :  
> git clone --filter=blob:none https://github.com/bmFicg/audioexp.git    
> cd audioexp  
> git sparse-checkout init  
> git sparse-checkout set fdn  
  
    
#### using the *.bat file to build on Windows  
  
assuming gcc compiler and ffmpeg / ffplay is set in PATH system environment variable  
> **_NOTE:_**  Linux remove the underscore in _popen function

the program  
- will render an 8 seconds audiosampel with kick and snare to a sound.wav file  
- play it using ffplay  
  
    
ffplay screenshot  
![alt text](additionalresources/ffplay_img.jpg?raw=true "ffplay screenshot")  
  
octave plot  
![alt text](additionalresources/fdn_plot.jpg "octave plot")  
