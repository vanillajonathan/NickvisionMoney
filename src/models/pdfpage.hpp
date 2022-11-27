#pragma once

#include <string>
#include <hpdf.h>

namespace NickvisionMoney::Models
{
	/**
	 * A wrapper for HPDF_Page
	 */
	class PDFPage
	{
	public:
		/**
		 * Constructs a PDFPage
		 *
		 * @param parent The HPDF_Doc
		 * @param size The HPDF_PageSizes
		 * @param orientation The HPDF_PageDirection
		 */
		PDFPage(HPDF_Doc parent, HPDF_PageSizes size, HPDF_PageDirection orientation);
		/**
		 * Gets the width of the page
		 *
		 * @returns The width of the page
		 */
		float getWidth() const;
		/**
		 * Gets the height of the page
		 *
		 * @returns The height of the page
		 */
		float getHeight() const;
		/**
		 * Sets the font of the page
		 *
		 * @param name The font name
		 * @param size The point size to use
		 * @returns True if successful, else false
		 */
		bool setFont(const std::string& name, float size = 9);
		/**
		 * Draws a rectangle on the page
		 *
		 * @param lineWidth The line width of the rectangle
		 * @param x The x position of the rectangle
		 * @param y The y position of the rectangle
		 * @param width The width of the rectangle
		 * @param height The height of the rectangle
		 * @returns True if successful, else false
		 */
		bool drawRectangle(float lineWidth, float x, float y, float width, float height);
		/**
		 * Draws a line on the page
		 *
		 * @param lineWidth The line width of the line
		 * @param x The x position of the line
		 * @param y The y position of the line
		 * @param width The width of the line
		 * @returns True if successful, else false
		 */
		bool drawLine(float lineWidth, float x, float y, float width);
		/**
		 * Draws text on the page
		 *
		 * @param x The x position of the text
		 * @param y The y position of the text
		 * @param text The string to draw
		 * @returns True if successful, else false
		 */
		bool drawText(float x, float y, const std::string& text);
		/**
		 * Draws a JPEG image on the page
		 *
		 * @param x The x position of the image
		 * @param y The y position of the image
		 * @param width The width of the image
		 * @param height The height of the image
		 * @param path The path to the JPEG image on disk
		 */
		bool drawJPEG(float x, float y, float width, float height, const std::string& path);

	private:
		HPDF_Doc m_parent;
		HPDF_Page m_handle;
		float m_width;
		float m_height;
	};
}