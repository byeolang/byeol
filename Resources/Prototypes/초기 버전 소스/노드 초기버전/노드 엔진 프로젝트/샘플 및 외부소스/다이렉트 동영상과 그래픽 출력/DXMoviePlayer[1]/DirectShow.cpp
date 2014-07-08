#include "DirectShow.h"

#define REGISTER_FILTERGRAPH
DWORD     g_dwGraphRegister=0;
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// ������
// Parameters :
// Return :
///////////////////////////////////////////////////////////////////////////////////////////////////////////
CDirectShow::CDirectShow()
{
	Initial();
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// �Ҹ���
// Parameters :
// Return :
///////////////////////////////////////////////////////////////////////////////////////////////////////////
CDirectShow::~CDirectShow()
{
	Release();	
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���̷�Ʈ� �ʱ�ȭ�Ѵ�~
// Parameters : ����.
// Return : ����.
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDirectShow::Initial()
{
	m_pGB = NULL;
	m_pMC = NULL;
	m_pME = NULL;
	m_pVW = NULL;
	m_pBA = NULL;
	m_pBV = NULL;
	m_pMS = NULL;
	m_pMP = NULL;
	m_pFS = NULL;

	m_dwStream = NULL;
	m_lVolume = VOLUME_FULL;	/* volume full */
	m_CPState = STOP;	/* clip play state = stop */
	m_bReady = FALSE;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���̷�Ʈ� �����Ѵ�~
// Parameters : ����.
// Return : ����.
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDirectShow::Release()
{
	if( m_pVW )
    {
        m_pVW->put_Visible( OAFALSE );
        m_pVW->put_Owner( (OAHWND)m_hWnd );
    }


#ifdef REGISTER_FILTERGRAPH
    if (g_dwGraphRegister)
    {
        RemoveGraphFromRot( g_dwGraphRegister );
        g_dwGraphRegister = 0;
    }
#endif

	if( m_pGB ) m_pGB->Release();
	if( m_pMC ) m_pMC->Release();
	if( m_pME ) m_pME->Release();
	if( m_pVW ) m_pVW->Release();
	if( m_pBA ) m_pBA->Release();
	if( m_pBV ) m_pBV->Release();
	if( m_pMS ) m_pMS->Release();
	if( m_pMP ) m_pMP->Release();
	if( m_pFS ) m_pFS->Release();

	m_pGB = NULL;
	m_pMC = NULL;
	m_pME = NULL;
	m_pVW = NULL;
	m_pBA = NULL;
	m_pBV = NULL;
	m_pMS = NULL;
	m_pMP = NULL;
	m_pFS = NULL;


	CoUninitialize();
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// ������ �Ǵ� ���������� �÷����Ѵ�~
// Parameters : (DWORD) STREAM TYPE
// Return :
///////////////////////////////////////////////////////////////////////////////////////////////////////////
CDirectShow::CDirectShow(DWORD dwStream)
{
	Initial();
	m_dwStream = dwStream;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// szFile�� ���� �÷����Ѵ�~
// Parameters : (LPTSTR)���� ���
// Return : (BOOL)
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <tchar.h>
#include <stdio.h>
BOOL CDirectShow::PlayClip(HWND hWnd, LPTSTR szFile)
{
	if( m_bReady == TRUE )
	{
		Release();
		m_bReady = FALSE;
	}


	HRESULT hResult;
	WCHAR wPath[MAX_PATH];
	

	m_hWnd = hWnd;
	UpdateWindow( hWnd );
	MultiByteToWideChar( CP_ACP, 0, szFile, -1, wPath, MAX_PATH );


	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	// COM�ʱ�ȭ~
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	hResult = CoInitialize( NULL );

	if( FAILED(hResult) )
	{
		::MessageBox( m_hWnd, "COM�ʱ�ȭ�� �����߽��ϴ�", "�˸��ϴ�", MB_OK );
		return FALSE;
	}


	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	// IID_IGraphBuilder�������̽� ����
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	hResult = CoCreateInstance( CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**)&m_pGB );


	if( FAILED(hResult) )
	{
		::MessageBox( m_hWnd, "���̷�Ʈ��� IID_IGraphBuilder ������ �����߽��ϴ�", "�˸��ϴ�", MB_OK );
		return FALSE;
	}

	if( SUCCEEDED(hResult) )
	{
		hResult = m_pGB->RenderFile( wPath, NULL );
		if( FAILED(hResult) )
		{
			::MessageBox( m_hWnd, "���̷�Ʈ��� RenderFile�� �����߽��ϴ�(CODE�� �����ϴ�)", "�˸��ϴ�", MB_OK );
			return FALSE;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////
		// QueryInterface for DirectShow interfaces
		///////////////////////////////////////////////////////////////////////////////////////////////////////////
		m_pGB->QueryInterface( IID_IMediaControl, (void**)&m_pMC );	/* media control */		
		m_pGB->QueryInterface( IID_IMediaEventEx, (void**)&m_pME );	/* media event ex */		
		m_pGB->QueryInterface( IID_IMediaSeeking, (void**)&m_pMS );	/* media seeking */		
		m_pGB->QueryInterface( IID_IMediaPosition, (void**)&m_pMP );	/* media position */

		///////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Query for video interfaces, which may not be relevant for audio files
		///////////////////////////////////////////////////////////////////////////////////////////////////////////
		m_pGB->QueryInterface( IID_IVideoWindow, (void **)&m_pVW );	/* video window */
        m_pGB->QueryInterface( IID_IBasicVideo, (void **)&m_pBV );	/* basic video */
		
		///////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Query for audio interfaces, which may not be relevant for video-only files
		///////////////////////////////////////////////////////////////////////////////////////////////////////////
        m_pGB->QueryInterface( IID_IBasicAudio, (void **)&m_pBA );	/* basic audio */

		m_pME->SetNotifyWindow( (OAHWND)m_hWnd, WM_GRAPHNOTIFY, 0 );
	}


	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ����Ʈ���� ����~
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	if( m_dwStream == STREAM_TYPE_MOVIE )
	{
		m_pVW->put_Owner( (OAHWND)m_hWnd );
		m_pVW->put_WindowStyle( WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN );
		InitVideoWindow( 1, 1 );
		GetFrameStepInterface();
	}

	
	ShowWindow( hWnd, SW_SHOWNORMAL );
    UpdateWindow( hWnd );
    SetForegroundWindow( hWnd );
    SetFocus( hWnd );


#ifdef REGISTER_FILTERGRAPH
	hResult = AddGraphToRot( m_pGB, &g_dwGraphRegister );
	if( FAILED(hResult) )
	{
		g_dwGraphRegister = 0;
	}
#endif

	m_bReady = TRUE;
	hResult = m_pMC->Run();

	if( FAILED( hResult) )
	{
		::MessageBox( m_hWnd, "����Ŭ���� �÷��� �� �� �����ϴ�", "�˸��ϴ�", MB_OK );
		return FALSE;
	}
	m_CPState = RUNNING;
	
	
	/*
    LONG lMode;
    static HWND hDrain=0;
	
	// Read current state
	m_pVW->get_FullScreenMode(&lMode);

	if (lMode == OATRUE)
	{
		// Save current message drain
		m_pVW->get_MessageDrain((OAHWND *) &hDrain);

		// Set message drain to application main window
		m_pVW->put_MessageDrain((OAHWND) m_hWnd);

		// Switch to full-screen mode
		lMode = OATRUE;
		m_pVW->put_FullScreenMode(lMode);
	}
	else
	{
		// Switch back to windowed mode
		lMode = OAFALSE;
		m_pVW->put_FullScreenMode(lMode);

		// Undo change of message drain
		m_pVW->put_MessageDrain((OAHWND) hDrain);

		// Reset video window
		m_pVW->SetWindowForeground(-1);

		// Reclaim keyboard focus for player application
		UpdateWindow(m_hWnd);
		SetForegroundWindow(m_hWnd);
		SetFocus(m_hWnd);
	}
	*/
	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// �غ�� Ŭ���� �÷����Ѵ�
// Parameters : ����.
// Return : (BOOL)
///////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDirectShow::PlayClip()
{
	HRESULT hResult;
	if( (!m_pMC) || (!m_pMS) )
		return FALSE;

	hResult = m_pMC->Run();

	if( FAILED( hResult) )
	{
		m_CPState = STOP;
		::MessageBox( m_hWnd, "����Ŭ���� �÷��� �� �� �����ϴ�", "�˸��ϴ�", MB_OK );
		return FALSE;
	}

	m_CPState = RUNNING;
	return TRUE;
}


#define	SAFE_RELEASE(X)		if (X)	{	X->Release();	X = NULL;	}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// �������� Ŭ���� �����
// Parameters : ����.
// Return : ����.
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDirectShow::StopClip()
{
	HRESULT hResult;

	if( (!m_pMC) || (!m_pMS) )
		return ;

	if( m_dwStream == STREAM_TYPE_MOVIE )
	{
		if( (m_CPState == PAUSE) || (m_CPState == RUNNING) )
		{
			LONGLONG pos = 0;
			hResult = m_pMC->Stop();
			m_CPState = STOP;

			m_pVW->put_Visible(OAFALSE);
			m_pVW->put_Owner( (OAHWND)m_hWnd );
			m_pME->SetNotifyWindow((OAHWND)NULL, 0, 0);
		}

	#ifdef REGISTER_FILTERGRAPH
		if (g_dwGraphRegister)
		{
			RemoveGraphFromRot(g_dwGraphRegister);
			g_dwGraphRegister = 0;
		}
	#endif
		SAFE_RELEASE(m_pME);
		SAFE_RELEASE(m_pMS);
		SAFE_RELEASE(m_pMP);
		SAFE_RELEASE(m_pMC);
		SAFE_RELEASE(m_pBA);
		SAFE_RELEASE(m_pBV);
		SAFE_RELEASE(m_pVW);
		SAFE_RELEASE(m_pFS);
		SAFE_RELEASE(m_pGB);
	}
	else if( m_dwStream == STREAM_TYPE_MUSIC )
	{
		if( (m_CPState == PAUSE) || (m_CPState == RUNNING) )
		{
			LONGLONG pos = 0;
			hResult = m_pMC->Stop();
			m_CPState = STOP;

			// Seek to the beginning
			hResult = m_pMS->SetPositions( &pos, AM_SEEKING_AbsolutePositioning, NULL, AM_SEEKING_NoPositioning );
			hResult = m_pMC->Pause();
		}
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// �������� Ŭ���� PAUSE�Ѵ�
// Parameters : ����.
// Return : ����.
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDirectShow::PauseClip()
{
	if( !m_pMC )
        return;
    
    if((m_CPState == PAUSE) || (m_CPState == STOP))
    {
        if (SUCCEEDED(m_pMC->Run()))
            m_CPState = RUNNING;
    }
    else
    {
        if (SUCCEEDED(m_pMC->Pause()))
            m_CPState = PAUSE;
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���������츦 �ʱ�ȭ�Ѵ�
// Parameters : (int)(int)
// Return : (HRESULT)
///////////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT CDirectShow::InitVideoWindow(int nMultiplier, int nDivider)
{
	
	//HRESULT hr = S_OK;
	//RECT rect;
    //if( !m_pBV )
    //    return S_OK;
    //GetClientRect( m_hWnd, &rect );
    //m_pVW->SetWindowPosition( rect.left, rect.top, rect.right, rect.bottom );

	LONG lHeight, lWidth;
    HRESULT hr = S_OK;
    RECT rect;

    if (!m_pBV)
        return S_OK;

    // Read the default video size
    hr = m_pBV->GetVideoSize(&lWidth, &lHeight);
    if (hr == E_NOINTERFACE)
        return S_OK;

    // Account for requests of normal, half, or double size
    lWidth  = lWidth  * nMultiplier / nDivider;
    lHeight = lHeight * nMultiplier / nDivider;

    //SetWindowPos(m_hWnd, NULL, 0, 0, lWidth, lHeight,
    //             SWP_NOMOVE | SWP_NOOWNERZORDER);

    int nTitleHeight  = GetSystemMetrics(SM_CYCAPTION);
    int nBorderWidth  = GetSystemMetrics(SM_CXBORDER);
    int nBorderHeight = GetSystemMetrics(SM_CYBORDER);

    // Account for size of title bar and borders for exact match
    // of window client area to default video size
    //SetWindowPos(m_hWnd, NULL, 0, 0, lWidth + 2*nBorderWidth,
    //        lHeight + nTitleHeight + 2*nBorderHeight,
    //        SWP_NOMOVE | SWP_NOOWNERZORDER);


    GetClientRect(m_hWnd, &rect);
//	m_pVW->put_MessageDrain((OAHWND) m_hWnd);
//  m_pVW->put_FullScreenMode( OATRUE );
//	m_pVW->SetWindowForeground( OATRUE );

    m_pVW->SetWindowPosition(rect.left, rect.top, rect.right, rect.bottom);
    

	return S_OK;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// Parameters : ����.
// Return : (BOOL)
///////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDirectShow::GetFrameStepInterface()
{
    HRESULT hr;
    IVideoFrameStep *pFSTest = NULL;

    // Get the frame step interface, if supported
    hr = m_pGB->QueryInterface(__uuidof(IVideoFrameStep), (PVOID *)&pFSTest);
    if (FAILED(hr))
        return FALSE;

    // Check if this decoder can step
    hr = pFSTest->CanStep(0L, NULL);

    if (hr == S_OK)
    {
        m_pFS = pFSTest;  // Save interface to global variable for later use
        return TRUE;
    }
    else
    {
        pFSTest->Release();
        return FALSE;
    }

	return TRUE;
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////
// �̵���� �÷��̰� ���������� üũ�Ѵ�
// Parameters : ����.
// Return : (BOOL)
///////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDirectShow::IS_FINISHED()
{
	HRESULT hResult;
	LONGLONG lStopPosition, lCurrentPosition;

	if( m_pMS )
	{
		hResult = m_pMS->GetStopPosition( &lStopPosition );
		if( FAILED(hResult) )
		{
			::MessageBox( m_hWnd, "IS_FINISHED���� m_pMS->GetStopPosition�Լ��� �����ϴµ� �����߽��ϴ�", "�˸��ϴ�", MB_OK );
			return FALSE;
		}
		hResult = m_pMS->GetCurrentPosition( &lCurrentPosition );
		if( FAILED(hResult) )
		{
			::MessageBox( m_hWnd, "IS_FINISHED���� m_pMS->GetStopPosition�Լ��� �����ϴµ� �����߽��ϴ�", "�˸��ϴ�", MB_OK );
			return FALSE;
		}


		///////////////////////////////////////////////////////////////////////////////////////////////////////////
		// ������ �÷��̰� ��� ������~
		///////////////////////////////////////////////////////////////////////////////////////////////////////////
		if( lStopPosition == lCurrentPosition )
		{
			return TRUE;
		}
	}

	return FALSE;
}



#ifdef REGISTER_FILTERGRAPH

HRESULT AddGraphToRot(IUnknown *pUnkGraph, DWORD *pdwRegister) 
{
    IMoniker * pMoniker;
    IRunningObjectTable *pROT;
    if (FAILED(GetRunningObjectTable(0, &pROT))) 
    {
        return E_FAIL;
    }

    WCHAR wsz[128];
    wsprintfW(wsz, L"FilterGraph %08x pid %08x", (DWORD_PTR)pUnkGraph, 
              GetCurrentProcessId());

    HRESULT hr = CreateItemMoniker(L"!", wsz, &pMoniker);
    if (SUCCEEDED(hr)) 
    {
        hr = pROT->Register(0, pUnkGraph, pMoniker, pdwRegister);
        pMoniker->Release();
    }

    pROT->Release();
    return hr;
}


void RemoveGraphFromRot(DWORD pdwRegister)
{
    IRunningObjectTable *pROT;

    if (SUCCEEDED(GetRunningObjectTable(0, &pROT))) 
    {
        pROT->Revoke(pdwRegister);
        pROT->Release();
    }
}


#endif



HRESULT CDirectShow::HandleGraphEvent()
{
	LONG evCode, evParam1, evParam2;
    HRESULT hr=S_OK;

    // Make sure that we don't access the media event interface
    // after it has already been released.
    if (!m_pME)
	    return S_OK;

    // Process all queued events
    while(SUCCEEDED(m_pME->GetEvent(&evCode, (LONG_PTR *) &evParam1,
                    (LONG_PTR *) &evParam2, 0)))
    {
        // Free memory associated with callback, since we're not using it
        hr = m_pME->FreeEventParams(evCode, evParam1, evParam2);

        // If this is the end of the clip, reset to beginning
        if(EC_COMPLETE == evCode)
        {
            LONGLONG pos=0;

            // Reset to first frame of movie
            hr = m_pMS->SetPositions(&pos, AM_SEEKING_AbsolutePositioning ,
                                   NULL, AM_SEEKING_NoPositioning);
            if (FAILED(hr))
            {
                // Some custom filters (like the Windows CE MIDI filter)
                // may not implement seeking interfaces (IMediaSeeking)
                // to allow seeking to the start.  In that case, just stop
                // and restart for the same effect.  This should not be
                // necessary in most cases.
                if (FAILED(hr = m_pMC->Stop()))
                {
                    //Msg(TEXT("Failed(0x%08lx) to stop media clip!\r\n"), hr);
                    break;
                }

                if (FAILED(hr = m_pMC->Run()))
                {
                    //Msg(TEXT("Failed(0x%08lx) to reset media clip!\r\n"), hr);
                    break;
                }
            }
        }
    }

	return hr;
}





