//
// Created by flyon21 on 09.11.24.
//
#include "Transaction.h"
#include "TransactionVisitor.h"

void Deposit::accept(TransactionVisitor &visitor) {
    visitor.visit(*this);
}

void Withdrawal::accept(TransactionVisitor &visitor) {
    visitor.visit(*this);
}

void Transfer::accept(TransactionVisitor &visitor) {
    visitor.visit(*this);
}

void BillPayment::accept(TransactionVisitor &visitor) {
    visitor.visit(*this);
}