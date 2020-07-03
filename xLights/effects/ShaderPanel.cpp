/***************************************************************
 * This source files comes from the xLights project
 * https://www.xlights.org
 * https://github.com/smeighan/xLights
 * See the github commit history for a record of contributing
 * developers.
 * Copyright claimed based on commit dates recorded in Github
 * License: https://github.com/smeighan/xLights/blob/master/License.txt
 **************************************************************/

#include "ShaderPanel.h"
#include "ShaderEffect.h"
#include "../BulkEditControls.h"
#include "EffectPanelUtils.h"
#include "ShaderDownloadDialog.h"

#include "../xLightsMain.h"
#include "../xLightsApp.h"
#include "../TimingPanel.h"
#include "../osxMacUtils.h"

//(*InternalHeaders(ShaderPanel)
#include <wx/artprov.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)
#include <wx/artprov.h>
#include <wx/progdlg.h>

//(*IdInit(ShaderPanel)
const long ShaderPanel::ID_STATICTEXT1 = wxNewId();
const long ShaderPanel::ID_0FILEPICKERCTRL_IFS = wxNewId();
const long ShaderPanel::ID_BUTTON1 = wxNewId();
const long ShaderPanel::ID_STATICTEXT2 = wxNewId();
const long ShaderPanel::IDD_SLIDER_Shader_LeadIn = wxNewId();
const long ShaderPanel::ID_TEXTCTRL_Shader_LeadIn = wxNewId();
const long ShaderPanel::ID_STATICTEXT3 = wxNewId();
const long ShaderPanel::ID_SLIDER_Shader_Speed = wxNewId();
const long ShaderPanel::ID_VALUECURVE_Shader_Speed = wxNewId();
const long ShaderPanel::IDD_TEXTCTRL_Shader_Speed = wxNewId();
//*)

ShaderPreview::ShaderPreview( wxWindow* parent, wxWindowID id, const wxPoint &pos, const wxSize &size, long style, const wxString &name, bool coreProfile)
   : xlGLCanvas( parent, id, pos, size, style, name, coreProfile )
{

}

ShaderPreview::~ShaderPreview()
{

}

void ShaderPreview::InitializeGLContext()
{
   // should just be doing init stuff?
   SetCurrentGLContext();
}

const long ShaderPanel::ID_CANVAS = wxNewId();

BEGIN_EVENT_TABLE(ShaderPanel,wxPanel)
	//(*EventTable(ShaderPanel)
	//*)
END_EVENT_TABLE()

