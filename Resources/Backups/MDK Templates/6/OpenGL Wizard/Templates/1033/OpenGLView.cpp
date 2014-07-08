// OpenGLView.cpp : COpenGLView 클래스의 구현
//

#include "stdafx.h"
#include "[!output PROJECT_NAME].h"

#include "[!output PROJECT_NAME]Doc.h"
#include "[!output PROJECT_NAME]View.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COpenGLView

IMPLEMENT_DYNCREATE(C[!output PROJECT_NAME]View, CView)

BEGIN_MESSAGE_MAP(C[!output PROJECT_NAME]View, CView)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_DESTROY()
[!if SUPPORT_ANIMATION_EFFECT]	
	ON_WM_TIMER()
[!endif]
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// COpenGLView 생성/소멸

C[!output PROJECT_NAME]View::C[!output PROJECT_NAME]View()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	m_spinx=0;
	m_spiny=0;

	m_lbutton=FALSE;
}

C[!output PROJECT_NAME]View::~C[!output PROJECT_NAME]View()
{
}

BOOL C[!output PROJECT_NAME]View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	// Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// COpenGLView 그리기

void C[!output PROJECT_NAME]View::OnDraw(CDC* /*pDC*/)
{
	C[!output PROJECT_NAME]Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// COpenGLView 인쇄

BOOL C[!output PROJECT_NAME]View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void C[!output PROJECT_NAME]View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void C[!output PROJECT_NAME]View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// C[!output PROJECT_NAME]View 진단

#ifdef _DEBUG
void C[!output PROJECT_NAME]View::AssertValid() const
{
	CView::AssertValid();
}

void C[!output PROJECT_NAME]View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

C[!output PROJECT_NAME]Doc* C[!output PROJECT_NAME]View::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(C[!output PROJECT_NAME]Doc)));
	return (C[!output PROJECT_NAME]Doc*)m_pDocument;
}
#endif //_DEBUG


// C[!output PROJECT_NAME]View 메시지 처리기

void C[!output PROJECT_NAME]View::SetDCPixelFormat(HDC hDC)
{
    int nPixelFormat;

	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),	// Size of this structure
		1,					// Version of this structure	
		PFD_DRAW_TO_WINDOW |		// Draw to Window (not to bitmap)
		PFD_SUPPORT_OPENGL |		// Support OpenGL calls in window
		PFD_DOUBLEBUFFER,			// Double buffered mode
		PFD_TYPE_RGBA,			// RGBA Color mode
		16,					// Want 16 bit color 
		0,0,0,0,0,0,				// Not used to select mode
		0,0,					// Not used to select mode
		0,0,0,0,0,			// Not used to select mode
		24,					// Size of depth buffer
		0,					// Not used to select mode
		PFD_MAIN_PLANE,			// Draw in main plane
		0,					// Not used to select mode
		0,					// Not used to select mode
		0,0,0 };					// Not used to select mode

	nPixelFormat = ChoosePixelFormat(hDC, &pfd);

	SetPixelFormat(hDC, nPixelFormat, &pfd);
}

void C[!output PROJECT_NAME]View::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	width=cx;
	height=cy;
	
	if(cx && cy)                            // 만약 윈도우의 가로나 세로 길이가 0 가 아니라면
	{                                       // 그에 맞춰 OpenGL의 ViewPort를 재정립한다.
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		InitProjection();

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glViewport(0, 0, cx, cy);
	}
}


void C[!output PROJECT_NAME]View::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CMainFrame* m_pFrame = (CMainFrame*)AfxGetMainWnd();

[!if SUPPORT_SELECTION_EFFECT]
	int selecteditem;
[!endif]

	m_lbutton = TRUE;                         // 마우스의 왼쪽 버튼이 눌러질 경우 flag을 set하고
                                              // 현재의 mouse pointer위치를 지억시켜둔다.
	m_prevpoint.x=point.x;
	m_prevpoint.y=point.y;

[!if SUPPORT_SELECTION_EFFECT]
	if(selecteditem=Picking(point))
	{
		CString temp;
		temp.Format("You Selected Object %d", selecteditem);
//		m_pFrame->m_wndStatusBar.SetWindowText(temp);
		m_lbutton = FALSE;
	}
