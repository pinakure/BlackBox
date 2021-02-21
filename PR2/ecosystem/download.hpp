#ifndef _DOWNLOAD_HPP
#define _DOWNLOAD_HPP

#include <windows.h>
#include <string>

class Download : public IBindStatusCallback{
private:
    int progress, filesize;
    int AbortDownload;
public:


    STDMETHOD(OnStartBinding)(DWORD dwReserved,IBinding __RPC_FAR* pib){
        AbortDownload = 0;
        progress = 0;
        filesize = 0;
        return E_NOTIMPL;
    }

    STDMETHOD(AbortDownl)() {
        AbortDownload = 1;
        return E_NOTIMPL;
    }

    void draw(std::string msg="Downloading");

    STDMETHOD(GetProgress)(){ return progress; }
    
    STDMETHOD(GetFileSize)(){ return filesize; }

    STDMETHOD(GetPriority)(LONG __RPC_FAR* pnPriority){return E_NOTIMPL;}

    STDMETHOD(OnLowResource)(DWORD reserved){return E_NOTIMPL;}
    STDMETHOD(OnProgress)(
        /* [in] */ ULONG ulProgress,
        /* [in] */ ULONG ulProgressMax,
        /* [in] */ ULONG ulStatusCode,
        /* [in] */ LPCWSTR wszStatusText);

    STDMETHOD(OnStopBinding)(
        /* [in] */ HRESULT hresult,
        /* [unique][in] */ LPCWSTR szError)
    {
        return E_NOTIMPL;
    }

    STDMETHOD(GetBindInfo)(
        /* [out] */ DWORD __RPC_FAR* grfBINDF,
        /* [unique][out][in] */ BINDINFO __RPC_FAR* pbindinfo)
    {
        return E_NOTIMPL;
    }

    STDMETHOD(OnDataAvailable)(
        /* [in] */ DWORD grfBSCF,
        /* [in] */ DWORD dwSize,
        /* [in] */ FORMATETC __RPC_FAR* pformatetc,
        /* [in] */ STGMEDIUM __RPC_FAR* pstgmed)
    {
        return E_NOTIMPL;
    }

    STDMETHOD(OnObjectAvailable)(
        /* [in] */ REFIID riid,
        /* [iid_is][in] */ IUnknown __RPC_FAR* punk)
    {
        return E_NOTIMPL;
    }

    // IUnknown methods.  Note that IE never calls any of these methods, since
    // the caller owns the IBindStatusCallback interface, so the methods all
    // return zero/E_NOTIMPL.

    STDMETHOD_(ULONG, AddRef)()
    {
        return 0;
    }

    STDMETHOD_(ULONG, Release)()
    {
        return 0;
    }

    STDMETHOD(QueryInterface)(
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ void __RPC_FAR* __RPC_FAR* ppvObject)
    {
        return E_NOTIMPL;
    }
};

#endif 