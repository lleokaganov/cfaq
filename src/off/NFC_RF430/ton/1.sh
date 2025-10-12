#!/bin/bash

clear

time=$(date +%s)
from=$((time-10))

#curl "https://tonapi.io/v2/accounts/UQCQH2TZP4cpMska9YXOaEML1fnD6Q7OUn5LBqWRFYXKP6EQ/transactions" | jq
#curl "https://toncenter.com/api/v2/getTransactions?address=UQCQH2TZP4cpMska9YXOaEML1fnD6Q7OUn5LBqWRFYXKP6EQ\
#&start_utime=${from}&end_utime=${time}" | jq

lt_value="EAAAAAAAAA=="  # Подставь `lt` последней транзакции
curl "https://toncenter.com/api/v2/getTransactions?address=UQCQH2TZP4cpMska9YXOaEML1fnD6Q7OUn5LBqWRFYXKP6EQ&before=${lt_value}&limit=20" | jq

exit

&limit=5\

&start_utime=$(date -d 'yesterday' +%s)&end_utime=$(date +%s)\

&lt=100000000&gt=100000000" | jq
    100000000

По количеству (limit)
По времени (start_utime / end_utime)
По адресу отправителя / получателя (account / to_account)
По сумме транзакции (lt)