#ifndef _OLEIDL_H
#define _OLEIDL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <objfwd.h>
typedef interface IParseDisplayname *LPPARSEDISPLAYNAME;
typedef interface IOleContainer *LPOLECONTAINER;
typedef interface IOleClientSite *LPOLECLIENTSITE;
typedef interface IOleObject *LPOLEOBJECT;
typedef interface IDropTarget *LPDROPTARGET;
typedef interface IDropSource *LPDROPSOURCE;
typedef interface IEnumOleDocumentViews *LPENUMOLEDOCUMENTVIEWS;
typedef interface IEnumOleUndoUnits *LPENUMOLEUNDOUNITS;
typedef interface IEnumOLEVERB *LPENUMOLEVERB;
typedef interface IOleWindow *LPOLEWINDOW;
typedef interface IOleInPlaceUIWindow *LPOLEINPLACEUIWINDOW;
typedef interface IOleInPlaceActiveObject *LPOLEINPLACEACTIVEOBJECT;
typedef interface IOleInPlaceFrame *LPOLEINPLACEFRAME;
typedef interface IOleAdviseHolder *LPOLEADVISEHOLDER;
typedef interface IViewObject *LPVIEWOBJECT;
typedef interface IViewObject2 *LPVIEWOBJECT2;

typedef enum tagOLEWHICHMK {
	OLEWHICHMK_CONTAINER=1,
	OLEWHICHMK_OBJREL,
	OLEWHICHMK_OBJFULL
} OLEWHICHMK;
typedef enum tagOLEGETMONIKER {
	OLEGETMONIKER_ONLYIFTHERE=1,
	OLEGETMONIKER_FORCEASSIGN,
	OLEGETMONIKER_UNASSIGN,
	OLEGETMONIKER_TEMPFORUSER
} OLEGETMONIKER;
typedef enum tagDROPEFFECT {
	DROPEFFECT_NONE=0,
	DROPEFFECT_COPY=1,
	DROPEFFECT_MOVE=2,
	DROPEFFECT_LINK=4,
	DROPEFFECT_SCROLL=0x80000000
} DROPEFFECT;
typedef struct tagOleMenuGroupWidths {
	LONG width[6];
} OLEMENUGROUPWIDTHS,*LPOLEMENUGROUPWIDTHS;
typedef HGLOBAL HOLEMENU;
typedef enum tagOLECLOSE {
	OLECLOSE_SAVEIFDIRTY,
	OLECLOSE_NOSAVE,
	OLECLOSE_PROMPTSAVE
} OLECLOSE;
typedef struct tagOLEVERB {
	LONG lVerb;
	LPWSTR lpszVerbName;
	DWORD fuFlags;
	DWORD grfAttribs;
} OLEVERB,*LPOLEVERB;
typedef RECT BORDERWIDTHS;
typedef LPRECT LPBORDERWIDTHS;
typedef LPCRECT LPCBORDERWIDTHS;
typedef struct tagOIFI {
	UINT cb;
	BOOL fMDIApp;
	HWND hwndFrame;
	HACCEL haccel;
	UINT cAccelEntries;
}OLEINPLACEFRAMEINFO,*LPOLEINPLACEFRAMEINFO;

/*DECLARE_ENUMERATOR_(IEnumOleDocumentViews,IOleDocumentView);*/
DECLARE_ENUMERATOR(OLEVERB);
typedef IEnumOLEVERB IEnumOleVerb;

#undef INTERFACE
#define INTERFACE IParseDisplayName
DECLARE_INTERFACE_(IParseDisplayName,IUnknown)
{
	STDMETHOD(QueryInterface)(THIS_ REFIID,PVOID*) PURE;
	STDMETHOD_(ULONG,AddRef)(THIS) PURE;
	STDMETHOD_(ULONG,Release)(THIS) PURE;
	STDMETHOD(ParseDisplayName)(THIS_ IBindCtx*,LPOLESTR,ULONG*,IMoniker**) PURE;
};

#undef INTERFACE
#define INTERFACE IOleContainer
DECLARE_INTERFACE_(IOleContainer,IParseDisplayName)
{
	STDMETHOD(QueryInterface)(THIS_ REFIID,PVOID*) PURE;
	STDMETHOD_(ULONG,AddRef)(THIS) PURE;
	STDMETHOD_(ULONG,Release)(THIS) PURE;
	STDMETHOD(ParseDisplayName)(THIS_ IBindCtx*,LPOLESTR,ULONG*,IMoniker**) PURE;
	STDMETHOD(EnumObjects)(THIS_ DWORD,IEnumUnknown**) PURE;
	STDMETHOD(LockContainer)(THIS_ BOOL) PURE;
};

