#pragma once

#include "../eterBase/Utils.h"
#ifdef INGAME_WIKI
#include <cstdint>
#endif

namespace UI
{
	class CWindow
	{
		public:
			typedef std::list<CWindow *> TWindowContainer;

			static DWORD Type();
			BOOL IsType(DWORD dwType);

			enum EHorizontalAlign
			{
				HORIZONTAL_ALIGN_LEFT = 0,
				HORIZONTAL_ALIGN_CENTER = 1,
				HORIZONTAL_ALIGN_RIGHT = 2,					
			};

			enum EVerticalAlign
			{
				VERTICAL_ALIGN_TOP = 0,
				VERTICAL_ALIGN_CENTER = 1,
				VERTICAL_ALIGN_BOTTOM = 2,
			};

			enum EFlags
			{
				FLAG_MOVABLE			= (1 <<  0),
				FLAG_LIMIT				= (1 <<  1),
				FLAG_SNAP				= (1 <<  2),
				FLAG_DRAGABLE			= (1 <<  3),
				FLAG_ATTACH				= (1 <<  4),
				FLAG_RESTRICT_X			= (1 <<  5),
				FLAG_RESTRICT_Y			= (1 <<  6),
				FLAG_NOT_CAPTURE		= (1 <<  7),
				FLAG_FLOAT				= (1 <<  8),
				FLAG_NOT_PICK			= (1 <<  9),
				FLAG_IGNORE_SIZE		= (1 << 10),
				FLAG_RTL				= (1 << 11),
			};

		public:
			CWindow(PyObject * ppyObject);
			virtual ~CWindow();

			void			AddChild(CWindow * pWin);

			void			Clear();
			void			DestroyHandle();
			void			Update();
			void			Render();

			void			SetName(const char * c_szName);
			const char *	GetName()		{ return m_strName.c_str(); }
			void			SetSize(long width, long height);
			long			GetWidth()		{ return m_lWidth; }
			long			GetHeight()		{ return m_lHeight; }

			void			SetHorizontalAlign(DWORD dwAlign);
			void			SetVerticalAlign(DWORD dwAlign);
			void			SetPosition(long x, long y);
			void			GetPosition(long * plx, long * ply);
			long			GetPositionX( void ) const		{ return m_x; }
			long			GetPositionY( void ) const		{ return m_y; }
			RECT &			GetRect()		{ return m_rect; }
			void			GetLocalPosition(long & rlx, long & rly);
			void			GetMouseLocalPosition(long & rlx, long & rly);
			long			UpdateRect();

			RECT &			GetLimitBias()	{ return m_limitBiasRect; }
			void			SetLimitBias(long l, long r, long t, long b) { m_limitBiasRect.left = l, m_limitBiasRect.right = r, m_limitBiasRect.top = t, m_limitBiasRect.bottom = b; }

			void			Show();
			void			Hide();
#ifdef INGAME_WIKI
			virtual	bool	IsShow();
			void			OnHideWithChilds();
			void			OnHide();
#else
			bool			IsShow() { return m_bShow; }
#endif
			bool			IsRendering();

			bool			HasParent()		{ return m_pParent ? true : false; }
			bool			HasChild()		{ return m_pChildList.empty() ? false : true; }
			int				GetChildCount()	{ return m_pChildList.size(); }

			CWindow *		GetRoot();
			CWindow *		GetParent();
#ifdef INGAME_WIKI
			bool			IsChild(CWindow* pWin, bool bCheckRecursive = false);
#else
			bool			IsChild(CWindow* pWin);
#endif
			void			DeleteChild(CWindow * pWin);
			void			SetTop(CWindow * pWin);

			bool			IsIn(long x, long y);
			bool			IsIn();
			CWindow *		PickWindow(long x, long y);
			CWindow *		PickTopWindow(long x, long y);

			void			__RemoveReserveChildren();

