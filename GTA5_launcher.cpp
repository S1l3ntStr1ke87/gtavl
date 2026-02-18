#include <windows.h>
#include <fstream>
#include <string>
#include <iostream>

// Function to check if a file exists
bool fileExists(const std::string& filePath) {
    std::ifstream file(filePath);
    return file.good();
}

// now we get to the good stuff, the main function, the function that will launch PlayGTAV.exe, that function
int main() {

	// Set the path that this executable is in as the working directory to make sure the PlayGTAV.exe is found
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string::size_type pos = std::string(buffer).find_last_of("\\/");
    std::string exePath;
    if (pos != std::string::npos) {
        exePath = std::string(buffer).substr(0, pos);
    }

	//this is only to make sure PlayGTAV.exe exists and is not used to launch the game, STARTUPINFOW is used to launch the game
    else {
        exePath = ".";
    }    std::string PlayGTAVExePath = exePath + "\\PlayGTAV.exe";

    // no PlayGTAV.exe? PANIC!!! SOUND THE FUCKING ALARM!!!! AAHHHHHHHHHHHHH!!!!!!!!
    if (!fileExists(PlayGTAVExePath)) {
        MessageBoxA(NULL, "Error: PlayGTAV.exe not found. Please place this file in the same directory as PlayGTAV.exe. It seems you did not do that.", "GTAV Launcher Error", MB_OK | MB_ICONERROR);
        return 1;
    }

    // the actual running of PlayGTAV.exe
    STARTUPINFOW si = { sizeof(si) };
    PROCESS_INFORMATION pi;

// Run PlayGTAV.exe with proper launch arguments to start up Grand Theft Auto V, NOTE: this is for the legacy edition.
    // CreateProcessW requires a modifiable buffer, so we create a writable wstring
 
    /* Please note that because this is the required launch method for this version,                         */
    /* the way that it works will cause your browser and / or AV to panic and say "virus!!! virussss!!11!!1" */
    /* There's likely no way to avoid this, sorry. I'll try my best tho. ~veeλnti<3                          */
    std::wstring commandLine = L"\"PlayGTAV.exe\" -nobattleye %*";
    
    // Convert working directory to wide string
    std::wstring wExePath(exePath.begin(), exePath.end());
    
    BOOL result = CreateProcessW(
        NULL,                           // lpApplicationName
        &commandLine[0],                // lpCommandLine (modifiable buffer)
        NULL,                           // lpProcessAttributes
        NULL,                           // lpThreadAttributes
        FALSE,                          // bInheritHandles
        0,                              // dwCreationFlags
        NULL,                           // lpEnvironment
        wExePath.c_str(),               // lpCurrentDirectory (set working directory)
        &si,                            // lpStartupInfo
        &pi                             // lpProcessInformation
    );

	// The horror! The absolute horror! IT FAILED TO LAUNCH PlayGTAV.EXE AHHHHHHHHHHHH!!!!!!!!
	// (i don't know why this happened, there's no way to know why it failed to launch either, so it's entirely on your end)
    if (!result) {
        DWORD error = GetLastError();
        std::cerr << "Error: Failed to launch PlayGTAV.exe. Error code: " << error << std::endl;
        system("pause");
        return 1;
    }
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return 0;
}