ShaderPanel::ShaderPanel(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
    // I have deliberately given the file picker a ID_- prefix to force it to be processed first

	//(*Initialize(ShaderPanel)
	wxFlexGridSizer* FlexGridSizer2;
	wxFlexGridSizer* FlexGridSizer3;

	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	FlexGridSizer1 = new wxFlexGridSizer(0, 1, 0, 0);
	FlexGridSizer1->AddGrowableCol(0);
	FlexGridSizer2 = new wxFlexGridSizer(0, 3, 0, 0);
	FlexGridSizer2->AddGrowableCol(1);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Shader File:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	FlexGridSizer2->Add(StaticText1, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	FilePickerCtrl1 = new BulkEditFilePickerCtrl(this, ID_0FILEPICKERCTRL_IFS, wxEmptyString, wxEmptyString, _T("*.fs"), wxDefaultPosition, wxDefaultSize, wxFLP_FILE_MUST_EXIST|wxFLP_OPEN|wxFLP_USE_TEXTCTRL, wxDefaultValidator, _T("ID_0FILEPICKERCTRL_IFS"));
	FlexGridSizer2->Add(FilePickerCtrl1, 1, wxALL|wxEXPAND, 2);
	Button_Download = new wxButton(this, ID_BUTTON1, _("Download"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
	FlexGridSizer2->Add(Button_Download, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	FlexGridSizer1->Add(FlexGridSizer2, 1, wxALL|wxEXPAND, 2);
	FlexGridSizer3 = new wxFlexGridSizer(0, 4, 0, 0);
	FlexGridSizer3->AddGrowableCol(1);
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Lead in frames:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	FlexGridSizer3->Add(StaticText2, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	Slider_Shader_LeadIn = new BulkEditSlider(this, IDD_SLIDER_Shader_LeadIn, 0, 0, 1000, wxDefaultPosition, wxSize(200,-1), 0, wxDefaultValidator, _T("IDD_SLIDER_Shader_LeadIn"));
	FlexGridSizer3->Add(Slider_Shader_LeadIn, 1, wxALL|wxEXPAND, 2);
	FlexGridSizer3->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	TextCtrl_Shader_LeadIn = new BulkEditTextCtrl(this, ID_TEXTCTRL_Shader_LeadIn, _("0"), wxDefaultPosition, wxDLG_UNIT(this,wxSize(40,-1)), wxTE_RIGHT, wxDefaultValidator, _T("ID_TEXTCTRL_Shader_LeadIn"));
	FlexGridSizer3->Add(TextCtrl_Shader_LeadIn, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Time Speed:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	FlexGridSizer3->Add(StaticText3, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	Slider_Shader_Speed = new BulkEditSliderF2(this, ID_SLIDER_Shader_Speed, 100, -1000, 1000, wxDefaultPosition, wxSize(200,-1), 0, wxDefaultValidator, _T("ID_SLIDER_Shader_Speed"));
	FlexGridSizer3->Add(Slider_Shader_Speed, 1, wxALL|wxEXPAND, 2);
	BitmapButton_Shader_Speed = new BulkEditValueCurveButton(this, ID_VALUECURVE_Shader_Speed, wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("xlART_valuecurve_notselected")),wxART_BUTTON), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|wxNO_BORDER, wxDefaultValidator, _T("ID_VALUECURVE_Shader_Speed"));
	FlexGridSizer3->Add(BitmapButton_Shader_Speed, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	TextCtrl_Shader_Speed = new BulkEditTextCtrlF2(this, IDD_TEXTCTRL_Shader_Speed, _("1.00"), wxDefaultPosition, wxDLG_UNIT(this,wxSize(40,-1)), wxTE_RIGHT, wxDefaultValidator, _T("IDD_TEXTCTRL_Shader_Speed"));
	FlexGridSizer3->Add(TextCtrl_Shader_Speed, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	FlexGridSizer1->Add(FlexGridSizer3, 1, wxALL|wxEXPAND, 2);
	FlexGridSizer_Dynamic = new wxFlexGridSizer(0, 3, 0, 0);
	FlexGridSizer_Dynamic->AddGrowableCol(1);
	FlexGridSizer1->Add(FlexGridSizer_Dynamic, 1, wxALL|wxEXPAND, 2);
	SetSizer(FlexGridSizer1);
	FlexGridSizer1->Fit(this);
	FlexGridSizer1->SetSizeHints(this);

	Connect(ID_0FILEPICKERCTRL_IFS,wxEVT_COMMAND_FILEPICKER_CHANGED,(wxObjectEventFunction)&ShaderPanel::OnFilePickerCtrl1FileChanged);
	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ShaderPanel::OnButton_DownloadClick);
	//*)

    Connect(ID_VALUECURVE_Shader_Speed, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)& ShaderPanel::OnVCButtonClick);

    Connect(wxID_ANY, EVT_VC_CHANGED, (wxObjectEventFunction)& ShaderPanel::OnVCChanged, 0, this);

    BitmapButton_Shader_Speed->GetValue()->SetLimits(SHADER_SPEED_MIN, SHADER_SPEED_MAX);
    BitmapButton_Shader_Speed->GetValue()->SetDivisor(SHADER_SPEED_DIVISOR);

	_preview = new ShaderPreview( this, ID_CANVAS );
}

ShaderPanel::~ShaderPanel()
{
    if (_shaderConfig != nullptr) delete _shaderConfig;
	//(*Destroy(ShaderPanel)
	//*)
}

PANEL_EVENT_HANDLERS(ShaderPanel)

void ShaderPanel::OnFilePickerCtrl1FileChanged(wxFileDirPickerEvent& event)
{
    static wxString last = "";

    wxString newf = FilePickerCtrl1->GetFileName().GetFullName();
    ObtainAccessToURL(newf.ToStdString());
    // if shader name hasnt changed dont reset
    if (newf == last && (newf == "" || wxFile::Exists(FilePickerCtrl1->GetFileName().GetFullPath())))
    {
        return;
    }

    // restore time to defaults
    BitmapButton_Shader_Speed->SetActive(false);
    Slider_Shader_LeadIn->SetValue(0);
    Slider_Shader_Speed->SetValue(100);
    TextCtrl_Shader_LeadIn->SetValue("0");
    TextCtrl_Shader_Speed->SetValue("1.0");

    if (wxFile::Exists(FilePickerCtrl1->GetFileName().GetFullPath()))
    {
        if (BuildUI(FilePickerCtrl1->GetFileName().GetFullPath()))
        {
            last = newf;
        }
    }
    else
    {
        Freeze();
        last = "";
        FlexGridSizer_Dynamic->DeleteWindows();
        FilePickerCtrl1->UnsetToolTip();
        Thaw();
    }
}

bool ShaderPanel::BuildUI(const wxString& filename)
{
    if (_shaderConfig != nullptr && _shaderConfig->GetFilename() == filename)
    {
        return false;
    }

    Freeze();

    FlexGridSizer_Dynamic->DeleteWindows();
    FilePickerCtrl1->UnsetToolTip();

    _shaderConfig = ShaderEffect::ParseShader(filename);

    if (_shaderConfig != nullptr)
    {
        wxString desc = _shaderConfig->GetDescription();
        if (desc != "") desc += "\n";
		if (_shaderConfig->IsCanvasShader())
		{
			desc += "Use Canvas Mode for this shader.";

			// Turn on canvas mode as this really only makes sense in canvas mode
			xLightsFrame* frame = xLightsApp::GetFrame();
			TimingPanel* layerBlendingPanel = frame->GetLayerBlendingPanel();
			layerBlendingPanel->CheckBox_Canvas->SetValue(true);
		}
        wxString const shortName = wxFileName(_shaderConfig->GetFilename()).GetFullName();
        FilePickerCtrl1->SetToolTip(shortName + "\n\n" + desc);

        for (const auto& it : _shaderConfig->GetParms())
        {
            if (it.ShowParm())
            {
                if (it._type == ShaderParmType::SHADER_PARM_FLOAT)
                {
                    auto staticText = new wxStaticText(this, wxNewId(), it.GetLabel(), wxDefaultPosition, wxDefaultSize, 0, it.GetId(ShaderCtrlType::SHADER_CTRL_STATIC));
                    FlexGridSizer_Dynamic->Add(staticText, 1, wxALL | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 2);

                    auto sizer = new wxFlexGridSizer(0, 2, 0, 0);
                    sizer->AddGrowableCol(0);

                    auto slider = new BulkEditSliderF2(this, wxNewId(), it._default * 100, it._min * 100, it._max * 100, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, it.GetId(ShaderCtrlType::SHADER_CTRL_SLIDER));
                    sizer->Add(slider, 1, wxALL | wxEXPAND, 2);
                    auto id = wxNewId();
                    auto vcb = new BulkEditValueCurveButton(this, id, wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("xlART_valuecurve_notselected")), wxART_BUTTON), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW | wxNO_BORDER, wxDefaultValidator, it.GetId(ShaderCtrlType::SHADER_CTRL_VALUECURVE));
                    sizer->Add(vcb, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 2);
                    vcb->GetValue()->SetLimits(it._min*100, it._max*100);
                    vcb->GetValue()->SetDivisor(100);
                    Connect(id, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)& ShaderPanel::OnVCButtonClick);

                    FlexGridSizer_Dynamic->Add(sizer, 1, wxALL | wxEXPAND, 0);

                    auto def = wxString::Format("%.2f", it._default);
                    auto text = new BulkEditTextCtrlF2(this, wxNewId(), def, wxDefaultPosition, wxDLG_UNIT(this, wxSize(30, -1)), 0, wxDefaultValidator, it.GetId(ShaderCtrlType::SHADER_CTRL_TEXTCTRL));
                    FlexGridSizer_Dynamic->Add(text, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 2);
                }
                else if (it._type == ShaderParmType::SHADER_PARM_LONG)
                {
                    auto staticText = new wxStaticText(this, wxNewId(), it.GetLabel(), wxDefaultPosition, wxDefaultSize, 0, it.GetId(ShaderCtrlType::SHADER_CTRL_STATIC));
                    FlexGridSizer_Dynamic->Add(staticText, 1, wxALL | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 2);

                    auto sizer = new wxFlexGridSizer(0, 2, 0, 0);
                    sizer->AddGrowableCol(0);

                    auto slider = new BulkEditSlider(this, wxNewId(), it._default, it._min, it._max, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, it.GetId(ShaderCtrlType::SHADER_CTRL_SLIDER));
                    sizer->Add(slider, 1, wxALL | wxEXPAND, 2);
                    auto id = wxNewId();
                    auto vcb = new BulkEditValueCurveButton(this, id, wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("xlART_valuecurve_notselected")), wxART_BUTTON), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW | wxNO_BORDER, wxDefaultValidator, it.GetId(ShaderCtrlType::SHADER_CTRL_VALUECURVE));
                    sizer->Add(vcb, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 2);
                    vcb->GetValue()->SetLimits(it._min, it._max);
                    Connect(id, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)& ShaderPanel::OnVCButtonClick);

                    FlexGridSizer_Dynamic->Add(sizer, 1, wxALL | wxEXPAND, 0);

                    auto def = wxString::Format("%l", (long)it._default);
                    auto text = new BulkEditTextCtrl(this, wxNewId(), def, wxDefaultPosition, wxDLG_UNIT(this, wxSize(30, -1)), 0, wxDefaultValidator, it.GetId(ShaderCtrlType::SHADER_CTRL_TEXTCTRL));
                    FlexGridSizer_Dynamic->Add(text, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 2);
                }
                else if (it._type == ShaderParmType::SHADER_PARM_LONGCHOICE)
                {
                    auto staticText = new wxStaticText(this, wxNewId(), it.GetLabel(), wxDefaultPosition, wxDefaultSize, 0, it.GetId(ShaderCtrlType::SHADER_CTRL_STATIC));
                    FlexGridSizer_Dynamic->Add(staticText, 1, wxALL | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 2);

                    auto choice = new BulkEditChoice(this, wxNewId(), wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, it.GetId(ShaderCtrlType::SHADER_CTRL_CHOICE));
                    for (auto it2 : it.GetChoices())
                    {
                        choice->AppendString(it2);
                    }
                    choice->SetSelection(it._default);
                    FlexGridSizer_Dynamic->Add(choice, 1, wxTOP | wxBOTTOM | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 5);
                    FlexGridSizer_Dynamic->Add(-1, -1, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 2);
                }
                else if (it._type == ShaderParmType::SHADER_PARM_BOOL)
                {
                    auto staticText = new wxStaticText(this, wxNewId(), it.GetLabel(), wxDefaultPosition, wxDefaultSize, 0, it.GetId(ShaderCtrlType::SHADER_CTRL_STATIC));
                    FlexGridSizer_Dynamic->Add(staticText, 1, wxALL | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 2);
                    auto checkbox = new BulkEditCheckBox(this, wxNewId(), _(""), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, it.GetId(ShaderCtrlType::SHADER_CTRL_CHECKBOX));
                    checkbox->SetValue(it._default == 1);
                    FlexGridSizer_Dynamic->Add(checkbox, 1, wxALL | wxEXPAND, 2);
                    FlexGridSizer_Dynamic->Add(-1, -1, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 2);
                }
                else if (it._type == ShaderParmType::SHADER_PARM_POINT2D)
                {
                    auto staticText = new wxStaticText(this, wxNewId(), it.GetLabel() + " X", wxDefaultPosition, wxDefaultSize, 0, it.GetId(ShaderCtrlType::SHADER_CTRL_STATIC) +"X");
                    FlexGridSizer_Dynamic->Add(staticText, 1, wxALL | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 2);

                    auto sizer = new wxFlexGridSizer(0, 2, 0, 0);
                    sizer->AddGrowableCol(0);

                    auto slider = new BulkEditSliderF2(this, wxNewId(), it._defaultPt.x*100, it._minPt.x*100, it._maxPt.x*100, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, it.GetId(ShaderCtrlType::SHADER_CTRL_SLIDER)+"X");
                    sizer->Add(slider, 1, wxALL | wxEXPAND, 2);
                    auto id = wxNewId();
                    auto vcb = new BulkEditValueCurveButton(this, id, wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("xlART_valuecurve_notselected")), wxART_BUTTON), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW | wxNO_BORDER, wxDefaultValidator, it.GetId(ShaderCtrlType::SHADER_CTRL_VALUECURVE)+"X");
                    sizer->Add(vcb, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 2);
                    vcb->GetValue()->SetLimits(it._minPt.x*100, it._maxPt.x*100);
                    vcb->GetValue()->SetDivisor(100);
                    Connect(id, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)& ShaderPanel::OnVCButtonClick);

                    FlexGridSizer_Dynamic->Add(sizer, 1, wxALL | wxEXPAND, 0);

                    auto def = wxString::Format("%.2f", it._defaultPt.x);
                    auto text = new BulkEditTextCtrlF2(this, wxNewId(), def, wxDefaultPosition, wxDLG_UNIT(this, wxSize(30, -1)), 0, wxDefaultValidator, it.GetId(ShaderCtrlType::SHADER_CTRL_TEXTCTRL)+"X");
                    FlexGridSizer_Dynamic->Add(text, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 2);

                    staticText = new wxStaticText(this, wxNewId(), it.GetLabel() + " Y", wxDefaultPosition, wxDefaultSize, 0, it.GetId(ShaderCtrlType::SHADER_CTRL_STATIC) + "Y");
                    FlexGridSizer_Dynamic->Add(staticText, 1, wxALL | wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL, 2);

                    sizer = new wxFlexGridSizer(0, 2, 0, 0);
                    sizer->AddGrowableCol(0);

                    slider = new BulkEditSliderF2(this, wxNewId(), it._defaultPt.y * 100, it._minPt.y * 100, it._maxPt.y * 100, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, it.GetId(ShaderCtrlType::SHADER_CTRL_SLIDER) + "Y");
                    sizer->Add(slider, 1, wxALL | wxEXPAND, 2);
                    id = wxNewId();
                    vcb = new BulkEditValueCurveButton(this, id, wxArtProvider::GetBitmap(wxART_MAKE_ART_ID_FROM_STR(_T("xlART_valuecurve_notselected")), wxART_BUTTON), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW | wxNO_BORDER, wxDefaultValidator, it.GetId(ShaderCtrlType::SHADER_CTRL_VALUECURVE) + "Y");
                    sizer->Add(vcb, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 2);
                    vcb->GetValue()->SetLimits(it._minPt.y * 100, it._maxPt.y * 100);
                    vcb->GetValue()->SetDivisor(100);
                    Connect(id, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)& ShaderPanel::OnVCButtonClick);

                    FlexGridSizer_Dynamic->Add(sizer, 1, wxALL | wxEXPAND, 0);

                    def = wxString::Format("%.2f", it._defaultPt.y);
                    text = new BulkEditTextCtrlF2(this, wxNewId(), def, wxDefaultPosition, wxDLG_UNIT(this, wxSize(30, -1)), 0, wxDefaultValidator, it.GetId(ShaderCtrlType::SHADER_CTRL_TEXTCTRL) + "Y");
                    FlexGridSizer_Dynamic->Add(text, 1, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 2);
                }
            }
        }
    }

    Layout();

    SetMinSize(FlexGridSizer1->CalcMin());

    wxScrolledWindow* sw = dynamic_cast<wxScrolledWindow*>(GetParent());
    sw->FitInside();
    sw->SetScrollRate(5, 5);
    sw->Refresh();

    Thaw();

    return true;
}

void ShaderPanel::OnButton_DownloadClick(wxCommandEvent& event)
{
    wxProgressDialog prog("Shader download", "Downloading shaders ...", 100, this, wxPD_APP_MODAL | wxPD_AUTO_HIDE);
    prog.Show();
    ShaderDownloadDialog dlg(this);
    SetCursor(wxCURSOR_WAIT);
    if (dlg.DlgInit(&prog, 0, 100))
    {
        prog.Update(100);
        SetCursor(wxCURSOR_DEFAULT);
        if (dlg.ShowModal() == wxID_OK)
        {
            FilePickerCtrl1->SetFileName(wxFileName(dlg.GetShaderFile()));
            wxFileDirPickerEvent e(wxEVT_COMMAND_FILEPICKER_CHANGED, FilePickerCtrl1, ID_0FILEPICKERCTRL_IFS, FilePickerCtrl1->GetFileName().GetFullPath());
            wxPostEvent(this, e);
        }
    }
    else
    {
        SetCursor(wxCURSOR_DEFAULT);
    }
}