			void			AddFlag(DWORD flag)		{ SET_BIT(m_dwFlag, flag);		}
			void			RemoveFlag(DWORD flag)	{ REMOVE_BIT(m_dwFlag, flag);	}
			bool			IsFlag(DWORD flag)		{ return (m_dwFlag & flag) ? true : false;	}
#ifdef INGAME_WIKI
			void			SetInsideRender(BOOL flag);
			void			GetRenderBox(RECT* box);
			void			UpdateTextLineRenderBox();
			void			UpdateRenderBox();
			void			UpdateRenderBoxRecursive();
#endif

			void			SetDiffuseColor(float r, float g, float b, float a);

			virtual void	OnRender();
#ifdef INGAME_WIKI
			virtual void	OnAfterRender();
			virtual void	OnUpdateRenderBox() {}
#endif
			virtual void	OnUpdate();
			virtual void	OnChangePosition(){}

			virtual void	OnSetFocus();
			virtual void	OnKillFocus();

			virtual void	OnMouseDrag(long lx, long ly);
			virtual void	OnMouseOverIn();
			virtual void	OnMouseOverOut();
			virtual void	OnMouseOver();
			virtual void	OnDrop();
			virtual void	OnTop();
			virtual void	OnIMEUpdate();

			virtual void	OnMoveWindow(long x, long y);

			BOOL			RunIMETabEvent();
			BOOL			RunIMEReturnEvent();
			BOOL			RunIMEKeyDownEvent(int ikey);

			CWindow *		RunKeyDownEvent(int ikey);
			BOOL			RunKeyUpEvent(int ikey);
			BOOL			RunPressEscapeKeyEvent();
			BOOL			RunPressExitKeyEvent();

			virtual BOOL	OnIMETabEvent();
			virtual BOOL	OnIMEReturnEvent();
			virtual BOOL	OnIMEKeyDownEvent(int ikey);

			virtual BOOL	OnIMEChangeCodePage();
			virtual BOOL	OnIMEOpenCandidateListEvent();
			virtual BOOL	OnIMECloseCandidateListEvent();
			virtual BOOL	OnIMEOpenReadingWndEvent();
			virtual BOOL	OnIMECloseReadingWndEvent();

			virtual BOOL	OnMouseLeftButtonDown();
			virtual BOOL	OnMouseLeftButtonUp();
			virtual BOOL	OnMouseLeftButtonDoubleClick();
			virtual BOOL	OnMouseRightButtonDown();
			virtual BOOL	OnMouseRightButtonUp();
			virtual BOOL	OnMouseRightButtonDoubleClick();
			virtual BOOL	OnMouseMiddleButtonDown();
			virtual BOOL	OnMouseMiddleButtonUp();
			virtual BOOL	OnMouseWheel(int nLen);

			virtual BOOL	OnKeyDown(int ikey);
			virtual BOOL	OnKeyUp(int ikey);
			virtual BOOL	OnPressEscapeKey();
			virtual BOOL	OnPressExitKey();

			virtual void	SetColor(DWORD dwColor){}
			virtual BOOL	OnIsType(DWORD dwType);

			virtual BOOL	IsWindow() { return TRUE; }

#ifdef INGAME_WIKI
		public:
			virtual void	iSetRenderingRect(int iLeft, int iTop, int iRight, int iBottom);
			virtual void	SetRenderingRect(float fLeft, float fTop, float fRight, float fBottom);
			virtual int		GetRenderingWidth();
			virtual int		GetRenderingHeight();
			void			ResetRenderingRect(bool bCallEvent = true);

		private:
			virtual void	OnSetRenderingRect();
#endif

		protected:
			std::string			m_strName;

			EHorizontalAlign	m_HorizontalAlign;
			EVerticalAlign		m_VerticalAlign;
			long				m_x, m_y;
			long				m_lWidth, m_lHeight;
			RECT				m_rect;
			RECT				m_limitBiasRect;

#ifdef INGAME_WIKI
			RECT				m_renderingRect;
#endif
			bool				m_bMovable;
			bool				m_bShow;

