#define _COMMON_CWINDOW_H_
#include "common.h"

#include "wargame.h"

#include "thankyou.hpp"

PPROCESSHEADER(CThankYouDlg)
{
	STARTPROCESSING()

		PONCHAR()

		STARTCMDPROCESSING()
			PONCMDPUSHBUTTON()
		ENDCMDPROCESSING()
	ENDPROCESSING()
}

BOOL CThankYouDlg::OnChar(MRESULT &, USHORT flags, UCHAR, UCHAR, USHORT, USHORT Virtual)
{
	if (flags & KC_KEYUP)
		return FALSE;

	if (Virtual == VK_ESC)
		return TRUE;

	return FALSE;
}


BOOL CThankYouDlg::OnCmdPushButton(MRESULT &, USHORT Which, BOOL)
{
	switch(Which)
	{
		case BID_OKAY:
							DismissDlg(TRUE);
							return TRUE;
	}
	return FALSE;
}

