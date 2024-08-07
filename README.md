# ESP8266 AC Control

This repo contains the source as well as some instructions on how I setup an ESP8266 to:

* Monitor Temperature & Humidity
* Control my AC

## Motivation

I wanted to track the temperature of my room over time to see how effective the AC is, and also observe how internal temperature tracks with the weather in Hong Kong. It's also just fun to see graphs.

But the main thing was to be able to control my AC when I'm not at home. I don't own a "smart" AC, nor would I ever trust one. This project allows me to send commands (well an HTTP request) to my ESP8266, which sends a pulse to an IR LED, which is what actually turns the AC on/off.

TODO: Talk about the setup of exposing this over the Internet via Yubikey auth.

## Building

The ESP8266 circuit diagram is something like this:

TODO: Insert image

### Set environment variable

The WiFi network's password is fed via a macro build flag, the macro being `WPASSWORD`, which PlatformIO is configured to set to the value of the environment variable `ENV_WIFI_PW`.

So, set the WiFi password as the environment variable like so (Windows example)

```
$env:ENV_WIFI_PW = 'password'
```

### Upload

Then, ypu can upload it using `platformio.exe` directly to get the environment variable set

```
C:\Users\poiasd\.platformio\penv\Scripts\platformio.exe run --target upload --upload-port COM7
```