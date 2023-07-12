#pragma once

#include "GrpBase.h"
#include "GrpTexture.h"
#include "../GameLib/in_game_wiki.h"

class CGraphicWikiRenderTargetTexture : public CGraphicTexture
{
	public:
		CGraphicWikiRenderTargetTexture();
		virtual	~CGraphicWikiRenderTargetTexture();
	
	public:
		bool				Create(_wint32 width, _wint32 height, D3DFORMAT texFormat, D3DFORMAT depthFormat);
		
		void				CreateTextures();
		bool				CreateRenderTexture(_wint32 width, _wint32 height, D3DFORMAT format);
		void				ReleaseTextures();
		LPDIRECT3DTEXTURE8	GetD3DRenderTargetTexture() const;
		
		bool				CreateRenderDepthStencil(_wint32 width, _wint32 height, D3DFORMAT format);
		
		void				SetRenderTarget();
		void				ResetRenderTarget();
		
		void				SetRenderingRect(RECT* rect);
		RECT*				GetRenderingRect();
		void				SetRenderingBox(RECT* renderBox);
		RECT*				GetRenderingBox();
		
		static void			Clear();
		void				Render() const;
	
	protected:
		void Reset();
	
	protected:
		LPDIRECT3DTEXTURE8	m_lpd3dRenderTexture{};
		LPDIRECT3DSURFACE8	m_lpd3dRenderTargetSurface{};
		LPDIRECT3DSURFACE8	m_lpd3dDepthSurface{};
		LPDIRECT3DSURFACE8	m_lpd3dOriginalRenderTarget{};
		LPDIRECT3DSURFACE8	m_lpd3dOldDepthBufferSurface{};
		
		D3DFORMAT			m_d3dFormat;
		D3DFORMAT			m_depthStencilFormat;
		
		RECT				m_renderRect{};
		RECT				m_renderBox{};
};