			DWORD				m_dwFlag;			

			PyObject *			m_poHandler;

			CWindow	*			m_pParent;
			TWindowContainer	m_pChildList;

			BOOL				m_isUpdatingChildren;
#ifdef INGAME_WIKI
			BOOL				m_isInsideRender;
			RECT				m_renderBox;
#endif
			TWindowContainer	m_pReserveChildList;
		
#ifdef _DEBUG
		public:
			DWORD				DEBUG_dwCounter;
#endif
	};

	class CLayer : public CWindow
	{
		public:
			CLayer(PyObject * ppyObject) : CWindow(ppyObject) {}
			virtual ~CLayer() {}

			BOOL IsWindow() { return FALSE; }
	};

#ifdef INGAME_WIKI
	class CUiWikiRenderTarget : public CWindow
	{
		public:
			CUiWikiRenderTarget(PyObject* ppyObject);
			virtual ~CUiWikiRenderTarget();

		public:
			bool	SetWikiRenderTargetModule(int iRenderTargetModule);
			void	OnUpdateRenderBox();

		protected:
			void	OnRender();

		protected:
			DWORD	m_dwIndex;
	};
#endif

	class CBox : public CWindow
	{
		public:
			CBox(PyObject * ppyObject);
			virtual ~CBox();

			void SetColor(DWORD dwColor);

		protected:
			void OnRender();

		protected:
			DWORD m_dwColor;
	};

	class CBar : public CWindow
	{
		public:
			CBar(PyObject * ppyObject);
			virtual ~CBar();

			void SetColor(DWORD dwColor);

		protected:
			void OnRender();

		protected:
			DWORD m_dwColor;
	};

	class CLine : public CWindow
	{
		public:
			CLine(PyObject * ppyObject);
			virtual ~CLine();

			void SetColor(DWORD dwColor);

		protected:
			void OnRender();

		protected:
			DWORD m_dwColor;
	};

	class CBar3D : public CWindow
	{
		public:
			static DWORD Type();

		public:
			CBar3D(PyObject * ppyObject);
			virtual ~CBar3D();

			void SetColor(DWORD dwLeft, DWORD dwRight, DWORD dwCenter);

		protected:
			void OnRender();

		protected:
			DWORD m_dwLeftColor;
			DWORD m_dwRightColor;
			DWORD m_dwCenterColor;
	};

	class CTextLine : public CWindow
	{
#ifdef INGAME_WIKI
		public:
			static DWORD Type();
#endif

		public:
			CTextLine(PyObject * ppyObject);
			virtual ~CTextLine();

			void SetMax(int iMax);
			void SetHorizontalAlign(int iType);
			void SetVerticalAlign(int iType);
			void SetSecret(BOOL bFlag);
			void SetOutline(BOOL bFlag);
			void SetFeather(BOOL bFlag);
			void SetMultiLine(BOOL bFlag);
			void SetFontName(const char * c_szFontName);
			void SetFontColor(DWORD dwColor);
			void SetLimitWidth(float fWidth);
#ifdef INGAME_WIKI
			void SetFixedRenderPos(WORD startPos, WORD endPos) { m_TextInstance.SetFixedRenderPos(startPos, endPos); }
			void GetRenderPositions(WORD& startPos, WORD& endPos) { m_TextInstance.GetRenderPositions(startPos, endPos); }
#endif

			void ShowCursor();
			void HideCursor();
#ifdef INGAME_WIKI
			bool IsShowCursor();
#endif
			int GetCursorPosition();

			void SetText(const char * c_szText);
			const char * GetText();
			
			void GetTextSize(int* pnWidth, int* pnHeight);

#ifdef INGAME_WIKI
			bool IsShow();
			int GetRenderingWidth();
			int GetRenderingHeight();
			void OnSetRenderingRect();
#endif

