#define _COMMON_CWINDOW_H_
#include "common.h"

#include "wargame.h"

#include "sorry.hpp"

PPROCESSHEADER(CSorryDlg)
{
	STARTPROCESSING()

		PONCHAR()

		STARTCMDPROCESSING()
			PONCMDPUSHBUTTON()
		ENDCMDPROCESSING()
	ENDPROCESSING()
}

BOOL CSorryDlg::OnChar(MRESULT &, USHORT flags, UCHAR, UCHAR, USHORT, USHORT Virtual)
{
	if (flags & KC_KEYUP)
		return FALSE;

	if (Virtual == VK_ESC)
		return TRUE;

	return FALSE;
}


BOOL CSorryDlg::OnCmdPushButton(MRESULT &, USHORT Which, BOOL)
{
	switch(Which)
	{
		case BID_OKAY:
							DismissDlg(TRUE);
							return TRUE;
	}
	return FALSE;
}

