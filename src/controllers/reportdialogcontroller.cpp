#include "reportdialogcontroller.hpp"

using namespace NickvisionMoney::Controllers;

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
