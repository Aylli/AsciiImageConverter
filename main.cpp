#include <wx/wxprec.h>
 
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/filedlg.h>
#include <wx/wfstream.h>
#include <wx/image.h>
#include <wx/statbmp.h>
#include <wx/bitmap.h>
#include <wx/control.h>
#include "src/defaultImage.xpm"
#include "src/symbols.h"
 
class AsciiConverterApp : public wxApp
{
public:
    virtual bool OnInit();
};
 
class MyFrame : public wxFrame
{
public:
    MyFrame();
 
private:
    wxImage Image;
    wxStaticBitmap *DisplayedImage;
    wxTextCtrl *text;
    int accurancy = 5; // Точность выборки. Чем меньше значение, тем больше точность

    void OnHello(wxCommandEvent& event);
    void OnFileOpen(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
};
 
enum
{
    ID_OpenDialog = 1
};
 
wxIMPLEMENT_APP(AsciiConverterApp);
 
bool AsciiConverterApp::OnInit()
{
    FreeConsole();
    MyFrame *frame = new MyFrame();
    frame->Show(true);
    return true;
}

MyFrame::MyFrame()
    : wxFrame(nullptr, wxID_ANY, "Converter")
{
    // ------- Settings -------
    wxInitAllImageHandlers();

    // ------- Window -------
    wxMenuBar *menuBar = new wxMenuBar;
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_OpenDialog, "&Open image");
    menuBar->Append(menuFile, "&File");

    SetMenuBar( menuBar );

    wxGridSizer *gs = new wxGridSizer(1, 2, 1, 1);

    DisplayedImage = new wxStaticBitmap(this, wxID_ANY, wxBitmap(wxImage(default_image_xpm)));
    DisplayedImage->SetScaleMode(wxStaticBitmap::ScaleMode::Scale_Fill);

    text = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
    text->SetFont(wxFont{8, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL});

    gs->Add(DisplayedImage, 0, wxEXPAND);
    gs->Add(text, 1, wxEXPAND);

    SetSizer(gs);
 
    CreateStatusBar();
    SetStatusText("Open an image");

    Bind(wxEVT_MENU, &MyFrame::OnFileOpen, this, ID_OpenDialog);
}


// Открытие файла
void MyFrame::OnFileOpen(wxCommandEvent& event)
{
    // Создаём диалоговое окно
    wxFileDialog openFileDialog(this, ("Open image"), "", "",
                                "BMP and GIF files (*.bmp;*.gif)|*.bmp;*.gif|PNG files (*.png)|*.png|JPG files (*.jpg;*.jpeg)|*.jpg;*.jpeg",
                                wxFD_OPEN|wxFD_FILE_MUST_EXIST);

    if(openFileDialog.ShowModal() == wxID_CANCEL) return; // При нажатии "Отмена"
    wxFileInputStream input_stream(openFileDialog.GetPath()); // Сохраняем выбранный адрес файла
    if(!input_stream.IsOk()) {
        wxLogError("Something wrong with file '%s'", openFileDialog.GetPath());
        return;
    } else {
        text->Clear();
        // Сообщаем в строке состояния об открытии и отображаем файл на экране
        SetStatusText(openFileDialog.GetPath() + " is opened");
        Image.LoadFile(input_stream, wxBITMAP_TYPE_ANY, wxID_ANY);
        float heightScale = (float)Image.GetHeight() / (float)Image.GetWidth();
        Image = Image.Rescale(400, ceil(heightScale * 400));
        DisplayedImage->SetBitmap(Image);

        // Обработаем изображение пока тут
        std::string imageStr;
        for(int y = 0; y < Image.GetHeight(); y+=accurancy*2) {
            for(int x = 0; x < Image.GetWidth(); x+=accurancy) 
                imageStr.push_back(_symbols[ceil((((float)Image.GetRed(x, y) + (float)Image.GetGreen(x, y) + (float)Image.GetBlue(x, y))/3)/256*18)]);
            imageStr.push_back('\n');
        }
        text->AppendText(imageStr); // Добавляем изображения в поле ввода
        imageStr.clear();
    }
}