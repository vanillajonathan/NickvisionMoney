#include "accountview.hpp"
#include "groupdialog.hpp"
#include "transactiondialog.hpp"
#include "../controls/messagedialog.hpp"
#include "../controls/progressdialog.hpp"
#include "../../helpers/translation.hpp"

using namespace NickvisionMoney::Controllers;
using namespace NickvisionMoney::Models;
using namespace NickvisionMoney::UI::Controls;
using namespace NickvisionMoney::UI::Views;

AccountView::AccountView(GtkWindow* parentWindow, AdwTabView* parentTabView, GtkWidget* btnFlapToggle, const AccountViewController& controller) : m_controller{ controller }, m_parentWindow{ parentWindow }
{
    //Flap
    m_flap = adw_flap_new();
    g_object_bind_property(btnFlapToggle, "active", m_flap, "reveal-flap", (GBindingFlags)(G_BINDING_BIDIRECTIONAL | G_BINDING_SYNC_CREATE));
    //Left Pane
    m_scrollPane = gtk_scrolled_window_new();
    gtk_widget_add_css_class(m_scrollPane, "background");
    gtk_widget_set_size_request(m_scrollPane, 350, -1);
    adw_flap_set_flap(ADW_FLAP(m_flap), m_scrollPane);
    //Pane Box
    m_paneBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_hexpand(m_paneBox, false);
    gtk_widget_set_vexpand(m_paneBox, true);
    gtk_widget_set_margin_top(m_paneBox, 10);
    gtk_widget_set_margin_start(m_paneBox, 10);
    gtk_widget_set_margin_end(m_paneBox, 10);
    gtk_widget_set_margin_bottom(m_paneBox, 10);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(m_scrollPane), m_paneBox);
    //Account Total
    m_lblTotal = gtk_label_new("");
    gtk_widget_add_css_class(m_lblTotal, "accent");
    m_rowTotal = adw_action_row_new();
    adw_preferences_row_set_title(ADW_PREFERENCES_ROW(m_rowTotal), _("Total"));
    adw_action_row_add_suffix(ADW_ACTION_ROW(m_rowTotal), m_lblTotal);
    //Account Income
    m_lblIncome = gtk_label_new("");
    gtk_widget_add_css_class(m_lblIncome, "success");
    m_chkIncome = gtk_check_button_new();
    gtk_check_button_set_active(GTK_CHECK_BUTTON(m_chkIncome), true);
    gtk_widget_add_css_class(m_chkIncome, "selection-mode");
    m_rowIncome = adw_action_row_new();
    adw_preferences_row_set_title(ADW_PREFERENCES_ROW(m_rowIncome), _("Income"));
    adw_action_row_add_prefix(ADW_ACTION_ROW(m_rowIncome), m_chkIncome);
    adw_action_row_add_suffix(ADW_ACTION_ROW(m_rowIncome), m_lblIncome);
    //Account Expense
    m_lblExpense = gtk_label_new("");
    gtk_widget_add_css_class(m_lblExpense, "error");
    m_chkExpense = gtk_check_button_new();
    gtk_check_button_set_active(GTK_CHECK_BUTTON(m_chkExpense), true);
    gtk_widget_add_css_class(m_chkExpense, "selection-mode");
    m_rowExpense = adw_action_row_new();
    adw_preferences_row_set_title(ADW_PREFERENCES_ROW(m_rowExpense), _("Expense"));
    adw_action_row_add_prefix(ADW_ACTION_ROW(m_rowExpense), m_chkExpense);
    adw_action_row_add_suffix(ADW_ACTION_ROW(m_rowExpense), m_lblExpense);
    //Overview Buttons Box
    m_boxButtonsOverview = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
    //Button Menu Account Actions
    m_btnMenuAccountActions = gtk_menu_button_new();
    gtk_widget_add_css_class(m_btnMenuAccountActions, "flat");
    GtkWidget* btnMenuAccountActionsContent{ adw_button_content_new() };
    adw_button_content_set_icon_name(ADW_BUTTON_CONTENT(btnMenuAccountActionsContent), "document-properties-symbolic");
    adw_button_content_set_label(ADW_BUTTON_CONTENT(btnMenuAccountActionsContent), _("Actions"));
    gtk_menu_button_set_child(GTK_MENU_BUTTON(m_btnMenuAccountActions), btnMenuAccountActionsContent);
    GMenu* menuActions{ g_menu_new() };
    g_menu_append(menuActions, _("Export as CSV"), "account.exportAsCSV");
    g_menu_append(menuActions, _("Import from CSV"), "account.importFromCSV");
    gtk_menu_button_set_menu_model(GTK_MENU_BUTTON(m_btnMenuAccountActions), G_MENU_MODEL(menuActions));
    g_object_unref(menuActions);
    gtk_box_append(GTK_BOX(m_boxButtonsOverview), m_btnMenuAccountActions);
    //Button Reset Overview Filter
    m_btnResetOverview = gtk_button_new_from_icon_name("edit-clear-all-symbolic");
    gtk_widget_set_sensitive(m_btnResetOverview, false);
    gtk_widget_add_css_class(m_btnResetOverview, "flat");
    gtk_widget_set_tooltip_text(m_btnResetOverview, _("Reset Overview Filters"));
    gtk_box_append(GTK_BOX(m_boxButtonsOverview), m_btnResetOverview);
    //Overview Group
    m_grpOverview = adw_preferences_group_new();
    adw_preferences_group_set_title(ADW_PREFERENCES_GROUP(m_grpOverview), _("Overview"));
    adw_preferences_group_add(ADW_PREFERENCES_GROUP(m_grpOverview), m_rowTotal);
    adw_preferences_group_add(ADW_PREFERENCES_GROUP(m_grpOverview), m_rowIncome);
    adw_preferences_group_add(ADW_PREFERENCES_GROUP(m_grpOverview), m_rowExpense);
    adw_preferences_group_set_header_suffix(ADW_PREFERENCES_GROUP(m_grpOverview), m_boxButtonsOverview);
    gtk_box_append(GTK_BOX(m_paneBox), m_grpOverview);
    //Group Buttons Box
    m_boxButtonsGroups = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
    //Button New Group
    m_btnNewGroup = gtk_button_new();
    gtk_widget_add_css_class(m_btnNewGroup, "flat");
    GtkWidget* btnNewGroupContent{ adw_button_content_new() };
    adw_button_content_set_icon_name(ADW_BUTTON_CONTENT(btnNewGroupContent), "list-add-symbolic");
    adw_button_content_set_label(ADW_BUTTON_CONTENT(btnNewGroupContent), pgettext("Group", "New"));
    gtk_widget_set_tooltip_text(m_btnNewGroup, _("New Group (Ctrl+G)"));
    gtk_actionable_set_detailed_action_name(GTK_ACTIONABLE(m_btnNewGroup), "account.newGroup");
    gtk_button_set_child(GTK_BUTTON(m_btnNewGroup), btnNewGroupContent);
    gtk_box_append(GTK_BOX(m_boxButtonsGroups), m_btnNewGroup);
    //Button Reset Groups Filter
    m_btnResetGroups = gtk_button_new_from_icon_name("edit-clear-all-symbolic");
    gtk_widget_set_sensitive(m_btnResetGroups, false);
    gtk_widget_add_css_class(m_btnResetGroups, "flat");
    gtk_widget_set_tooltip_text(m_btnResetGroups, _("Reset Groups Filters"));
    gtk_box_append(GTK_BOX(m_boxButtonsGroups), m_btnResetGroups);
    //Groups Group
    m_grpGroups = adw_preferences_group_new();
    adw_preferences_group_set_title(ADW_PREFERENCES_GROUP(m_grpGroups), _("Groups"));
    adw_preferences_group_set_header_suffix(ADW_PREFERENCES_GROUP(m_grpGroups), m_boxButtonsGroups);
    gtk_box_append(GTK_BOX(m_paneBox), m_grpGroups);
    //Calendar Widget
    m_calendar = gtk_calendar_new();
    gtk_widget_add_css_class(m_calendar, "card");
    gtk_widget_add_css_class(m_calendar, "osd");
    //Button Reset Calendar Filter
    m_btnResetCalendar = gtk_button_new_from_icon_name("edit-clear-all-symbolic");
    gtk_widget_set_sensitive(m_btnResetCalendar, false);
    gtk_widget_add_css_class(m_btnResetCalendar, "flat");
    gtk_widget_set_tooltip_text(m_btnResetCalendar, _("Reset Dates Filters"));
    //Range DropDowns
    m_ddStartYear = gtk_drop_down_new(G_LIST_MODEL(gtk_string_list_new(NULL)), NULL);
    gtk_widget_set_valign(m_ddStartYear, GTK_ALIGN_CENTER);
    gtk_drop_down_set_show_arrow(GTK_DROP_DOWN(m_ddStartYear), false);
    m_ddStartMonth = gtk_drop_down_new(G_LIST_MODEL(gtk_string_list_new(NULL)), NULL);
    gtk_widget_set_valign(m_ddStartMonth, GTK_ALIGN_CENTER);
    gtk_drop_down_set_show_arrow(GTK_DROP_DOWN(m_ddStartMonth), false);
    m_ddStartDay = gtk_drop_down_new(G_LIST_MODEL(gtk_string_list_new(NULL)), NULL);
    gtk_widget_set_valign(m_ddStartDay, GTK_ALIGN_CENTER);
    gtk_drop_down_set_show_arrow(GTK_DROP_DOWN(m_ddStartDay), false);
    m_ddEndYear = gtk_drop_down_new(G_LIST_MODEL(gtk_string_list_new(NULL)), NULL);
    gtk_widget_set_valign(m_ddEndYear, GTK_ALIGN_CENTER);
    gtk_drop_down_set_show_arrow(GTK_DROP_DOWN(m_ddEndYear), false);
    m_ddEndMonth = gtk_drop_down_new(G_LIST_MODEL(gtk_string_list_new(NULL)), NULL);
    gtk_widget_set_valign(m_ddEndMonth, GTK_ALIGN_CENTER);
    gtk_drop_down_set_show_arrow(GTK_DROP_DOWN(m_ddEndMonth), false);
    m_ddEndDay = gtk_drop_down_new(G_LIST_MODEL(gtk_string_list_new(NULL)), NULL);
    gtk_widget_set_valign(m_ddEndDay, GTK_ALIGN_CENTER);
    gtk_drop_down_set_show_arrow(GTK_DROP_DOWN(m_ddEndDay), false);
    //Range Boxes
    m_boxStartRange = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
    gtk_box_append(GTK_BOX(m_boxStartRange), m_ddStartYear);
    gtk_box_append(GTK_BOX(m_boxStartRange), m_ddStartMonth);
    gtk_box_append(GTK_BOX(m_boxStartRange), m_ddStartDay);
    m_boxEndRange = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
    gtk_box_append(GTK_BOX(m_boxEndRange), m_ddEndYear);
    gtk_box_append(GTK_BOX(m_boxEndRange), m_ddEndMonth);
    gtk_box_append(GTK_BOX(m_boxEndRange), m_ddEndDay);
    //Start Range Row
    m_rowStartRange = adw_action_row_new();
    adw_preferences_row_set_title(ADW_PREFERENCES_ROW(m_rowStartRange), _("Start"));
    adw_action_row_add_suffix(ADW_ACTION_ROW(m_rowStartRange), m_boxStartRange);
    //End Range Row
    m_rowEndRange = adw_action_row_new();
    adw_preferences_row_set_title(ADW_PREFERENCES_ROW(m_rowEndRange), _("End"));
    adw_action_row_add_suffix(ADW_ACTION_ROW(m_rowEndRange), m_boxEndRange);
    //Select Range Group
    m_grpRange = adw_preferences_group_new();
    //Expander Row Select Range
    m_expRange = adw_expander_row_new();
    adw_preferences_row_set_title(ADW_PREFERENCES_ROW(m_expRange), _("Select Range"));
    adw_expander_row_set_enable_expansion(ADW_EXPANDER_ROW(m_expRange), false);
    adw_expander_row_set_show_enable_switch(ADW_EXPANDER_ROW(m_expRange), true);
    adw_expander_row_add_row(ADW_EXPANDER_ROW(m_expRange), m_rowStartRange);
    adw_expander_row_add_row(ADW_EXPANDER_ROW(m_expRange), m_rowEndRange);
    adw_preferences_group_add(ADW_PREFERENCES_GROUP(m_grpRange), m_expRange);
    //Calendar Group
    m_grpCalendar = adw_preferences_group_new();
    adw_preferences_group_set_title(ADW_PREFERENCES_GROUP(m_grpCalendar), _("Calendar"));
    adw_preferences_group_set_header_suffix(ADW_PREFERENCES_GROUP(m_grpCalendar), m_btnResetCalendar);
    adw_preferences_group_add(ADW_PREFERENCES_GROUP(m_grpCalendar), m_calendar);
    gtk_box_append(GTK_BOX(m_paneBox), m_grpCalendar);
    gtk_box_append(GTK_BOX(m_paneBox), m_grpRange);
    //Separator
    adw_flap_set_separator(ADW_FLAP(m_flap), gtk_separator_new(GTK_ORIENTATION_VERTICAL));
    //Button New Transaction
    m_btnNewTransaction = gtk_button_new();
    gtk_widget_add_css_class(m_btnNewTransaction, "pill");
    gtk_widget_add_css_class(m_btnNewTransaction, "suggested-action");
    GtkWidget* btnNewTransactionContent{ adw_button_content_new() };
    adw_button_content_set_icon_name(ADW_BUTTON_CONTENT(btnNewTransactionContent), "list-add-symbolic");
    adw_button_content_set_label(ADW_BUTTON_CONTENT(btnNewTransactionContent), pgettext("Transaction", "New"));
    gtk_widget_set_tooltip_text(m_btnNewTransaction, _("New Transaction (Ctrl+Shift+N)"));
    gtk_actionable_set_detailed_action_name(GTK_ACTIONABLE(m_btnNewTransaction), "account.newTransaction");
    gtk_button_set_child(GTK_BUTTON(m_btnNewTransaction), btnNewTransactionContent);
    gtk_widget_set_halign(m_btnNewTransaction, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(m_btnNewTransaction, GTK_ALIGN_END);
    gtk_widget_set_margin_bottom(m_btnNewTransaction, 10);
    //Sort Box and Buttons
    m_btnSortTopBottom = gtk_toggle_button_new();
    gtk_button_set_icon_name(GTK_BUTTON(m_btnSortTopBottom), "view-sort-descending-symbolic");
    gtk_widget_set_tooltip_text(m_btnSortTopBottom, "Sort From Top To Bottom");
    g_signal_connect(m_btnSortTopBottom, "clicked", G_CALLBACK((void (*)(GtkToggleButton*, gpointer))[](GtkToggleButton*, gpointer data) { reinterpret_cast<AccountView*>(data)->onAccountInfoChanged(); }), this);
    m_btnSortBottomTop = gtk_toggle_button_new();
    gtk_button_set_icon_name(GTK_BUTTON(m_btnSortBottomTop), "view-sort-ascending-symbolic");
    gtk_widget_set_tooltip_text(m_btnSortBottomTop, "Sort From Bottom To Top");
    g_signal_connect(m_btnSortBottomTop, "clicked", G_CALLBACK((void (*)(GtkToggleButton*, gpointer))[](GtkToggleButton*, gpointer data) { reinterpret_cast<AccountView*>(data)->onAccountInfoChanged(); }), this);
    g_object_bind_property(m_btnSortTopBottom, "active", m_btnSortBottomTop, "active", (GBindingFlags)(G_BINDING_BIDIRECTIONAL | G_BINDING_SYNC_CREATE | G_BINDING_INVERT_BOOLEAN));
    m_boxSort = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_append(GTK_BOX(m_boxSort), m_btnSortTopBottom);
    gtk_box_append(GTK_BOX(m_boxSort), m_btnSortBottomTop);
    gtk_widget_set_valign(m_boxSort, GTK_ALIGN_CENTER);
    gtk_widget_add_css_class(m_boxSort, "linked");
    //Transactions Group
    m_grpTransactions = adw_preferences_group_new();
    adw_preferences_group_set_title(ADW_PREFERENCES_GROUP(m_grpTransactions), _("Transactions"));
    adw_preferences_group_set_header_suffix(ADW_PREFERENCES_GROUP(m_grpTransactions), m_boxSort);
    //Transactions Flow Box
    m_flowBox = gtk_flow_box_new();
    gtk_flow_box_set_homogeneous(GTK_FLOW_BOX(m_flowBox), true);
    gtk_flow_box_set_column_spacing(GTK_FLOW_BOX(m_flowBox), 10);
    gtk_flow_box_set_row_spacing(GTK_FLOW_BOX(m_flowBox), 10);
    gtk_widget_set_halign(m_flowBox, GTK_ALIGN_FILL);
    gtk_widget_set_valign(m_flowBox, GTK_ALIGN_START);
    gtk_flow_box_set_selection_mode(GTK_FLOW_BOX(m_flowBox), GTK_SELECTION_NONE);
    //Transactions Scrolled Window
    m_scrollTransactions = gtk_scrolled_window_new();
    gtk_widget_set_size_request(m_scrollTransactions, 300, -1);
    gtk_widget_set_vexpand(m_scrollTransactions, true);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(m_scrollTransactions), m_flowBox);
    //Main Box
    m_boxMain = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_margin_start(m_boxMain, 10);
    gtk_widget_set_margin_top(m_boxMain, 10);
    gtk_widget_set_margin_end(m_boxMain, 10);
    gtk_widget_set_margin_bottom(m_boxMain, 10);
    gtk_widget_set_hexpand(m_boxMain, true);
    gtk_widget_set_vexpand(m_boxMain, true);
    gtk_box_append(GTK_BOX(m_boxMain), m_grpTransactions);
    gtk_box_append(GTK_BOX(m_boxMain), m_scrollTransactions);
    //Main Overlay
    m_overlayMain = gtk_overlay_new();
    gtk_widget_set_vexpand(m_overlayMain, true);
    gtk_overlay_set_child(GTK_OVERLAY(m_overlayMain), m_boxMain);
    gtk_overlay_add_overlay(GTK_OVERLAY(m_overlayMain), m_btnNewTransaction);
    adw_flap_set_content(ADW_FLAP(m_flap), m_overlayMain);
    //Tab Page
    m_gobj = adw_tab_view_append(parentTabView, m_flap);
    adw_tab_page_set_title(m_gobj, m_controller.getAccountPath().c_str());
    //Action Map
    m_actionMap = g_simple_action_group_new();
    gtk_widget_insert_action_group(m_flap, "account", G_ACTION_GROUP(m_actionMap));
    //Export as CSV Action
    m_actExportAsCSV = g_simple_action_new("exportAsCSV", nullptr);
    g_signal_connect(m_actExportAsCSV, "activate", G_CALLBACK((void (*)(GSimpleAction*, GVariant*, gpointer))[](GSimpleAction*, GVariant*, gpointer data) { reinterpret_cast<AccountView*>(data)->onExportAsCSV(); }), this);
    g_action_map_add_action(G_ACTION_MAP(m_actionMap), G_ACTION(m_actExportAsCSV));
    //Import from CSV Action
    m_actImportFromCSV = g_simple_action_new("importFromCSV", nullptr);
    g_signal_connect(m_actImportFromCSV, "activate", G_CALLBACK((void (*)(GSimpleAction*, GVariant*, gpointer))[](GSimpleAction*, GVariant*, gpointer data) { reinterpret_cast<AccountView*>(data)->onImportFromCSV(); }), this);
    g_action_map_add_action(G_ACTION_MAP(m_actionMap), G_ACTION(m_actImportFromCSV));
    //New Group Action
    m_actNewGroup = g_simple_action_new("newGroup", nullptr);
    g_signal_connect(m_actNewGroup, "activate", G_CALLBACK((void (*)(GSimpleAction*, GVariant*, gpointer))[](GSimpleAction*, GVariant*, gpointer data) { reinterpret_cast<AccountView*>(data)->onNewGroup(); }), this);
    g_action_map_add_action(G_ACTION_MAP(m_actionMap), G_ACTION(m_actNewGroup));
    //New Transaction Action
    m_actNewTransaction = g_simple_action_new("newTransaction", nullptr);
    g_signal_connect(m_actNewTransaction, "activate", G_CALLBACK((void (*)(GSimpleAction*, GVariant*, gpointer))[](GSimpleAction*, GVariant*, gpointer data) { reinterpret_cast<AccountView*>(data)->onNewTransaction(); }), this);
    g_action_map_add_action(G_ACTION_MAP(m_actionMap), G_ACTION(m_actNewTransaction));
    //Shortcut Controller
    m_shortcutController = gtk_shortcut_controller_new();
    gtk_shortcut_controller_set_scope(GTK_SHORTCUT_CONTROLLER(m_shortcutController), GTK_SHORTCUT_SCOPE_MANAGED);
    gtk_shortcut_controller_add_shortcut(GTK_SHORTCUT_CONTROLLER(m_shortcutController), gtk_shortcut_new(gtk_shortcut_trigger_parse_string("<Ctrl>G"), gtk_named_action_new("account.newGroup")));
    gtk_shortcut_controller_add_shortcut(GTK_SHORTCUT_CONTROLLER(m_shortcutController), gtk_shortcut_new(gtk_shortcut_trigger_parse_string("<Ctrl><Shift>N"), gtk_named_action_new("account.newTransaction")));
    gtk_widget_add_controller(m_flap, m_shortcutController);
    //Account Info Changed Callback
    m_controller.registerAccountInfoChangedCallback([&]() { onAccountInfoChanged(); });
    //Load Information
    if(m_controller.getSortFirstToLast())
    {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(m_btnSortTopBottom), true);
    }
    else
    {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(m_btnSortBottomTop), true);
    }
    onAccountInfoChanged();
}

