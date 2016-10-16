#ifndef _WINDOW_H
#define _WINDOW_H

#include "System.h"

#ifndef _WINDOWS_
	#include <Windows.h>
#endif

#include "context.h"

struct WindowData : public SystemData
{
	WindowData();
	WindowData(int w, int h, const std::tstring& title = _T(""), int b = 32, bool fs = false);

	int width;
	int height;

	int bits;

	std::tstring title;

	bool fullscreen;
};

struct ResizeData
{
	ResizeData();
	ResizeData(bool resize, int nw, int nh);

	bool mustResize;

	int newWidth;
	int newHeight;
};

class Window :
	public System
{
	friend class Engine;

public:
	int GetWidth() const;
	int GetHeight() const;

	HWND GetWindowHandle() const;
	HDC GetDeviceContex() const;
	HINSTANCE GetInstance() const;

	ResizeData& GetResizeData() { return m_ResizeData; }
	LRESULT HandleEvent(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

protected:
	Window(const WindowData& data);
	virtual ~Window();

	bool Initialize() override;
	bool Update(Context& context) override;
	bool ShutDown() override;

private:
	bool CreatedWindow();
	LRESULT CALLBACK Window_Procedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

private:
	HWND m_hWindow;
	HDC m_hDC;
	HINSTANCE m_hInst;

	int m_Width;
	int m_Height;
	int m_Bits;
	bool m_FullScreen;

	ResizeData m_ResizeData;

	std::tstring m_Title;

};


#endif