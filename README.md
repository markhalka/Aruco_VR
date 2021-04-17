DiveVR is a virtual reality system that aims at making virutal reality more accessable, by using everyday electronics, a cheap controller and aruco markers.

<b>The software also has built-in functionality to allow users to play any game in VR, regardless of whether or not it is compatible with VR. This is done using Windows OS software features, which are used to match the actions of user in the real world, to keystrokes and mouse movments that match the real-world movments to ones in the game.</b>

In order to use the code you must have OpenCV downloaded, with a verion above 3.

Currently the software only works on Windows and Android, however, future implementations with ios and iPhone will be available soon.

Multiple sample Aruco marker images can be found in the bin folder, however, these can be customized, generated and printed from the downloadable software. 

<h2> How it works: </h2>
DiveVR works by using OpenCV to find markers in your enviornment, and estimate your position and orientation using them. To increase accuracy, reliability, and smoothness
we also use the phones IMU, and combine the measurments with a Kalman filter to obtain better position and orientation estimates. Finally, using Windows featuers,
the users movments are converted to keystrokes and mouse movents to any game, allowing the user to play that game in VR.

<h2> Instructions: </h2>
1. First place ARUCO markers around the room, such that they are not too far from each other (a size of about 10cm x 10cm per marker works well)
2. Next, open the app on your computer, and on your phone, the apps on both will connect automatically.
3. Then if it is your first time, you must initialize your room (otherwise you can use the preexisting configruation). 
4. Next, initialize the camera of the phone if it has not been done before. This is done by pointing the phone camera at a designated calibration board. (included in the rep)
5. Finally, you can place your phone in the headset and begin playing your chosen game.

(Note: steps 3 and 4 only apply if it is your first time using the system)

For more information about the project, feel free to contact me at mark.halka2001@gmail.com. 
