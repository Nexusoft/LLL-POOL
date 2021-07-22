#ifndef NEXUSPOOL_PERSISTANCE_COMMAND_COMMAND_IMPL_HPP
#define NEXUSPOOL_PERSISTANCE_COMMAND_COMMAND_IMPL_HPP

#include "persistance/command/command.hpp"
#include "sqlite/sqlite3.h"
#include <memory>

namespace spdlog { class logger; }
namespace nexuspool {
namespace persistance {
namespace command {


template<typename Result, typename CommandType>
class Command_banned_user_and_ip_impl : public Command_base_database<Result, CommandType>
{
public:

	Command_banned_user_and_ip_impl(sqlite3* handle)
		: m_handle{handle}
		, m_banned_user_and_ip_stmt{nullptr}
	{
		sqlite3_prepare_v2(m_handle,
			"SELECT * FROM banned_users_connections WHERE user = ':user' AND ip = ':ip';",
			-1, &m_banned_user_and_ip_stmt, NULL);
	}
	~Command_banned_user_and_ip_impl()
	{
		sqlite3_finalize(m_banned_user_and_ip_stmt);
	}

	Class get_class() const override { return Class::database_sqlite; }
	CommandType get_command() const override
	{
		return m_banned_user_and_ip_stmt;
	}

	Type get_type() const override { return Type::get_banned_user_and_ip; }

private:
	sqlite3* m_handle;
	sqlite3_stmt* m_banned_user_and_ip_stmt;

};

template<typename Result, typename CommandType>
class Command_banned_api_ip_impl : public Command_base_database<Result, CommandType>
{
public:

	Command_banned_api_ip_impl(sqlite3* handle)
		: m_handle{ handle }
		, m_banned_api_ip_stmt{ nullptr }
	{
		sqlite3_prepare_v2(m_handle,
			"SELECT * FROM banned_connections_api WHERE ip = ':ip';",
			-1, &m_banned_api_ip_stmt, NULL);
	}
	~Command_banned_api_ip_impl()
	{
		sqlite3_finalize(m_banned_api_ip_stmt);
	}

	Class get_class() const override { return Class::database_sqlite; }
	CommandType get_command() const override { return m_banned_api_ip_stmt; }
	Type get_type() const override { return Type::get_banned_api_ip; }

private:
	sqlite3* m_handle;
	sqlite3_stmt* m_banned_api_ip_stmt;

};

template<typename Result, typename CommandType>
class Command_create_db_schema_impl : public Command_base_database<Result, CommandType>
{
public:

	Command_create_db_schema_impl(sqlite3* handle)
		: m_handle{ handle }
		, m_create_tables_stmt{ nullptr }
	{
		std::string create_tables{R"("CREATE TABLE IF NOT EXISTS round (
				  id INTEGER PRIMARY KEY AUTOINCREMENT,
				  round_number INTEGER NOT NULL,
				  total_shares REAL,
				  total_reward REAL,
				  blocks INTEGER,
				  connection_count INTEGER
				);

				CREATE TABLE IF NOT EXISTS block (
				  id INTEGER PRIMARY KEY AUTOINCREMENT,
				  hash TEXT NOT NULL,
				  height INTEGER NOT NULL,
				  type TEXT NOT NULL
				  reward INTEGER NOT NULL,
				  difficulty REAL NOT NULL,
				  orphan INTEGER NOT NULL,
				  block_finder TEXT NOT NULL,
				  round INTEGER NOT NULL,
				  block_found_time TEXT NOT NULL,
				  FOREIGN KEY(round) REFERENCES round(id),
				  FOREIGN KEY(block_finder) REFERENCES account(name)
				);

				CREATE TABLE IF NOT EXISTS account (
				  name TEXT PRIMARY KEY,
				  created_at TEXT NOT NULL,
				  last_active TEXT,
				  connection_count INTEGER,
				  shares REAL,
				  reward REAL,
				  hashrate REAL
				);

				CREATE TABLE IF NOT EXISTS banned_connections_api (
				  id INTEGER PRIMARY KEY AUTOINCREMENT,
				  ip TEXT NOT NULL
				);

				CREATE TABLE IF NOT EXISTS banned_users_connections (
				  user TEXT,
				  ip TEXT,
				  PRIMARY KEY (user, ip)
				);)"
		};

		sqlite3_prepare_v2(m_handle, create_tables.c_str(), -1, &m_create_tables_stmt, NULL);
	}

	~Command_create_db_schema_impl()
	{
		sqlite3_finalize(m_create_tables_stmt);
	}

	Class get_class() const override { return Class::database_sqlite; }
	CommandType get_command() const override
	{
		return m_create_tables_stmt;
	}

	Type get_type() const override { return Type::create_db_schema; }

private:
	sqlite3* m_handle;
	sqlite3_stmt* m_create_tables_stmt;


};

}
}
}

#endif 
