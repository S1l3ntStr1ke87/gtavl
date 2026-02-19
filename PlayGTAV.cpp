/* Copyright (C) 2020 Mr Goldberg
   This file is part of the Goldberg Emulator

   The Goldberg Emulator is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 3 of the License, or (at your option) any later version.

   The Goldberg Emulator is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the Goldberg Emulator; if not, see
   <http://www.gnu.org/licenses/>.  */

#include <windows.h>
#include <string>

int WinMain(
  HINSTANCE hInstance,
  HINSTANCE hPrevInstance,
  LPSTR     lpCmdLine,
  int       nShowCmd
)
//int main()
{
    std::string filename = "GTA5.exe";
    STARTUPINFOA lpStartupInfo;
    PROCESS_INFORMATION lpProcessInfo;

    ZeroMemory( &lpStartupInfo, sizeof( lpStartupInfo ) );
    lpStartupInfo.cb = sizeof( lpStartupInfo );
    ZeroMemory( &lpProcessInfo, sizeof( lpProcessInfo ) );

    char *commandLine = GetCommandLineA();
    if (*commandLine == '"') {
      commandLine++;
      while (*commandLine){
        if (*(commandLine++) == '"'){
          break;
        }
      }
    } else {
        while (*commandLine && *commandLine != ' ' && *commandLine != '\t') {
          commandLine++;
        }
    }

     CreateProcessA( NULL,
                const_cast<char *>(filename.append(commandLine).c_str()), NULL, NULL,
                NULL, NULL, NULL, NULL,
                &lpStartupInfo,
                &lpProcessInfo
                );

    HANDLE hPipe = INVALID_HANDLE_VALUE, hThread; 
    LPCTSTR lpszPipename = TEXT("\\\\.\\pipe\\GTAVLauncher_Pipe"); 
    hPipe = CreateNamedPipe(lpszPipename, PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_NOWAIT, PIPE_UNLIMITED_INSTANCES, 4096, 4096, 0, NULL);

    while (WAIT_TIMEOUT == WaitForSingleObject( lpProcessInfo.hProcess, 100 )) {
      DWORD cbBytesRead = 0;
      bool fSuccess = false;
      char output_buf[4096];
      fSuccess = ReadFile(hPipe, output_buf, sizeof(output_buf), &cbBytesRead, NULL);
      if (fSuccess && cbBytesRead) {
        uint32_t len = 0;
        memcpy(&len, output_buf, sizeof(len));
        std::string msg(output_buf + 4, len);
        if (msg.substr(0, 5) == "INIT:") {
          std::string output = std::string("TINI:launcher:") + msg.substr(5);
          uint32_t len = output.size();
          output = std::string((char *)&len, 4) + output;
          DWORD cbBytesRead = 0;
          WriteFile(hPipe, output.data(), output.size(), &cbBytesRead, NULL);
        } else {
          //??
        }
      }
    }

    // Close process and thread handles. 
    CloseHandle( lpProcessInfo.hProcess );
    CloseHandle( lpProcessInfo.hThread );
}
