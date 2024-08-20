#include <wx/wx.h>
#include <wx/url.h>
#include <wx/listbook.h>
#include <wx/wrapsizer.h>
#include <wx/socket.h>
#include <iostream>
#include <CJsonObject.hpp>
#include <fstream>
#include <string>

#define	self 0
//#define DEBUG_MODE
//------------TCP协议(IP & PORT)--------------
#if self

#define IP  "192.168.0.100"
#define PROT 1111

#else

#define IP  "9tv5583978sv.vicp.fun"//"192.168.10.8"//"8.135.10.183"
#define PROT 30087//1111

#endif // self debug



int CountNonemptyStrings(wxString array[],int size);
enum
{
    wxID_MenuFile = wxID_HIGHEST + 1,
    wxID_MenuHelp,
    wxID_MenuAbout

};

class FirmwareGUI : public wxFrame
{
public:
    FirmwareGUI(const wxString& title);

private:

    void OnHelp(wxCommandEvent& event);
	void QueryFirmwareVersion(wxCommandEvent& event);
	void Sizer_network(wxWindow* myparent,wxString* radioBox_breathChoices, wxString* radioBox_heightChoices, wxString* radioBox_humanChoices);
	void Sizer_current();
	void Sizer_loadBurn(wxWindow* myparent);
	void Ondownload_breath(wxCommandEvent& event);

    wxPanel* Panel;
    wxMenu* FileMenu;
    wxMenuItem* helpMenu;
    wxMenuBar* FirmwareMenuBar;
    
	wxListbook* m_listbook1;
	wxPanel* m_panel_network;
	wxPanel* m_panel_current;
	wxPanel* m_panel_load;
	
	//Dialog Path
	//wxDialog* ChoiseDialog;
	//Network path
	virtual void OnradioChoiseVersion_breath(wxCommandEvent& event);
	virtual void OnradioChoiseVersion_height(wxCommandEvent& event);
	virtual void OnradioChoiseVersion_human(wxCommandEvent& event);
	virtual void Oncancel_dialog(wxCommandEvent& event);
	virtual void Onyes_dialog(wxCommandEvent& event);
	void Download_breath(int n,const char command[]);

	wxRadioBox* m_radioBox_breath;
	wxRadioBox* m_radioBox_height;
	wxRadioBox* m_radioBox_human;

	wxButton* button_yes_dialog;
	wxButton* button_cancel_dialog;

#define Size_radioBoxChoies 6
	std::string versions_breath;
	std::string versions_height;
	std::string versions_human;

	wxString m_radioBox_breathChoices[Size_radioBoxChoies] = { "默认版本" };
	wxString m_radioBox_heightChoices[Size_radioBoxChoies] = { "默认版本" };
	wxString m_radioBox_humanChoices[Size_radioBoxChoies] = { "默认版本" };
	//Current path
	virtual void OnserachCurrent(wxCommandEvent& event);

	wxButton* m_button_serachCurrent;
	wxTextCtrl* m_textCtrl;
	//LoadBurn path
	virtual void OnComboBoxGloseup_burn(wxCommandEvent& event);
	virtual void OnComboBoxDropdown_burn(wxCommandEvent& event);
	virtual void OnOpenfile(wxCommandEvent& event);
	virtual void OnBurnFirmware(wxCommandEvent& event);

	wxComboBox* m_comboBox_burn;
	wxButton* m_button_brower;
	wxButton* m_button_burn;

};



