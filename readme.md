[![Build Status](https://travis-ci.org/splytanalytics/splyt-cpp.svg?branch=master)](https://travis-ci.org/splytanalytics/splyt-cpp)

# Splyt C++ SDK
This is the Splyt C++ software development kit and should provide everything needed to integrate your C++ application with Splyt Analytics.

This documentation does not explain how to use the Splyt Platform its self, but how to integrate this specific SDK and the use of its API.

This SDK makes use of the [JsonCpp library](https://github.com/open-source-parsers/jsoncpp) to parse and use the JSON format, the [libcurl library](http://curl.haxx.se/libcurl/) for HTTP requests, and the [Boost library](http://www.boost.org/) for multiplatform threading.


# Installation
How to install the SDK into your application.

### UNIX Systems
Ensure you have the libcurl library and Boost library installed. Most UNIX like operating systems will come with libcurl but if you have a build error referring to missing "curl/curl.h" then the issue is that you do not have libcurl installed. Version v7.35.0 or later of libcurl is required.

As for the Boost library, only the threading portion is required so you can run something like the following:
```
sudo apt-get install libboost-thread-dev
```
This SDK was built with Boost v1.54, chances are though it will work with most other versions.

After that you can build the shared object to include in your application. This can only be built on a UNIX like system. Ex: Linux, Mac, etc.
```
make unix
```
It will build the file and place it in "lib/libsplyt.so", link the file with your compiler along with the Splyt header files located in "src", "platform/curl", and "vendor". Then include the needed header:
```c++
#include "splyt.h"
```
Installation complete, continue to the [Overview](#overview) section to get started with coding.

### Windows
There are two options for building with windows, as shown below. The [libcurl library](http://curl.haxx.se/libcurl/) Windows build and [Boost library](http://www.boost.org/) threading portion is distributed with this SDK.

##### Cygwin and MinGW
Build the DLL and static library to include in your application. This can only be built on a Windows system.
```
make win
```
It will build the files and place them in "lib/libsplyt.dll" and "lib/libsplyt.a". You must then link the ".a" file with your compiler. Then link "libcurl.a" and "libcurldll.a" located in "projects/visual-studio/vendor/curl". After that include the Splyt header and vendor header files located in "src", "platform/curl", "projects/visual-studio/vendor", and "vendor". Examples for this are shown in the Makefile for building the unit tests.

Finally include the needed header in your code:
```c++
#include "splyt.h"
```
Installation complete, continue to the [Overview](#overview) section to get started with coding.

##### Visual Studio
A Visual Studio 2010 project is included in this repo that builds the required DLL and lib file, if you have a version later than 2010 you can update the project to your version through the Project menu. Once that is done you can continue with these steps:

1. In the project you are wishing to integrate with go to File->Add->Existing Project.
2. Navigate to the splyt-cpp directory and find the Visual Studio project LibSplyt located in projects/visual-studio.
3. To your existing project, add the LibSplyt project as a Project Dependency.
4. Go to the properties of your existing project, then Configuration Properties->VC++ Directories
5. Add the following folders to the Include Directories:
    -projects/visual-studio/vendor
    -vendor
    -src
    -platform/curl
6. Add the folder in which LibSplyt will be compiling to in the Library Directories field.
7. Go to Configuration Properties->Linker->Input and add libsplyt.lib to Additional Dependencies.
8. You should now be able to add "#include splyt.h" into your code and begin programming with the API.
10. When LibSplyt is built it will copy the required libcurl dlls to the $(OutputDir) macro.

Installation complete, continue to the [Overview](#overview) section to get started with coding.


### Source Install
The instructions above involve building the libary into an shared object for use, but that is not neccessary if you just want to build/compile the project directly into your application by including the source and header files in your project. The required files are located in "src", "platform/curl", and "vendor". You will need to have the  [libcurl library](http://curl.haxx.se/libcurl/) and the [Boost library](http://www.boost.org/) threading portion(boost-thread) installed.

Then include the needed header file:
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
You can now use the instance of Splyt returned to make api calls like so:
```c++
//Begin a transaction.
splyt->transaction->Begin("testtransaction", "testcategory", 3600, "testContext");
```
There are also asynchronous versions for most API functions that take a function callback as the first argument as shown [here](#asynchronous-functions).

More examples can be found in the [API Functions section](#api-functions) as well as below.

### Responses
#### Synchronous Functions
All synchronous API functions, with the exception of Init, return an instance of the SplytResponse class that contains content returned from the network call. The content is an instance of the Json::Value class from the [JsonCpp library](https://github.com/open-source-parsers/jsoncpp). Documentation for the use of this class can be found [here](https://github.com/open-source-parsers/jsoncpp/wiki).

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
Any errors that occur with a synchronous API function will throw a splyt_exception, this can be handled as shown in the [Errors and Exceptions section](#errors-and-exceptions).

#### Asynchronous Functions
All asynchronous API functions can be identified by the suffix Async. These functions do not return anything, instead a callback can be passed as the first argument to get the response.
```c++
void TestCallback(splytapi::SplytResponse response)
{
    //Do something with the response.
}

//Begin a transaction asynchronously.
splyt->transaction->BeginAsync(&TestCallback, "testtransaction", "testcategory", 3600, "testContext");
```
If you are not interested in seeing the result you can simply pass NULL as the callback argument like so.
```c++
//Begin a transaction asynchronously without a callback.
splyt->transaction->BeginAsync(NULL, "testtransaction", "testcategory", 3600, "testContext");
```
Any errors that occur will be stored in the splytapi::SplytResponse instance that is passed to the callback, exceptions will only be thrown for configuration issues.

**NOTE:** The kNetworkEnableThreading configuration variable must be set to true for any asynchronous calls to be made.

### Errors and Exceptions
All synchronous API functions throw a splyt_exception when an error has occurred, these exceptions contain a SplytResponse holding error information needed to understand the issue. An example for handling these exceptions is shown below.
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

//Change network call timeout, measured in seconds.
splytapi::Config::kNetworkTimeout = 5;
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

**NOTE:** The kNetworkEnableThreading configuration variable must be set to true for any asynchronous calls to be made.

**NOTE:** All synchronous API functions, with the exception of Init, return a SplytResponse. Information on how to handle this response is shown in the [Responses section](#responses).

**NOTE:** All synchronous API functions throw an splyt_exception when an error occurs. Information on how to handle this is shown in the [Errors and Exceptions section](#errors-and-exceptions).

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
- HttpInterface httpint - Optional. HttpInterface used for HTTP requests. If one is not passed, it will use the default OverrideHttpInterface.

The HttpInterface is used to make POST calls to the Splyt servers and is located in "network/http_interface.h", you can create a class that extends the HttpInterface and pass it to the Init function if you wish. If you do not an instance of the OverrideHttpInterface will be passed to make network calls. The default OverrideHttpInterface is located in "platform/curl", this interface uses libcurl to send HTTP requests If the platform you are wishing to compile for does not support this and we do not yet have an SDK for it you can implement a new OverrideHttpInterface yourself.


### splyt->UpdateCollection
Updates collection, used for virtual currencies or collections.
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

### splyt->UpdateCollectionAsync
Asynchronous counterpart to the above API function that adds a callback as the first required argument.
```c++
void UpdateCollectionAsync(NetworkCallback callback, std::string name, double balance, double balance_delta, bool is_currency, std::string context, std::string user_id = "", std::string device_id = "");
```


### splyt->RecordPurchase
Records purchases, used for real currencies.
```c++
SplytResponse RecordPurchase(std::string name, double price, std::string currency_code, std::string result, std::string offer_id, std::string point_of_sale, std::string item_name, std::string context, std::string user_id = "", std::string device_id = "");
```
- std::string name - A name for this purchase.
- double price - Price of item purchased.
- std::string currency_code - Currency of the purchase. Ex. "USD"
- std::string result - Result of the purchase. Ex. "success"
- std::string offer_id - Generic offer identifier.
- std::string point_of_sale - Sale description.
- std::string item_name - Name of the item purchased.
- std::string context - Context of this API call.
- std::string user_id - Optional. If not supplied will be set to the user_id passed during initialization.
- std::string device_id - Optional. If not supplied will be set to the user_id passed during initialization.

### splyt->RecordPurchaseAsync
Asynchronous counterpart to the above API function that adds a callback as the first required argument.
```c++
void RecordPurchaseAsync(NetworkCallback callback, std::string name, double price, std::string currency_code, std::string result, std::string offer_id, std::string point_of_sale, std::string item_name, std::string context, std::string user_id = "", std::string device_id = "");
```

## Users and Devices
Functions that are related to users and devices.

### splyt->NewUser
Creates a new user.
```c++
SplytResponse NewUser(std::string user_id, std::string context);
```
- std::string user_id
- std::string context - Context of this API call.

### splyt->NewUserAsync
Asynchronous counterpart to the above API function that adds a callback as the first required argument.
```c++
void NewUserAsync(NetworkCallback callback, std::string user_id, std::string context);
```


### splyt->NewDevice
Creates a new device.
```c++
SplytResponse NewDevice(std::string device_id, std::string context);
```
- std::string device_id
- std::string context - Context of this API call.

### splyt->NewDeviceAsync
Asynchronous counterpart to the above API function that adds a callback as the first required argument.
```c++
void NewDeviceAsync(NetworkCallback callback, std::string device_id, std::string context);
```


### splyt->NewUserChecked
Creates a new user if it does not already exist.
```c++
SplytResponse NewUserChecked(std::string user_id, std::string context);
```
- std::string user_id
- std::string context - Context of this API call.

### splyt->NewUserCheckedAsync
Asynchronous counterpart to the above API function that adds a callback as the first required argument.
```c++
void NewUserCheckedAsync(NetworkCallback callback, std::string user_id, std::string context);
```


### splyt->NewDeviceChecked
Creates a new device if it does not already exist.
```c++
SplytResponse NewDeviceChecked(std::string device_id, std::string context);
```
- std::string device_id
- std::string context - Context of this API call.

### splyt->NewDeviceCheckedAsync
Asynchronous counterpart to the above API function that adds a callback as the first required argument.
```c++
void NewDeviceCheckedAsync(NetworkCallback callback, std::string device_id, std::string context);
```


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

### splyt->UpdateUserStateAsync
Asynchronous counterpart to the above API function that adds a callback as the first required argument.
```c++
void UpdateUserStateAsync(NetworkCallback callback, std::string user_id, std::string context, Json::Value properties = Json::Value::null);
```

### splyt->UpdateDeviceState
Updates a devices entity state.
```c++
SplytResponse UpdateDeviceState(std::string device_id, std::string context, Json::Value properties = Json::Value::null);
```
- std::string device_id
- std::string context - Context of this API call.
- Json::Value properties - Optional. Properties to set for this device, if not supplied properties are set to null.

### splyt->UpdateDeviceStateAsync
Asynchronous counterpart to the above API function that adds a callback as the first required argument.
```c++
void UpdateDeviceStateAsync(NetworkCallback callback, std::string device_id, std::string context, Json::Value properties = Json::Value::null);
```


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

### splyt->transaction->BeginAsync
Asynchronous counterpart to the above API function that adds a callback as the first required argument.
```c++
void BeginAsync(NetworkCallback callback, std::string transaction_id, std::string category, int timeout, std::string context, std::string user_id = "", std::string device_id = "", Json::Value properties = Json::Value::null);
```


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

### splyt->transaction->UpdateAsync
Asynchronous counterpart to the above API function that adds a callback as the first required argument.
```c++
void UpdateAsync(NetworkCallback callback, std::string transaction_id, std::string category, double progress, std::string context, std::string user_id = "", std::string device_id = "", Json::Value properties = Json::Value::null);
```


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

### splyt->transaction->EndAsync
Asynchronous counterpart to the above API function that adds a callback as the first required argument.
```c++
void EndAsync(NetworkCallback callback, std::string transaction_id, std::string category, std::string result, std::string context, std::string user_id = "", std::string device_id = "", Json::Value properties = Json::Value::null);
```


### splyt->transaction->BeginEnd
Begin and end a new transaction.
```c++
SplytResponse BeginEnd(std::string transaction_id, std::string category, std::string result, std::string context, std::string user_id = "", std::string device_id = "", Json::Value properties = Json::Value::null);
```
- std::string transaction_id
- std::string category - This transactions category.
- std::string result - Result for this transaction, can be anything. Ex. "success"
- std::string context - Context of this API call.
- std::string user_id - Optional. If not supplied will be set to the user_id passed during initialization.
- std::string device_id - Optional. If not supplied will be set to the device_id passed during initialization.
- Json::Value properties - Optional. Properties to set for this transaction, if not supplied properties are set to null. Note that properties will only be set for the end transaction HTTP request.

### splyt->transaction->BeginEndAsync
Asynchronous counterpart to the above API function that adds a callback as the first required argument.
```c++
void BeginEndAsync(NetworkCallback callback, std::string transaction_id, std::string category, std::string result, std::string context, std::string user_id = "", std::string device_id = "", Json::Value properties = Json::Value::null);
```


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

### splyt->tuning->RecordValueAsync
Asynchronous counterpart to the above API function that adds a callback as the first required argument.
```c++
void RecordValueAsync(NetworkCallback callback, std::string name, std::string default_value, std::string user_id = "", std::string device_id = "", std::string context = "defaultContext");
```

