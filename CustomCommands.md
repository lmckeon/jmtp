# Introduction #

The Windows Portable Device API's allows sending custom commands directly to the driver. JMTP supports this functionality since beta 2. This advanced functionality is (currently) only supported on Windows.

# Details #

To be able to send custom commands, the PortableDevice object must be cast to an PortableDeviceImplWin32 object. Then the sendCommand method can be used. This method accepts a PortableDeviceValuesImpl32 collection and also returns a PortableDeviceValuesImpl32 which is Java implementations of an interface found in the Windows SDK (for more information about this and related classes, please read the information provided in the Windows SDK). The COMExceptions thrown are wrapped HRESULT status codes.

All the GUID’s and PropertyKey’s found in “PortableDeviceApi.h”, are defined in the java class jmtp.Win32WPDDefines. This allows easy use of all this values.

Please note: this is advanced functionality. You must be familiar with both the mtp-protocol and the WPD-API to properly use this functionality. For most applictions this functionality is not required.

# Example #

```
PortableDeviceManager manager;
PortableDevice device;
PortableDeviceImplWin32 device32;
PortableDeviceValuesImplWin32 input;
PortableDeviceValuesImplWin32 results;
COMException wpdError;
long driverError;
    	
manager = new PortableDeviceManager();
device = manager.getDevices()[0];
device.open();

PropertyKey commandKey = Win32WPDDefines.WPD_COMMAND_COMMON_RESET_DEVICE;
    	
device32 = (PortableDeviceImplWin32)device;
try {
    input = new PortableDeviceValuesImplWin32();
    input.setGuidValue(Win32WPDDefines.WPD_PROPERTY_COMMON_COMMAND_CATEGORY, 
        commandKey.getFmtid());
    input.setUnsignedIntegerValue(Win32WPDDefines.WPD_PROPERTY_COMMON_COMMAND_ID, 
        commandKey.getPid());
    		
    results = device32.sendCommand(input);
    		
    //check for success or failure to carry out the command
    try {
    	wpdError = results.getErrorValue(Win32WPDDefines.WPD_PROPERTY_COMMON_HRESULT);
    }
    catch(COMException e) {
    	//ignore exception if "ERROR_NOT_FOUND" -> item not in collection
    	if (e.getHresult() != Win32WPDDefines.ERROR_NOT_FOUND) {
    		System.out.println("Error: " + e.getErrorCode());
    	}
    }

    //check driver-specific error code
    try {
    	driverError = results.getUnsignedIntegerValue(Win32WPDDefines.WPD_PROPERTY_COMMON_DRIVER_ERROR_CODE);
    	System.out.println("Driver Error Code: " + driverError);
    }
    catch(COMException e) {
    	//ignore exception if "ERROR_NOT_FOUND" -> item not in collection
    	if (e.getHresult() != Win32WPDDefines.ERROR_NOT_FOUND) {
    		System.out.println("Error: " + e.getErrorCode());
    	}
    }
}
catch(COMException e) {
    System.out.println("Error: " + e.getErrorCode());
}
```