#pragma once

#if defined _WINDLL && !defined VENUS_NO_EXPORT
#define V2D_API __declspec (dllexport)
#else
#define V2D_API
#endif

VENUS_BEG

enum Device2DTypeE
{
	Device2DTypeUnknown,
	Device2DTypeGdi,
	Device2DTypeGdip,
	Device2DTypeDirect2D,
};

class V2D_API I2DRTarget : public IObject
{
public:
};

VENUS_END

#include "image/image.h"
#include "image/CImage.h"

#include "paint/textformat.h"
#include "paint/IDocument.h"
#include "paint/IPaint.h"
#include "paint/CPaintBase.h"
#include "paint/CSamplerBase.h"

#include "paint/CPath.h"

#include "ui/base/UIBase.h"
#include "ui/base/BasePaint.h"
#include "ui/base/Menu.h"

#include "ui/controls/IControl.h"
#include "ui/controls/CControl.h"
#include "ui/controls/CSpace.h"
#include "ui/controls/CLabel.h"
#include "ui/controls/CStatic.h"
#include "ui/controls/CButton.h"
#include "ui/controls/CRadio.h"
#include "ui/controls/CCheckBox.h"
#include "ui/controls/CProgressBar.h"
#include "ui/controls/CScrollBar.h"
#include "ui/controls/CGroupBox.h"
#include "ui/controls/CTextLine.h"
#include "ui/controls/CTextBox.h"
#include "ui/controls/CTabControl.h"
#include "ui/controls/CMenuBar.h"
#include "ui/controls/CListBox.h"
#include "ui/controls/CComboBox.h"
#include "ui/controls/CLayout.h"

#include "ui/controls/CListView.h"
#include "ui/controls/CTreeView.h"
#include "ui/controls/CFoldingView.h"

#include "ui/controls/CPanel.h"
#include "ui/controls/CPane.h"

#include "ui/dock/IDock.h"
#include "ui/dock/CDockPane.h"
#include "ui/dock/CDockNode.h"
#include "ui/dock/CDockFrame.h"

#include "ui/dock/CDockFrameTest.h"

#include "ui/forms/CForm.h"
#include "ui/forms/CMessageBox.h"

#include "ui/tools/CUILoaderXml.h"