#undef INTERFACE
#define INTERFACE IOleClientSite
DECLARE_INTERFACE_(IOleClientSite,IUnknown)
{
	STDMETHOD(QueryInterface)(THIS_ REFIID,PVOID*) PURE;
	STDMETHOD_(ULONG,AddRef)(THIS) PURE;
	STDMETHOD_(ULONG,Release)(THIS) PURE;
	STDMETHOD(SaveObject)(THIS) PURE;
	STDMETHOD(GetMoniker)(THIS_ DWORD,DWORD,LPMONIKER*) PURE;
	STDMETHOD(GetContainer)(THIS_ LPOLECONTAINER*) PURE;
	STDMETHOD(ShowObject)(THIS) PURE;
	STDMETHOD(OnShowWindow)(THIS_ BOOL) PURE;
	STDMETHOD(RequestNewObjectLayout)(THIS) PURE;
};

#undef INTERFACE
#define INTERFACE IOleObject
DECLARE_INTERFACE_(IOleObject,IUnknown)
{
	STDMETHOD(QueryInterface)(THIS_ REFIID,PVOID*) PURE;
	STDMETHOD_(ULONG,AddRef)(THIS) PURE;
	STDMETHOD_(ULONG,Release)(THIS) PURE;
	STDMETHOD(SetClientSite)(THIS_ LPOLECLIENTSITE) PURE;
	STDMETHOD(GetClientSite)(THIS_ LPOLECLIENTSITE*) PURE;
	STDMETHOD(SetHostNames)(THIS_ LPCOLESTR,LPCOLESTR) PURE;
	STDMETHOD(Close)(THIS_ DWORD) PURE;
	STDMETHOD(SetMoniker)(THIS_ DWORD,LPMONIKER) PURE;
	STDMETHOD(GetMoniker)(THIS_ DWORD,DWORD,LPMONIKER*) PURE;
	STDMETHOD(InitFromData)(THIS_ LPDATAOBJECT,BOOL,DWORD) PURE;
	STDMETHOD(GetClipboardData)(THIS_ DWORD,LPDATAOBJECT*) PURE;
	STDMETHOD(DoVerb)(THIS_ LONG,LPMSG,LPOLECLIENTSITE,LONG,HWND,LPCRECT) PURE;
	STDMETHOD(EnumVerbs)(THIS_ LPENUMOLEVERB*) PURE;
	STDMETHOD(Update)(THIS) PURE;
	STDMETHOD(IsUpToDate)(THIS) PURE;
	STDMETHOD(GetUserClassID)(THIS_ LPCLSID) PURE;
	STDMETHOD(GetUserType)(THIS_ DWORD,LPOLESTR) PURE;
	STDMETHOD(SetExtent)(THIS_ DWORD,SIZEL*) PURE;
	STDMETHOD(GetExtent)(THIS_ DWORD,SIZEL*) PURE;
	STDMETHOD(Advise)(THIS_ LPADVISESINK,PDWORD) PURE;
	STDMETHOD(Unadvise)(THIS_ DWORD) PURE;
	STDMETHOD(EnumAdvise)(THIS_ LPENUMSTATDATA*) PURE;
	STDMETHOD(GetMiscStatus)(THIS_ DWORD,PDWORD) PURE;
	STDMETHOD(SetColorScheme)(THIS_ LPLOGPALETTE) PURE;
};

#undef INTERFACE
#define INTERFACE IOleWindow
DECLARE_INTERFACE_(IOleWindow,IUnknown)
{
	STDMETHOD(QueryInterface)(THIS_ REFIID,PVOID*) PURE;
	STDMETHOD_(ULONG,AddRef)(THIS) PURE;
	STDMETHOD_(ULONG,Release)(THIS) PURE;
	STDMETHOD(GetWindow)(THIS_ HWND*) PURE;
	STDMETHOD(ContextSensitiveHelp)(THIS_ BOOL) PURE;
};

#undef INTERFACE
#define INTERFACE IOleInPlaceUIWindow
DECLARE_INTERFACE_(IOleInPlaceUIWindow,IOleWindow)
{
	STDMETHOD(QueryInterface)(THIS_ REFIID,PVOID*) PURE;
	STDMETHOD_(ULONG,AddRef)(THIS) PURE;
	STDMETHOD_(ULONG,Release)(THIS) PURE;
	STDMETHOD(GetWindow)(THIS_ HWND*) PURE;
	STDMETHOD(ContextSensitiveHelp)(THIS_ BOOL) PURE;
	STDMETHOD(GetBorder)(THIS_ LPRECT) PURE;
	STDMETHOD(RequestBorderSpace)(THIS_ LPCBORDERWIDTHS) PURE;
	STDMETHOD(SetBorderSpace)(THIS_ LPCBORDERWIDTHS) PURE;
	STDMETHOD(SetActiveObject)(THIS_ LPOLEINPLACEACTIVEOBJECT,LPCOLESTR) PURE;
};