[!endif]
	
	CView::OnLButtonDown(nFlags, point);
}


void C[!output PROJECT_NAME]View::OnPaint() 
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );  // 새로은 scene을 그리기 위하여 color buffer와
	                                                       // depth buffer를 clear한다.
	glMatrixMode(GL_PROJECTION);                                  // projection matrix의 초기화
	glLoadIdentity();

	InitProjection();

	glMatrixMode(GL_MODELVIEW);                                   // modelview matrix의 초기화
	glLoadIdentity();

	glColor4f(0.6f,0.8f,0.6f,0.6f);

[!if SUPPORT_SELECTION_EFFECT]
		glPushMatrix();

		glTranslatef(-3.0f,0.0f,0.0f);
[!endif]

	DrawScene();

[!if SUPPORT_SELECTION_EFFECT]
		glPopMatrix();

		glPushMatrix();

		glTranslatef(3.0f,0.0f,0.0f);

		DrawScene();

		glPopMatrix();
[!endif]

	glFlush();			// 위의 API에 의하여 구성된 instruction들을 buffer
	SwapBuffers(hDC);	// 로부터 flush시킨다. 그후 page를 swap시켜서 화면을
	                    // 갱신한다.
	ValidateRect(NULL);
}

void C[!output PROJECT_NAME]View::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_lbutton = FALSE;                        // 마우스의 왼쪽버튼이 그만 누를 경우 flag을 reset한다
	
	CView::OnLButtonUp(nFlags, point);
}

void C[!output PROJECT_NAME]View::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(m_lbutton)							  // 마우스가 움직일 경우 이전에 저장해 두었던 mouse pointer의
		                                      // 위치와 현재의 위치를 비교하여 x축과 y축의 회전울을 계산한다.
											  // 이 값은 OnPaint에서 object에 대한 retation값으로 이용된다.
	{
		m_spinx += (float)point.x-(float)m_prevpoint.x;
		m_spiny += (float)point.y-(float)m_prevpoint.y;

		m_prevpoint.x = point.x;
		m_prevpoint.y = point.y;
	
		InvalidateRect(NULL, FALSE);
	}
	
	CView::OnMouseMove(nFlags, point);
}

BOOL C[!output PROJECT_NAME]View::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;                      // 배경화면을 지우지 않는다.
}

int C[!output PROJECT_NAME]View::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	hDC = ::GetDC(m_hWnd);                                    // OpenGL rendering context를 설정하고 초기화함
	SetDCPixelFormat(hDC);		
	hRC = wglCreateContext(hDC);
	wglMakeCurrent(hDC, hRC);
	InitOpenGL();

[!if SUPPORT_ANIMATION_EFFECT]	
	SetTimer(100,50,NULL);
[!endif]

	return 0;	
}

[!if SUPPORT_ANIMATION_EFFECT]	
void C[!output PROJECT_NAME]View::OnTimer(UINT nIDEvent) 
{
	m_spinx+=0.5f;

	InvalidateRect(NULL);
	
	CView::OnTimer(nIDEvent);
}
[!endif]

void C[!output PROJECT_NAME]View::OnChangeSize(int cx, int cy)
{
	width=cx;
	height=cy;

	SetWindowPos( &wndTop, 0,0,cx,cy,SWP_NOMOVE);
}

void C[!output PROJECT_NAME]View::InitOpenGL(void)
{
[!if SUPPORT_ALPHA_BLEND]
	glEnable(GL_DEPTH_TEST);                                      // depth buffering(z-buffering) 가동
[!endif]

	glEnable(GL_NORMALIZE);                                       // 앞으로 쓰게될 vector들을 자동으로 unit vector로 변환함
	glEnable(GL_COLOR_MATERIAL);

	Obj=gluNewQuadric();

[!if SUPPORT_OMNI_LIGHT || SUPPORT_SPOT_LIGHT]
	InitLight();
[!endif]

[!if SUPPORT_TEXTURE_EFFECT]
	InitTexture();
[!endif]

[!if SUPPORT_ALPHA_BLEND]
	InitAlphaTest();
[!endif]
[!if SUPPORT_FOG_EFFECT]
	InitFog();
[!endif]
[!if NOSUPPORT_FOG_EFFECT]
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
[!endif]
	InitShadingModel();

	glMatrixMode(GL_PROJECTION);                                  // projection matrix의 초기화
	glLoadIdentity();

	InitProjection();

	glMatrixMode(GL_MODELVIEW);                                   // modelview matrix의 초기화
	glLoadIdentity();
}

