# Splyt C++ SDK
This is the Splyt C++ software development kit. This SDK should provide everything needed to integrate your C++ application with Splyt Analytics.

This documentation does not explain how to use the Splyt Platform its self, but how to integrate this specific SDK and the use of its API.

This SDK makes use of the [JsonCpp library](https://github.com/open-source-parsers/jsoncpp) to parse and use the JSON format.


# Installation
How to install the SDK into your application.

UNIX Systems
----------------
Build the shared object to include in your application. This can only be built on a UNIX like system. Ex: Linux, Mac, etc.
```
make unix
```
It will build the file and place it in "lib/libsplyt.so", link the file with your compiler along with the Splyt header files and include the needed header:
```c++
#include "splyt.h"
```
Installation complete, continue to the [Overview](#overview) section to get started with coding.

Windows
------------
Build the DLL to include in your application. This can only be built on a Windows system.
```
make win
```
It will build the file and place it in "lib/libsplyt.dll", link the file with your compiler along with the Splyt header files and include the needed header:
```c++
#include "splyt.h"
```
Installation complete, continue to the [Overview](#overview) section to get started with coding.



# Overview
How to use the SDK in your application once it has been [installed](#installation).

Initialization
-----------------
The Splyt SDK must be initialized before use, this initialization function returns an instance of the Splyt class that handles API calls.
```c++
//Initialize the Splyt API.
splytapi::Splyt* splyt = splytapi::Init("knetik-bubblepop-test", "testuser", "testdevice", "testContext");
```
The intialization function requires at least these parameters in this order:
- std::string customer_id - Customer ID provided by Splyt.
- std::string user_id - Optional when providing device_id.
- std::string device_id - Optional when providing user_id.
- std::string context - Context of this API call.

There are more advanced options to be passed to the initalization function that is explained in the [API section](#api) of this documentation.

You must provide either user_id or device_id to initalize the SDK. If you do not wish to pass one simply pass the parameter as an empty string like so.
```c++
//Initialize Splyt without a device ID.
splytapi::Splyt* splyt = splytapi::Init("knetik-bubblepop-test", "testuser", "", "testContext");
```
This of course works the other way for user_id.

Usage
-------------
You can now use the instance of Splyt returned to make api calls like so.
```c++
//Begin a transaction.
splyt->transaction->Begin("testtransaction", "testcategory", 3600, "testContext");
```
This is of course only simple one example, more examples and their uses can be found in the [API section](#api).

Configuration
------------
Configuration for the SDK is located in the file "util/config.h", this contains static variables that can be overwritten before initialization, like so.
```c++
//Change the network host.
splytapi::Config::kNetworkHost = "https://api.example.com";
splytapi::Config::kTuningCacheTtl = 10000;
```

The configurations and their explanations are located in "util/config.h" so you can look there for what is able to be modified.

It is not recommended to change configuration once a Splyt instance has been initalized, although it is possible.


# Tests
How to run run SDK tests, tests are of course only needed for development of the SDK.

UNIX Systems
----------------
Inside of the root folder run this command:
```
make unix-tests
```
This will build the needed shared object and then include/build the test files. The executable test files are located in the "bin" folder as something like "test.o".

After build completion the tests will run.

Windows
------------
Inside of the root folder run this command:
```
make win-tests
```
This will build the needed DLL and then include/build the test files. The executable test files are located in the "bin" folder as something like "test.exe".

After build completion the tests will run.


# API
WIP

A list of the API functions and their use currently implemented in the SDK. All of these functions, with the exception of Init, require an instance of the Splyt class to be used. An instance of said class can be aquired through the Init function.

splytapi::Init()
----------------
This function ise used to intialize and return an instance of the Splyt class that handles all API calls, it is the only function that does not require an instance of the Splyt class to be used.
```c++
Splyt* Init(std::string customer_id, std::string user_id, std::string device_id, std::string context, HttpInterface* httpint);
```
The user_id and device_id parameters are optional, but you must at least pass one or the other.
- std::string customer_id - Customer ID provided by Splyt.
- std::string user_id - Optional
- std::string device_id - Optional
- std::string context - Context of this API call.
- HttpInterface httpint - Optional HttpInterface used for HTTP requests. If one is not passed, it will use the default CurlHttpInterface.

The HttpInterface is used to make POST calls to the Splyt servers and is located in "network/http_interfaces.h", you can create a class that extends the HttpInterface and pass it to the Init function if you wish. If you do not an instance of the CurlHttpInterface will be passed to make network calls.


splyt->NewUser()
--------------
Creates a new user.
```c++
SplytResponse NewUser(std::string user_id, std::string context);
```
- std::string user_id
- std::string context - Context of this API call.

