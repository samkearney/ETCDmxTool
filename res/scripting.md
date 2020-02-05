# Scripting with ETCDMXTool

ETCDmxTool supports *scripting* - the ability to write a simple program within the application that will control DMX outputs. This can be useful for automating tests or tedious or long running tasks.

In addition to the basics of Javascript, ETCDmxTool allows you to:
* Send DMX levels
* Wait for a period of time
* Read or write data from a serial port on your PC
* Write logging data to a file

The scripting is based on ECMAScript (Javascript), which is a well known language used in web pages. If you're not familiar with Javascript, visit https://www.w3schools.com/js/ for a short introduction.

This document assumes you have some familiarity with Javascript, and will just describe the specialized functions available with ETCDmxTool.

The specialist functions are provided through four objects : dmx, [utils](Utils), serial, file and console. We will cover each of these below.

## Console

The *console* object provides the ability to output data to the debugging console at the bottom of the script screen. This can be useful when testing your script, to inspect variable values. For example, the script:

    for(i=0; i<4; i++>)
    {
        console.log("i is : " + i);
    }

will output to the log window:

    i is 0
    i is 1
    i is 2
    i is 3
    i is 4

The `console.log` function outputs the supplied data to the console

## Utils

The *utils* object provides general utilities for scripts.

### utils.sleep

The `utils.sleep` function pauses the script for the specified amount of time, in milliseconds. For example:


    for(i=0; i<4; i++>)
    {
        console.log("i is : " + i);
        utils.sleep(1000);
    }

This script will output the same as above, but with a 1 second pause between each line of output.

### utils.timestamp

The `utils.timestamp` function returns the current time as a string. For example:

    console.log(utils.timestamp());

Might print out `Mon Jan 13 13:07:13 2020` to the console.

## DMX

The DMX object provides access to DMX using the DMX output device. At the moment, only DMX output is supported.

### dmx.enable
The `dmx.enable` function enables the DMX output from the capture device. The device will begin outputting DMX once this is called.

### dmx.disable
The `dmx.disable` function disables the DMX output from the capture device. The device will stop outputting DMX once this is called.

### dmx.setLevel
The `dmx.setLevel` function sets a level for a particular DMX address in the output. DMX addresses are counted from 1; and levels are specified as 0-255, not percent. So for example:

    dmx.enable();
    dmx.setLevel(1, 0);
    dmx.setLevel(2, 255);
    dmx.setLevel(3, 128);
    dmx.setLevel(4, 255);

Would set DMX address 1 to 0%, 2 to 100%, 3 to 50% and 4 to 100%.

## File
The `file` object allows writing of data to a file, for example to log data from a test.

Before using the `file` object, in the application you will need to press the File button at the top of the screen to select the file location to save data to.

## Serial