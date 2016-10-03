# Pulse

An addictive, twitch based game which will put your skills to the test!

Created with love by Tandem. Enjoy!

**Note:** Pulse is in alpha and under active development. Please expect and report bugs!

## Features

- **Accelerometer based movement**: dodge those pesky obstacles like a ninja
- **Time manipulation**: slow down time by touching and holding on the screen
- **Multiple modes**: choose from Classic, Free Play or Reverse
- **Smooth as butter**: those frames are all yours. No please, have them all!

## Initial setup

The entire codebase is fairly large, mainly due to the inclusion of the entire Cocos2d-x project. This is done to aid development, remedy the fairly mediocre documentation and fix the occasional bug. Don't be alarmed if the initial clone takes some time.

1. Download the Pulse repository and all of its dependencies.

    ``` shell
    $ git clone --recursive https://github.com/Rypac/pulse.git
    ```

2. Setup Cocos2d-x by downloading its dependencies (repeat after each update).

    ``` shell
    $ cd pulse
    $ python libs/cocos2d/download-deps.py
    ```

3. Update all of the cocos2d dependencies.

    ``` shell
    $ cd libs/cocos2d
    $ git submodule update --init
    ```

## iOS

**Prerequisites:**

- Command line tools
- Xcode

Building and running Pulse on iOS requires Mac OS X and Xcode. The simplest method of running the game is to fire up Xcode, connect your iDevice and hit the run button.

Yes, that's it. It's really the easy.

**Note:** You will need to setup a developer certificate to run it on a physical device. Consult the Apple Developer site on how to acquire and setup and certificate if one is not already present.

## Android

**Prerequisites:**

- Ant
- Java JDK 1.8
- Android SDK
- Android NDK

The Java JDK can be downloaded [here](http://www.oracle.com/technetwork/java/javase/downloads/index.html).
The Android SDK can be downloaded [here](https://developer.android.com/studio/index.html).

Pulse uses the CrystaX NDK as a drop in replacement for the Google NDK. This NDK provides the same features as well as a number of improvements and most importantly, far better support for C++11/C++14 features. The CrystaX NDK can be downloaded [here](https://www.crystax.net/en/download).

The following environment variables are required to ensure that Cocos2d-x can find all of the prerequisites.

``` shell
$ export ANT_ROOT=/path/to/ant/bim
$ export JAVA_HOME=/path/to/jdk1.8.0_92.jdk/Contents/Home
$ export ANDROID_HOME=/path/to/android/sdk
$ export ANDROID_SDK_ROOT="$ANDROID_HOME"
$ export ANDROID_NDK=/path/to/crystax-ndk
$ export NDK_ROOT="$ANDROID_NDK"
$ export PATH="$ANDROID_HOME/tools:$ANDROID_HOME/platform-tools:$PATH"
```

The game is built and run using the `cocos2d-console` which is included in the cocos2d submodule.

``` shell
# Run from the root of the pulse project directory
$ cd "$HOME/path/to/pulse"
$ alias cocos='./libs/cocos2d/tools/cocos2d-console/bin/cocos'
$ cocos compile -p android
```

To run the game on a physical device, connect it via ADB and ensure that USB debugging is enabled on the device. Run the following commands when with the device connected to ensure that ADB is communicating correctly.

``` shell
$ adb start-server
$ adb devices
```

Optionally (though highly recommended), remote wireless debugging can be enabled if connected to the same WiFi network. Substitute the following with the IP address of your device.

``` shell
$ adb tcpip 5555
$ adb connect 192.168.1.50:5555
```

With the device connected (via USB or wirelessly), use the following command to rebuild and run the game.

``` shell
$ cocos run -p android
```
