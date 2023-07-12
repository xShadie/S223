#include "StdAfx.h"

#ifdef INGAME_WIKI

#include "PythonApplication.h"

CPythonWikiRenderTarget::~CPythonWikiRenderTarget() {}
CPythonWikiRenderTarget::CPythonWikiRenderTarget()
{
	_bCanRenderModules = false;
	_RenderWikiModules.clear();
}

/*----------------------------
--------PUBLIC CLASS FUNCTIONS
-----------------------------*/

_wint32 CPythonWikiRenderTarget::GetFreeID()
{
	const size_t render_wiki_size = _RenderWikiModules.size();
	_wint32 new_module = START_MODULE;
	
	if (!render_wiki_size)
		return new_module;
	
	for (const auto &[module_id, module_wnd] : _RenderWikiModules) {
		if (module_id != new_module)
			break;
		
		++new_module;
	}
	
	return new_module;
}

void CPythonWikiRenderTarget::RegisterRenderModule(_wint32 module_id, _wint32 module_wnd)
{
	if (module_id == DELETE_PARM)
	{
		auto it = std::find_if(_RenderWikiModules.begin(), _RenderWikiModules.end(),
				[=](const std::tuple<_wint32, std::shared_ptr<UI::CUiWikiRenderTarget>>& _s)
				{
					return std::get<0>(_s) == module_id;
				} );
		
		if (it != _RenderWikiModules.end())
			_RenderWikiModules.erase(it);
		
		CWikiRenderTargetManager::Instance().DeleteRenderTarget(module_id);
	}
	else
	{
		if (!module_wnd)
			return;
		
		UI::CUiWikiRenderTarget* _hwn = (UI::CUiWikiRenderTarget*)module_wnd;
		_RenderWikiModules.emplace_back(std::make_tuple(module_id, _hwn));
		
		if (!_InitializeWindow(module_id, _hwn))
		{
			TraceError("RegisterRenderModule Cant Regist Module ID : %d", module_id);
			return;
		}
	}
}

void CPythonWikiRenderTarget::ManageModelViewVisibility(_wint32 module_id, bool flag)
{
	auto _Wrt = _GetRenderTargetHandle(module_id);
	if (!_Wrt)
		return;
	
	_Wrt->SetVisibility(flag);
}

bool CPythonWikiRenderTarget::CanRenderWikiModules() const
{
	bool _canRender = false;
	
	if (_bCanRenderModules)
	{
		const auto hWnd = CPythonApplication::Instance().GetWindowHandle();
		const auto isMinimized = static_cast<bool>(IsIconic(hWnd));
		_canRender = isMinimized ? false : true;
	}
	
	return _canRender;
}

void CPythonWikiRenderTarget::SetModelViewModel(_wint32 module_id, _wint32 module_vnum)
{
	auto _Wrt = _GetRenderTargetHandle(module_id);
	if (!_Wrt)
		return;
	
	_Wrt->SelectModel(module_vnum);
}

void CPythonWikiRenderTarget::SetWeaponModel(_wint32 module_id, _wint32 weapon_vnum)
{
	auto _Wrt = _GetRenderTargetHandle(module_id);
	if (!_Wrt)
		return;
	
	_Wrt->SetWeapon(weapon_vnum);
}

void CPythonWikiRenderTarget::SetModelForm(_wint32 module_id, _wint32 main_vnum)
{
	auto _Wrt = _GetRenderTargetHandle(module_id);
	if (!_Wrt)
		return;
	
	_Wrt->SetArmor(main_vnum);
}

void CPythonWikiRenderTarget::SetModelHair(_wint32 module_id, _wint32 hair_vnum)
{
	auto _Wrt = _GetRenderTargetHandle(module_id);
	if (!_Wrt)
		return;
	
	_Wrt->SetHair(hair_vnum);
}

void CPythonWikiRenderTarget::SetModelV3Eye(_wint32 module_id, _wfloat x, _wfloat y, _wfloat z)
{
	auto _Wrt = _GetRenderTargetHandle(module_id);
	if (!_Wrt)
		return;
	
	_Wrt->SetModelV3Eye(x, y, z);
}

void CPythonWikiRenderTarget::SetModelV3Target(_wint32 module_id, _wfloat x, _wfloat y, _wfloat z)
{
	auto _Wrt = _GetRenderTargetHandle(module_id);
	if (!_Wrt)
		return;
	
	_Wrt->SetModelV3Target(x, y, z);
}

/*----------------------------
------PROTECTED CLASS FUNCTIONS
-----------------------------*/

bool CPythonWikiRenderTarget::_InitializeWindow(_wint32 module_id, UI::CUiWikiRenderTarget* handle_window)
{
	if (!handle_window)
		return false;
	
	return handle_window->SetWikiRenderTargetModule(module_id);
}

std::shared_ptr<CWikiRenderTarget> CPythonWikiRenderTarget::_GetRenderTargetHandle(_wint32 module_id)
{
	auto it = std::find_if(_RenderWikiModules.begin(), _RenderWikiModules.end(),
			[=](const std::tuple<_wint32, std::shared_ptr<UI::CUiWikiRenderTarget>>& _s)
			{
				return std::get<0>(_s) == module_id;
			} );
	
	if (it == _RenderWikiModules.end())
		return nullptr;
	
	return CWikiRenderTargetManager::Instance().GetRenderTarget(module_id);
}
#endif
