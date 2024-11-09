//
// Created by flyon21 on 09.11.24.
//

#ifndef C_PRO_TRANSACTIONVISITOR_H
#define C_PRO_TRANSACTIONVISITOR_H
#include "Logger.h"

class Deposit;
class Withdrawal;
class Transfer;
class BillPayment;

class TransactionVisitor {
public:
    virtual void visit(Deposit &deposit) = 0;
    virtual void visit(Withdrawal &withdrawal) = 0;
    virtual void visit(Transfer &transfer) = 0;
    virtual void visit(BillPayment &billPayment) = 0;
    virtual ~TransactionVisitor() = default;
};

class ShortReportVisitor final : public TransactionVisitor {
public:
    void visit(Deposit &deposit) override;
    void visit(Withdrawal &withdrawal) override;
    void visit(Transfer &transfer) override;
    void visit(BillPayment &billPayment) override;
};

class DetailedReportVisitor final : public TransactionVisitor {
public:
    void visit(Deposit &deposit) override;
    void visit(Withdrawal &withdrawal) override;
    void visit(Transfer &transfer) override;
    void visit(BillPayment &billPayment) override;
};
#endif //C_PRO_TRANSACTIONVISITOR_H
