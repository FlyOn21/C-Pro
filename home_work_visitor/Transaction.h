//
// Created by flyon21 on 09.11.24.
//

#ifndef C_PRO_TRANSACTION_H
#define C_PRO_TRANSACTION_H
#include "TransactionVisitor.h"

class Transaction {
public:
    virtual void accept(TransactionVisitor &visitor) = 0;
    virtual ~Transaction() noexcept = default;
};

class Deposit final : public Transaction {
public:
    void accept(TransactionVisitor &visitor) override;
};

class Withdrawal final : public Transaction {
public:
    void accept(TransactionVisitor &visitor) override;
};

class Transfer final : public Transaction {
public:
    void accept(TransactionVisitor &visitor) override;
};

class BillPayment final : public Transaction {
public:
    void accept(TransactionVisitor &visitor) override;
};
#endif //C_PRO_TRANSACTION_H
