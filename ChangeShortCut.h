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

    // COM���C�u�����̏�����
    hres = CoInitialize(NULL);
    if (FAILED(hres))
        return false;

    // IShellLink�C���^�[�t�F�C�X�̎擾
    hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&psl);
    if (SUCCEEDED(hres)) {
        IPersistFile* ppf;

        // IPersistFile�C���^�[�t�F�C�X�̎擾
        hres = psl->QueryInterface(IID_IPersistFile, (void**)&ppf);
        if (SUCCEEDED(hres)) {
            // �V���[�g�J�b�g�t�@�C����ǂݍ���
            hres = ppf->Load(shortcutPath, STGM_READWRITE);
            if (SUCCEEDED(hres)) {
                // �V�����^�[�Q�b�g�p�X��ݒ�
                char newTargetPathMultiByte[MAX_PATH];
                WideCharToMultiByte(CP_ACP, 0, newTargetPath, -1, newTargetPathMultiByte, MAX_PATH, NULL, NULL);
                hres = psl->SetPath(newTargetPathMultiByte);
                if (SUCCEEDED(hres)) {
                    // �V���[�g�J�b�g�t�@�C����ۑ�����
                    hres = ppf->Save(NULL, TRUE);
                }
            }
            ppf->Release();
        }
        psl->Release();
    }

    // COM���C�u�����̏I��
    CoUninitialize();

    return SUCCEEDED(hres);
}