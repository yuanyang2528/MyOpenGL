#pragma once


// closewindow dialog

class closewindow : public CDialogEx
{
	DECLARE_DYNAMIC(closewindow)

public:
	closewindow(CWnd* pParent = NULL);   // standard constructor
	virtual ~closewindow();

// Dialog Data
	enum { IDD = IDD_RUBIKSCUBE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
