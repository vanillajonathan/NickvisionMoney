#pragma once

#include <functional>
#include <locale>
#include <map>
#include <string>
#include <unordered_map>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include "groupdialogcontroller.hpp"
#include "transactiondialogcontroller.hpp"
#include "transferdialogcontroller.hpp"
#include "../models/account.hpp"
#include "../models/configuration.hpp"
#include "../models/group.hpp"
#include "../models/transaction.hpp"
#include "../models/transfer.hpp"

namespace NickvisionMoney::Controllers
{
	/**
	 * A controller for the AccountView
	 */
	class AccountViewController
	{
	public:
		/**
		 * Constructs an AccountViewController
		 *
		 * @param path The path of the account to open
		 * @param configuration The configuration
		 * @param sendToastCallback A callback function for sending a toast notification to the UI
		 */
		AccountViewController(const std::string& path, NickvisionMoney::Models::Configuration& configuration, const std::function<void(const std::string& message)>& sendToastCallback);
		/**
		 * Gets the path of the account
		 *
		 * @returns The path of the account
		 */
		const std::string& getAccountPath() const;
		/**
		 * Gets the user's locale
		 *
		 * @returns The user's locale
		 */
		const std::locale& getLocale() const;
		/**
		 * Gets the total of the account
		 *
		 * @returns The total of the account
		 */
		boost::multiprecision::cpp_dec_float_50 getAccountTotal() const;
		/**
		 * Gets the total of the account as a string
		 *
		 * @returns The total of the account
		 */
		std::string getAccountTotalString() const;
		/**
		 * Gets the income of the account as a string
		 *
		 * @returns The income of the account
		 */
		std::string getAccountIncomeString() const;
		/**
		 * Gets the expense of the account as a string
		 *
		 * @returns The expense of the account
		 */
		std::string getAccountExpenseString() const;
		/**
		 * Gets the map of groups in the account
		 *
		 * @returns The groups in the account
		 */
		const std::map<unsigned int, NickvisionMoney::Models::Group>& getGroups() const;
		/**
		 * Gets the map of transactions in the account
		 *
		 * @returns The transactions in the account
		 */
		const std::map<unsigned int, NickvisionMoney::Models::Transaction>& getTransactions() const;
		/**
		 * Sends a toast to the main window
		 *
		 * @param message The message to send in the toast
		 */
		void sendToast(const std::string& message);
		/**
		 * Registers a callback for updating the UI when an account's info is changed
		 *
		 * @param callback A void() function
		 */
		void registerAccountInfoChangedCallback(const std::function<void()>& callback);
		/**
		 * Registers a callback for send a transfer to an account to receive
		 *
		 * @param callback A void(const NickvisionMoney::Models::Transfer&) function
		 */
		void registerReceiveTransferCallback(const std::function<void(const NickvisionMoney::Models::Transfer&)>& callback);
		/**
		 * Creates a TransferDialogController for a new transfer
		 *
		 * @returns A new TransferDialogController
		 */
		TransferDialogController createTransferDialogController() const;
		/**
		 * Creates an expense transaction for the transfer and triggers receiveTransfer() for the destination account
		 *
		 * @param transfer The transfer information
		 */
		void sendTransfer(const NickvisionMoney::Models::Transfer& transfer);
		/**
		 * Creates an income transaction for the transfer
		 *
		 * @param transfer The transfer information
		 */
		void receiveTransfer(const NickvisionMoney::Models::Transfer& transfer);
		/**
		 * Export the account as a PDF file
		 *
		 * @param path The path to the PDF file
		 */
		void exportAsPDF(std::string& path) const;
		/**
		 * Export the account as a CSV file
		 *
		 * @param path The path to the CSV file
		 */
		void exportAsCSV(std::string& path) const;
		/**
		 * Import transactions from a CSV file
		 *
		 * @param path The path to the csv file
		 */
		void importFromCSV(const std::string& path);
		/**
		 * Adds a new group to the account
		 *
		 * @param group The group to add
		 */
		void addGroup(const NickvisionMoney::Models::Group& group);
		/**
		 * Updates a group in the account
		 *
		 * @param group The group to update
		 */
		void updateGroup(const NickvisionMoney::Models::Group& group);
		/**
		 * Deletes a group in the account
		 *
		 * @param id The id of the group to delete
		 */
		void deleteGroup(unsigned int id);
		/**
		 * Creates a GroupDialogController for a new group
		 *
		 * @returns A new GroupDialogController
		 */
		GroupDialogController createGroupDialogController() const;
		/**
		 * Creates a GroupDialogController for an existing group
		 *
		 * @param id The id of the group to edit
		 * @returns A new GroupDialogController
		 */
		GroupDialogController createGroupDialogController(unsigned int id) const;
		/**
		 * Adds a new transaction to the account
		 *
		 * @param transaction The transaction to add
		 */
		void addTransaction(const NickvisionMoney::Models::Transaction& transaction);
		/**
		 * Updates a transaction in the account
		 *
		 * @param transaction The transaction to update
		 */
		void updateTransaction(const NickvisionMoney::Models::Transaction& transaction);
		/**
		 * Deletes a transaction in the account
		 *
		 * @param id The id of the transaction to delete
		 */
		void deleteTransaction(unsigned int id);
		/**
		 * Creates a TransactionDialogController for a new transaction
		 *
		 * @returns A new TransactionDialogController
		 */
		TransactionDialogController createTransactionDialogController() const;
		/**
		 * Creates a TransactionDialogController for an existing transaction
		 *
		 * @param id The id of the transaction to edit
		 * @returns A new TransactionDialogController
		 */
		TransactionDialogController createTransactionDialogController(unsigned int id) const;
		/**
		 * Gets whether or not to sort transactions from first to last
		 *
		 * @returns True for first to last, false for last to first
		 */
		bool getSortFirstToLast() const;
		/**
		 * Sets whether or not to sort transactions from first to last
		 *
		 * @param sortFirstToLast True for first to last, false for last to first
		 */
		void setSortFirstToLast(bool sortFirstToLast);
		/**
		 * Gets a list of filtered transactions
		 *
		 * @returns The list of filtered transactions
		 */
		std::vector<NickvisionMoney::Models::Transaction> getFilteredTransactions() const;
		/**
		 * Gets whether or not a filter is active
		 *
		 * @param key The key of the filter
		 * @returns True if active, else false
		 */
		bool getIfFilterActive(int key) const;
		/**
		 * Updates a filter
		 *
		 * @param key The group id or -3 for income or -2 for expense
		 * @param value True to include the filter, else false
		 */
		void updateFilterValue(int key, bool value);
		/**
		 * Gets a list of years to display in the date range filter
		 *
		 * @returns The list of years to display
		 */
		std::vector<std::string> getYearsForRangeFilter() const;
		/**
		 * Resets the date filter
		 */
		void resetDateFilter();
		/**
		 * Sets the start date of the filter
		 *
		 * @param date The new start date
		 */
		void setFilterStartDate(const boost::gregorian::date& date);
		/**
		 * Sets the end date of the filter
		 *
		 * @param date The new end date
		 */
		void setFilterEndDate(const boost::gregorian::date& date);

	private:
		NickvisionMoney::Models::Configuration& m_configuration;
		NickvisionMoney::Models::Account m_account;
		std::function<void(const std::string& message)> m_sendToastCallback;
		std::function<void()> m_accountInfoChangedCallback;
		std::function<void(const NickvisionMoney::Models::Transfer&)> m_receiveTransferCallback;
		std::unordered_map<int, bool> m_mapFilters;
		boost::gregorian::date m_filterStartDate;
		boost::gregorian::date m_filterEndDate;
	};
}