AdwTabPage* AccountView::gobj()
{
    return m_gobj;
}

void AccountView::onAccountInfoChanged()
{
    //Overview
    gtk_label_set_label(GTK_LABEL(m_lblTotal), m_controller.getAccountTotalString().c_str());
    gtk_label_set_label(GTK_LABEL(m_lblIncome), m_controller.getAccountIncomeString().c_str());
    gtk_label_set_label(GTK_LABEL(m_lblExpense), m_controller.getAccountExpenseString().c_str());
    //Groups
    for(const std::shared_ptr<GroupRow>& groupRow : m_groupRows)
    {
        adw_preferences_group_remove(ADW_PREFERENCES_GROUP(m_grpGroups), groupRow->gobj());
    }
    m_groupRows.clear();
    std::vector<Group> groups;
    for(const std::pair<const unsigned int, Group>& pair : m_controller.getGroups())
    {
        groups.push_back(pair.second);
    }
    std::sort(groups.begin(), groups.end());
    for(const Group& group : groups)
    {
        std::shared_ptr<GroupRow> row{ std::make_shared<GroupRow>(group, m_controller.getLocale()) };
        row->registerEditCallback([&](unsigned int id) { onEditGroup(id); });
        row->registerDeleteCallback([&](unsigned int id) { onDeleteGroup(id); });
        adw_preferences_group_add(ADW_PREFERENCES_GROUP(m_grpGroups), row->gobj());
        m_groupRows.push_back(row);
        g_main_context_iteration(g_main_context_default(), false);
    }
    //Transactions
    for(const std::shared_ptr<TransactionRow>& transactionRow : m_transactionRows)
    {
        gtk_flow_box_remove(GTK_FLOW_BOX(m_flowBox), transactionRow->gobj());
    }
    m_transactionRows.clear();
    for(const std::pair<const unsigned int, Transaction>& pair : m_controller.getTransactions())
    {
        std::shared_ptr<TransactionRow> row{ std::make_shared<TransactionRow>(pair.second, m_controller.getLocale()) };
        row->registerEditCallback([&](unsigned int id) { onEditTransaction(id); });
        row->registerDeleteCallback([&](unsigned int id) { onDeleteTransaction(id); });
        if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(m_btnSortTopBottom)))
        {
            gtk_flow_box_append(GTK_FLOW_BOX(m_flowBox), row->gobj());
        }
        else
        {
            gtk_flow_box_prepend(GTK_FLOW_BOX(m_flowBox), row->gobj());
        }
        m_transactionRows.push_back(row);
        g_main_context_iteration(g_main_context_default(), false);
    }
    //Remember Sort Setting
    m_controller.setSortFirstToLast(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(m_btnSortTopBottom)));
}

