# Thread Hijacking 

> Shellcode execution without `CreateRemoteThread` hijack an existing thread, redirect `RIP`, let it do the work.

---

This project demonstrates **local thread execution hijacking** as a process injection primitive. Rather than spawning a new thread the technique allocates executable memory within the current process, copies position-independent shellcode into it, then locates a secondary thread via `CreateToolhelp32Snapshot`, suspends it, captures its full `CONTEXT`, redirects the `RIP` register to the shellcode address via `SetThreadContext`, and resumes the thread. A dedicated stack is allocated and 16-byte aligned before context is committed, preserving ABI requirements. 

The included payload is a standard x64 `calc.exe` shellcode demonstrating end-to-end execution. The main thread is excluded from hijacking by comparing against `GetCurrentThreadId()`, and memory permissions are staged `PAGE_READWRITE` during copy, then hardened to `PAGE_EXECUTE_READ` before the thread is released to avoid writing directly to executable pages.

---

## Source Layout

```
src/
├── main.cpp            # Shellcode staging
├── thread_hijack.cpp   # GetLocalThreadHandle + HijackThreadWithStack
└── thread_hijack.h     # Declarations
```


## References

- [MITRE ATT&CK — T1055.003: Thread Execution Hijacking](https://attack.mitre.org/techniques/T1055/003/)
- [`SetThreadContext` — Microsoft Docs](https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-setthreadcontext)
- [`CreateToolhelp32Snapshot` — Microsoft Docs](https://learn.microsoft.com/en-us/windows/win32/api/tlhelp32/nf-tlhelp32-createtoolhelp32snapshot)
- [Windows Internals, Part 1 — Russinovich et al.](https://learn.microsoft.com/en-us/sysinternals/)