		protected:
			void OnUpdate();
			void OnRender();
			void OnChangePosition();

			virtual void OnSetText(const char * c_szText);

#ifdef INGAME_WIKI
			void OnUpdateRenderBox() {
				UpdateTextLineRenderBox();
				m_TextInstance.SetRenderBox(m_renderBox);
			}
#endif

		protected:
			CGraphicTextInstance m_TextInstance;
	};

	class CNumberLine : public CWindow
	{
		public:
			CNumberLine(PyObject * ppyObject);
			CNumberLine(CWindow * pParent);
			virtual ~CNumberLine();

			void SetPath(const char * c_szPath);
			void SetHorizontalAlign(int iType);
			void SetNumber(const char * c_szNumber);

		protected:
			void ClearNumber();
			void OnRender();
			void OnChangePosition();

		protected:
			std::string m_strPath;
			std::string m_strNumber;
			std::vector<CGraphicImageInstance *> m_ImageInstanceVector;

			int m_iHorizontalAlign;
			DWORD m_dwWidthSummary;
	};

	class CImageBox : public CWindow
	{
		public:
			CImageBox(PyObject * ppyObject);
			virtual ~CImageBox();

#ifdef INGAME_WIKI
			void UnloadImage()
			{
				OnDestroyInstance();
				SetSize(GetWidth(), GetHeight());
				UpdateRect();
			}
#endif

			BOOL LoadImage(const char * c_szFileName);
			void SetDiffuseColor(float fr, float fg, float fb, float fa);

			int GetWidth();
			int GetHeight();

		protected:
			virtual void OnCreateInstance();
			virtual void OnDestroyInstance();

			virtual void OnUpdate();
			virtual void OnRender();
			void OnChangePosition();

		protected:
			CGraphicImageInstance * m_pImageInstance;
	};	
	class CMarkBox : public CWindow
	{
		public:
			CMarkBox(PyObject * ppyObject);
			virtual ~CMarkBox();

			void LoadImage(const char * c_szFilename);
			void SetDiffuseColor(float fr, float fg, float fb, float fa);
			void SetIndex(UINT uIndex);
			void SetScale(FLOAT fScale);
			void SetImageClipRect(float fLeft, float fTop, float fRight, float fBottom, bool bIsVertical = false);


		protected:
			virtual void OnCreateInstance();
			virtual void OnDestroyInstance();

			virtual void OnUpdate();
			virtual void OnRender();
			void OnChangePosition();
		protected:
			CGraphicMarkInstance * m_pMarkInstance;
	};
	class CExpandedImageBox : public CImageBox
	{
		public:
			static DWORD Type();

		public:
			CExpandedImageBox(PyObject * ppyObject);
			virtual ~CExpandedImageBox();

			void SetScale(float fx, float fy);
			void SetOrigin(float fx, float fy);
			void SetRotation(float fRotation);
			void SetRenderingRect(float fLeft, float fTop, float fRight, float fBottom);
			void SetRenderingMode(int iMode);
#ifdef INGAME_WIKI
			int GetRenderingWidth();
			int GetRenderingHeight();
			void OnSetRenderingRect();
			void SetExpandedRenderingRect(float fLeftTop, float fLeftBottom, float fTopLeft, float fTopRight, float fRightTop, float fRightBottom, float fBottomLeft, float fBottomRight);
			void SetTextureRenderingRect(float fLeft, float fTop, float fRight, float fBottom);
			DWORD GetPixelColor(DWORD x, DWORD y);
#endif

		protected:
			void OnCreateInstance();
			void OnDestroyInstance();
#ifdef INGAME_WIKI
			void OnUpdateRenderBox();
#endif

			virtual void OnUpdate();
			virtual void OnRender();

			BOOL OnIsType(DWORD dwType);
	};
	class CAniImageBox : public CWindow
	{
		public:
			static DWORD Type();

