#ifndef _REGISTER_HPP_
#define _REGISTER_HPP_

BOOL VerifyRegister();

class CRegisterDlg : public CDialogBox {
		CEntryFieldWindow m_Code1;
		CEntryFieldWindow m_Code2;
	public:
		DPROCESSHEADER()

		DONINITDLG()
//		DONCHAR()
		DONCMDPUSHBUTTON()

		void CheckCode();
};

#endif
