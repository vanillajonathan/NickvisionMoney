#include "reportdialogcontroller.hpp"
#include <filesystem>
#include "../helpers/moneyhelpers.hpp"

using namespace NickvisionMoney::Controllers;
using namespace NickvisionMoney::Helpers;

ReportDialogController::ReportDialogController()
{

}

const std::string& ReportDialogController::getResponse() const
{
    return m_response;
}

void ReportDialogController::setResponse(const std::string& response)
{
    m_response = response;
}
