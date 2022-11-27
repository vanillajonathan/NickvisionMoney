#pragma once

#include <string>
#include <hpdf.h>

namespace NickvisionMoney::Models
{
	class PDFPage
	{
	public:
		PDFPage(HPDF_Doc parent, HPDF_PageSizes size, HPDF_PageDirection orientation);
		float getWidth() const;
		float getHeight() const;
		bool setFont(const std::string& name, float size = 9);
		bool drawRectangle(float lineWidth, float x, float y, float width, float height);
		bool drawLine(float lineWidth, float x, float y, float width);
		bool drawText(float x, float y, const std::string& text);
		bool drawJPEG(float x, float y, float width, float height, const std::string& pathToJPEG);

	private:
		HPDF_Doc m_parent;
		HPDF_Page m_handle;
		float m_width;
		float m_height;
	};
}