FirmwareGUI::FirmwareGUI(const wxString& title) : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(600, 400))
{

	this->SetSizeHints(wxDefaultSize, wxDefaultSize);

	wxBoxSizer* bSizer_frame_Top;
	bSizer_frame_Top = new wxBoxSizer(wxVERTICAL);

	m_listbook1 = new wxListbook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLB_DEFAULT);
	//QueryFirmwareVersion();
	m_panel_network = new wxPanel(m_listbook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	//Sizer_network(m_panel_network,m_radioBox_breathChoices);

	wxButton* serachbutton;
	serachbutton = new wxButton(m_panel_network, wxID_ANY, _("查询"), wxDefaultPosition, wxDefaultSize, 0);
	Connect(serachbutton->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(FirmwareGUI::QueryFirmwareVersion));
	m_panel_network->Layout();
	


	m_listbook1->AddPage(m_panel_network, _("查询网络固件版本"), true);

	m_panel_current = new wxPanel(m_listbook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	Sizer_current();
	m_listbook1->AddPage(m_panel_current, _("查询当前固件版本"), false);
	
	m_panel_load = new wxPanel(m_listbook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	Sizer_loadBurn(m_panel_load);
	m_listbook1->AddPage(m_panel_load, _("烧录"), false);

#ifdef __WXGTK__ // Small icon style not supported in GTK
	wxListView* m_listbook1ListView = m_listbook1->GetListView();
	long m_listbook1Flags = m_listbook1ListView->GetWindowStyleFlag();
	if (m_listbook1Flags & wxLC_SMALL_ICON)
	{
		m_listbook1Flags = (m_listbook1Flags & ~wxLC_SMALL_ICON) | wxLC_ICON;
	}
	m_listbook1ListView->SetWindowStyleFlag(m_listbook1Flags);
#endif

	bSizer_frame_Top->Add(m_listbook1, 1, wxEXPAND | wxALL, 5);


	this->SetSizer(bSizer_frame_Top);
	this->Layout();

	this->Centre(wxBOTH);


    FileMenu = new wxMenu;
    
    helpMenu = new wxMenuItem(FileMenu,wxID_MenuHelp,"&Help","Open Help!");
    FileMenu->Append(helpMenu);

    FirmwareMenuBar = new wxMenuBar;
    FirmwareMenuBar->Append(FileMenu, "File");


   Connect(helpMenu->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(FirmwareGUI::OnHelp));

 
    SetMenuBar(FirmwareMenuBar);
    CreateStatusBar();

}

void FirmwareGUI::OnHelp(wxCommandEvent& event)
{
    wxMessageBox("If you have any questions, please contact me in time !\nJunday 3229648965@qq.com", "Help", wxOK | wxICON_INFORMATION,this);
}


void FirmwareGUI::Ondownload_breath(wxCommandEvent& event)
{    //m_radioBox_breath->FindString("1.0.bin")   如果选项有则为真
	wxURL url("http://....");
	if (url.GetError() != wxURL_NOERR)
	{
		wxLogError("can not access");
		return;
	}
	wxInputStream* InStream = url.GetInputStream();
	if (!InStream)
	{
		wxLogError("failed to get input stream!");
		return;
	}		
	//接受缓存
	wxString wxReceiData;
	char Buffer[1024];
	while (! InStream->Read(Buffer, sizeof(Buffer)))
	{
	    wxReceiData += wxString(Buffer, InStream->LastRead());
	}
	//.....
	m_radioBox_breathChoices[2] =  "99";
	
	wxMessageBox("ok", "获取breath", wxID_OK, this);  
}
void FirmwareGUI::Download_breath(int n,const char command[])
{
	//--------------------TCP协议---------------------------
	wxIPV4address addr;
	addr.Hostname(IP); // 替换为服务器的IP地址
	addr.Service(PROT); // 替换为服务器的端口号

	wxSocketClient* socket = new wxSocketClient();
	//socket->SetEventHandler(*this, wxID_ANY);
	socket->SetNotify(wxSOCKET_CONNECTION_FLAG | wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG);
	socket->Notify(true);

	socket->Connect(addr);
	//发送指令command
	socket->Write(command, sizeof(command));
	
	char buf[2040] = { 0 };
	socket->Read(buf, sizeof(buf));
	socket->Destroy();
	//wxString test(buf);
	//wxMessageBox(test);
	std::string wxTostdData(buf);
	//——————————————————————————————————————————
	////-------------打开文件夹----------
    // 弹出保存对话框
	wxFileDialog saveDialog(this, "Save File", "", "", "Binary Files (*.bin)|*.txt", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if (saveDialog.ShowModal() == wxID_CANCEL) {
		return; // 用户取消
	}

	// 获取用户选择的文件路径
	wxString filePath = saveDialog.GetPath();

	//// 使用 wxFile 打开文件进行写入
	//wxFile file(filePath, wxFile::write);
	//if (!file.IsOpened()) {
	//	wxMessageBox("Cannot open file for writing.", "Error", wxICON_ERROR);
	//	return;
	//}
	//------------写入文件-------------
	std::string filePathStd = filePath.ToStdString();
	std::ofstream outputFile(filePathStd , std::ios::binary); // 打开输出文件
	if(wxTostdData.empty()) 
	{
		wxLogError("Can not recive File");
		return ;
	}
	if(!outputFile) 
	{
		wxLogError("Can not open File");
		return ;
	}
	
	outputFile.write(buf,sizeof(buf));
	if (outputFile.fail()) {
		wxLogError("File write error");
		return ;
	}
	outputFile.close();

}

 
void FirmwareGUI::OnradioChoiseVersion_breath(wxCommandEvent& event)
{
	switch (m_radioBox_breath->GetSelection())
	{
	
	case 1:	
		//请求下载固件
		Download_breath(1, "breath1.1");
		wxMessageBox(wxT("this is 1.0 and done!"), wxT("breath固件"), wxOK | wxICON_INFORMATION, this);
		break;
	case 2:
		//请求下载固件
		Download_breath(2, "breath2.0");
		wxMessageBox(wxT("this is 2.0 and done!"), wxT("breath固件"), wxOK | wxICON_INFORMATION, this);
		break;
	case 3:
		//请求下载固件
		Download_breath(3, "breath_3_0");
		wxMessageBox(wxT("this is 3.0 and done!"), wxT("breath固件"), wxOK | wxICON_INFORMATION, this);
		break;
	case 4:
		//请求下载固件
		Download_breath(4, "breath_4_0");
		wxMessageBox(wxT("this is 4.0 and done!"), wxT("breath固件"), wxOK | wxICON_INFORMATION, this);
		break;
	default:
		break;
	}
	
	event.Skip();
}
void FirmwareGUI::OnradioChoiseVersion_height(wxCommandEvent& event)
{	
	//event --> 已绑定事件 
	switch (event.GetSelection())
	{
	
	case 1:
		//请求下载固件
		Download_breath(1, "height_1_0_bin");
		wxMessageBox(wxT("this is 1.0 and done!"), wxT("height固件"), wxOK | wxICON_INFORMATION, this);
		break;
	case 2:
		//请求下载固件
		Download_breath(2, "height_2_0_bin");
		wxMessageBox(wxT("this is 2.0 and done!"), wxT("height固件"), wxOK | wxICON_INFORMATION, this);
		break;
	case 3:
		//请求下载固件
		Download_breath(3, "height_3_0_bin");
		wxMessageBox(wxT("this is 3.0 and done!"), wxT("height固件"), wxOK | wxICON_INFORMATION, this);
		break;
	case 4:
		//请求下载固件
		Download_breath(4, "height_4_0_bin");
		wxMessageBox(wxT("this is 4.0 and done!"), wxT("height固件"), wxOK | wxICON_INFORMATION, this);
		break;
	default:
		break;
	}

	event.Skip();
}
void FirmwareGUI::OnradioChoiseVersion_human(wxCommandEvent& event)
{
	switch (m_radioBox_human->GetSelection())
	{
	
		
	case 1:
		//请求下载固件
		Download_breath(1, "human_1_0_bin");
		wxMessageBox(wxT("this is 1.0 and done!"), wxT("human固件"), wxOK | wxICON_INFORMATION, this);
		break;
	case 2:
		//请求下载固件
		Download_breath(2, "human_2_0_bin");
		wxMessageBox(wxT("this is 2.0 and done!"), wxT("human固件"), wxOK | wxICON_INFORMATION, this);
		break;
	case 3:
		//请求下载固件
		Download_breath(3, "human_3_0_bin");
		wxMessageBox(wxT("this is 3.0 and done!"), wxT("human固件"), wxOK | wxICON_INFORMATION, this);
		break;
	case 4:
		//请求下载固件
		Download_breath(4, "human_4_0_bin");
		wxMessageBox(wxT("this is 4.0 and done!"), wxT("human固件"), wxOK | wxICON_INFORMATION, this);
		break;
	default :
		break;
	}

	event.Skip();
}
void FirmwareGUI::Onyes_dialog(wxCommandEvent& event)
{
	//ChoiseDialog->Destroy();
	
	wxMessageBox(" Quit", "Firmware Search", wxOK | wxICON_INFORMATION, this);
	event.Skip();
}
void FirmwareGUI::Oncancel_dialog(wxCommandEvent& event)
{
	wxMessageBox(" Quit", "Firmware Search", wxOK | wxICON_INFORMATION, this);
	event.Skip();
}

void FirmwareGUI::OnComboBoxGloseup_burn(wxCommandEvent& event)
{

	//wxMessageBox(" 关闭", "Firmware Search", wxOK | wxICON_INFORMATION, this);
	event.Skip();
}
void FirmwareGUI::OnComboBoxDropdown_burn(wxCommandEvent& event)
{

	//wxMessageBox(" 下拉", "Firmware Search", wxOK | wxICON_INFORMATION, this);
	event.Skip();
}
void FirmwareGUI::OnOpenfile(wxCommandEvent& event)
{
	wxFileDialog dialog(this, "Choose a file", "", "", "All files (*.*)|*.*", wxFD_OPEN);
	if (dialog.ShowModal() == wxID_OK) 
	{
		// 获取选中的文件路径
		wxString filePath = dialog.GetPath();
		// 将文件路径添加到ComboBox中
		m_comboBox_burn->Append(filePath);
		m_comboBox_burn->SetStringSelection(filePath);
	}
	event.Skip();
}
void FirmwareGUI::OnBurnFirmware(wxCommandEvent& event)
{
	wxString filepath =  m_comboBox_burn->GetStringSelection();
	if (!filepath.IsEmpty())
	{
		//调用串口烧录固件
	
		wxMessageBox(filepath , "Firmware Burn", wxOK | wxICON_INFORMATION, this);
	}
	else
	{
		wxLogError("No File Path!\nPlease Brower A File.");
	}
	event.Skip();
}
void FirmwareGUI::OnserachCurrent(wxCommandEvent& event)
{
	//调用串口通信下位机

	wxString Version_current = "human_1.2.Bin";
	wxMessageBox(Version_current, "Firmware Current", wxOK | wxICON_INFORMATION, this);
	m_textCtrl->SetValue("当前版本：");
	m_textCtrl->AppendText(Version_current);

	event.Skip();
}
void FirmwareGUI::QueryFirmwareVersion(wxCommandEvent& event)
{
//	  //-----------------------Http-------------------------------
 //   wxURL url("http://9tv5583978sv.vicp.fun.com");
 //   if (url.GetError() != wxURL_NOERR)
 //   {
 //       wxLogError("can not access");
 //       return;
 //   }
 //   wxInputStream* InStream = url.GetInputStream();
 //   if (!InStream)
 //   {
 //       wxLogError("Failed to connect server!\nPlease check your network connection!");
 //       return;
 //   }
 //   //接受缓存
 //   wxString wxReceiData;
 //   char Buffer[1024];
 //   while (! InStream->Read(Buffer, sizeof(Buffer)))
 //   {
 //       wxReceiData += wxString(Buffer, InStream->LastRead());
 //   }
	//std::string wxTostdData = wxReceiData.ToStdString();

	//--------------------TCP协议---------------------------
	wxIPV4address addr;
	addr.Hostname(IP); // 替换为服务器的IP地址
	addr.Service(PROT); // 替换为服务器的端口号

	wxSocketClient* socket = new wxSocketClient();
	//———————设置socket事件————————————
	socket->SetEventHandler(*this, wxID_ANY);
	socket->SetNotify(wxSOCKET_CONNECTION_FLAG | wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG);
	socket->Notify(true);
	//—————————-----——————————————
	if (!socket->Connect(addr))
	{
		wxLogError("Fail Connect Server!\nPlease Check out addr!");
		delete socket;
		return;
	}
	//---------------发送请求命令-------------------------
	socket->Write("QueryFirmwareVersion", sizeof("QueryFirmwareVersion"));
	
	char buf[2040] = {0};
	socket->Read(buf, sizeof(buf));
	socket->Destroy();
#ifdef DEBUG_MODE
	wxString test(buf);
	wxMessageBox( test, "Origin recveie data");
	std::string DebugData(buf);
	////-------------打开文件夹----------
	// 弹出保存对话框
	wxFileDialog saveDialog(this, "Save File", "", "", "Binary Files (*.bin)|*.txt", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if (saveDialog.ShowModal() == wxID_CANCEL) {
		return; // 用户取消
	}

	// 获取用户选择的文件路径
	wxString filePath = saveDialog.GetPath();
	//------------写入文件-------------
	std::string filePathStd = filePath.ToStdString();
	std::ofstream outputFile(filePathStd, std::ios::binary); // 打开输出文件
	if (DebugData.empty())
	{
		wxLogError("Can not recive File");
		return;
	}
	if (!outputFile)
	{
		wxLogError("Can not open File");
		return;
	}

	outputFile.write(buf, sizeof(buf));
	if (outputFile.fail()) {
		wxLogError("File write error");
		return;
	}
	outputFile.close();

#endif
	std::string wxTostdData(buf);
	if (wxTostdData.empty())
	{
		wxLogError("Fail receive data!");
		return;
	}
	//——————————————————————————————————————————
	
    //std::ifstream file("abc.txt");
    //if (!file.is_open())
    //{
    //    wxLogError("fail open file");
    //    return;
    //}
    //char c;
    //std::string filedata;
    //while (file.get(c)) {
    //    filedata += c;
    //}
    //file.close();
	//------------------解析Json数据------------------------
    std::string str = "praise";
    neb::CJsonObject Ojson;
	//if (Ojson.Parse(filedata))
		
	if (Ojson.Parse(wxTostdData))
	{
		/*
			{
				"Top": {
							"breath": ["1.0.bin"],
							"height": ["1.0.bin","2.0.bin"],
							"human"	: ["1.0.bin","2.0.bin","3.0.bin"]
						}
			}
		*/
#ifdef DEBUG_MODE
		str = Ojson.ToString();
		wxMessageBox(str,"Json data");
#endif // DEBUG
		//————————雷达功能种类——————————
		const int num_out = 3;
		std::string funcName[num_out] = { "breath","height","human"};
		
		for (int i = 0;i < num_out; i++) 
		{

			int num_in = Ojson["Top"][funcName[i]].GetArraySize();
			for (int j = 0; j < num_in ;j++)
			{
				std::string strtmp = Ojson["Top"][funcName[i]](j);
				//wxMessageBox(strtmp);
				switch (i)
				{
				case 0:	m_radioBox_breathChoices[j+1] = strtmp; //避免覆盖默认值
					break;
				case 1: m_radioBox_heightChoices[j+1] = strtmp;
					break;
				case 2:	m_radioBox_humanChoices[j+1] = strtmp;
					break;
				default:
					break;
				}
			}
			//Ojson["Top"].Get("breath", versions_breath);
			//wxMessageBox(versions_breath);
		}
    }
    else
    {
        wxMessageBox("failed to parse Json data!");
		return;
    }
	//------------------------创建查询对话框----------------------------------------------- 
	//ChoiseDialog = new wxDialog(NULL, wxID_ANY, "Choises", wxDefaultPosition, wxSize(480,385), wxDEFAULT_DIALOG_STYLE);
	wxDialog ChoiseDialog(NULL, wxID_ANY, "Choises", wxDefaultPosition, wxSize(480, 385), wxDEFAULT_DIALOG_STYLE);
	wxPanel* ChoisePanel = new wxPanel(&ChoiseDialog);
	Sizer_network(&ChoiseDialog, m_radioBox_breathChoices, m_radioBox_heightChoices, m_radioBox_humanChoices);
	//dialog退出判断（showModal（）：自动调用destroy（））

	if (ChoiseDialog.ShowModal() == wxID_CANCEL || ChoiseDialog.ShowModal() == wxID_OK)
	{
		delete ChoisePanel;
		//wxMessageBox(" Succeful", "Firmware Download", wxOK | wxICON_INFORMATION, this);
	}
	event.Skip();
}

int CountNonemptyStrings(wxString array[], int size)
{
	int count = 0;
	for (int i = 0; i < size;i++)
	{
		if (!array[i].empty())
			count++;
	}
	return count;
}

void FirmwareGUI::Sizer_network(wxWindow* myparent,wxString* radioBox_breathChoices, wxString* radioBox_heightChoices, wxString* radioBox_humanChoices)
{
	
	wxBoxSizer* bSizer_TopTop;
	bSizer_TopTop = new wxBoxSizer(wxVERTICAL);

	wxScrolledWindow* m_scrolledWindow = new wxScrolledWindow(myparent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL);
	m_scrolledWindow->SetScrollRate(5, 5);

	wxBoxSizer* bSizer_Top;
	bSizer_Top = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer_breath;
	bSizer_breath = new wxBoxSizer(wxHORIZONTAL);

	wxStaticBoxSizer* sbSizer_breath;
	sbSizer_breath = new wxStaticBoxSizer(new wxStaticBox(m_scrolledWindow, wxID_ANY, _("Breath")), wxHORIZONTAL);

	wxStaticText* m_staticText_breath;
	m_staticText_breath = new wxStaticText(sbSizer_breath->GetStaticBox(), wxID_ANY, _("Version:"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL | wxALIGN_RIGHT | wxST_ELLIPSIZE_END);
	m_staticText_breath->Wrap(-1);
	sbSizer_breath->Add(m_staticText_breath, 0, wxALL, 5);

	int m_radioBox_breathNChoices = CountNonemptyStrings(radioBox_breathChoices, Size_radioBoxChoies);
	m_radioBox_breath = new wxRadioBox(sbSizer_breath->GetStaticBox(), wxID_ANY, _("Bin"), wxDefaultPosition, wxDefaultSize, m_radioBox_breathNChoices, radioBox_breathChoices, 1, wxRA_SPECIFY_ROWS);
	m_radioBox_breath->SetSelection(0);
	sbSizer_breath->Add(m_radioBox_breath, 0, wxALL, 5);
	bSizer_breath->Add(sbSizer_breath, 1, wxEXPAND, 5);
	bSizer_Top->Add(bSizer_breath, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer_height;
	bSizer_height = new wxBoxSizer(wxHORIZONTAL);

	wxStaticBoxSizer* sbSizer_height;
	sbSizer_height = new wxStaticBoxSizer(new wxStaticBox(m_scrolledWindow, wxID_ANY, _("Height")), wxHORIZONTAL);

	wxStaticText* m_staticText_height;
	m_staticText_height = new wxStaticText(sbSizer_height->GetStaticBox(), wxID_ANY, _("Version:"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL | wxALIGN_RIGHT | wxST_ELLIPSIZE_END);
	m_staticText_height->Wrap(-1);
	sbSizer_height->Add(m_staticText_height, 0, wxALL, 5);
	
	int m_radioBox_heightNChoices = CountNonemptyStrings(radioBox_heightChoices, Size_radioBoxChoies);
	m_radioBox_height = new wxRadioBox(sbSizer_height->GetStaticBox(), wxID_ANY, _("Bin"), wxDefaultPosition, wxDefaultSize, m_radioBox_heightNChoices, radioBox_heightChoices, 1, wxRA_SPECIFY_ROWS);
	m_radioBox_height->SetSelection(0);
	sbSizer_height->Add(m_radioBox_height, 0, wxALL, 5);
	bSizer_height->Add(sbSizer_height, 1, wxEXPAND, 5);
	bSizer_Top->Add(bSizer_height, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer_human;
	bSizer_human = new wxBoxSizer(wxHORIZONTAL);

	wxStaticBoxSizer* sbSizer_human;
	sbSizer_human = new wxStaticBoxSizer(new wxStaticBox(m_scrolledWindow, wxID_ANY, _("Human")), wxHORIZONTAL);
	wxStaticText* m_staticText_human;
	m_staticText_human = new wxStaticText(sbSizer_human->GetStaticBox(), wxID_ANY, _("Version:"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL | wxALIGN_RIGHT | wxST_ELLIPSIZE_END);
	m_staticText_human->Wrap(-1);
	sbSizer_human->Add(m_staticText_human, 0, wxALL, 5);

	int m_radioBox_humanNChoices = CountNonemptyStrings(radioBox_humanChoices, Size_radioBoxChoies);
	m_radioBox_human = new wxRadioBox(sbSizer_human->GetStaticBox(), wxID_ANY, _("Bin"), wxDefaultPosition, wxDefaultSize, m_radioBox_humanNChoices, radioBox_humanChoices, 1, wxRA_SPECIFY_ROWS);
	m_radioBox_human->SetSelection(0);
	sbSizer_human->Add(m_radioBox_human, 0, wxALL, 5);
	bSizer_human->Add(sbSizer_human, 1, wxEXPAND, 5);
	bSizer_Top->Add(bSizer_human, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer_save;
	bSizer_save = new wxBoxSizer(wxHORIZONTAL);
	bSizer_save->Add(0, 0, 1, wxEXPAND, 5);

	button_yes_dialog = new wxButton(m_scrolledWindow, wxID_OK, _("确定"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer_save->Add(button_yes_dialog, 0, wxALL, 5);

	button_cancel_dialog = new wxButton(m_scrolledWindow, wxID_CANCEL, _("取消"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer_save->Add(button_cancel_dialog, 0, wxALL, 5);
	//添加spacer控件
	bSizer_Top->Add(bSizer_save, 1, wxEXPAND, 5);

	m_scrolledWindow->SetSizer(bSizer_Top);
	m_scrolledWindow->Layout();
	bSizer_Top->Fit(m_scrolledWindow);
	bSizer_TopTop->Add(m_scrolledWindow, 1, wxEXPAND | wxALL, 5);

	myparent->SetSizer(bSizer_TopTop);
	myparent->Layout();

	// Connect Events
	m_radioBox_breath->Connect(wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler(FirmwareGUI::OnradioChoiseVersion_breath),NULL,this);
	m_radioBox_height->Connect(wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler(FirmwareGUI::OnradioChoiseVersion_height), NULL, this);
	m_radioBox_human->Connect(wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler(FirmwareGUI::OnradioChoiseVersion_human), NULL, this);
	button_yes_dialog->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(FirmwareGUI::Onyes_dialog), NULL, this);
	button_cancel_dialog->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(FirmwareGUI::Oncancel_dialog), NULL, this);
	
}
void FirmwareGUI::Sizer_current()
{
	wxBoxSizer* bSizer_current_Top;
	bSizer_current_Top = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer_current_sub;
	bSizer_current_sub = new wxBoxSizer(wxHORIZONTAL);

	m_button_serachCurrent = new wxButton(m_panel_current, wxID_ANY, _("查询"), wxDefaultPosition, wxDefaultSize, 0);
	m_button_serachCurrent->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(FirmwareGUI::OnserachCurrent),NULL,this);
	bSizer_current_sub->Add(m_button_serachCurrent, 0, wxALL, 5);

	m_textCtrl = new wxTextCtrl(m_panel_current, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	m_textCtrl->SetValue("请点击查询！");
	m_textCtrl->SetMinSize(wxSize(200, -1));
	bSizer_current_sub->Add(m_textCtrl, 0, wxALL, 5);

	bSizer_current_Top->Add(bSizer_current_sub, 1, wxEXPAND, 5);

	m_panel_current->SetSizer( bSizer_current_Top);
	m_panel_current->Layout();
	bSizer_current_Top->Fit(m_panel_current);
	
}
void FirmwareGUI::Sizer_loadBurn(wxWindow* myparent)
{
	wxBoxSizer* bSizer_burn;
	bSizer_burn = new wxBoxSizer(wxHORIZONTAL);

	m_comboBox_burn = new wxComboBox(myparent, wxID_ANY, _("点击Brower获取文件地址"), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_DROPDOWN | wxCB_SORT);
	m_comboBox_burn->SetSelection(1);
	m_comboBox_burn->SetMinSize(wxSize(300, -1));//only windows system  
	bSizer_burn->Add(m_comboBox_burn, 0, wxALL, 5);

	m_button_brower = new wxButton(myparent, wxID_ANY, _("Brower"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer_burn->Add(m_button_brower, 0, wxALL, 5);

	m_button_burn = new wxButton(myparent, wxID_ANY, _("一键烧录"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer_burn->Add(m_button_burn, 0, wxALL, 5);

	myparent->SetSizer(bSizer_burn);
	myparent->Layout();

	// Connect Events
	m_comboBox_burn->Connect(wxEVT_COMBOBOX_CLOSEUP, wxCommandEventHandler(FirmwareGUI::OnComboBoxGloseup_burn), NULL, this);
	m_comboBox_burn->Connect(wxEVT_COMBOBOX_DROPDOWN, wxCommandEventHandler(FirmwareGUI::OnComboBoxDropdown_burn), NULL, this);
	m_button_brower->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(FirmwareGUI::OnOpenfile), NULL, this);
	m_button_burn->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(FirmwareGUI::OnBurnFirmware), NULL, this);
	
}
//std::string* filetestdemo(const char* filename)
//{
//    std::ifstream file(filename);
//    if (!file.is_open())
//    {
//        wxLogError("fail open file");
//        return NULL;
//    }
//    char c;
//    std::string* filedata = new(std::string);
//    while (file.get(c)) {
//        filedata += c;
//    }
//    file.close();
//}
class MyApp : public wxApp
{
public:
    bool OnInit()
    {
        FirmwareGUI* frame = new FirmwareGUI("固件管理工具");
        frame->Show(true);
        return true;
    }
};



wxIMPLEMENT_APP(MyApp);