#!/bin/bash

ADB="adb"
if [ "$ANDROID_HOME" != "" ]; then
	ADB="${ANDROID_HOME}/platform-tools/adb"
fi

${ADB} -s 10cc9b60 shell dumpsys meminfo ch.rptd.dragengine.deandroidlauncher

# end of script
