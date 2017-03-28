//===----------------------------------------------------------------------===//
//
//                         Peloton
//
// tcop.h
//
// Identification: src/include/tcop/tcop.h
//
// Copyright (c) 2015-16, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#define UNUSED(x) (void)(x)

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <mutex>
#include <stack>
#include <vector>

#include "common/portal.h"
#include "common/statement.h"
#include "concurrency/transaction.h"
#include "executor/plan_executor.h"
#include "optimizer/abstract_optimizer.h"
#include "parser/sql_statement.h"
#include "type/type.h"
#include "type/types.h"

namespace peloton {

namespace tcop {
//===--------------------------------------------------------------------===//
// TRAFFIC COP
//===--------------------------------------------------------------------===//

class MockTrafficCop {
  MockTrafficCop(MockTrafficCop const &) = delete;

 public:
  MockTrafficCop() {
    return;
  }
  ~MockTrafficCop() {
    return;
  }

  // reset this object
  void Reset() {
    return;
  }

  // PortalExec - Execute query string
  ResultType ExecuteStatement(const std::string &query,
                          std::vector<StatementResult> &result,
                          std::vector<FieldInfo> &tuple_descriptor,
                          int &rows_changed, std::string &error_message,
                          const size_t thread_id = 0) {
    UNUSED(query);
    UNUSED(error_message);
    UNUSED(thread_id);

    result.clear();
    result.push_back(std::make_pair(std::vector<unsigned char>(), std::vector<unsigned char>('r', '1')));
    result.push_back(std::make_pair(std::vector<unsigned char>(), std::vector<unsigned char>('r', '2')));
    result.push_back(std::make_pair(std::vector<unsigned char>(), std::vector<unsigned char>('r', '3')));

    tuple_descriptor.push_back(std::tuple<std::string, oid_t, size_t>("col1", 1, 2));
    tuple_descriptor.push_back(std::tuple<std::string, oid_t, size_t>("col2", 2, 2));
    tuple_descriptor.push_back(std::tuple<std::string, oid_t, size_t>("col3", 3, 2));

    rows_changed = 1;

    return ResultType::SUCCESS;
  }

  // ExecPrepStmt - Execute a statement from a prepared and bound statement
  ResultType ExecuteStatement(
      const std::shared_ptr<Statement> &statement,
      const std::vector<type::Value> &params, const bool unnamed,
      std::shared_ptr<stats::QueryMetric::QueryParams> param_stats,
      const std::vector<int> &result_format, std::vector<StatementResult> &result,
      int &rows_changed, std::string &error_message,
      const size_t thread_id = 0) {

    UNUSED(statement);
    UNUSED(params);
    UNUSED(unnamed);
    UNUSED(param_stats);
    UNUSED(result_format);
    UNUSED(error_message);
    UNUSED(thread_id);

    result.clear();
    result.push_back(std::make_pair(std::vector<unsigned char>(), std::vector<unsigned char>('r', '1')));
    result.push_back(std::make_pair(std::vector<unsigned char>(), std::vector<unsigned char>('r', '2')));

    rows_changed = 1;

    return ResultType::SUCCESS;

  }


  // InitBindPrepStmt - Prepare and bind a query from a query string
  std::shared_ptr<Statement> PrepareStatement(const std::string &statement_name,
                                              const std::string &query_string,
                                              std::string &error_message) {
    UNUSED(statement_name);
    UNUSED(query_string);
    UNUSED(error_message);
    // TODO: change more for the PARSE the prepared statement statement
    return std::shared_ptr<Statement>(nullptr);
  }
};

}  // End tcop namespace
}  // End peloton namespace
