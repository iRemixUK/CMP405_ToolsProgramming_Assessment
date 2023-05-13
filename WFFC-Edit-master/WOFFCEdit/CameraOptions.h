#pragma once
#include "afxdialogex.h"
#include "resource.h"
#include "afxwin.h"
#include "ToolMain.h"


// CameraOptions dialog

class CameraOptions : public CDialogEx
{
	DECLARE_DYNAMIC(CameraOptions)

public:
	CameraOptions(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CameraOptions();
	void SetObjectData(ToolMain* toolMain);

	CEdit m_CameraSpeed;
	CEdit m_CameraRotationSpeed;

private:
	ToolMain* m_toolMain;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = ID_CAMERA_OPTIONS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnBnClickedOk();

	DECLARE_MESSAGE_MAP()
};
