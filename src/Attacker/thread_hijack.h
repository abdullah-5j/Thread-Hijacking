#pragma once

#ifndef THREAD_HIJACK_H
#define THREAD_HIJACK_H

#include <Windows.h>
#include <TlHelp32.h>
#include <processthreadsapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>

extern unsigned char shellcode[];
extern SIZE_T shellcodeSize;

BOOL GetLocalThreadHandle(DWORD MainThreadID, DWORD* TargetThreadID, HANDLE* HThread);
BOOL HijackThreadWithStack(HANDLE HThread, PVOID PayloadAddress);

#endif
