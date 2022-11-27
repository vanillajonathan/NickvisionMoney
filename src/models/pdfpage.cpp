#include "pdfdocument.hpp"
#include <filesystem>

using namespace NickvisionMoney::Models;

PDFPage::PDFPage(HPDF_Doc parent, HPDF_PageSizes size, HPDF_PageDirection orientation) : m_parent{ parent }, m_handle{ HPDF_AddPage(m_parent) }
{
    HPDF_Page_SetSize(m_handle, size, orientation);
    m_width = HPDF_Page_GetWidth(m_handle);
    m_height = HPDF_Page_GetHeight(m_handle);
}

float PDFPage::getWidth() const
{
    return m_width;
}

float PDFPage::getHeight() const
{
    return m_height;
}

bool PDFPage::setFont(const std::string& name, float size)
{
    try
    {
        HPDF_Font font{ HPDF_GetFont(m_parent, name.c_str(), nullptr) };
        HPDF_Page_SetFontAndSize(m_handle, font, size);
        return true;
    }
    catch(...)
    {
        return false;
    }
}

bool PDFPage::drawRectangle(float lineWidth, float x, float y, float width, float height)
{
    try
    {
        HPDF_Page_SetLineWidth(m_handle, lineWidth);
        HPDF_Page_Rectangle(m_handle, x, y, width, height);
        HPDF_Page_Stroke(m_handle);
        return true;
    }
    catch(...)
    {
        return false;
    }
}

bool PDFPage::drawLine(float lineWidth, float x, float y, float width)
{
    return drawRectangle(lineWidth, x, y, width, 0);
}

bool PDFPage::drawText(float x, float y, const std::string& text)
{
    try
    {
        HPDF_Page_BeginText(m_handle);
        HPDF_Page_MoveTextPos(m_handle, x, y);
        HPDF_Page_ShowText(m_handle, text.c_str());
        HPDF_Page_EndText(m_handle);
        return true;
    }
    catch(...)
    {
        return false;
    }
}

bool PDFPage::drawJPEG(float x, float y, float width, float height, const std::string& pathToJPEG)
{
    if(!std::filesystem::exists(pathToJPEG))
    {
        return false;
    }
    try
    {
        HPDF_Image image{ HPDF_LoadJpegImageFromFile(m_parent, pathToJPEG.c_str()) };
        HPDF_Page_DrawImage(m_handle, image, x, y, width, height);
        return true;
    }
    catch(...)
    {
        return false;
    }
}