void AccountView::onExportAsCSV()
{
    GtkFileChooserNative* saveFileDialog{ gtk_file_chooser_native_new(_("Export as CSV"), m_parentWindow, GTK_FILE_CHOOSER_ACTION_SAVE, _("_Save"), _("_Cancel")) };
    gtk_native_dialog_set_modal(GTK_NATIVE_DIALOG(saveFileDialog), true);
    GtkFileFilter* filter{ gtk_file_filter_new() };
    gtk_file_filter_set_name(filter, "CSV (*.csv)");
    gtk_file_filter_add_pattern(filter, "*.csv");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(saveFileDialog), filter);
    g_object_unref(filter);
    g_signal_connect(saveFileDialog, "response", G_CALLBACK((void (*)(GtkNativeDialog*, gint, gpointer))([](GtkNativeDialog* dialog, gint response_id, gpointer data)
    {
        if(response_id == GTK_RESPONSE_ACCEPT)
        {
            AccountView* accountView{ reinterpret_cast<AccountView*>(data) };
            GFile* file{ gtk_file_chooser_get_file(GTK_FILE_CHOOSER(dialog)) };
            std::string path{ g_file_get_path(file) };
            ProgressDialog progressDialog{ accountView->m_parentWindow, "Exporting as CSV...", [accountView, &path]() { accountView->m_controller.exportAsCSV(path); } };
            progressDialog.run();
            g_object_unref(file);
        }
        g_object_unref(dialog);
    })), this);
    gtk_native_dialog_show(GTK_NATIVE_DIALOG(saveFileDialog));
}

