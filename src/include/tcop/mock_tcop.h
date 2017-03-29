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
#include "parser/parser.h"

#include "type/type.h"
#include "type/types.h"

#include "catalog/catalog.h"
#include "common/abstract_tuple.h"
#include "common/logger.h"
#include "common/macros.h"
#include "common/portal.h"
#include "type/type.h"
#include "type/types.h"

#include "configuration/configuration.h"

#include "expression/aggregate_expression.h"
#include "expression/expression_util.h"
#include "optimizer/simple_optimizer.h"
#include "parser/parser.h"
#include "parser/select_statement.h"

#include "catalog/catalog.h"
#include "executor/plan_executor.h"
#include "optimizer/simple_optimizer.h"

#include "planner/plan_util.h"

#include <boost/algorithm/string.hpp>

namespace peloton {

namespace tcop {
//===--------------------------------------------------------------------===//
// TRAFFIC COP
//===--------------------------------------------------------------------===//

class MockTrafficCop {
  MockTrafficCop(MockTrafficCop const &) = delete;

 public:
  MockTrafficCop() {
    LOG_TRACE("Starting a new TrafficCop");
    optimizer_.reset(new optimizer::SimpleOptimizer());
  }
  ~MockTrafficCop() {
    return;
  }

  // reset this object
  void Reset() {
    optimizer_->Reset();
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
    LOG_TRACE("Prepare Statement name: %s", statement_name.c_str());
    LOG_TRACE("Prepare Statement query: %s", query_string.c_str());

    std::shared_ptr<Statement> statement(
        new Statement(statement_name, query_string));
    try {
      auto &peloton_parser = parser::Parser::GetInstance();
      auto sql_stmt = peloton_parser.BuildParseTree(query_string);
      if (sql_stmt->is_valid == false) {
        throw ParserException("Error parsing SQL statement");
      }
      auto plan = optimizer_->BuildPelotonPlanTree(sql_stmt);
      statement->SetPlanTree(plan);

      // Get the tables that our plan references so that we know how to
      // invalidate it at a later point when the catalog changes
      const std::set<oid_t> table_oids =
          planner::PlanUtil::GetTablesReferenced(plan.get());
      statement->SetReferencedTables(table_oids);

      for (auto stmt : sql_stmt->GetStatements()) {
        LOG_TRACE("SQLStatement: %s", stmt->GetInfo().c_str());
        if (stmt->GetType() == StatementType::SELECT) {
          auto tuple_descriptor = GenerateTupleDescriptor(stmt);
          statement->SetTupleDescriptor(tuple_descriptor);
        }
        break;
      }

  #ifdef LOG_DEBUG_ENABLED
      if (statement->GetPlanTree().get() != nullptr) {
        LOG_DEBUG("Statement Prepared: %s", statement->GetInfo().c_str());
        LOG_TRACE("%s", statement->GetPlanTree().get()->GetInfo().c_str());
      }
  #endif
      return std::move(statement);
    } catch (Exception &e) {
      error_message = e.what();
      return nullptr;
    }
  }

  std::vector<FieldInfo> GenerateTupleDescriptor(
      parser::SQLStatement *select_stmt) {
    std::vector<FieldInfo> tuple_descriptor;

    UNUSED(select_stmt);

    tuple_descriptor.push_back(std::tuple<std::string, oid_t, size_t>("col1", 1, 2));
    tuple_descriptor.push_back(std::tuple<std::string, oid_t, size_t>("col2", 2, 2));
    tuple_descriptor.push_back(std::tuple<std::string, oid_t, size_t>("col3", 3, 2));
    return tuple_descriptor;
  }

 private:
  // The optimizer used for this connection
  std::unique_ptr<optimizer::AbstractOptimizer> optimizer_;

};

}  // End tcop namespace
}  // End peloton namespace