#undef INTERFACE
#define INTERFACE IOleInPlaceActiveObject
DECLARE_INTERFACE_(IOleInPlaceActiveObject,IOleWindow)
{
	STDMETHOD(QueryInterface)(THIS_ REFIID,PVOID*) PURE;
	STDMETHOD_(ULONG,AddRef)(THIS) PURE;
	STDMETHOD_(ULONG,Release)(THIS) PURE;
	STDMETHOD(GetWindow)(THIS_ HWND*) PURE;
	STDMETHOD(ContextSensitiveHelp)(THIS_ BOOL) PURE;
	STDMETHOD(TranslateAccelerator)(THIS_ LPMSG) PURE;
	STDMETHOD(OnFrameWindowActivate)(THIS_ BOOL) PURE;
	STDMETHOD(OnDocWindowActivate)(THIS_ BOOL) PURE;
	STDMETHOD(ResizeBorder)(THIS_ LPCRECT,LPOLEINPLACEUIWINDOW,BOOL) PURE;
	STDMETHOD(EnableModeless)(THIS_ BOOL) PURE;
};

#undef INTERFACE
#define INTERFACE IOleInPlaceFrame
DECLARE_INTERFACE_(IOleInPlaceFrame,IOleInPlaceUIWindow)
{
	STDMETHOD(QueryInterface)(THIS_ REFIID,PVOID*) PURE;
	STDMETHOD_(ULONG,AddRef)(THIS) PURE;
	STDMETHOD_(ULONG,Release)(THIS) PURE;
	STDMETHOD(GetWindow)(THIS_ HWND*) PURE;
	STDMETHOD(ContextSensitiveHelp)(THIS_ BOOL) PURE;
	STDMETHOD(GetBorder)(THIS_ LPRECT) PURE;
	STDMETHOD(RequestBorderSpace)(THIS_ LPCBORDERWIDTHS) PURE;
	STDMETHOD(SetBorderSpace)(THIS_ LPCBORDERWIDTHS) PURE;
	STDMETHOD(SetActiveObject)(THIS_ LPOLEINPLACEACTIVEOBJECT,LPCOLESTR) PURE;
	STDMETHOD(InsertMenus)(THIS_ HMENU,LPOLEMENUGROUPWIDTHS) PURE;
	STDMETHOD(SetMenu)(THIS_ HMENU,HOLEMENU,HWND) PURE;
	STDMETHOD(RemoveMenus)(THIS_ HMENU) PURE;
	STDMETHOD(SetStatusText)(THIS_ LPCOLESTR) PURE;
	STDMETHOD(EnableModeless)(THIS_ BOOL) PURE;
	STDMETHOD(TranslateAccelerator)(THIS_ LPMSG,WORD) PURE;
};

#undef INTERFACE
#define INTERFACE IOleAdviseHolder
DECLARE_INTERFACE_(IOleAdviseHolder,IUnknown)
{
	STDMETHOD(QueryInterface)(THIS_ REFIID,PVOID*) PURE;
	STDMETHOD_(ULONG,AddRef)(THIS) PURE;
	STDMETHOD_(ULONG,Release)(THIS) PURE;
	STDMETHOD(Advise)(THIS_ LPADVISESINK,PDWORD) PURE;
	STDMETHOD(Unadvise)(THIS_ DWORD) PURE;
	STDMETHOD(EnumAdvise)(THIS_ LPENUMSTATDATA*) PURE;
	STDMETHOD(SendOnRename)(THIS_ LPMONIKER) PURE;
	STDMETHOD(SendOnSave)(THIS) PURE;
	STDMETHOD(SendOnClose)(THIS) PURE;
};

#undef INTERFACE
#define INTERFACE IDropSource
DECLARE_INTERFACE_(IDropSource,IUnknown)
{
	STDMETHOD(QueryInterface)(THIS_ REFIID,PVOID*) PURE;
	STDMETHOD_(ULONG,AddRef)(THIS) PURE;
	STDMETHOD_(ULONG,Release)(THIS) PURE;
	STDMETHOD(QueryContinueDrag)(THIS_ BOOL,DWORD) PURE;
	STDMETHOD(GiveFeedback)(THIS_ DWORD);
};