		public:
			CAniImageBox(PyObject * ppyObject);
			virtual ~CAniImageBox();

			void SetDelay(int iDelay);
			void AppendImage(const char * c_szFileName);
			void SetRenderingRect(float fLeft, float fTop, float fRight, float fBottom);
			void SetRenderingMode(int iMode);
			void SetDiffuseColor(float r, float g, float b, float a);

			void ResetFrame();

		protected:
			void OnUpdate();
			void OnRender();
			void OnChangePosition();
			virtual void OnEndFrame();

			BOOL OnIsType(DWORD dwType);

		protected:
			BYTE m_bycurDelay;
			BYTE m_byDelay;
			BYTE m_bycurIndex;
			std::vector<CGraphicExpandedImageInstance*> m_ImageVector;
	};

	class CButton : public CWindow
	{
		public:
			CButton(PyObject * ppyObject);
			virtual ~CButton();

			BOOL SetUpVisual(const char * c_szFileName);
			BOOL SetOverVisual(const char * c_szFileName);
			BOOL SetDownVisual(const char * c_szFileName);
			BOOL SetDisableVisual(const char * c_szFileName);

			const char * GetUpVisualFileName();
			const char * GetOverVisualFileName();
			const char * GetDownVisualFileName();

			void Flash();
			void Enable();
			void Disable();

			void SetUp();
			void Up();
			void Over();
			void Down();

			BOOL IsDisable();
			BOOL IsPressed();
#ifdef INGAME_WIKI
			void OnSetRenderingRect();
#endif

		protected:
			void OnUpdate();
			void OnRender();
			void OnChangePosition();

			BOOL OnMouseLeftButtonDown();
			BOOL OnMouseLeftButtonDoubleClick();
			BOOL OnMouseLeftButtonUp();
			void OnMouseOverIn();
			void OnMouseOverOut();

			BOOL IsEnable();
#ifdef INGAME_WIKI
			void SetCurrentVisual(CGraphicExpandedImageInstance* pVisual);
#else
			void SetCurrentVisual(CGraphicImageInstance* pVisual);
#endif

		protected:
			BOOL m_bEnable;
			BOOL m_isPressed;
			BOOL m_isFlash;

#ifdef INGAME_WIKI
			CGraphicExpandedImageInstance* m_pcurVisual;
			CGraphicExpandedImageInstance m_upVisual;
			CGraphicExpandedImageInstance m_overVisual;
			CGraphicExpandedImageInstance m_downVisual;
			CGraphicExpandedImageInstance m_disableVisual;
#else
			CGraphicImageInstance* m_pcurVisual;
			CGraphicImageInstance m_upVisual;
			CGraphicImageInstance m_overVisual;
			CGraphicImageInstance m_downVisual;
			CGraphicImageInstance m_disableVisual;
#endif
	};
	class CRadioButton : public CButton
	{
		public:
			CRadioButton(PyObject * ppyObject);
			virtual ~CRadioButton();

		protected:
			BOOL OnMouseLeftButtonDown();
			BOOL OnMouseLeftButtonUp();
			void OnMouseOverIn();
			void OnMouseOverOut();
	};
	class CToggleButton : public CButton
	{
		public:
			CToggleButton(PyObject * ppyObject);
			virtual ~CToggleButton();

		protected:
			BOOL OnMouseLeftButtonDown();
			BOOL OnMouseLeftButtonUp();
			void OnMouseOverIn();
			void OnMouseOverOut();
	};
	class CDragButton : public CButton
	{
		public:
			CDragButton(PyObject * ppyObject);
			virtual ~CDragButton();

			void SetRestrictMovementArea(int ix, int iy, int iwidth, int iheight);

		protected:
			void OnChangePosition();
			void OnMouseOverIn();
			void OnMouseOverOut();

		protected:
			RECT m_restrictArea;
	};
};

extern BOOL g_bOutlineBoxEnable;