// Projection Support
void C[!output PROJECT_NAME]View::InitProjection()
{
[!if SUPPORT_PERSPECTIVE_PROJECTION]
	gluPerspective( 45.0f, (float)width/height, 2.0f, 100.0f );                   // 시야각이 45도인 입체적인 projection
[!endif]
[!if SUPPORT_OTHOGONAL_PROJECTION]
	glOrtho( -7.0f*(float)width/(float)height, 7.0f*(float)width/(float)height, -7.0f, 7.0f, 2.0f, 100.0f );  // orthogonal projection
[!endif]
	gluLookAt( 2.0f, 4.0f, 17.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f ); // 2,4,17에서 0,0,0을 y축 orientation으로 바라봄
}

void C[!output PROJECT_NAME]View::DrawScene()
{
	glPushMatrix();

		glRotatef(m_spinx, 0.0f, 1.0f, 0.0f);                                            // OnMouseMove에서 구한 회전율에 따라 
		glRotatef(m_spiny-90.0f, 1.0f, 0.0f, 0.0f);
																						 // rotation을 적용한다.
		gluQuadricDrawStyle(Obj, GLU_FILL);
		gluSphere(Obj, 2.3f, 10, 10 );
		gluQuadricDrawStyle(Obj, GLU_LINE);
		gluSphere(Obj, 2.4f, 10, 10 );
			
	glPopMatrix();
}

void C[!output PROJECT_NAME]View::OnDestroy() 
{
	CView::OnDestroy();
	
[!if SUPPORT_ANIMATION_EFFECT]	
	KillTimer(100);
[!endif]

	wglMakeCurrent(hDC,NULL);                                 // OpenGL rendering context를 해제하고 window가 파괴됨
	wglDeleteContext(hRC);
	::ReleaseDC( m_hWnd, hDC );
		
}

void C[!output PROJECT_NAME]View::InitShadingModel()
{
[!if SUPPORT_SMOOTH_SHADING]
	glShadeModel(GL_SMOOTH);                                      // Smooth shading을 선택
[!endif]
[!if SUPPORT_FLAT_SHADING]
	glShadeModel(GL_FLAT);                                        // Flat shading을 선택
[!endif]
}

[!if SUPPORT_ALPHA_BLEND]
void C[!output PROJECT_NAME]View::InitAlphaTest()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
}
[!endif]

