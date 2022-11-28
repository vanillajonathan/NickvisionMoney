#include "transactiondialogcontroller.hpp"
#include <boost/date_time/gregorian/gregorian.hpp>
#include "../helpers/moneyhelpers.hpp"
#include "../helpers/translation.hpp"

using namespace NickvisionMoney::Controllers;
using namespace NickvisionMoney::Helpers;
using namespace NickvisionMoney::Models;

TransactionDialogController::TransactionDialogController(unsigned int newId, const std::map<unsigned int, Group>& groups, Configuration& configuration) : m_response{ "cancel" }, m_configuration{ configuration }, m_transaction{ newId }, m_groups{ groups }
{
    for(const std::pair<const unsigned int, Group>& pair : m_groups)
    {
        m_groupNames.push_back(pair.second.getName());
    }
    std::sort(m_groupNames.begin(), m_groupNames.end());
    m_groupNames.insert(m_groupNames.begin(), _("None"));
}

TransactionDialogController::TransactionDialogController(const Transaction& transaction, const std::map<unsigned int, Group>& groups, Configuration& configuration) : m_response{ "cancel" }, m_configuration{ configuration }, m_transaction{ transaction }, m_groups{ groups }
{
    for(const std::pair<const unsigned int, Group>& pair : m_groups)
    {
        m_groupNames.push_back(pair.second.getName());
    }
    std::sort(m_groupNames.begin(), m_groupNames.end());
    m_groupNames.insert(m_groupNames.begin(), _("None"));
}

const std::string& TransactionDialogController::getResponse() const
{
    return m_response;
}

void TransactionDialogController::setResponse(const std::string& response)
{
    m_response = response;
}

const Transaction& TransactionDialogController::getTransaction() const
{
    return m_transaction;
}

std::string TransactionDialogController::getIdAsString() const
{
    return std::to_string(m_transaction.getId());
}

int TransactionDialogController::getYear() const
{
    return m_transaction.getDate().year();
}

int TransactionDialogController::getMonth() const
{
    return m_transaction.getDate().month();
}

int TransactionDialogController::getDay() const
{
    return m_transaction.getDate().day();
}

const std::string& TransactionDialogController::getDescription() const
{
    return m_transaction.getDescription();
}

TransactionType TransactionDialogController::getType() const
{
    return m_transaction.getType();
}

int TransactionDialogController::getRepeatIntervalAsInt() const
{
    return static_cast<int>(m_transaction.getRepeatInterval());
}

const std::vector<std::string>& TransactionDialogController::getGroupNames() const
{
    return m_groupNames;
}

int TransactionDialogController::getGroupAsIndex() const
{
    if(m_transaction.getGroupId() == -1)
    {
        return 0;
    }
    const Group& group{ m_groups.at(m_transaction.getGroupId()) };
    return std::find(m_groupNames.begin(), m_groupNames.end(), group.getName()) - m_groupNames.begin();
}

const std::string& TransactionDialogController::getRGBA() const
{
    return m_transaction.getRGBA();
}

const std::string& TransactionDialogController::getTransactionDefaultColor() const
{
    return m_configuration.getTransactionDefaultColor();
}

std::string TransactionDialogController::getAmountAsString() const
{
    return MoneyHelpers::boostMoneyToLocaleString(m_transaction.getAmount(), m_configuration.getLocale());
}

bool TransactionDialogController::isLocaleDotDecimalSeperated() const
{
    return MoneyHelpers::isLocaleDotDecimalSeperated(m_configuration.getLocale());
}

TransactionCheckStatus TransactionDialogController::updateTransaction(const std::string& dateString, const std::string& description, TransactionType type, int repeatInterval, int groupIndex, const std::string& rgba, std::string amountString)
{
    if(description.empty())
    {
        return TransactionCheckStatus::EmptyDescription;
    }
    if(amountString.empty())
    {
        return TransactionCheckStatus::EmptyAmount;
    }
    boost::multiprecision::cpp_dec_float_50 amount{ MoneyHelpers::localeStringToBoostMoney(amountString, m_configuration.getLocale()) };
    if(amount == 0)
    {
        return TransactionCheckStatus::InvalidAmount;
    }
    m_transaction.setDate(boost::gregorian::from_string(dateString));
    m_transaction.setDescription(description);
    m_transaction.setType(type);
    m_transaction.setRepeatInterval(static_cast<RepeatInterval>(repeatInterval));
    if(groupIndex == 0)
    {
         m_transaction.setGroupId(-1);
    }
    else
    {
        const std::string& groupName{ m_groupNames[groupIndex] };
        if(groupName == "None")
        {
            m_transaction.setGroupId(-1);
        }
        else
        {
            for(const std::pair<const unsigned int, Group>& pair : m_groups)
            {
                if(pair.second.getName() == groupName)
                {
                    m_transaction.setGroupId(pair.second.getId());
                    break;
                }
            }
        }
    }
    m_transaction.setRGBA(rgba);
    m_transaction.setAmount(amount);
    return TransactionCheckStatus::Valid;
}
