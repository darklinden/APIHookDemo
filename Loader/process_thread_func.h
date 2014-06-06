#include <iostream>


DWORD GetProcessIDByName(char * Aproname)
{
    HANDLE                  hThreadSnap = INVALID_HANDLE_VALUE; 
    PROCESSENTRY32          pe32;
    DWORD                   ret = 0;
    int                     cnt = 0;

    hThreadSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 ); 

    if( INVALID_HANDLE_VALUE == hThreadSnap ) 
    {
        OutputDebugStringA("CreateToolhelp32Snapshot Failed!");
    }
    else
    {
        //pe32.dwSize = sizeof( PROCESSENTRY32 ); 

        if( !Process32First( hThreadSnap, &pe32 ) ) 
        {
            OutputDebugStringA( "Process32First Failed!" );
        }
        else
        {
            do 
            { 
                if(! strcmp(pe32.szExeFile, Aproname) )
                {
                    char * outs = new char [0x200];
                    ZeroMemory(outs, 0x200);
                    sprintf_s(outs, 0x200, "Find Process %s with ID %ld", pe32.szExeFile, pe32.th32ProcessID);
                    OutputDebugStringA(outs);
                    ret = pe32.th32ProcessID;
                    cnt++;
                }
            } while( Process32Next( hThreadSnap, &pe32 ) ); 
        }
    }

    if (cnt > 1)
    {
        ret = - cnt;
    }

    CloseHandle( hThreadSnap );
    return ret;

}
//////////////////////////////////////////////////////////////////////////

DWORD tasktid = 0;

BOOL CALLBACK EnumWindowsProcGetThread( HWND hwnd, LPARAM lPa)
{
    DWORD tid = 0, pid = 0;

    tid = ::GetWindowThreadProcessId(hwnd, &pid);
    if ( pid == *(DWORD*)lPa)
    {
        tasktid = tid; 
    }    

    return TRUE;
}

DWORD GetProcessWindowThread(DWORD Apid)
{
    EnumWindows( EnumWindowsProcGetThread, (LPARAM)&Apid );	

    return tasktid;

}
//////////////////////////////////////////////////////////////////////////
DWORD GetWindowThreadByClass(char * AclassName)
{
    HWND	hwnd;
    DWORD	pid = 0, tid = 0;
    hwnd = FindWindowA( AclassName, NULL );

    if ( NULL == hwnd )
    {
        OutputDebugStringA("Such window not found!");
        return 0;
    }
    else
    {
        tid = GetWindowThreadProcessId( hwnd, &pid );
        if ( !tid )
        {
            OutputDebugStringA("Window thread not found!");
            return 0;
        } 
        else
        {
            return tid;
        }
    }
    


}