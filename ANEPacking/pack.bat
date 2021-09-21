xcopy "..\NativeDiscordWrapper\bin\Debug\NativeDiscordWrapper.dll" /s /Y
xcopy "..\AIRDiscordLibrary\bin" /s /Y

copy AIRDiscordLibrary.swc AIRDiscordLibrary.zip
tar -xf AIRDiscordLibrary.zip
del AIRDiscordLibrary.zip
del catalog.xml

adt -package -target ane DiscordWrapperANE.ane AIRDiscordLibrary-extension.xml -swc AIRDiscordLibrary.swc -platform Windows-x86 library.swf NativeDiscordWrapper.dll