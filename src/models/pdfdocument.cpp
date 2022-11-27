#include "pdfdocument.hpp"
#include <stdexcept>

using namespace NickvisionMoney::Models;

PDFDocument::PDFDocument(const std::string& path) : m_path{ path }, m_success{ true }
{
    m_handle = HPDF_New([](HPDF_STATUS, HPDF_STATUS, void*){ throw std::runtime_error("PDF Error"); }, nullptr);
}

PDFDocument::~PDFDocument()
{
    HPDF_Free(m_handle);
}

bool PDFDocument::save() const
{
    try
    {
         HPDF_SaveToFile(m_handle, m_path.c_str());
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
