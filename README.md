# salmap_rv
Bottom-up attention (Saliency Map) computation library using opencv

# Requirements
opencv
C++17
boost
bash
GNU make

# Compile example program
make itti_formal.exe

# Run example program
Using default webcam:
./build/itti_formal.exe cam NULL 90 640 0 0 models/chenetal2020marmo/params

Using video file ~/video.mp4:
./build/itti_formal.exe cam ~/video.mp4 90 640 0 0 models/chenetal2020marmo/params
