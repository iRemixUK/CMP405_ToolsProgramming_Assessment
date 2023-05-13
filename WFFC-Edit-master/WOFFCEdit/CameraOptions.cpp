// CameraOptions.cpp : implementation file
//

#include "stdafx.h"
#include "CameraOptions.h"


// CameraOptions dialog

IMPLEMENT_DYNAMIC(CameraOptions, CDialogEx)

CameraOptions::CameraOptions(CWnd* pParent /*=nullptr*/)
	: CDialogEx(ID_CAMERA_OPTIONS, pParent)
{

}

CameraOptions::~CameraOptions()
{
}

void CameraOptions::SetObjectData(ToolMain* toolMain)
{
	m_toolMain = toolMain;
	CString cameraMovementSpeed;
	CString cameraRotationRate;

	cameraMovementSpeed.Format(_T("%f"), m_toolMain->GetCameraMovementSpeed());
	m_CameraSpeed.SetWindowTextW(cameraMovementSpeed);

	cameraRotationRate.Format(_T("%f"), m_toolMain->GetCameraRotationRate());
	m_CameraRotationSpeed.SetWindowTextW(cameraRotationRate);
}

void CameraOptions::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MOVEMENT_SPEED,m_CameraSpeed);
	DDX_Control(pDX, IDC_ROTATION_SPEED, m_CameraRotationSpeed);
}

void CameraOptions::OnBnClickedOk()
{
	CString cameraMovementSpeed;
	CString cameraRotationRate;

	m_CameraSpeed.GetWindowText(cameraMovementSpeed);
	m_CameraRotationSpeed.GetWindowText(cameraRotationRate);

	if (CString::StringLength(cameraMovementSpeed) > 0 && CString::StringLength(cameraRotationRate) > 0)
	{
		m_toolMain->SetCameraMovementSpeed(_ttof(cameraMovementSpeed));
		m_toolMain->SetCameraRotationRate(_ttof(cameraRotationRate));

		CDialogEx::OnOK();
		DestroyWindow();
	}
	else {
		MessageBox(L"INVALID INPUT", MB_OK);
		SetObjectData(m_toolMain);
	}
}

BEGIN_MESSAGE_MAP(CameraOptions, CDialogEx)
	ON_BN_CLICKED(IDOK, &CameraOptions::OnBnClickedOk)
	END_MESSAGE_MAP()
