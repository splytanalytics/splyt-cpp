[![Build Status](https://travis-ci.org/splytanalytics/splyt-cpp.svg?branch=master)](https://travis-ci.org/splytanalytics/splyt-cpp)

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
The instructions above involve building the libary into an shared object for use, but that is not neccessary(Though I do recommend that method) if you just want to build the project directly into your application by including the source and header files in your project. Then by using the code below to include the needed header file.
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

There are more advanced options to be passed to the initalization function that is explained in the [API Functions section](#api-functions) of this documentation.

The Init function(as well as all other API functions) will throw a splyt_exception when there is some sort of error with initialization. More information on this available in the [Errors and Exceptions section](#errors-and-exceptions).

You must provide either user_id or device_id to initalize the SDK. If you do not wish to pass one simply pass the parameter as an empty string like so.
```c++
//Initialize Splyt without a device ID.
splytapi::Splyt* splyt = splytapi::Init("knetik-bubblepop-test", "testuser", "", "testContext");

//Initialize Splyt without a user ID.
splytapi::Splyt* splyt = splytapi::Init("knetik-bubblepop-test", "", "testdevice", "testContext");
```

### Usage
You can now use the instance of Splyt returned to make api calls like so.
```c++
//Begin a transaction.
splyt->transaction->Begin("testtransaction", "testcategory", 3600, "testContext");
```
More examples can be found in the [API Functions section](#api-functions) as well as below.

### Responses
All API functions, with the exception of Init, return an instance of the SplytResponse class that contains content returned from the network call. The content is an instance of the Json::Value class from the [JsonCpp library](https://github.com/open-source-parsers/jsoncpp). Documentation for the use of this class can be found [here](https://github.com/open-source-parsers/jsoncpp/wiki).

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
You can also get the response from the server by using the GetContent() function in SplytResponse class, as shown in the [Responses section](#responses).


### Configuration
Configuration for the SDK is located in the file "util/config.h", this contains static variables that can be overwritten before initialization, like so.
```c++
//Set the network host.
splytapi::Config::kNetworkHost = "https://data.splyt.com";

//Change the tuning variable cache time to live.
splytapi::Config::kTuningCacheTtl = 10000;

//Change network call timeout, measured in milliseconds.
splytapi::Config::kNetworkTimeout = 1500;
```

The configurations and their explanations are located in "util/config.h" so you can look there for what is able to be modified.

It is not recommended to change configuration once a Splyt instance has been initalized, although it is possible.


# Unit Tests
How to run the SDK unit tests, tests are only needed for development of the SDK.

### UNIX Systems
Inside of the root folder run this command:
```
make unix-tests
```
This will build the needed shared object and then include/build the test files. The executable test files are located in the "bin" folder as something like "test.o".

### Windows
Inside of the root folder run this command:
```
make win-tests
```
This will build the needed DLL and then include/build the test files. The executable test files are located in the "bin" folder as something like "test.exe".


# API Functions
A list of the API functions and their use currently implemented in the SDK. All of these functions, with the exception of Init, require an instance of the Splyt class to be used. An instance of said class can be aquired through the Init function.

**NOTE:** All API functions, with the exception of Init, return a SplytResponse. Information on how to handle this response is shown in the [Responses section](#responses).

**NOTE:** All API functions throw an splyt_exception when an error occurs. Information on how to handle this is shown in the [Errors and Exceptions section](#errors-and-exceptions).

## General
Functions that do not fall under a specific category.

### splytapi::Init
This function ise used to intialize and return an instance of the Splyt class that handles all API calls, it is the only function that does not require an instance of the Splyt class to be used.
```c++
Splyt* Init(std::string customer_id, std::string user_id, std::string device_id, std::string context, HttpInterface* httpint);
```
The user_id and device_id parameters are optional, but you must at least pass one or the other.
- std::string customer_id - Customer ID provided by Splyt.
- std::string user_id - Optional.
- std::string device_id - Optional.
- std::string context - Context of this API call.
- HttpInterface httpint - Optional. HttpInterface used for HTTP requests. If one is not passed, it will use the default CurlHttpInterface.

The HttpInterface is used to make POST calls to the Splyt servers and is located in "network/http_interfaces.h", you can create a class that extends the HttpInterface and pass it to the Init function if you wish. If you do not an instance of the CurlHttpInterface will be passed to make network calls.


### splyt->UpdateCollection
Update a collection, used to record purchases.
```c++
SplytResponse UpdateCollection(std::string name, double balance, double balance_delta, bool is_currency, std::string context, std::string user_id = "", std::string device_id = "");
```
- std::string name - A name for this collection.
- double balance - Current balance.
- double balance_delta - The change applied to the current balance.
- bool is_currency - Specifies if this is real or digital currency.
- std::string context - Context of this API call.
- std::string user_id - Optional. If not supplied will be set to the user_id passed during initialization.
- std::string device_id - Optional. If not supplied will be set to the device_id passed during initialization.


## Users and Devices
Functions that are related to users and devices.

### splyt->NewUser
Creates a new user.
```c++
SplytResponse NewUser(std::string user_id, std::string context);
```
- std::string user_id
- std::string context - Context of this API call.


### splyt->NewDevice
Creates a new device.
```c++
SplytResponse NewDevice(std::string device_id, std::string context);
```
- std::string device_id
- std::string context - Context of this API call.


### splyt->NewUserChecked
Creates a new user if it does not already exist.
```c++
SplytResponse NewUserChecked(std::string user_id, std::string context);
```
- std::string user_id
- std::string context - Context of this API call.


### splyt->NewDeviceChecked
Creates a new device if it does not already exist.
```c++
SplytResponse NewDeviceChecked(std::string device_id, std::string context);
```
- std::string device_id
- std::string context - Context of this API call.

## Entity States
Functions that are related to managing entity states.

### splyt->UpdateUserState
Updates a users entity state.
```c++
SplytResponse UpdateUserState(std::string user_id, std::string context, Json::Value properties = Json::Value::null);
```
- std::string user_id
- std::string context - Context of this API call.
- Json::Value properties - Optional. Properties to set for this user, if not supplied properties are set to null.


### splyt->UpdateDeviceState
Updates a devices entity state.
```c++
SplytResponse UpdateDeviceState(std::string device_id, std::string context, Json::Value properties = Json::Value::null);
```
- std::string device_id
- std::string context - Context of this API call.
- Json::Value properties - Optional. Properties to set for this device, if not supplied properties are set to null.


## Transactions
Functions that are specific to transactions.

### splyt->transaction->Begin
Begin a new transaction.
```c++
SplytResponse Begin(std::string transaction_id, std::string category, int timeout, std::string context, std::string user_id = "", std::string device_id = "", Json::Value properties = Json::Value::null);
```
- std::string transaction_id
- std::string category - This transactions category.
- int timeout
- std::string context - Context of this API call.
- std::string user_id - Optional. If not supplied will be set to the user_id passed during initialization.
- std::string device_id - Optional. If not supplied will be set to the device_id passed during initialization.
- Json::Value properties - Optional. Properties to set for this transaction, if not supplied properties are set to null.


### splyt->transaction->Update
Update an existing transaction.
```c++
SplytResponse Update(std::string transaction_id, std::string category, double progress, std::string context, std::string user_id = "", std::string device_id = "", Json::Value properties = Json::Value::null);
```
- std::string transaction_id
- std::string category - This transactions category.
- double progress - Progress for this transaction, expressed as a percentage.
- std::string context - Context of this API call.
- std::string user_id - Optional. If not supplied will be set to the user_id passed during initialization.
- std::string device_id - Optional. If not supplied will be set to the device_id passed during initialization.
- Json::Value properties - Optional. Properties to set for this transaction, if not supplied properties are set to null.


### splyt->transaction->End
End an existing transaction.
```c++
SplytResponse End(std::string transaction_id, std::string category, std::string result, std::string context, std::string user_id = "", std::string device_id = "", Json::Value properties = Json::Value::null);
```
- std::string transaction_id
- std::string category - This transactions category.
- std::string result - Result for this transaction, can be anything. Ex. "success"
- std::string context - Context of this API call.
- std::string user_id - Optional. If not supplied will be set to the user_id passed during initialization.
- std::string device_id - Optional. If not supplied will be set to the device_id passed during initialization.
- Json::Value properties - Optional. Properties to set for this transaction, if not supplied properties are set to null.


## Tuning
Functions that are related to managing tuning variables.

**NOTE:** These values are cached per Splyt instance and entity. The cache TTL for these can be set using the variable kTuningCacheTtl", more information for configuration can be found in the [Configuration section](#configuration).

### splyt->tuning->GetAllValues
Get all tuning values for an entity.
```c++
SplytResponse GetAllValues(std::string entity_id, EntityType entity_type);
```
- std::string entity_id
- EntityType entity_type - EntityType Enum, possible values: [kEntityTypeUser, kEntityTypeDevice]


### splyt->tuning->GetValue
Get a single tuning value by name.
```c++
SplytResponse GetValue(std::string name, std::string default_value, std::string entity_id, EntityType entity_type);
```
- std::string name - Name of the tuning value.
- std::string default_value - Value returned if the Splyt network is not available.
- std::string entity_id
- EntityType entity_type - EntityType Enum, possible values: [kEntityTypeUser, kEntityTypeDevice]


### splyt->tuning->RecordValue
Record the use of a tuning value.
```c++
SplytResponse RecordValue(std::string name, std::string default_value, std::string user_id = "", std::string device_id = "");
```
- std::string name - Name of the tuning value.
- std::string default_value
- std::string user_id - Optional. If not supplied will be set to the user_id passed during initialization.
- std::string device_id - Optional. If not supplied will be set to the device_id passed during initialization.
