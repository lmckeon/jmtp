# Getting Started with jMTP #
The first thing you need to do, is create a new PortableDeviceManager object to handle all the connected media players.  You can then iterate over all the devices:

```
PortableDeviceManager manager = new PortableDeviceManager();
for(PortableDevice device : manager) {
	//do something with the device...
}
```

If you want to access a device it is important that you open a connection to it first:

```
device.open()
```

Optionally you can use a special open version that allows you to authenticate you application to the device.
Next you can iterate over the files:

```
for(PortableDeviceObject object : device.getRootObjects()) {
	//do something with the root objects...
}
```

A PortableDeviceObject can always be of a more specialized type such as a storage: these are the root storage objects, most of the other root objects are virtual setting objects (for more information, please see the MSDN), you can check if an object is an storage object with:

```
If(object instanceof PortableDeviceStorageObject) {
	PortableDeviceStorageObject storage = (PortableDeviceStorageObject)object;
	//do something with the storage...
}
```

You can then retrieve the childobjects of the storage with the method getChildObjects(), child’s can be all kind of objects (folders, audio, unspecified objets, …) you can always use the instanceof to check the exact type of an object on the device.

If you encounter a problem or a bug: don't be afraid you may always contact me on: phd[.md](.md)ryc[.md](.md)ke’at’gm][ail’dot’com (remove all the [and ](.md) and change ‘at’ to @ and ‘dot’ to .)