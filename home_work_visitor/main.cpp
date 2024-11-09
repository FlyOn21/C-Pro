//
// Created by flyon21 on 09.11.24.
//
#include "Transaction.h"
#include "TransactionVisitor.h"

int main() {
    Deposit deposit;
    Withdrawal withdrawal;
    Transfer transfer;
    BillPayment billPayment;

    ShortReportVisitor shortReport;
    DetailedReportVisitor detailedReport;

    deposit.accept(shortReport);
    withdrawal.accept(shortReport);
    transfer.accept(shortReport);
    billPayment.accept(shortReport);

    deposit.accept(detailedReport);
    withdrawal.accept(detailedReport);
    transfer.accept(detailedReport);
    billPayment.accept(detailedReport);

    return 0;
}