#pragma once

#include <string>
#include <vector>
#include <hpdf.h>
#include "pdfpage.hpp"

namespace NickvisionMoney::Models
{
	/**
	 * A wrapper for HPDF_Doc
	 */
	class PDFDocument
	{
	public:
		/**
		 * Constructs a PDFDocument
		 */
		PDFDocument();
		/**
		 * Destructs a PDFDocument
		 */
		~PDFDocument();
		/**
		 * Saves the PDF to disk
		 *
		 * @param path The save path
		 * @returns True if successful, else false
		 */
		bool save(const std::string& path) const;
		/**
		 * Gets the list of pages added to the PDF
		 *
		 * @returns The reference list of pages added to the PDF
		 */
		std::vector<PDFPage>& getPages();
		/**
		 * Adds a page to the PDF
		 *
		 * @param size The HPDF_PageSizes
		 * @param orientation The HPDF_PageDirection
		 * @returns The index of the added page. -1 if unable to add page
		 */
		int addPage(HPDF_PageSizes size, HPDF_PageDirection orientation);

	private:
		HPDF_Doc m_handle;
		std::vector<PDFPage> m_pages;
	};
}