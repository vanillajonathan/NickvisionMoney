#pragma once

#include <string>
#include <adwaita.h>
#include "../../controllers/reportdialogcontroller.hpp"

namespace NickvisionMoney::UI::Views
{
	/**
     	* A dialog for showing a finance report
     	*/
 	class ReportDialog
 	{
 	public:
 		/**
		 * Constructs a ReportDialog
		 *
		 * @param parent The parent window for the dialog
		 * @param controller The ReportDialogController
		 */
		ReportDialog(GtkWindow* parent, NickvisionMoney::Controllers::ReportDialogController& controller);
		/**
 	   	 * Gets the GtkWidget* representing the ReportDialog
    		 *
    		 * @returns The GtkWidget* representing the ReportDialog
    		 */
		GtkWidget* gobj();
		/**
	    	 * Run the ReportDialog
	    	 *
	    	 * @returns True if export as PDF option was accepted, else false
	    	 */
		bool run();

 	private:
 		NickvisionMoney::Controllers::ReportDialogController& m_controller;
		GtkWidget* m_gobj;
		GtkWidget* m_boxMain{ nullptr };
		GtkWidget* m_btnBalance{ nullptr };
		GtkWidget* m_btnIncome{ nullptr };
		GtkWidget* m_btnExpense{ nullptr };
		GtkWidget* m_btnGroups{ nullptr };
		GtkWidget* m_boxSwitcher{ nullptr };
		GtkWidget* m_drArea{ nullptr };
		GtkWidget* m_pgList{ nullptr };
		GtkWidget* m_scrollWindow{ nullptr };
		/**
		 * Changes mode after button click
		 *
		 * @param modeButton The button that is clicked to change mode
		 */
		void onModeChanged(GtkToggleButton* modeButton);
		/**
    	 * Sets the response
    	 *
    	 * @param The new response
    	 */
		void setResponse(const std::string& response);
 	};
}