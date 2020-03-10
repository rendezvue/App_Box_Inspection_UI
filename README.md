# NG(Crack, Color) 검사 프로그램

* * *
## Compile
1. Install QT lib. and QT Creator
2. Install OpenCV lib. (v4.x)
3. Install Boost lib.
4. Get App_Box_Inspection_UI source code
<pre><code>
$git clone https://github.com/rendezvue/App_Box_Inspection_UI.git<br>
$cd App_Box_Inspection_UI
</code></pre>
5. Submodule update
<pre><code>$git submodule init<br>
$git submodule update</code></pre>
6. Open "App_Box_Inspection_UI.pro" and Compile using QT Creator
7. Enjoy!

## How to install dependency libraries on Ubuntu.
sudo apt-get install libdc1394-22 libavcodec-dev libswscale-dev libavformat-dev libopencv-dev libboost-all-dev 

## How to install QT Creator on Ubuntu.
sudo apt-get install build-essential qtcreator qt5-default

* * *
![Program](https://github.com/rendezvue/App_Box_Inspection_UI/blob/master/doc/screenshot.png)
