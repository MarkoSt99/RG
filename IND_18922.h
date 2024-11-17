
// IND_18922.h : main header file for the IND_18922 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CIND18922App:
// See IND_18922.cpp for the implementation of this class
//

class CIND18922App : public CWinApp
{
public:
	CIND18922App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CIND18922App theApp;
