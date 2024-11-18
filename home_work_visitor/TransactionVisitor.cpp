//
// Created by flyon21 on 09.11.24.
//
#include "TransactionVisitor.h"
#include "Transaction.h"

void ShortReportVisitor::visit(Deposit &deposit) {
    Logger::getInstance().log("Short report for Deposit transaction", Logger::INFO);
}

void ShortReportVisitor::visit(Withdrawal &withdrawal) {
    Logger::getInstance().log("Short report for Withdrawal transaction", Logger::INFO);
}

void ShortReportVisitor::visit(Transfer &transfer) {
    Logger::getInstance().log("Short report for Transfer transaction", Logger::INFO);
}

void ShortReportVisitor::visit(BillPayment &billPayment) {
    Logger::getInstance().log("Short report for BillPayment transaction", Logger::INFO);
}

void DetailedReportVisitor::visit(Deposit &deposit) {
    Logger::getInstance().log("Detailed report for Deposit transaction", Logger::INFO);
}

void DetailedReportVisitor::visit(Withdrawal &withdrawal) {
    Logger::getInstance().log("Detailed report for Withdrawal transaction", Logger::INFO);
}

void DetailedReportVisitor::visit(Transfer &transfer) {
    Logger::getInstance().log("Detailed report for Transfer transaction", Logger::INFO);
}

void DetailedReportVisitor::visit(BillPayment &billPayment) {
    Logger::getInstance().log("Detailed report for BillPayment transaction", Logger::INFO);
}