#undef INTERFACE
#define INTERFACE IDropTarget
DECLARE_INTERFACE_(IDropTarget,IUnknown)
{
	STDMETHOD(QueryInterface)(THIS_ REFIID,PVOID*) PURE;
	STDMETHOD_(ULONG,AddRef)(THIS) PURE;
	STDMETHOD_(ULONG,Release)(THIS) PURE;
	STDMETHOD(DragEnter)(THIS_ LPDATAOBJECT,DWORD,POINTL,PDWORD) PURE;
	STDMETHOD(DragOver)(THIS_ DWORD,POINTL,PDWORD) PURE;
	STDMETHOD(DragLeave)(THIS) PURE;
	STDMETHOD(Drop)(THIS_ LPDATAOBJECT,DWORD,POINTL,PDWORD) PURE;
};

typedef BOOL(CALLBACK *__IView_pfncont)(DWORD);
#undef INTERFACE
#define INTERFACE IViewObject
DECLARE_INTERFACE_(IViewObject,IUnknown)
{
	STDMETHOD(QueryInterface)(THIS_ REFIID,PVOID*) PURE;
	STDMETHOD_(ULONG,AddRef)(THIS) PURE;
	STDMETHOD_(ULONG,Release)(THIS) PURE;
	STDMETHOD(Draw)(THIS_ DWORD,LONG,PVOID,DVTARGETDEVICE*,HDC,HDC,LPCRECTL,LPCRECTL,__IView_pfncont pfnContinue,DWORD) PURE;
	STDMETHOD(GetColorSet)(THIS_ DWORD,LONG,PVOID,DVTARGETDEVICE*,HDC,LPLOGPALETTE*) PURE;
	STDMETHOD(Freeze)(THIS_ DWORD,LONG,PVOID,PDWORD) PURE;
	STDMETHOD(Unfreeze)(THIS_ DWORD);
	STDMETHOD(SetAdvise)(THIS_ DWORD,DWORD,IAdviseSink*) PURE;
	STDMETHOD(GetAdvise)(THIS_ PDWORD,PDWORD,IAdviseSink**) PURE;
};

#undef INTERFACE
#define INTERFACE IViewObject2
DECLARE_INTERFACE_(IViewObject2,IUnknown)
{
	STDMETHOD(QueryInterface)(THIS_ REFIID,PVOID*) PURE;
	STDMETHOD_(ULONG,AddRef)(THIS) PURE;
	STDMETHOD_(ULONG,Release)(THIS) PURE;
	STDMETHOD(Draw)(THIS_ DWORD,LONG,PVOID,DVTARGETDEVICE*,HDC,HDC,LPCRECTL,LPCRECTL,__IView_pfncont pfnContinue,DWORD) PURE;
	STDMETHOD(GetColorSet)(THIS_ DWORD,LONG,PVOID,DVTARGETDEVICE*,HDC,LPLOGPALETTE*) PURE;
	STDMETHOD(Freeze)(THIS_ DWORD,LONG,PVOID,PDWORD) PURE;
	STDMETHOD(Unfreeze)(THIS_ DWORD);
	STDMETHOD(SetAdvise)(THIS_ DWORD,DWORD,IAdviseSink*) PURE;
	STDMETHOD(GetAdvise)(THIS_ PDWORD,PDWORD,IAdviseSink**) PURE;
	STDMETHOD(GetExtent)(THIS_ DWORD,DWORD,DVTARGETDEVICE*,LPSIZEL) PURE;
};

#undef INTERFACE
#define INTERFACE ISupportErrorInfo
DECLARE_INTERFACE_(ISupportErrorInfo, IUnknown)
{
	STDMETHOD(QueryInterface)(THIS_ REFIID,PVOID*) PURE;
	STDMETHOD_(ULONG,AddRef)(THIS) PURE;
	STDMETHOD_(ULONG,Release)(THIS) PURE;
	STDMETHOD(InterfaceSupportsErrorInfo)(THIS_ REFIID) PURE;

};

#undef INTERFACE
#define INTERFACE IErrorInfo
DECLARE_INTERFACE_(IErrorInfo, IUnknown)
{
	STDMETHOD(QueryInterface)(THIS_ REFIID,PVOID*) PURE;
	STDMETHOD_(ULONG,AddRef)(THIS) PURE;
	STDMETHOD_(ULONG,Release)(THIS) PURE;

	STDMETHOD(GetGUID)(THIS_ REFIID) PURE;
	STDMETHOD(GetSource)(THIS_ BSTR*) PURE;
	STDMETHOD(GetDescription)(THIS_ BSTR*) PURE;
	STDMETHOD(GetHelpContext)(THIS_ DWORD*) PURE;

 };

#ifdef __cplusplus
}
#endif
#endif

