#include <ShObjIdl.h>
#include <ShlGuid.h>
#include <windows.h>
#include <shobjidl.h>
#include <shlguid.h>
#include <objbase.h>
#include <objidl.h>
#include <strsafe.h>


bool ModifyShortcut(const wchar_t* shortcutPath, const wchar_t* newTargetPath) {
    HRESULT hres;
    IShellLink* psl;

    // COMライブラリの初期化
    hres = CoInitialize(NULL);
    if (FAILED(hres))
        return false;

    // IShellLinkインターフェイスの取得
    hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&psl);
    if (SUCCEEDED(hres)) {
        IPersistFile* ppf;

        // IPersistFileインターフェイスの取得
        hres = psl->QueryInterface(IID_IPersistFile, (void**)&ppf);
        if (SUCCEEDED(hres)) {
            // ショートカットファイルを読み込む
            hres = ppf->Load(shortcutPath, STGM_READWRITE);
            if (SUCCEEDED(hres)) {
                // 新しいターゲットパスを設定
                char newTargetPathMultiByte[MAX_PATH];
                WideCharToMultiByte(CP_ACP, 0, newTargetPath, -1, newTargetPathMultiByte, MAX_PATH, NULL, NULL);
                hres = psl->SetPath(newTargetPathMultiByte);
                if (SUCCEEDED(hres)) {
                    // ショートカットファイルを保存する
                    hres = ppf->Save(NULL, TRUE);
                }
            }
            ppf->Release();
        }
        psl->Release();
    }

    // COMライブラリの終了
    CoUninitialize();

    return SUCCEEDED(hres);
}