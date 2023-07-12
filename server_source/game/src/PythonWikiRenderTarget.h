#pragma once

#include "../gamelib/in_game_wiki.h"
#include "../EterPythonLib/PythonWindow.h"
#include "../eterLib/CWikiRenderTargetManager.h"
#include "../eterLib/GrpWikiRenderTargetTexture.h"

class CPythonWikiRenderTarget : public CSingleton<CPythonWikiRenderTarget>
{
	public:
		CPythonWikiRenderTarget();
		virtual ~CPythonWikiRenderTarget();
	
	public:
		const static	_wint32 DELETE_PARM = -1;
		const static	_wint32 START_MODULE = 1;
		
		typedef std::vector<std::tuple<_wint32, std::shared_ptr<UI::CUiWikiRenderTarget>>> TWikiRenderTargetModules;
		
		_wint32		GetFreeID();
		void		RegisterRenderModule(_wint32 module_id, _wint32 module_wnd);
		
		void		ManageModelViewVisibility(_wint32 module_id, bool flag);
		
		void		ShowModelViewManager(bool flag) { _bCanRenderModules = flag; }
		bool		CanRenderWikiModules() const;
		
		void		SetModelViewModel(_wint32 module_id, _wint32 module_vnum);
		void		SetWeaponModel(_wint32 module_id, _wint32 weapon_vnum);
		void		SetModelForm(_wint32 module_id, _wint32 main_vnum);
		void		SetModelHair(_wint32 module_id, _wint32 hair_vnum);
		void		SetModelV3Eye(_wint32 module_id, _wfloat x, _wfloat y, _wfloat z);
		void		SetModelV3Target(_wint32 module_id, _wfloat x, _wfloat y, _wfloat z);
	
	protected:
		bool									_InitializeWindow(_wint32 module_id, UI::CUiWikiRenderTarget* handle_window);
		std::shared_ptr<CWikiRenderTarget>		_GetRenderTargetHandle(_wint32 module_id);
	
	private:
		TWikiRenderTargetModules				_RenderWikiModules;
		bool									_bCanRenderModules;
};
