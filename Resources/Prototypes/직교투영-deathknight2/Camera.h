#pragma once
//������������������������������������������������������������������������������
// Narusia.07 - 10 - 10 ī�޶� ���� Ŭ����
//������������������������������������������������������������������������������.
#include "defines.h"

class CCamera
{
private:
	D3DXVECTOR3		m_vPosition;
	D3DXMATRIXA16	m_Rot;		// RoatationYawPitchRoll �� ���� ���
	D3DXMATRIX		m_View;
	D3DXMATRIX		m_Projection;
	float					m_fYaw;
	float					m_fPitch;
	float					m_fRoll;

public:
	void SetUpProjection(LPDIRECT3DDEVICE9 Device, float fovY, float Aspect, float fnear, float ffar); // ī�޶� ���� ����
	void UpdateYallPitchRoll(float fYaw, float fPitch, float fRoll);
	void SetPosition(D3DXVECTOR3*	Pos);
	void Walk(float Units);
	void Strafe(float Units);
	void Fly(float Units);
	void Update(LPDIRECT3DDEVICE9 Device);
	D3DXMATRIX* GetViewMatrix(void)       { return &m_View; }
	CCamera(void);
public:
	virtual ~CCamera(void);
};
