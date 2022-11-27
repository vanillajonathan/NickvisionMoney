#include "pdfdocument.hpp"
#include <filesystem>
#include <stdexcept>

using namespace NickvisionMoney::Models;

PDFDocument::PDFDocument()
{
    m_handle = HPDF_New([](HPDF_STATUS, HPDF_STATUS, void*){ throw std::runtime_error("PDF Error"); }, nullptr);
    HPDF_UseUTFEncodings(m_handle);
}

PDFDocument::~PDFDocument()
{
    HPDF_Free(m_handle);
}

bool PDFDocument::save(const std::string& path) const
{
    try
    {
         HPDF_SaveToFile(m_handle, path.c_str());
         return true;
    }
    catch(...)
    {
        return false;
    }
}

std::vector<PDFPage>& PDFDocument::getPages()
{
    return m_pages;
}

int PDFDocument::addPage(HPDF_PageSizes size, HPDF_PageDirection orientation)
{
    try
    {
        m_pages.push_back({ m_handle, size, orientation });
        return m_pages.size() - 1;
    }
    catch(...)
    {
        return -1;
    }
}

std::string PDFDocument::loadTTFontFromFile(const std::string& path)
{
    if(!std::filesystem::exists(path))
    {
        return "";
    }
    return HPDF_LoadTTFontFromFile(m_handle, path.c_str(), HPDF_TRUE);
}
