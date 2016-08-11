// LMS511_interface.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CLMS511_interfaceApp:
// See LMS511_interface.cpp for the implementation of this class
//

class CLMS511_interfaceApp : public CWinApp
{
public:
	CLMS511_interfaceApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CLMS511_interfaceApp theApp;