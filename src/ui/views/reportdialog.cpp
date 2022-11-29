#include "reportdialog.hpp"
#include "../../helpers/translation.hpp"

using namespace NickvisionMoney::Controllers;
using namespace NickvisionMoney::UI::Views;

ReportDialog::ReportDialog(GtkWindow* parent, ReportDialogController& controller) : m_controller{ controller }, m_gobj{ adw_message_dialog_new(parent, _("Finance Report"), nullptr) }
{
    //Dialog Settings
    gtk_window_set_hide_on_close(GTK_WINDOW(m_gobj), true);
    adw_message_dialog_add_responses(ADW_MESSAGE_DIALOG(m_gobj), "close", _("Close"), "export", _("Export as PDF"), nullptr);
    adw_message_dialog_set_response_appearance(ADW_MESSAGE_DIALOG(m_gobj), "export", ADW_RESPONSE_SUGGESTED);
    adw_message_dialog_set_default_response(ADW_MESSAGE_DIALOG(m_gobj), "close");
    adw_message_dialog_set_close_response(ADW_MESSAGE_DIALOG(m_gobj), "close");
    g_signal_connect(m_gobj, "response", G_CALLBACK((void (*)(AdwMessageDialog*, gchar*, gpointer))([](AdwMessageDialog*, gchar* response, gpointer data) { reinterpret_cast<ReportDialog*>(data)->setResponse({ response }); })), this);
    //Main Box
    m_boxMain = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    //Mode Switcher Buttons
    m_btnBalance = gtk_toggle_button_new_with_label(_("Balance"));
    gtk_widget_set_name(m_btnBalance, "btnBalance");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(m_btnBalance), true);
    g_signal_connect(m_btnBalance, "clicked", G_CALLBACK((void (*)(GtkWidget*, gpointer))[](GtkWidget* btn, gpointer data) { reinterpret_cast<ReportDialog*>(data)->onModeChanged(btn); }), this);
    m_btnIncome = gtk_toggle_button_new_with_label(_("Income"));
    gtk_widget_set_name(m_btnIncome, "btnIncome");
    g_signal_connect(m_btnIncome, "clicked", G_CALLBACK((void (*)(GtkWidget*, gpointer))[](GtkWidget* btn, gpointer data) { reinterpret_cast<ReportDialog*>(data)->onModeChanged(btn); }), this);
    m_btnExpense = gtk_toggle_button_new_with_label(_("Expense"));
    gtk_widget_set_name(m_btnExpense, "btnExpense");
    g_signal_connect(m_btnExpense, "clicked", G_CALLBACK((void (*)(GtkWidget*, gpointer))[](GtkWidget* btn, gpointer data) { reinterpret_cast<ReportDialog*>(data)->onModeChanged(btn); }), this);
    m_btnGroups = gtk_toggle_button_new_with_label(_("Groups"));
    gtk_widget_set_name(m_btnGroups, "btnGroups");
    g_signal_connect(m_btnGroups, "clicked", G_CALLBACK((void (*)(GtkWidget*, gpointer))[](GtkWidget* btn, gpointer data) { reinterpret_cast<ReportDialog*>(data)->onModeChanged(btn); }), this);
    //Mode Switcher
    m_boxSwitcher = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_add_css_class(m_boxSwitcher, "linked");
    gtk_widget_set_halign(m_boxSwitcher, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(m_boxSwitcher), m_btnBalance);
    gtk_box_append(GTK_BOX(m_boxSwitcher), m_btnIncome);
    gtk_box_append(GTK_BOX(m_boxSwitcher), m_btnExpense);
    gtk_box_append(GTK_BOX(m_boxSwitcher), m_btnGroups);
    gtk_box_append(GTK_BOX(m_boxMain), m_boxSwitcher);
    //Drawing Area
    m_drArea = gtk_drawing_area_new();
    gtk_widget_add_css_class(m_drArea, "card");
    gtk_widget_set_size_request(m_drArea, 400, 250);
    gtk_box_append(GTK_BOX(m_boxMain), m_drArea);
    //List Preferences Group
    m_pgList = adw_preferences_group_new();
    gtk_widget_set_margin_top(m_pgList, 10);
    gtk_widget_set_margin_start(m_pgList, 10);
    gtk_widget_set_margin_end(m_pgList, 10);
    gtk_widget_set_margin_bottom(m_pgList, 10);
    //Scrolled Window
    m_scrollWindow = gtk_scrolled_window_new();
    gtk_widget_add_css_class(m_scrollWindow, "card");
    gtk_widget_set_size_request(m_scrollWindow, 400, 200);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(m_scrollWindow), m_pgList);
    gtk_box_append(GTK_BOX(m_boxMain), m_scrollWindow);
    //Layout
    adw_message_dialog_set_extra_child(ADW_MESSAGE_DIALOG(m_gobj), m_boxMain);
}

GtkWidget* ReportDialog::gobj()
{
    return m_gobj;
}

bool ReportDialog::run()
{
    gtk_widget_show(m_gobj);
    gtk_window_set_modal(GTK_WINDOW(m_gobj), true);
    while(gtk_widget_is_visible(m_gobj))
    {
        g_main_context_iteration(g_main_context_default(), false);
    }
    // if(m_controller.getResponse() == "export")
    // {
    //     gtk_widget_hide(m_gobj);
    //     gtk_window_set_modal(GTK_WINDOW(m_gobj), false);

    // }
    gtk_window_destroy(GTK_WINDOW(m_gobj));
    return m_controller.getResponse() == "export";
}

void ReportDialog::onModeChanged(GtkWidget* modeButton)
{
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(modeButton), true);
    std::string modeButtonName = gtk_widget_get_name(modeButton);
    if(modeButtonName.find("Balance") == std::string::npos)
    {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(m_btnBalance), false);
    }
    if(modeButtonName.find("Income") == std::string::npos)
    {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(m_btnIncome), false);
    }
    if(modeButtonName.find("Expense") == std::string::npos)
    {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(m_btnExpense), false);
    }
    if(modeButtonName.find("Groups") == std::string::npos)
    {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(m_btnGroups), false);
    }
}

void ReportDialog::setResponse(const std::string& response)
{
    m_controller.setResponse(response);
}
