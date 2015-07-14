# Splyt C++ SDK
This is the Splyt C++ software development kit. This SDK should provide everything needed to integrate your C++ application with Splyt Analytics.

This documentation does not explain how to use the Splyt Platform its self, but how to integrate this specific SDK and the use of its API.

This SDK makes use of the [JsonCpp library](https://github.com/open-source-parsers/jsoncpp) to parse and use the JSON format.


# Installation
How to install the SDK into your application.

### UNIX Systems
Build the shared object to include in your application. This can only be built on a UNIX like system. Ex: Linux, Mac, etc.
```
make unix
```
It will build the file and place it in "lib/libsplyt.so", link the file with your compiler along with the Splyt header files and include the needed header:
```c++
#include "splyt.h"
```
Installation complete, continue to the [Overview](#overview) section to get started with coding.

### Windows
Build the DLL to include in your application. This can only be built on a Windows system.
```
make win
```
It will build the file and place it in "lib/libsplyt.dll", link the file with your compiler along with the Splyt header files and include the needed header:
```c++
#include "splyt.h"
```
Installation complete, continue to the [Overview](#overview) section to get started with coding.

### Source Install
The instructions above involve building the libary into an shared object for use, but that is not neccessary(Though I do recommend that method) if you just want to build the project directly into your application by including the source and header files in your project. Then of course using the code below to include the needed header file.
```c++
#include "splyt.h"
```
After that the installation is complete, continue to the [Overview](#overview) section to get started with coding.



# Overview
How to use the SDK in your application once it has been [installed](#installation).

### Initialization
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

The Init function(as well as all other API functions) will throw a splyt_exception when there is some sort of error with initialization. More information on this available in the [exceptions section](#errors-and-exceptions).

You must provide either user_id or device_id to initalize the SDK. If you do not wish to pass one simply pass the parameter as an empty string like so.
```c++
//Initialize Splyt without a device ID.
splytapi::Splyt* splyt = splytapi::Init("knetik-bubblepop-test", "testuser", "", "testContext");
```
This of course works the other way for user_id.

### Usage
You can now use the instance of Splyt returned to make api calls like so.
```c++
//Begin a transaction.
splyt->transaction->Begin("testtransaction", "testcategory", 3600, "testContext");
```
This is of course only one simple example, more examples and their uses can be found in the [API section](#api).

### Responses
All API functions, with the exception of Init, return an instance of the SplytResponse class that contains content returned from the network call. The content is an instance of the Json::Value class from the [JsonCpp library](https://github.com/open-source-parsers/jsoncpp). Documentation for the use of this clas can be found [here](https://github.com/open-source-parsers/jsoncpp/wiki).

Response Example:
```c++
splytapi::Splyt* splyt = //... Splyt initalization.

//Get all tuning values for a user.
splytapi::SplytResponse resp = splyt->tuning->GetAllValues("testuser", splytapi::kEntityTypeUser);

//Get the response content.
Json::Value content = resp.GetContent();

//Get a value from the content as a string.
std::string testval = content["testval"].asString();

//Print the JSON content recieved from the server.
std::cout << "testval value: " + testval << std::endl;
```
Any errors that occur will throw a splyt_exception, this can be handled as shown below.


### Errors and Exceptions
All API functions throw a splyt_exception when an error has occurred, these exceptions contain a SplytResponse holding error information needed to understand the issue. An example for handling these exceptions is shown below.
```c++
splytapi::Splyt* splyt = //... Splyt initalization.

try {
    splytapi::SplytResponse resp = splyt->tuning->GetAllValues("testuser", splytapi::kEntityTypeUser);

    //... Do something with the response here.
} catch (splytapi::splyt_exception e) {
    //Get the SplytResponse from the exception.
    splytapi::SplytResponse resp = e.GetResponse();

    //Get the error message from the SplytResponse.
    std::cout << "error: " + resp.GetErrorMessage() << std::endl;
}
```
You can also get the response from the server by using the GetContent() function in SplytResponse class, as shown in the [responses section](#responses).


### Configuration
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

### UNIX Systems
Inside of the root folder run this command:
```
make unix-tests
```
This will build the needed shared object and then include/build the test files. The executable test files are located in the "bin" folder as something like "test.o".

After build completion the tests will run.

### Windows
Inside of the root folder run this command:
```
make win-tests
```
This will build the needed DLL and then include/build the test files. The executable test files are located in the "bin" folder as something like "test.exe".

After build completion the tests will run.


# API
WIP

A list of the API functions and their use currently implemented in the SDK. All of these functions, with the exception of Init, require an instance of the Splyt class to be used. An instance of said class can be aquired through the Init function.

### splytapi::Init()
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


### splyt->NewUser()
Creates a new user.
```c++
SplytResponse NewUser(std::string user_id, std::string context);
```
- std::string user_id
- std::string context - Context of this API call.

