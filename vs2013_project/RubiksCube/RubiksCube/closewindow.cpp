// closewindow.cpp : implementation file
//

#include "stdafx.h"
#include "RubiksCube.h"
#include "closewindow.h"
#include "afxdialogex.h"


// closewindow dialog

IMPLEMENT_DYNAMIC(closewindow, CDialogEx)

closewindow::closewindow(CWnd* pParent /*=NULL*/)
	: CDialogEx(closewindow::IDD, pParent)
{

}

closewindow::~closewindow()
{
}

void closewindow::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(closewindow, CDialogEx)
END_MESSAGE_MAP()


// closewindow message handlers
