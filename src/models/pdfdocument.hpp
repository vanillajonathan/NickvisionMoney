#pragma once

#include <optional>
#include <string>
#include <vector>
#include <hpdf.h>
#include "pdfpage.hpp"

namespace NickvisionMoney::Models
{
	class PDFDocument
	{
	public:
		PDFDocument(const std::string& path);
		~PDFDocument();
		bool save() const;
		std::vector<PDFPage>& getPages();
		int addPage(HPDF_PageSizes size, HPDF_PageDirection orientation);

	private:
		std::string m_path;
		bool m_success;
		HPDF_Doc m_handle;
		std::vector<PDFPage> m_pages;
	};
}