void AccountView::onImportFromCSV()
{
    GtkFileChooserNative* openFileDialog{ gtk_file_chooser_native_new(_("Import from CSV"), m_parentWindow, GTK_FILE_CHOOSER_ACTION_OPEN, _("_Open"), _("_Cancel")) };
    gtk_native_dialog_set_modal(GTK_NATIVE_DIALOG(openFileDialog), true);
    GtkFileFilter* filter{ gtk_file_filter_new() };
    gtk_file_filter_set_name(filter, "CSV (*.csv)");
    gtk_file_filter_add_pattern(filter, "*.csv");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(openFileDialog), filter);
    g_object_unref(filter);
    g_signal_connect(openFileDialog, "response", G_CALLBACK((void (*)(GtkNativeDialog*, gint, gpointer))([](GtkNativeDialog* dialog, gint response_id, gpointer data)
    {
        if(response_id == GTK_RESPONSE_ACCEPT)
        {
            AccountView* accountView{ reinterpret_cast<AccountView*>(data) };
            GFile* file{ gtk_file_chooser_get_file(GTK_FILE_CHOOSER(dialog)) };
            std::string path{ g_file_get_path(file) };
            ProgressDialog progressDialog{ accountView->m_parentWindow, "Importing from CSV...", [accountView, &path]() { accountView->m_controller.importFromCSV(path); } };
            progressDialog.run();
            g_object_unref(file);
        }
        g_object_unref(dialog);
    })), this);
    gtk_native_dialog_show(GTK_NATIVE_DIALOG(openFileDialog));
}

