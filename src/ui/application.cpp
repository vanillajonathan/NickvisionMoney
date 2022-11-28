#include "application.hpp"
#include <filesystem>
#include "../controllers/mainwindowcontroller.hpp"
#include "../helpers/translation.hpp"

using namespace NickvisionMoney::Controllers;
using namespace NickvisionMoney::Models;
using namespace NickvisionMoney::UI;
using namespace NickvisionMoney::UI::Views;

Application::Application(const std::string& id, GApplicationFlags flags) : m_adwApp{ adw_application_new(id.c_str(), flags) }
{
    //Load Resource
    if(std::filesystem::exists("/usr/share/org.nickvision.money/org.nickvision.money.gresource"))
    {
        g_resources_register(g_resource_load("/usr/share/org.nickvision.money/org.nickvision.money.gresource", nullptr));
    }
    else if(std::filesystem::exists("/app/share/org.nickvision.money/org.nickvision.money.gresource"))
    {
        g_resources_register(g_resource_load("/app/share/org.nickvision.money/org.nickvision.money.gresource", nullptr));
    }
    else
    {
        g_resources_register(g_resource_load("org.nickvision.money.gresource", nullptr));
    }
    //AppInfo
    m_appInfo.setId(id);
    m_appInfo.setName("Nickvision Money");
    m_appInfo.setShortName("Money");
    m_appInfo.setDescription(_("A personal finance manager."));
    m_appInfo.setVersion("2022.12.0-next");
    m_appInfo.setChangelog("<ul><li>Added the ability to change the default color for transactions</li><li>Added translation credits to About dialog</li></ul>");
    m_appInfo.setGitHubRepo("https://github.com/nlogozzo/NickvisionMoney");
    m_appInfo.setIssueTracker("https://github.com/nlogozzo/NickvisionMoney/issues/new");
    m_appInfo.setSupportUrl("https://github.com/nlogozzo/NickvisionMoney/discussions");
    m_appInfo.generateTranslatorCredits();
    //Signals
    g_signal_connect(m_adwApp, "activate", G_CALLBACK((void (*)(GtkApplication*, gpointer))[](GtkApplication* app, gpointer data) { reinterpret_cast<Application*>(data)->onActivate(app); }), this);
    g_signal_connect(m_adwApp, "open", G_CALLBACK((void (*)(GtkApplication*, gpointer, int, char*, gpointer))[](GtkApplication* app, gpointer files, int, char*, gpointer data) { reinterpret_cast<Application*>(data)->onOpen(app, files); }), this);
}

int Application::run(int argc, char* argv[])
{
    return g_application_run(G_APPLICATION(m_adwApp), argc, argv);
}

void Application::onActivate(GtkApplication* app)
{
    if(m_configuration.getTheme() == Theme::System)
    {
         adw_style_manager_set_color_scheme(adw_style_manager_get_default(), ADW_COLOR_SCHEME_PREFER_LIGHT);
    }
    else if(m_configuration.getTheme() == Theme::Light)
    {
         adw_style_manager_set_color_scheme(adw_style_manager_get_default(), ADW_COLOR_SCHEME_FORCE_LIGHT);
    }
    else if(m_configuration.getTheme() == Theme::Dark)
    {
         adw_style_manager_set_color_scheme(adw_style_manager_get_default(), ADW_COLOR_SCHEME_FORCE_DARK);
    }
    m_mainWindow = std::make_shared<MainWindow>(app, MainWindowController(m_appInfo, m_configuration));
    gtk_application_add_window(app, GTK_WINDOW(m_mainWindow->gobj()));
    m_mainWindow->start();
}

void Application::onOpen(GtkApplication* app, gpointer files)
{
    GFile** gFiles{ reinterpret_cast<GFile**>(files) };
    std::string pathOfFirstFile{ g_file_get_path(gFiles[0]) };
    onActivate(app);
    m_mainWindow->openAccountByPath(pathOfFirstFile);
}