[!if SUPPORT_TEXTURE_EFFECT]
void C[!output PROJECT_NAME]View::InitTexture()
{
	gluQuadricTexture(Obj, GL_TRUE);

	bits=LoadBMPfile("earth.bmp", &info);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, (GLint)info->bmiHeader.biWidth, (GLint)info->bmiHeader.biHeight, GL_RGB, GL_UNSIGNED_BYTE, bits);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glEnable(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void* C[!output PROJECT_NAME]View::LoadBMPfile(char *filename, BITMAPINFO **info)
{
	FILE *fp;
	void *bits;
	unsigned long bitsize, infosize;
	BITMAPFILEHEADER header;
	GLubyte *rgb, temp;
	unsigned int i;

	if( (fp=fopen(filename, "rb")) == NULL)
		return NULL;

	fread(&header, sizeof(BITMAPFILEHEADER), 1, fp);
	
	infosize = header.bfOffBits - sizeof(BITMAPFILEHEADER);
	
	*info=(BITMAPINFO*)malloc(infosize);

	fread(*info, 1, infosize, fp);
	
	bitsize = ((*info)->bmiHeader.biWidth*(*info)->bmiHeader.biBitCount+7)/8*abs((*info)->bmiHeader.biHeight);

	bits=malloc(bitsize);

	fread(bits, 1, bitsize, fp);

	rgb=(GLubyte*)bits;

	for(i=0; i<bitsize; i+=3)
	{
		temp=rgb[i];
		rgb[i]=rgb[i+2];
		rgb[i+2]=temp;
	}

	fclose(fp);
	return bits;
}
[!endif]

[!if SUPPORT_OMNI_LIGHT || SUPPORT_SPOT_LIGHT]
void C[!output PROJECT_NAME]View::InitLight()
{
	GLfloat lightposition[4] = {10.0f, 10.0f, 10.0f, 1.0f};
	GLfloat globalambient[4] = {0.3f, 0.3f, 0.3f, 1.0f};          // 광원의 설정을 위한 Array
	GLfloat lightcolor[4] = {0.9f, 0.9f, 0.9f, 1.0f};

	GLfloat matambient[4] = {0.1f, 0.1f, 0.1f, 0.6f};             //  물체의 재질 설정을 위한 Array
	GLfloat matdiffuse[4] = {0.6f, 0.8f, 0.6f, 0.6f};
	GLfloat matspecular[4] = {0.9f, 0.9f, 0.9f, 0.7f};
	GLbyte shine = 40;

	glEnable(GL_LIGHTING);                                        // 광원효과 이용
	glEnable(GL_LIGHT0);                                          // 광원0 ON

	glLightfv(GL_LIGHT0, GL_POSITION, lightposition);             // 광원의 위치설정
	glLightfv(GL_LIGHT0, GL_AMBIENT_AND_DIFFUSE, lightcolor);     // 광원의 색 설정
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalambient);        // ambient 광원을 설정

[!if SUPPORT_SPOT_LIGHT]
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 16.0f);

	GLfloat direction[4] = { -1.0f, -1.0f, -1.0f, 1.0f};
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, direction);
[!endif]

	glMaterialfv(GL_FRONT, GL_AMBIENT, matambient);               // 물체의 Ambient color값을 설정
	glMaterialfv(GL_FRONT, GL_DIFFUSE, matdiffuse);               // 물체의 diffuse color값을 설정
	glMaterialfv(GL_FRONT, GL_SPECULAR, matspecular);             // 물체의 spectular color값을 설정
	glMaterialf(GL_FRONT, GL_SHININESS, shine);                   // 물체의 shininess값을 설정
}
[!endif]

[!if SUPPORT_FOG_EFFECT]
void C[!output PROJECT_NAME]View::InitFog()
{
	GLfloat fogcolor[4] = {0.5f, 0.5f, 0.5f, 1.0f};

	glFogi(GL_FOG_MODE, GL_EXP);
	glFogfv(GL_FOG_COLOR, fogcolor);
	glFogf(GL_FOG_DENSITY, 0.25f);
	glHint(GL_FOG_HINT, GL_DONT_CARE);
	glFogf(GL_FOG_START, 1.0f);
	glFogf(GL_FOG_END, 50.0f);

	glEnable(GL_FOG);

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
}
[!endif]


[!if SUPPORT_SELECTION_EFFECT]
GLuint C[!output PROJECT_NAME]View::Picking(CPoint point)
{
	GLint viewport[4];
	GLuint picked=0, hits=0;

	glGetIntegerv(GL_VIEWPORT, viewport);
	glSelectBuffer(20, select_buffer);

	glRenderMode(GL_SELECT);

	glInitNames();
	glPushName(9999);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
												  /* set the render mode to selection */
		glLoadIdentity();
		gluPickMatrix(point.x, viewport[3] - point.y, 5.0, 5.0, viewport);

		glGetIntegerv(GL_VIEWPORT, viewport);
	
		InitProjection();

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glPushMatrix();

			glTranslatef(-3.0f,0.0f,0.0f);

			glLoadName(1);

			DrawScene();

		glPopMatrix();

		glPushMatrix();

			glTranslatef(3.0f,0.0f,0.0f);

			glLoadName(2);
			
			DrawScene();

		glPopMatrix();

		glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	glFlush();

	hits = glRenderMode(GL_RENDER);

	if(hits>=1)
		picked=select_buffer[3];

	return picked;
}
[!endif]
