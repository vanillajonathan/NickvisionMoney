#pragma once

#include <string>

namespace NickvisionMoney::Controllers
{
	/**
	 * A controller for the ReportDialog
	 */
	class ReportDialogController
	{
	public:
		/**
		 * Constructs a ReportDialogController
		 */
		ReportDialogController();
		/**
		 * Gets the response of the dialog
		 *
		 * @returns The response of the dialog
		 */
		const std::string& getResponse() const;
		/**
		 * Sets the response of the dialog
		 *
		 * @param response The new response of the dialog
		 */
		void setResponse(const std::string& response);

	private:
		std::string m_response;
	};
}