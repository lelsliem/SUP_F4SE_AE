// symresolve — resolve RVAs in a PDB to function names + source lines via DIA.
// Usage: symresolve <path-to.pdb> <rva-hex> [rva-hex ...]
// Build:  vcvars64 && cl /nologo /O2 /EHsc symresolve.cpp /link /LIBPATH:"...\DIA SDK\lib\amd64" msdia140.lib

#include <windows.h>
#include <dia2.h>
#include <stdio.h>
#include <wchar.h>

// From the DIA SDK headers (GUIDs resolved via msdia140.lib).
static const CLSID CLSID_DiaSource = {0xe6756135, 0x1e65, 0x4d17, {0x85, 0x76, 0x61, 0x07, 0x61, 0x39, 0x8c, 0x3c}};

static IDiaDataSource* CreateDataSource()
{
    // Try the registered COM class first (VS installs usually register it).
    IDiaDataSource* src = nullptr;
    if (SUCCEEDED(CoCreateInstance(CLSID_DiaSource, nullptr, CLSCTX_INPROC_SERVER,
                                   __uuidof(IDiaDataSource), (void**)&src))) {
        return src;
    }
    // Fallback: load msdia140.dll manually from common VS locations.
    const wchar_t* candidates[] = {
        L"C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\DIA SDK\\bin\\amd64\\msdia140.dll",
        L"C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\Common7\\IDE\\msdia140.dll",
        L"C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\Common7\\IDE\\Automation\\msdia140.dll",
        L"C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\Common7\\IDE\\CommonExtensions\\Microsoft\\TestWindow\\VsTest\\TestHostNetFramework\\x64\\msdia140.dll",
        L"C:\\Program Files (x86)\\Microsoft Visual Studio\\2022\\Community\\Common7\\IDE\\msdia140.dll",
    };
    for (const wchar_t* path : candidates) {
        HMODULE mod = LoadLibraryW(path);
        if (!mod)
            continue;
        auto dllGetClassObject = (HRESULT(STDAPICALLTYPE*)(REFCLSID, REFIID, LPVOID*))
            GetProcAddress(mod, "DllGetClassObject");
        if (!dllGetClassObject)
            continue;
        IClassFactory* factory = nullptr;
        if (FAILED(dllGetClassObject(CLSID_DiaSource, IID_IClassFactory, (LPVOID*)&factory)))
            continue;
        HRESULT hr = factory->CreateInstance(nullptr, __uuidof(IDiaDataSource), (void**)&src);
        factory->Release();
        if (SUCCEEDED(hr))
            return src;
    }
    return nullptr;
}

static void PrintSymbol(IDiaSession* a_session, DWORD a_rva)
{
    IDiaSymbol* sym = nullptr;
    if (FAILED(a_session->findSymbolByRVA(a_rva, SymTagNull, &sym)) || !sym) {
        printf("  0x%08X  <no symbol>\n", a_rva);
        return;
    }

    // Walk up the lexical parents to the enclosing function.
    IDiaSymbol* fn = sym;
    for (;;) {
        DWORD tag = SymTagNull;
        fn->get_symTag(&tag);
        if (tag == SymTagFunction || tag == SymTagNull)
            break;
        IDiaSymbol* parent = nullptr;
        if (FAILED(fn->get_lexicalParent(&parent)) || !parent)
            break;
        fn = parent;
    }

    DWORD tag = SymTagNull;
    fn->get_symTag(&tag);
    if (tag == SymTagFunction) {
        BSTR name = nullptr;
        fn->get_name(&name);
        DWORD fRva = 0;
        ULONGLONG len = 0;
        fn->get_relativeVirtualAddress(&fRva);
        fn->get_length(&len);
        if (name) {
            printf("  0x%08X  %S (+0x%X into 0x%llX-byte function at RVA 0x%X)",
                   a_rva, name, a_rva - fRva, len, fRva);
            SysFreeString(name);
        } else {
            printf("  0x%08X  <anonymous function RVA 0x%X>", a_rva, fRva);
        }
    } else {
        printf("  0x%08X  <no enclosing function>", a_rva);
    }
    sym->Release();

    // Source line, if the PDB has one for this RVA.
    IDiaEnumLineNumbers* lines = nullptr;
    if (SUCCEEDED(a_session->findLinesByRVA(a_rva, 1, &lines)) && lines) {
        IDiaLineNumber* line = nullptr;
        ULONG fetched = 0;
        if (SUCCEEDED(lines->Next(1, &line, &fetched)) && fetched && line) {
            DWORD ln = 0;
            line->get_lineNumber(&ln);
            IDiaSourceFile* file = nullptr;
            line->get_sourceFile(&file);
            if (file) {
                BSTR fname = nullptr;
                file->get_fileName(&fname);
                if (fname) {
                    printf("  %S:%u", fname, ln);
                    SysFreeString(fname);
                }
                file->Release();
            }
            line->Release();
        }
        lines->Release();
    }
    printf("\n");
}

int wmain(int argc, wchar_t** argv)
{
    if (argc < 3) {
        printf("usage: symresolve <pdb> <rva-hex> [rva-hex ...]\n");
        return 1;
    }

    CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    IDiaDataSource* src = CreateDataSource();
    if (!src) {
        printf("ERROR: could not create IDiaDataSource (msdia140.dll not found/registered)\n");
        return 1;
    }

    HRESULT hr = FAILED(src->loadDataFromPdb(argv[1]))
                    ? src->loadDataForExe(argv[1], nullptr, nullptr)
                    : S_OK;
    if (FAILED(hr)) {
        printf("ERROR: loadDataFromPdb/loadDataForExe failed for %S\n", argv[1]);
        src->Release();
        return 1;
    }

    IDiaSession* session = nullptr;
    if (FAILED(src->openSession(&session)) || !session) {
        printf("ERROR: openSession failed\n");
        src->Release();
        return 1;
    }

    for (int i = 2; i < argc; i++) {
        DWORD rva = (DWORD)wcstoul(argv[i], nullptr, 16);
        PrintSymbol(session, rva);
    }

    session->Release();
    src->Release();
    CoUninitialize();
    return 0;
}