void AccountView::onNewGroup()
{
    GroupDialogController controller{ m_controller.createGroupDialogController() };
    GroupDialog dialog{ m_parentWindow, controller };
    if(dialog.run())
    {
        m_controller.addGroup(controller.getGroup());
    }
}

void AccountView::onEditGroup(unsigned int id)
{
    GroupDialogController controller{ m_controller.createGroupDialogController(id) };
    GroupDialog dialog{ m_parentWindow, controller };
    if(dialog.run())
    {
        m_controller.updateGroup(controller.getGroup());
    }
}

void AccountView::onDeleteGroup(unsigned int id)
{
    MessageDialog messageDialog{ m_parentWindow, _("Delete Group?"), _("Are you sure you want to delete this group?\nThis action is irreversible."), _("No"), _("Yes") };
    if(messageDialog.run() == MessageDialogResponse::Destructive)
    {
        m_controller.deleteGroup(id);
    }
}

void AccountView::onNewTransaction()
{
    TransactionDialogController controller{ m_controller.createTransactionDialogController() };
    TransactionDialog dialog{ m_parentWindow, controller };
    if(dialog.run())
    {
        m_controller.addTransaction(controller.getTransaction());
    }
}

void AccountView::onEditTransaction(unsigned int id)
{
    TransactionDialogController controller{ m_controller.createTransactionDialogController(id) };
    TransactionDialog dialog{ m_parentWindow, controller };
    if(dialog.run())
    {
        m_controller.updateTransaction(controller.getTransaction());
    }
}

void AccountView::onDeleteTransaction(unsigned int id)
{
    MessageDialog messageDialog{ m_parentWindow, _("Delete Transaction?"), _("Are you sure you want to delete this transaction?\nThis action is irreversible."), _("No"), _("Yes") };
    if(messageDialog.run() == MessageDialogResponse::Destructive)
    {
        m_controller.deleteTransaction(id);
    }
}
