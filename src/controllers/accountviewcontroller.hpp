#pragma once

#include <string>
#include "../models/account.hpp"

namespace NickvisionMoney::Controllers
{
	class AccountViewController
	{
	public:
		AccountViewController(const std::string& path, const std::string& currencySymbol);
		const std::string& getAccountPath() const;
		std::string getAccountTotalString() const;
		std::string getAccountIncomeString() const;
		std::string getAccountExpenseString() const;

	private:
		std::string m_currencySymbol;
		NickvisionMoney::Models::Account m_account;
	};
}